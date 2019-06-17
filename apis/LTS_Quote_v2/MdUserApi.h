#pragma once

#include "../../include/CrossPlatform.h"
#include "../../include/ApiStruct.h"
#include "../../include/LTS_v2/SecurityFtdcMdApi.h"

#ifndef USE_CMAKE
#ifdef _WIN64
	#pragma comment(lib, "../../include/LTS_v2/win64/securitymduserapi.lib")
#else
	#pragma comment(lib, "../../include/LTS_v2/win32/securitymduserapi.lib")
#endif
#endif

#include <set>
#include <string>
#include <atomic>
#include <mutex>
#include <map>

using namespace std;

class CMsgQueue;

class CSubscribeManager;
class CSyntheticConfig;
class CSyntheticManager;
class CSyntheticCalculateFactory;

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

private:
	friend void* __stdcall Query(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	void QueryInThread(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	int _Init();
	//��¼����
	void ReqUserLogin();
	int _ReqUserLogin(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	//��������
	void Subscribe(const set<string>& instrumentIDs, const string& szExchangeID);
	void Unsubscribe(const set<string>& instrumentIDs, const string& szExchangeID);

	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData);

	//����Ƿ����
	bool IsErrorRspInfo(const char* szSource, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//��������Ϣ�͵���Ϣ����
	bool IsErrorRspInfo(CSecurityFtdcRspInfoField *pRspInfo);//���ͳ�����Ϣ

private:
	mutex						m_csMapInstrumentIDs;
	mutex						m_csMapQuoteInstrumentIDs;

	atomic<int>					m_lRequestID;			//����ID��ÿ������ǰ����
	
	CSecurityFtdcMdApi*			m_pApi;					//����API
	
	string						m_szPath;				//���������ļ���·��
	ServerInfoField				m_ServerInfo;
	UserInfoField				m_UserInfo;
	int							m_nSleep;

	CMsgQueue*					m_msgQueue;				//��Ϣ����ָ��
	CMsgQueue*					m_msgQueue_Query;
	void*						m_pClass;

	CSubscribeManager*			m_pSubscribeManager;
	CSyntheticConfig*			m_pSyntheticConfig;
	CSyntheticManager*			m_pSyntheticManager;
	CSyntheticCalculateFactory*	m_pCalculateFactory;
};

