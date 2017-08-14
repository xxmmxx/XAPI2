#pragma once

#include "../../include/LTS_v2/SecurityFtdcQueryApi.h"
#include "../../include/ApiStruct.h"
#include "../../include/QueueEnum.h"

#ifdef _WIN64
	#pragma comment(lib, "../../include/LTS_v2/win64/securityqueryapi.lib")
	#ifdef _DEBUG
	#pragma comment(lib, "../../lib/Queue_x64d.lib")
	#else
	#pragma comment(lib, "../../lib/Queue_x64.lib")
	#endif
#else
	#pragma comment(lib, "../../include/LTS_v2/win32/securityqueryapi.lib")
	#ifdef _DEBUG
	#pragma comment(lib, "../../lib/Queue_x86d.lib")
	#else
	#pragma comment(lib, "../../lib/Queue_x86.lib")
	#endif
#endif

#include <set>
#include <list>
#include <map>
#include <string>
#include <mutex>
#include <atomic>
#include <thread>
#include <unordered_map>

using namespace std;

class CMsgQueue;

class CQueryApi :
	public CSecurityFtdcQuerySpi
{
	//�������ݰ�����
	enum RequestType
	{
		E_Init = 100,

		E_ReqAuthenticateField,
		E_ReqUserLoginField,
		E_SettlementInfoConfirmField,
		
		E_QryInvestorField,

		E_QryInstrumentField,
		E_InputOrderField,
		E_InputOrderActionField,
		E_InputQuoteField,
		E_InputQuoteActionField,
		E_QryTradingAccountField,
		E_QryInvestorPositionField,
		E_QryInvestorPositionDetailField,
		E_QryInstrumentCommissionRateField,
		E_QryInstrumentMarginRateField,
		E_QryDepthMarketDataField,
		E_QrySettlementInfoField,
		E_QryOrderField,
		E_QryTradeField,

		E_AuthRandCodeField,
	};

public:
	CQueryApi(void);
	virtual ~CQueryApi(void);

	void Register(void* pCallback, void* pClass);

	void Connect(const string& szPath,
		ServerInfoField* pServerInfo,
		UserInfoField* pUserInfo);
	void Disconnect();

	//int ReqOrderInsert(
	//	OrderField* pOrder,
	//	int count,
	//	OrderIDType* pInOut);

	//int ReqOrderAction(OrderIDType* szIds, int count, OrderIDType* pOutput);
	//int ReqOrderAction(CSecurityFtdcOrderField *pOrder, int count, OrderIDType* pOutput);

	//int ReqQuoteInsert(
	//	int QuoteRef,
	//	OrderField* pOrderAsk,
	//	OrderField* pOrderBid);

	//int ReqQuoteAction(CSecurityFtdcQuoteField *pQuote);
	//int ReqQuoteAction(const string& szId);

	void ReqQuery(QueryType type, ReqQueryField* pQuery);

	//void ReqQryTradingAccount();
	//void ReqQryInvestorPosition(const string& szInstrumentId, const string& szExchange);
	//void ReqQryInvestorPositionDetail(const string& szInstrumentId);
	//void ReqQryInstrument(const string& szInstrumentId, const string& szExchange);
	//void ReqQryInstrumentCommissionRate(const string& szInstrumentId);
	//void ReqQryInstrumentMarginRate(const string& szInstrumentId, TSecurityFtdcHedgeFlagType HedgeFlag = SECURITY_FTDC_HF_Speculation);
	//void ReqQryDepthMarketData(const string& szInstrumentId);
	//void ReqQrySettlementInfo(const string& szTradingDay);

	//void ReqQryOrder();
	//void ReqQryTrade();

	//void ReqQryInvestor();

private:
	friend void* __stdcall Query_Q(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	void QueryInThread(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	void Clear();

	int _Init();

	void ReqUserLogin(TSecurityFtdcAuthCodeType	RandCode);
	int _ReqUserLogin(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	void ReqFetchAuthRandCode();
	int _ReqFetchAuthRandCode(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	int _ReqQryInstrument(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	int _ReqQryTradingAccount(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	int _ReqQryInvestorPosition(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	int _ReqQryInvestor(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	int _ReqQryOrder(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	int _ReqQryTrade(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	//int _ReqQryQuote(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	void OnOrder(CSecurityFtdcOrderField *pOrder, int nRequestID, bool bIsLast);
	void OnTrade(CSecurityFtdcTradeField *pTrade, int nRequestID, bool bIsLast);
	
	void OnTrade(TradeField *pTrade);

	//����Ƿ����
	bool IsErrorRspInfo(const char* szSource, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//����Ϣ���������Ϣ
	bool IsErrorRspInfo(CSecurityFtdcRspInfoField *pRspInfo);//�������Ϣ

	//����
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);

	//��֤
	virtual void OnRspFetchAuthRandCode(CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspAuthenticate(CSecurityFtdcRspAuthenticateField *pRspAuthenticateField, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspSettlementInfoConfirm(CSecurityFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInvestor(CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	////�µ�
	//virtual void OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo);

	////����
	//virtual void OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo);

	//�����ر�
	virtual void OnRspQryOrder(CSecurityFtdcOrderField *pOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRtnOrder(CSecurityFtdcOrderField *pOrder);

	//�ɽ��ر�
	virtual void OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRtnTrade(CSecurityFtdcTradeField *pTrade);

	//����¼��
	//virtual void OnRspQuoteInsert(CSecurityFtdcInputQuoteField *pInputQuote, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnErrRtnQuoteInsert(CSecurityFtdcInputQuoteField *pInputQuote, CSecurityFtdcRspInfoField *pRspInfo);
	//virtual void OnRtnQuote(CSecurityFtdcQuoteField *pQuote);

	//���۳���
	//virtual void OnRspQuoteAction(CSecurityFtdcInputQuoteActionField *pInputQuoteAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnErrRtnQuoteAction(CSecurityFtdcQuoteActionField *pQuoteAction, CSecurityFtdcRspInfoField *pRspInfo);

	//��λ
	virtual void OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspQryInvestorPositionDetail(CSecurityFtdcInvestorPositionDetailField *pInvestorPositionDetail, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspQryInvestorPositionCombineDetail(CSecurityFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	//�ʽ�
	virtual void OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//��Լ��������
	virtual void OnRspQryInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspQryInstrumentMarginRate(CSecurityFtdcInstrumentMarginRateField *pInstrumentMarginRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspQryInstrumentCommissionRate(CSecurityFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//��ѯ������Ӧ
	//virtual void OnRspQryDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//�����ѯͶ���߽�������Ӧ
	//virtual void OnRspQrySettlementInfo(CSecurityFtdcSettlementInfoField *pSettlementInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//����
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRtnInstrumentStatus(CSecurityFtdcInstrumentStatusField *pInstrumentStatus);

private:
	atomic<int>					m_lRequestID;			//����ID,�ñ�������

	CSecurityFtdcRspUserLoginField m_RspUserLogin;			//���صĵ�¼�ɹ���Ӧ��Ŀǰ���ô��ڳ�Ա���б�����������
	CSecurityFtdcInvestorField	m_Investor;

	OrderIDType					m_orderInsert_Id;
	OrderIDType					m_orderAction_Id;

	mutex						m_csOrderRef;
	int							m_nMaxOrderRef;			//�������ã��������ֱ�������������

	CSecurityFtdcQueryApi*		m_pApi;					//����API
	
	string						m_szPath;				//���������ļ���·��
	ServerInfoField				m_ServerInfo;
	UserInfoField				m_UserInfo;
	int							m_nSleep;

	unordered_map<string, OrderField*>				m_id_platform_order;
	unordered_map<string, CSecurityFtdcOrderField*>		m_id_api_order;
	unordered_map<string, string>					m_sysId_orderId;

	unordered_map<string, PositionField*>			m_id_platform_position;

	CMsgQueue*					m_msgQueue;				//��Ϣ����ָ��
	CMsgQueue*					m_msgQueue_Query;
	void*						m_pClass;
};

