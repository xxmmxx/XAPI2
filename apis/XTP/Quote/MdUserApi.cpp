#include "MdUserApi.h"
#include <iostream>
#include <string>

#include "../../include/QueueEnum.h"

#include "../../include/ApiHeader.h"
#include "../../include/ApiStruct.h"

#include "../../include/toolkit.h"
#include "../../include/ApiProcess.h"
#include "../TypeConvert.h"

#include "../../include/queue/MsgQueue.h"

#include "../../include/CSubscribeManager.h"
#include "../../include/synthetic/CSyntheticConfig.h"
#include "../../include/synthetic/CSyntheticManager.h"
#include "../../include/CSyntheticCalculate_DepthMarketDataNField.h"

void* __stdcall Query(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3)
{
	// ���ڲ����ã����ü���Ƿ�Ϊ��
	CMdUserApi* pApi = (CMdUserApi*)pApi2;
	pApi->QueryInThread(type, pApi1, pApi2, double1, double2, ptr1, size1, ptr2, size2, ptr3, size3);
	return nullptr;
}

CMdUserApi::CMdUserApi()
{
	m_pApi = nullptr;
	m_lRequestID = 0;
	m_nSleep = 1;

	m_pSyntheticConfig = new CSyntheticConfig();
	m_pSyntheticManager = new CSyntheticManager();
	m_pCalculateFactory = new CSyntheticCalculateFactory_XAPI();
	m_pSubscribeManager = new CSubscribeManager(m_pSyntheticConfig, m_pSyntheticManager, m_pCalculateFactory);
	m_pQuoteSubscribeManager = new CSubscribeManager(m_pSyntheticConfig, m_pSyntheticManager, m_pCalculateFactory);

	// �Լ�ά��������Ϣ����
	m_msgQueue = new CMsgQueue();
	m_msgQueue_Query = new CMsgQueue();

	m_msgQueue_Query->Register(Query);
	m_msgQueue_Query->StartThread();
}
CMdUserApi::~CMdUserApi()
{
	Disconnect();
}

void CMdUserApi::Register(void* pCallback, void* pClass)
{
	m_pClass = pClass;
	if (m_msgQueue == nullptr)
		return;

	m_msgQueue_Query->Register(Query);
	m_msgQueue->Register((fnOnResponse)pCallback);
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
ConfigInfoField* CMdUserApi::Config(ConfigInfoField* pConfigInfo)
{
	return nullptr;
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
}
void CMdUserApi::Disconnect()
{
	// �����ѯ����
	if (m_msgQueue_Query)
	{
		m_msgQueue_Query->StopThread();
		m_msgQueue_Query->Register(nullptr);
		m_msgQueue_Query->Clear();
		delete m_msgQueue_Query;
		m_msgQueue_Query = nullptr;
	}

	if (m_pApi)
	{
		m_pApi->Release();
		m_pApi = nullptr;

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
		m_msgQueue->Register(nullptr);
		m_msgQueue->Clear();
		delete m_msgQueue;
		m_msgQueue = nullptr;
	}

	if (m_pSubscribeManager)
	{
		delete m_pSubscribeManager;
		m_pSubscribeManager = nullptr;
	}
	if (m_pSyntheticConfig)
	{
		delete m_pSyntheticConfig;
		m_pSyntheticConfig = nullptr;
	}
	if (m_pSyntheticManager)
	{
		delete m_pSyntheticManager;
		m_pSyntheticManager = nullptr;
	}
	if (m_pCalculateFactory)
	{
		delete m_pCalculateFactory;
		m_pCalculateFactory = nullptr;
	}
}

void CMdUserApi::Subscribe(const string& szInstrumentIDs, const string& szExchangeID)
{
	if (nullptr == m_pApi)
		return;

	set<string> ss_new = m_pSubscribeManager->String2Set(szInstrumentIDs.c_str());
	Subscribe(ss_new, szExchangeID);
}
void CMdUserApi::Unsubscribe(const string& szInstrumentIDs, const string& szExchangeID)
{
	if (nullptr == m_pApi)
		return;

	set<string> ss_new = m_pSubscribeManager->String2Set(szInstrumentIDs.c_str());
	Unsubscribe(ss_new, szExchangeID);
}

void CMdUserApi::QueryInThread(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3)
{
	int iRet = 0;
	switch (type)
	{
	case E_Init:
		iRet = _Init();
		break;
	case E_Disconnect:
		Disconnect();
		// ����ѭ��
		return;

	}

	if (m_pApi)
	{
		switch (type)
		{
		case E_ReqUserLoginField:
			iRet = _ReqUserLogin(type, pApi1, pApi2, double1, double2, ptr1, size1, ptr2, size2, ptr3, size3);
			break;
		default:
			break;
		}
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

int CMdUserApi::_Init()
{
//	char *pszPath = new char[m_szPath.length() + 1024];
//	srand((unsigned int)time(NULL));
//	sprintf(pszPath, "%s/%s/%s/Md/%d/", m_szPath.c_str(), m_ServerInfo.BrokerID, m_UserInfo.UserID, rand());
//	makedirs(pszPath);

	m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Initialized, 0, nullptr, 0, nullptr, 0, nullptr, 0);

	m_pApi = XTP::API::QuoteApi::CreateQuoteApi(m_UserInfo.ExtInfoInt32, m_szPath.c_str());

	if (m_pApi)
	{
		m_pApi->RegisterSpi(this);

		// ��ʼ����������
		m_pApi->SetUDPBufferSize(1024);
		m_pApi->SetHeartBeatInterval(15);

		// XTPû�е���������ǰ�ýӿڣ���������ģ��ǰ�û����ӳɹ���Ϣ
		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Connecting, 0, nullptr, 0, nullptr, 0, nullptr, 0);
		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Connected, 0, nullptr, 0, nullptr, 0, nullptr, 0);

		//���ӳɹ����Զ������¼
		ReqUserLogin();
	}
	
	return 0;
}
void CMdUserApi::_DisconnectInThread()
{
	m_msgQueue_Query->Input_NoCopy(RequestType::E_Disconnect, m_msgQueue_Query, this, 0, 0,
		nullptr, 0, nullptr, 0, nullptr, 0);
}

void CMdUserApi::ReqUserLogin()
{
	m_msgQueue_Query->Input_NoCopy(RequestType::E_ReqUserLoginField, m_msgQueue_Query, this, 0, 0,
		nullptr, 0, nullptr, 0, nullptr, 0);
}
int CMdUserApi::_ReqUserLogin(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3)
{
	XTP_PROTOCOL_TYPE md_protocol = XTP_PROTOCOL_TCP;
	if (m_ServerInfo.IsUsingUdp)
	{
		md_protocol = XTP_PROTOCOL_UDP;
	}

	m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Logining, 0, nullptr, 0, nullptr, 0, nullptr, 0);
	int ret = m_pApi->Login(m_ServerInfo.Address, m_ServerInfo.Port, m_UserInfo.UserID, m_UserInfo.Password, md_protocol);
	if (ret == 0)
	{
		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Logined, 0, nullptr, 0, nullptr, 0, nullptr, 0);
		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Done, 0, nullptr, 0, nullptr, 0, nullptr, 0);

		{
			set<string> exchanges = m_pSubscribeManager->GetExchanges();
			for (auto exchange = exchanges.begin(); exchange != exchanges.end(); ++exchange)
			{
				set<string> mapOld = m_pSubscribeManager->GetParentInstruments(exchange->data());//�����ϴζ��ĵĺ�Լ
				m_pSubscribeManager->Clear();
				Subscribe(mapOld, exchange->data());//����
			}
		}
	}
	else
	{
		m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Disconnected, 0, nullptr, 0, nullptr, 0, nullptr, 0);

		// �Ͽ�����
		_DisconnectInThread();
	}

	return 0;
}

void CMdUserApi::Subscribe(const set<string>& instrumentIDs, const string& szExchangeID)
{
	if (nullptr == m_pApi)
		return;

	if (instrumentIDs.size() == 0)
		return;

	m_pSubscribeManager->Subscribe(instrumentIDs, szExchangeID);

	XTP_EXCHANGE_TYPE exchange_type = (XTP_EXCHANGE_TYPE)Str_2_XTPExchangeType(szExchangeID.c_str());
	
	char **instruments = (char**)malloc(sizeof(char*) * instrumentIDs.size());
	int i = 0;
	for (auto &s : instrumentIDs)
	{
		instruments[i] = (char*)malloc(s.size() + 1);
		strncpy(instruments[i], s.c_str(), s.size());
		instruments[i][s.size()] = '\0';
	}
	m_pApi->SubscribeMarketData(instruments, instrumentIDs.size(), exchange_type);

	for (int i = 0; i < instrumentIDs.size(); i++)
	{
		delete instruments[i];
	}
	delete instruments;
}
void CMdUserApi::Unsubscribe(const set<string>& instrumentIDs, const string& szExchangeID)
{
	if (nullptr == m_pApi)
		return;

	if (instrumentIDs.size() == 0)
		return;

	m_pSubscribeManager->Subscribe(instrumentIDs, szExchangeID);

	XTP_EXCHANGE_TYPE exchange_type;
	if (szExchangeID == "SH")
	{
		exchange_type = XTP_EXCHANGE_SH;
	}
	else if (szExchangeID == "SZ")
	{
		exchange_type = XTP_EXCHANGE_SZ;
	}
	else
	{
		exchange_type = XTP_EXCHANGE_UNKNOWN;
	}

	char **instruments = (char**)malloc(sizeof(char*) * instrumentIDs.size());
	int i = 0;
	for (auto &s : instrumentIDs)
	{
		instruments[i] = (char*)malloc(s.size() + 1);
		strncpy(instruments[i], s.c_str(), s.size());
		instruments[i][s.size()] = '\0';
	}
	m_pApi->UnSubscribeMarketData(instruments, instrumentIDs.size(), exchange_type);

	for (int i = 0; i < instrumentIDs.size(); i++)
	{
		delete instruments[i];
	}
	delete instruments;
}

void CMdUserApi::OnDisconnected(int reason)
{
	RspUserLoginField* pField = (RspUserLoginField*)m_msgQueue->new_block(sizeof(RspUserLoginField));
	//����ʧ�ܷ��ص���Ϣ��ƴ�Ӷ��ɣ���Ҫ��Ϊ��ͳһ���
	pField->RawErrorID = reason;

	m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnConnectionStatus, m_msgQueue, m_pClass, ConnectionStatus::ConnectionStatus_Disconnected, 0, pField, sizeof(RspUserLoginField), nullptr, 0, nullptr, 0);

	// �Ͽ�����
	_DisconnectInThread();
}

void CMdUserApi::OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last)
{
	// TODO: 
}
void CMdUserApi::OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last)
{
	// TODO:
}
void CMdUserApi::OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count)
{
	DepthMarketDataNField* pField = (DepthMarketDataNField*)m_msgQueue->new_block(sizeof(DepthMarketDataNField) + sizeof(DepthField) * 10);

	// ������
	switch (market_data->exchange_id)
	{
	case XTP_EXCHANGE_SH:
	{
		strcpy(pField->ExchangeID, "SSE");
		pField->Exchange = ExchangeType::ExchangeType_SSE;
	}break;
	case XTP_EXCHANGE_SZ:
	{
		strcpy(pField->ExchangeID, "SZSE");
		pField->Exchange = ExchangeType::ExchangeType_SZSE;
	}break;
	default:
	{
		strcpy(pField->ExchangeID, "Undefined");
		pField->Exchange = ExchangeType::ExchangeType_Undefined;
	}
	}

	// ��Ʊ����
	strcpy(pField->Symbol, market_data->ticker);

	// ����
	pField->LastPrice = market_data->last_price;
	pField->Volume = market_data->qty;
	pField->Turnover = market_data->turnover;
	pField->AveragePrice = market_data->avg_price;

	pField->OpenPrice = market_data->open_price;
	pField->ClosePrice = market_data->close_price;
	pField->HighestPrice = market_data->high_price;
	pField->LowestPrice = market_data->low_price;

	pField->UpperLimitPrice = market_data->upper_limit_price;
	pField->LowerLimitPrice = market_data->lower_limit_price;
	pField->PreClosePrice = market_data->pre_close_price;
	pField->PreSettlementPrice = market_data->pre_settl_price;

	InitBidAsk(pField);

	for (int i = 0; i < 10; i++)
	{
		if (market_data->bid[i] < FLT_EPSILON || market_data->bid_qty[i] < FLT_EPSILON)
		{
			break;
		}
		AddBid(pField, market_data->bid[i], market_data->bid_qty[i], 0);
	}

	for (int i = 0; i < 10; i++)
	{
		if (market_data->ask[i] < FLT_EPSILON || market_data->ask_qty[i] < FLT_EPSILON)
		{
			break;
		}
		AddAsk(pField, market_data->ask[i], market_data->ask_qty[i], 0);
	}

	m_msgQueue->Input_NoCopy(ResponseType::ResponseType_OnRtnDepthMarketData, m_msgQueue, m_pClass, 0, 0, pField, pField->Size, nullptr, 0, nullptr, 0);
}