#include "stdafx.h"
#include "MdUserApi.h"
#include "../../include/QueueEnum.h"
#include "../../include/QueueHeader.h"

#include "../../include/ApiHeader.h"
#include "../../include/ApiStruct.h"

#include "../../include/toolkit.h"
#include "../../include/ApiProcess.h"

#include "../../common/Queue/MsgQueue.h"
#ifdef _REMOTE
#include "../../common/Queue/RemoteQueue.h"
#endif

#include <string.h>

#include <mutex>
#include <vector>
using namespace std;

ExchangeType TSecurityFtdcExchangeIDType_2_ExchangeType(TSecurityFtdcExchangeIDType In)
{
	switch (In[1])
	{
	case 'S':
		return ExchangeType::ExchangeType_SSE;
	case 'Z':
		return ExchangeType::ExchangeType_SZSE;
	default:
		return ExchangeType::ExchangeType_Undefined;
	}
}

void* __stdcall Query(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3)
{
	// ���ڲ����ã����ü���Ƿ�Ϊ��
	CMdUserApi* pApi = (CMdUserApi*)pApi2;
	pApi->QueryInThread(type, pApi1, pApi2, double1, double2, ptr1, size1, ptr2, size2, ptr3, size3);
	return nullptr;
}

CMdUserApi::CMdUserApi(void)
{
	m_pApi = nullptr;
	m_lRequestID = 0;
	m_nSleep = 1;

	// �Լ�ά��������Ϣ����
	m_msgQueue = new CMsgQueue();
	m_msgQueue_Query = new CMsgQueue();

	m_msgQueue_Query->Register((void*)Query, this);
	m_msgQueue_Query->StartThread();

	m_remoteQueue = nullptr;
}

CMdUserApi::~CMdUserApi(void)
{
	Disconnect();
}

void CMdUserApi::QueryInThread(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3)
{
	int iRet = 0;
	switch (type)
	{
	case E_Init:
		iRet = _Init();
		break;
	case E_ReqUserLoginField:
		iRet = _ReqUserLogin(type, pApi1, pApi2, double1, double2, ptr1, size1, ptr2, size2, ptr3, size3);
		break;
	default:
		break;
	}

	if (0 == iRet)
	{
		//���سɹ�����ӵ��ѷ��ͳ�
		m_nSleep = 1;
	}
	else
	{
		m_msgQueue_Query->Input_Copy(type, pApi1, pApi2, double1, double2, ptr1, size1, ptr2, size2, ptr3, size3);
		//ʧ�ܣ���4���ݽ�����ʱ����������1s
		m_nSleep *= 4;
		m_nSleep %= 1023;
	}
	this_thread::sleep_for(chrono::milliseconds(m_nSleep));
}

void CMdUserApi::Register(void* pCallback, void* pClass)
{
	m_pClass = pClass;
	if (m_msgQueue == nullptr)
		return;

	m_msgQueue_Query->Register((void*)Query,this);
	m_msgQueue->Register(pCallback,this);
	if (pCallback)
	{
		m_msgQueue_Query->StartThread();
		m_msgQueue->StartThread();
	}
	else
	{
		m_msgQueue_Query->StopThread();
		m_msgQueue->StopThread();
	}
}

bool CMdUserApi::IsErrorRspInfo(const char* szSource, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bRet)
	{
		ErrorField* pField = (ErrorField*)m_msgQueue->new_block(sizeof(ErrorField));

		pField->RawErrorID = pRspInfo->ErrorID;
		strcpy(pField->Text, pRspInfo->ErrorMsg);
		strcpy(pField->Source, szSource);

		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnRtnError, m_msgQueue, m_pClass, bIsLast, 0, pField, sizeof(ErrorField), nullptr, 0, nullptr, 0);
	}
	return bRet;
}

bool CMdUserApi::IsErrorRspInfo(CSecurityFtdcRspInfoField *pRspInfo)
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));

	return bRet;
}

void CMdUserApi::Connect(const string& szPath,
	ServerInfoField* pServerInfo,
	UserInfoField* pUserInfo)
{
	m_szPath = szPath;
	memcpy(&m_ServerInfo, pServerInfo, sizeof(ServerInfoField));
	memcpy(&m_UserInfo, pUserInfo, sizeof(UserInfoField));

	m_msgQueue_Query->Input_NoCopy(RequestType::E_Init, m_msgQueue_Query, this, 0, 0,
		nullptr, 0, nullptr, 0, nullptr, 0);

#ifdef _REMOTE
	// ���յ�������ͨ��ZeroMQ���ͳ�ȥ
	if (strlen(m_ServerInfo.ExtendInformation) > 0)
	{
		m_remoteQueue = new CRemoteQueue(m_ServerInfo.ExtendInformation);
		m_remoteQueue->StartThread();
	}
#endif
}

int CMdUserApi::_Init()
{
	char *pszPath = new char[m_szPath.length() + 1024];
	srand((unsigned int)time(nullptr));
	sprintf(pszPath, "%s/%s/%s/Md/%d/", m_szPath.c_str(), m_ServerInfo.BrokerID, m_UserInfo.UserID, rand());
	makedirs(pszPath);

	m_pApi = CSecurityFtdcMdApi::CreateFtdcMdApi(pszPath);
	delete[] pszPath;

	m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Initialized, 0, nullptr, 0, nullptr, 0, nullptr, 0);

	if (m_pApi)
	{
		m_pApi->RegisterSpi(this);

		//��ӵ�ַ
		size_t len = strlen(m_ServerInfo.Address) + 1;
		char* buf = new char[len];
		strncpy(buf, m_ServerInfo.Address, len);

		char* token = strtok(buf, _QUANTBOX_SEPS_);
		while (token)
		{
			if (strlen(token)>0)
			{
				m_pApi->RegisterFront(token);
			}
			token = strtok(nullptr, _QUANTBOX_SEPS_);
		}
		delete[] buf;

		//��ʼ������
		m_pApi->Init();
		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Connecting, 0, nullptr, 0, nullptr, 0, nullptr, 0);
	}

	return 0;
}

void CMdUserApi::ReqUserLogin()
{
	CSecurityFtdcReqUserLoginField* pBody = (CSecurityFtdcReqUserLoginField*)m_msgQueue_Query->new_block(sizeof(CSecurityFtdcReqUserLoginField));

	strncpy(pBody->BrokerID, m_ServerInfo.BrokerID, sizeof(TSecurityFtdcBrokerIDType));
	strncpy(pBody->UserID, m_UserInfo.UserID, sizeof(TSecurityFtdcInvestorIDType));
	strncpy(pBody->Password, m_UserInfo.Password, sizeof(TSecurityFtdcPasswordType));

	m_msgQueue_Query->Input_NoCopy(RequestType::E_ReqUserLoginField, m_msgQueue_Query, this, 0, 0,
		pBody, sizeof(CSecurityFtdcReqUserLoginField), nullptr, 0, nullptr, 0);
}

int CMdUserApi::_ReqUserLogin(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3)
{
	m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Logining, 0, nullptr, 0, nullptr, 0, nullptr, 0);
	return m_pApi->ReqUserLogin((CSecurityFtdcReqUserLoginField*)ptr1, ++m_lRequestID);
}

void CMdUserApi::Disconnect()
{
	// �����ѯ����
	if (m_msgQueue_Query)
	{
		m_msgQueue_Query->StopThread();
		m_msgQueue_Query->Register(nullptr,nullptr);
		m_msgQueue_Query->Clear();
		delete m_msgQueue_Query;
		m_msgQueue_Query = nullptr;
	}

	if (m_pApi)
	{
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;

		// ȫ����ֻ�����һ��
		m_msgQueue->Clear();
		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Disconnected, 0, nullptr, 0, nullptr, 0, nullptr, 0);
		// ��������
		m_msgQueue->Process();
	}

	// ������Ӧ����
	if (m_msgQueue)
	{
		m_msgQueue->StopThread();
		m_msgQueue->Register(nullptr,nullptr);
		m_msgQueue->Clear();
		delete m_msgQueue;
		m_msgQueue = nullptr;
	}

	// �������
	if (m_remoteQueue)
	{
		m_remoteQueue->StopThread();
		m_remoteQueue->Register(nullptr, nullptr);
		m_remoteQueue->Clear();
		delete m_remoteQueue;
		m_remoteQueue = nullptr;
	}
}


void CMdUserApi::Subscribe(const string& szInstrumentIDs, const string& szExchangeID)
{
	if(nullptr == m_pApi)
		return;

	vector<char*> vct;
	set<char*> st;

	lock_guard<mutex> cl(m_csMapInstrumentIDs);

	set<string> _setInstrumentIDs;
	map<string, set<string> >::iterator it = m_mapInstrumentIDs.find(szExchangeID);
	if (it != m_mapInstrumentIDs.end())
	{
		_setInstrumentIDs = it->second;
	}

	char* pBuf = GetSetFromString(szInstrumentIDs.c_str(), _QUANTBOX_SEPS_, vct, st, 1, _setInstrumentIDs);
	m_mapInstrumentIDs[szExchangeID] = _setInstrumentIDs;

	if(vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->SubscribeMarketData(pArray, (int)vct.size(), (char*)(szExchangeID.c_str()));

		delete[] pArray;
	}
	delete[] pBuf;
}

void CMdUserApi::Subscribe(const set<string>& instrumentIDs, const string& szExchangeID)
{
	if(nullptr == m_pApi)
		return;

	string szInstrumentIDs;
	for(set<string>::iterator i=instrumentIDs.begin();i!=instrumentIDs.end();++i)
	{
		szInstrumentIDs.append(*i);
		szInstrumentIDs.append(";");
	}

	if (szInstrumentIDs.length()>1)
	{
		Subscribe(szInstrumentIDs, szExchangeID);
	}
}

void CMdUserApi::Unsubscribe(const string& szInstrumentIDs, const string& szExchangeID)
{
	if(nullptr == m_pApi)
		return;

	vector<char*> vct;
	set<char*> st;

	lock_guard<mutex> cl(m_csMapInstrumentIDs);

	set<string> _setInstrumentIDs;
	map<string, set<string> >::iterator it = m_mapInstrumentIDs.find(szExchangeID);
	if (it != m_mapInstrumentIDs.end())
	{
		_setInstrumentIDs = it->second;
	}

	char* pBuf = GetSetFromString(szInstrumentIDs.c_str(), _QUANTBOX_SEPS_, vct, st, -1, _setInstrumentIDs);
	m_mapInstrumentIDs[szExchangeID] = _setInstrumentIDs;

	if(vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->UnSubscribeMarketData(pArray, (int)vct.size(), (char*)(szExchangeID.c_str()));

		delete[] pArray;
	}
	delete[] pBuf;
}

void CMdUserApi::OnFrontConnected()
{
	m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Connected, 0, nullptr, 0, nullptr, 0, nullptr, 0);

	//���ӳɹ����Զ������¼
	ReqUserLogin();
}

void CMdUserApi::OnFrontDisconnected(int nReason)
{
	RspUserLoginField* pField = (RspUserLoginField*)m_msgQueue->new_block(sizeof(RspUserLoginField));
	//����ʧ�ܷ��ص���Ϣ��ƴ�Ӷ��ɣ���Ҫ��Ϊ��ͳһ���
	pField->RawErrorID = nReason;
	GetOnFrontDisconnectedMsg(nReason, pField->Text);

	m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Disconnected, 0, pField, sizeof(RspUserLoginField), nullptr, 0, nullptr, 0);
}

void CMdUserApi::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	RspUserLoginField* pField = (RspUserLoginField*)m_msgQueue->new_block(sizeof(RspUserLoginField));

	if (!IsErrorRspInfo(pRspInfo)
		&&pRspUserLogin)
	{
		//GetExchangeTime(pRspUserLogin->TradingDay, nullptr, pRspUserLogin->LoginTime,
		//	&pField->TradingDay, nullptr, &pField->LoginTime, nullptr);
		pField->TradingDay = GetDate(pRspUserLogin->TradingDay);
		pField->LoginTime = GetTime(pRspUserLogin->LoginTime);

		sprintf(pField->SessionID, "%d:%d", pRspUserLogin->FrontID, pRspUserLogin->SessionID);

		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Logined, 0, pField, sizeof(RspUserLoginField), nullptr, 0, nullptr, 0);
		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Done, 0, nullptr, 0, nullptr, 0, nullptr, 0);

		//�п��ܶ����ˣ������Ƕ������������¶���
		map<string, set<string> > mapOld = m_mapInstrumentIDs;//�����ϴζ��ĵĺ�Լ

		for (map<string, set<string> >::iterator i = mapOld.begin(); i != mapOld.end(); ++i)
		{
			string strkey = i->first;
			set<string> setValue = i->second;

			Subscribe(setValue, strkey);//����
		}

		//�п��ܶ����ˣ������Ƕ������������¶���
		//mapOld = m_setQuoteInstrumentIDs;//�����ϴζ��ĵĺ�Լ
		//SubscribeQuote(mapOld, "");//����
	}
	else
	{
		pField->RawErrorID = pRspInfo->ErrorID;
		strncpy(pField->Text, pRspInfo->ErrorMsg, sizeof(Char256Type));

		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Disconnected, 0, pField, sizeof(RspUserLoginField), nullptr, 0, nullptr, 0);
	}
}

void CMdUserApi::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	IsErrorRspInfo("OnRspError", pRspInfo, nRequestID, bIsLast);
}

void CMdUserApi::OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//��ģ��ƽ̨��������������ᴥ��������Ҫ�Լ�ά��һ���Ѿ����ĵĺ�Լ�б�
	if (!IsErrorRspInfo("OnRspSubMarketData", pRspInfo, nRequestID, bIsLast)
		&&pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapInstrumentIDs);

		set<string> _setInstrumentIDs;
		map<string, set<string> >::iterator it = m_mapInstrumentIDs.find(pSpecificInstrument->ExchangeID);
		if (it != m_mapInstrumentIDs.end())
		{
			_setInstrumentIDs = it->second;
		}

		_setInstrumentIDs.insert(pSpecificInstrument->InstrumentID);
		m_mapInstrumentIDs[pSpecificInstrument->ExchangeID] = _setInstrumentIDs;
	}
}

void CMdUserApi::OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//ģ��ƽ̨��������������ᴥ��
	if (!IsErrorRspInfo("OnRspUnSubMarketData", pRspInfo, nRequestID, bIsLast)
		&&pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapInstrumentIDs);

		set<string> _setInstrumentIDs;
		map<string, set<string> >::iterator it = m_mapInstrumentIDs.find(pSpecificInstrument->ExchangeID);
		if (it != m_mapInstrumentIDs.end())
		{
			_setInstrumentIDs = it->second;
		}
		_setInstrumentIDs.erase(pSpecificInstrument->InstrumentID);
		m_mapInstrumentIDs[pSpecificInstrument->ExchangeID] = _setInstrumentIDs;
	}
}

//����ص����ñ�֤�˺������췵��
void CMdUserApi::OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData)
{
	DepthMarketDataNField* pField = (DepthMarketDataNField*)m_msgQueue->new_block(sizeof(DepthMarketDataNField)+sizeof(DepthField)* 10);


	strcpy(pField->InstrumentID, pDepthMarketData->InstrumentID);
	strcpy(pField->ExchangeID, pDepthMarketData->ExchangeID);
	pField->Exchange = TSecurityFtdcExchangeIDType_2_ExchangeType(pDepthMarketData->ExchangeID);

	sprintf(pField->Symbol, "%s.%s", pField->InstrumentID, pDepthMarketData->ExchangeID);
	GetExchangeTime(pDepthMarketData->TradingDay, nullptr, pDepthMarketData->UpdateTime
		, &pField->TradingDay, &pField->ActionDay, &pField->UpdateTime, &pField->UpdateMillisec);
	pField->UpdateMillisec = pDepthMarketData->UpdateMillisec;

	pField->LastPrice = pDepthMarketData->LastPrice;
	pField->Volume = pDepthMarketData->Volume;
	pField->Turnover = pDepthMarketData->Turnover;
	pField->OpenInterest = pDepthMarketData->OpenInterest;
	pField->AveragePrice = pDepthMarketData->AveragePrice;

	pField->OpenPrice = pDepthMarketData->OpenPrice == DBL_MAX ? 0 : pDepthMarketData->OpenPrice;
	pField->HighestPrice = pDepthMarketData->HighestPrice == DBL_MAX ? 0 : pDepthMarketData->HighestPrice;
	pField->LowestPrice = pDepthMarketData->LowestPrice == DBL_MAX ? 0 : pDepthMarketData->LowestPrice;
	pField->ClosePrice = pDepthMarketData->ClosePrice == DBL_MAX ? 0 : pDepthMarketData->ClosePrice;
	pField->SettlementPrice = pDepthMarketData->SettlementPrice;

	pField->UpperLimitPrice = pDepthMarketData->UpperLimitPrice;
	pField->LowerLimitPrice = pDepthMarketData->LowerLimitPrice;
	pField->PreClosePrice = pDepthMarketData->PreClosePrice;
	pField->PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
	pField->PreOpenInterest = pDepthMarketData->PreOpenInterest;

	InitBidAsk(pField);

	do
	{
		if (pDepthMarketData->BidVolume1 == 0)
			break;
		AddBid(pField, pDepthMarketData->BidPrice1, pDepthMarketData->BidVolume1, 0);

		if (pDepthMarketData->BidVolume2 == 0)
			break;
		AddBid(pField, pDepthMarketData->BidPrice2, pDepthMarketData->BidVolume2, 0);

		if (pDepthMarketData->BidVolume3 == 0)
			break;
		AddBid(pField, pDepthMarketData->BidPrice3, pDepthMarketData->BidVolume3, 0);

		if (pDepthMarketData->BidVolume4 == 0)
			break;
		AddBid(pField, pDepthMarketData->BidPrice4, pDepthMarketData->BidVolume4, 0);

		if (pDepthMarketData->BidVolume5 == 0)
			break;
		AddBid(pField, pDepthMarketData->BidPrice5, pDepthMarketData->BidVolume5, 0);
	} while (false);

	do
	{
		if (pDepthMarketData->AskVolume1 == 0)
			break;
		AddAsk(pField, pDepthMarketData->AskPrice1, pDepthMarketData->AskVolume1, 0);

		if (pDepthMarketData->AskVolume2 == 0)
			break;
		AddAsk(pField, pDepthMarketData->AskPrice2, pDepthMarketData->AskVolume2, 0);

		if (pDepthMarketData->AskVolume3 == 0)
			break;
		AddAsk(pField, pDepthMarketData->AskPrice3, pDepthMarketData->AskVolume3, 0);

		if (pDepthMarketData->AskVolume4 == 0)
			break;
		AddAsk(pField, pDepthMarketData->AskPrice4, pDepthMarketData->AskVolume4, 0);

		if (pDepthMarketData->AskVolume5 == 0)
			break;
		AddAsk(pField, pDepthMarketData->AskPrice5, pDepthMarketData->AskVolume5, 0);
	} while (false);


	// ������������ͷѭ��Ҫ��죬��ɾ�����ܵ����Ҫ���ں���
	// �����ǰ�棬�ᵼ��Զ���յ�����
#ifdef _REMOTE
	if (m_remoteQueue)
	{
		m_remoteQueue->Input_Copy(ResponseType::ResponseType_OnRtnDepthMarketData, m_msgQueue, m_pClass, 0, 0, pField, pField->Size, nullptr, 0, nullptr, 0);
	}
#endif

	m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnRtnDepthMarketData, m_msgQueue, m_pClass, 0, 0, pField, pField->Size, nullptr, 0, nullptr, 0);
}
