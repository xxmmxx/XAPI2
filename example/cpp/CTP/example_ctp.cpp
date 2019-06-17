#if defined WINDOWS || _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "../../../include/XApiCpp.h"

#ifndef USE_CMAKE
#ifdef _WIN64
#pragma comment(lib, "../../lib/XAPI_CPP_x64.lib")
#else
#pragma comment(lib, "../../lib/XAPI_CPP_x86.lib")
#endif
#endif

class CXSpiImpl :public CXSpi
{
public:
	CXSpiImpl() :CXSpi()
	{
		count = 0;
	}

	virtual void OnConnectionStatus(CXApi* pApi, ConnectionStatus status, RspUserLoginField* pUserLogin, int size1)
	{
		printf("%d\r\n", status);
		if (pUserLogin)
		{
			printf("%s\r\n", pUserLogin->Text);
		}
		if (status == ConnectionStatus::ConnectionStatus_Done)
		{
			if ((pApi->GetApiTypes() & ApiType::ApiType_MarketData) == ApiType::ApiType_MarketData)
			{
				// �õ���ǰ��ʱ��
				time_t rawtime;
				struct tm * timeinfo;
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				int mon = timeinfo->tm_mon;
				char buf[64] = { 0 };
				for (int i = 0; i < 12; ++i)
				{
					int x = mon + i;
					int a = x / 12;
					int b = x % 12;
					sprintf(buf, "IF%d%02d", (1900 + timeinfo->tm_year + a) % 100, b + 1);
					pApi->Subscribe(buf, "");
					sprintf(buf, "TF%d%02d", (1900 + timeinfo->tm_year + a) % 100, b + 1);
					pApi->Subscribe(buf, "");
				}
			}
		}
	}
	virtual void OnRtnError(CXApi* pApi, ErrorField* pError) {};

	virtual void OnRtnDepthMarketDataN(CXApi* pApi, DepthMarketDataNField* pMarketData)
	{
		printf("%s,%f,%d\r\n", pMarketData->Symbol, pMarketData->LastPrice, ++count);

		int size = sizeof(DepthField);
		char* pBid = ((char*)pMarketData) + sizeof(DepthMarketDataNField);
		int AskCount = (pMarketData->Size - sizeof(DepthMarketDataNField)) / size - pMarketData->BidCount;
		char* pAsk = ((char*)pMarketData) + sizeof(DepthMarketDataNField) + pMarketData->BidCount*size;
		for (int i = 0; i < pMarketData->BidCount; ++i)
		{
			DepthField* pDF = (DepthField*)(pBid + i * size);
			printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
		}
		for (int i = 0; i < AskCount; ++i)
		{
			DepthField* pDF = (DepthField*)(pAsk + i * size);
			printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
		}
	}

	virtual void OnRtnQuoteRequest(CXApi* pApi, QuoteRequestField* pQuoteRequest) {};

	virtual void OnRspQryInstrument(CXApi* pApi, InstrumentField* pInstrument, int size1, bool bIsLast) {};
	virtual void OnRspQryTradingAccount(CXApi* pApi, AccountField* pAccount, int size1, bool bIsLast) {};
	virtual void OnRspQryInvestorPosition(CXApi* pApi, PositionField* pPosition, int size1, bool bIsLast)
	{
		printf("%s,%s\r\n", pPosition->InstrumentName, pPosition->InstrumentID);
	}
	virtual void OnRspQrySettlementInfo(CXApi* pApi, SettlementInfoField* pSettlementInfo, int size1, bool bIsLast) {};
	virtual void OnRspQryInvestor(CXApi* pApi, InvestorField* pInvestor, int size1, bool bIsLast) {};



	virtual void OnRtnOrder(CXApi* pApi, OrderField* pOrder)
	{
		printf("%d,%s\r\n", pOrder->RawErrorID, pOrder->Text);
	}
	virtual void OnRtnTrade(CXApi* pApi, TradeField* pTrade) {};
	virtual void OnRtnQuote(CXApi* pApi, QuoteField* pQuote) {};

	virtual void OnRspQryHistoricalTicks(CXApi* pApi, TickField* pTicks, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast) {};
	virtual void OnRspQryHistoricalBars(CXApi* pApi, BarField* pBars, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast) {};

	virtual bool OnFilterSubscribe(CXApi* pApi, ExchangeType exchange, int instrument_part1, int instrument_part2, int instrument_part3, char* pInstrument)
	{
		// ������Ϊ0ʱ��ֻ�жϽ�����
		// ��������Ϊ
		if (instrument_part1 == 0)
			// ֻҪ�Ϻ������ڣ�����������
			return exchange != ExchangeType::ExchangeType_NEEQ;

		//type = ExchangeType::SZSE;
		//double1 = 399300;

		int prefix1 = instrument_part1 / 100000;
		int prefix3 = instrument_part1 / 1000;
		switch (exchange)
		{
		case ExchangeType::ExchangeType_SSE:
			return (prefix1 == 6);
		case ExchangeType::ExchangeType_SZSE:
			return (prefix1 == 0) || (prefix3 == 300);
		default:
			break;
		}

		return true;
	}
public:
	//CXApi* m_pApi;
	int count;
};

int main(int argc, char* argv[])
{
	CXSpiImpl* p = new CXSpiImpl();
#if _WIN32
#if _WIN64
	char DLLPath1[250] = "CTP_Quote_x64.dll";
	char DLLPath2[250] = "CTP_Trade_x64.dll";
#else
	char DLLPath1[250] = "CTP_Quote_x86.dll";
	char DLLPath2[250] = "CTP_Trade_x86.dll";
#endif
#else
	char DLLPath1[1024] = "libCTP_Quote.so";
	char DLLPath2[1024] = "libCTP_Trade.so";
//	char dir_path[512] = {0};
//	char DLLPath1[1024] = {0};
//	char DLLPath2[1024] = {0};
//
//	if (getcwd(dir_path, sizeof(dir_path)) == NULL)
//	{
//		return 1;
//	}
//	snprintf(DLLPath1, sizeof(DLLPath1), "%s/libCTP_Quote.so", dir_path);
//	snprintf(DLLPath2, sizeof(DLLPath2), "%s/libCTP_Trade.so", dir_path);
#endif

	ServerInfoField				m_ServerInfo1 = { 0 };
	ServerInfoField				m_ServerInfo2 = { 0 };
	UserInfoField				m_UserInfo = { 0 };

	strcpy(m_ServerInfo1.BrokerID, "4040");
	strcpy(m_ServerInfo1.Address, "tcp://yhzx-front1.yhqh.com:41213;tcp://yhzx-front3.yhqh.com:41213");

	strcpy(m_ServerInfo2.BrokerID, "4040");
	strcpy(m_ServerInfo2.Address, "tcp://yhzx-front1.yhqh.com:51205;tcp://yhzx-front3.yhqh.com:52205");

	strcpy(m_UserInfo.UserID, "00000025");
	strcpy(m_UserInfo.Password, "123456");



	while (true)
	{
		CXApi* pApi1 = CXApi::CreateApi(DLLPath1);
		CXApi* pApi2 = CXApi::CreateApi(DLLPath2);

		if (!pApi1->Init())
		{
			printf("%s\r\n", pApi1->GetLastError());
			pApi1->Disconnect();
			break;
		}

		if (!pApi2->Init())
		{
			printf("%s\r\n", pApi2->GetLastError());
			pApi2->Disconnect();
			break;
		}

		pApi1->RegisterSpi(p);
		pApi1->Connect("./", &m_ServerInfo1, &m_UserInfo, 1);
		printf("�Ѿ�ִ����Connect\n");

		pApi2->RegisterSpi(p);
		pApi2->Connect("./", &m_ServerInfo2, &m_UserInfo, 1);
		printf("�Ѿ�ִ����Connect\n");
		getchar();

		pApi1->Disconnect();
		pApi2->Disconnect();
		printf("�Ѿ�ִ����Disconnect");
		//getchar();
	}

	return 0;
}
