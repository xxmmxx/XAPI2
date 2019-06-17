#ifndef XTP_TRADE_API_H_
#define XTP_TRADE_API_H_

#include <set>
#include <list>
#include <map>
#include <string>
#include <mutex>
#include <atomic>
#include <thread>
#include <unordered_map>

#include "../../include/ApiStruct.h"
#include "../../include/QueueEnum.h"

#include "Include.h"

using namespace std;

class CMsgQueue;

class CTraderApi : public XTP::API::TraderSpi
{
	//�������ݰ�����
	enum RequestType
	{
		E_Init = 100,
		E_Disconnect,
		E_ReqAuthenticateField,
		E_ReqUserLoginField,
		E_UserLogoutField,
		E_SettlementInfoConfirmField,

		E_QryInvestorField,

		E_QryInstrumentField,

		E_InputOrderField,
		E_InputOrderActionField,
		E_InputQuoteField,
		E_InputQuoteActionField,
		E_ParkedOrderField,
		E_QryTradingAccountField,
		E_QryInvestorPositionField,
		E_QryInvestorPositionDetailField,
		E_QryInstrumentCommissionRateField,
		E_QryInstrumentMarginRateField,
		E_QryDepthMarketDataField,
		E_QrySettlementInfoField,
		E_QryOrderField,
		E_QryTradeField,
		E_QryQuoteField,
	};

public:
	CTraderApi(void);
	virtual ~CTraderApi(void);

	void Register(void* pCallback, void* pClass);

	void Connect(const string& szPath,
		ServerInfoField* pServerInfo,
		UserInfoField* pUserInfo);
	void Disconnect();

	void ReqQuery(QueryType type, ReqQueryField* pQuery);

	char* ReqOrderInsert(
		OrderField* pOrder,
		int count,
		char* pszLocalIDBuf);
	char* ReqOrderAction(OrderIDType* szIds, int count, char* pzsRtn);
	char* ReqOrderAction(XTPOrderInfo *pOrder, int count, char* pzsRtn);

private:
	friend void* __stdcall Query(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	void QueryInThread(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	void Clear();

	int _Init();
	void _Disconnect(bool IsInQueue);
	void _DisconnectInThread();

	void ReqUserLogin();
	int _ReqUserLogin(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	void ReqUserLogout();
	int _ReqUserLogout(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	// ��ѯ�ʲ�
	int _ReqQryTradingAccount(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	// ��ѯ�ֲ�
	int _ReqQryInvestorPosition(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	// ��ѯ����
	int _ReqQryOrder(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	// ��ѯ�ɽ���
	int _ReqQryTrade(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	void _OnOrderInfo(XTPOrderInfo *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);
	void _OnTradeInfo(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	void OnTrade(TradeField *pTrade);

	void GetPositionID(PositionIDType position_id, const ExchangeIDType exchange_id, const SymbolType symbol, PositionSide dir);

	void CheckError(int ret, const char *fn);

public:

	// ���ͻ��˵�ĳ�������뽻�׺�̨ͨ�����ӶϿ�ʱ
	virtual void OnDisconnected(uint64_t session_id, int reason) override;

	// ����Ӧ��
	virtual void OnError(XTPRI *error_info) override;

	// ����֪ͨ
	virtual void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id) override;

	// �ɽ�֪ͨ
	virtual void OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id) override;

	// �����ѯ������Ӧ
	virtual void OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

	// �����ѯ�ɽ���Ӧ
	virtual void OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

	// �����ѯͶ���ֲ߳���Ӧ
	virtual void OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

	// �����ѯ�ʽ��˻���Ӧ
	virtual void OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) override;

private:
	XTP::API::TraderApi* m_pApi;			// ����API
	std::atomic<int> m_lRequestID;			// ����ID,�ñ�������

	int	m_nSleep;

	unordered_map<string, OrderField*>				m_id_platform_order;
	unordered_map<string, XTPOrderInfo*>			m_id_api_order;
	unordered_map<string, string>					m_sysId_orderId;

#ifdef HAS_Quote
	unordered_map<string, QuoteField*>				m_id_platform_quote;
	unordered_map<string, CThostFtdcQuoteField*>		m_id_api_quote;
	unordered_map<string, string>					m_sysId_quoteId;
#endif // HAS_Quote

	unordered_map<string, PositionField*>			m_id_platform_position;
	unordered_map<string, XTPQueryStkPositionRsp*>	m_id_api_position;

	string				m_szPath;
	ServerInfoField		m_ServerInfo;
	UserInfoField		m_UserInfo;

	CMsgQueue*	m_msgQueue;				//��Ϣ����ָ��
	CMsgQueue*	m_msgQueue_Query;
	void*		m_pClass;

	uint64_t m_session_id;
};

#endif