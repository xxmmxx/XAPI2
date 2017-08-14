#pragma once

#include "../../include/ApiStruct.h"
#include "../../include/LTS_v2/SecurityFtdcMdApi.h"

#ifdef _WIN64
	#pragma comment(lib, "../../include/LTS_v2/win64/securitymduserapi.lib")
	#ifdef _DEBUG
	#pragma comment(lib, "../../lib/Queue_x64d.lib")
	#else
	#pragma comment(lib, "../../lib/Queue_x64.lib")
	#endif
#else
	#pragma comment(lib, "../../include/LTS_v2/win32/securitymduserapi.lib")
	#ifdef _DEBUG
	#pragma comment(lib, "../../lib/Queue_x86d.lib")
	#else
	#pragma comment(lib, "../../lib/Queue_x86.lib")
	#endif
#endif

#include <set>
#include <string>
#include <atomic>
#include <mutex>
#include <map>

using namespace std;

class CMsgQueue;

class CMdUserApi :
	public CSecurityFtdcMdSpi
{
	enum RequestType
	{
		E_Init,
		E_ReqUserLoginField,
	};

public:
	CMdUserApi(void);
	virtual ~CMdUserApi(void);

	void Register(void* pCallback, void* pClass);

	void Connect(const string& szPath,
		ServerInfoField* pServerInfo,
		UserInfoField* pUserInfo);
	void Disconnect();

	void Subscribe(const string& szInstrumentIDs, const string& szExchangeID);
	void Unsubscribe(const string& szInstrumentIDs, const string& szExchangeID);

	//void SubscribeQuote(const string& szInstrumentIDs, const string& szExchangeID);
	//void UnsubscribeQuote(const string& szInstrumentIDs, const string& szExchangeID);

private:
	friend void* __stdcall Query(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	void QueryInThread(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	int _Init();
	//��¼����
	void ReqUserLogin();
	int _ReqUserLogin(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	//��������
	void Subscribe(const set<string>& instrumentIDs, const string& szExchangeID);
	//void SubscribeQuote(const set<string>& instrumentIDs, const string& szExchangeID);
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData);

	//virtual void OnRspSubForQuoteRsp(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspUnSubForQuoteRsp(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRtnForQuoteRsp(CSecurityFtdcForQuoteRspField *pForQuoteRsp);

	//����Ƿ����
	bool IsErrorRspInfo(const char* szSource, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//��������Ϣ�͵���Ϣ����
	bool IsErrorRspInfo(CSecurityFtdcRspInfoField *pRspInfo);//���ͳ�����Ϣ

private:
	mutex						m_csMapInstrumentIDs;
	mutex						m_csMapQuoteInstrumentIDs;

	atomic<int>					m_lRequestID;			//����ID��ÿ������ǰ����
	
	map<string, set<string> >	m_mapInstrumentIDs;		//���ڶ��ĵĺ�Լ
	map<string, set<string> >	m_mapQuoteInstrumentIDs;		//���ڶ��ĵĺ�Լ
	CSecurityFtdcMdApi*			m_pApi;					//����API
	
	string						m_szPath;				//���������ļ���·��
	ServerInfoField				m_ServerInfo;
	UserInfoField				m_UserInfo;
	int							m_nSleep;

	CMsgQueue*					m_msgQueue;				//��Ϣ����ָ��
	CMsgQueue*					m_msgQueue_Query;
	void*						m_pClass;

	CMsgQueue*					m_remoteQueue;
};

