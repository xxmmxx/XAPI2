﻿// CPP_TEST.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#if defined WINDOWS || _WIN32
#include <Windows.h>
#endif
#include <time.h>

#include <string.h>
#include "../../include/XApiCpp.h"

#ifdef _WIN64
#pragma comment(lib, "../../lib/XAPI_CPP_x64.lib")
#else
#pragma comment(lib, "../../lib/XAPI_CPP_x86.lib")
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
				// 得到当前的时间
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
	virtual void OnRtnError(CXApi* pApi, ErrorField* pError){};

	virtual void OnRtnDepthMarketDataN(CXApi* pApi, DepthMarketDataNField* pMarketData)
	{
		printf("%s,%f,%d\r\n", pMarketData->Symbol, pMarketData->LastPrice, ++count);

		int size = sizeof(DepthField);
		char* pBid = ((char*)pMarketData) + sizeof(DepthMarketDataNField);
		int AskCount = (pMarketData->Size - sizeof(DepthMarketDataNField)) / size - pMarketData->BidCount;
		char* pAsk = ((char*)pMarketData) + sizeof(DepthMarketDataNField)+pMarketData->BidCount*size;
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

	virtual void OnRtnQuoteRequest(CXApi* pApi, QuoteRequestField* pQuoteRequest){};

	virtual void OnRspQryInstrument(CXApi* pApi, InstrumentField* pInstrument, int size1, bool bIsLast){};
	virtual void OnRspQryTradingAccount(CXApi* pApi, AccountField* pAccount, int size1, bool bIsLast){};
	virtual void OnRspQryInvestorPosition(CXApi* pApi, PositionField* pPosition, int size1, bool bIsLast)
	{
		printf("%s,%s\r\n", pPosition->InstrumentName, pPosition->InstrumentID);
	}
	virtual void OnRspQrySettlementInfo(CXApi* pApi, SettlementInfoField* pSettlementInfo, int size1, bool bIsLast){};
	virtual void OnRspQryInvestor(CXApi* pApi, InvestorField* pInvestor, int size1, bool bIsLast){};
	
	
	
	virtual void OnRtnOrder(CXApi* pApi, OrderField* pOrder)
	{
		printf("%d,%s\r\n", pOrder->RawErrorID,pOrder->Text);
	}
	virtual void OnRtnTrade(CXApi* pApi, TradeField* pTrade){};
	virtual void OnRtnQuote(CXApi* pApi, QuoteField* pQuote){};

	virtual void OnRspQryHistoricalTicks(CXApi* pApi, TickField* pTicks, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast){};
	virtual void OnRspQryHistoricalBars(CXApi* pApi, BarField* pBars, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast){};

	virtual bool OnFilterSubscribe(CXApi* pApi, ExchangeType exchange, int instrument_part1, int instrument_part2, int instrument_part3, char* pInstrument)
	{
		// 当数字为0时，只判断交易所
		// 当交易所为
		if (instrument_part1 == 0)
			// 只要上海与深圳，不处理三板
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

int main_111(int argc, char* argv[])
{
	CXSpiImpl* p = new CXSpiImpl();
#if defined WINDOWS || _WIN32
	char DLLPath1[250] = "C:\\Program Files\\SmartQuant Ltd\\OpenQuant 2014\\XAPI\\x86\\CTP\\CTP_Quote_x86.dll";
	char DLLPath2[250] = "C:\\Program Files\\SmartQuant Ltd\\OpenQuant 2014\\XAPI\\x86\\CTP\\CTP_Trade_x86.dll";
#else
    char DLLPath1[250] = "libQuantBox_CTP_Quote.so";
	char DLLPath2[250] = "libQuantBox_CTP_Trade.so";
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

	CXApi* pApi1 = CXApi::CreateApi(DLLPath1);
	CXApi* pApi2 = CXApi::CreateApi(DLLPath2);
	if (pApi1 && pApi2)
	{
		if (!pApi1->Init())
		{
			printf("%s\r\n", pApi1->GetLastError());
			pApi1->Disconnect();
			return -1;
		}

		if (!pApi2->Init())
		{
			printf("%s\r\n", pApi2->GetLastError());
			pApi2->Disconnect();
			return -1;
		}

		pApi1->RegisterSpi(p);
		pApi2->RegisterSpi(p);

#if defined WINDOWS
        pApi1->Connect("D:\\", &m_ServerInfo1, &m_UserInfo, 1);
		pApi2->Connect("D:\\", &m_ServerInfo2, &m_UserInfo, 1);
#else
		pApi1->Connect("./", &m_ServerInfo1, &m_UserInfo, 1);
		pApi2->Connect("./", &m_ServerInfo2, &m_UserInfo, 1);
#endif

		getchar();

		pApi1->Subscribe("IF1504", "");

		OrderIDType Out = { 0 };
		OrderField* pOrder = new OrderField();
		memset(pOrder, 0, sizeof(OrderField));
		strcpy(pOrder->InstrumentID, "IF1504");
		pOrder->OpenClose = OpenCloseType::OpenCloseType_Open;
		pOrder->HedgeFlag = HedgeFlagType::HedgeFlagType_Speculation;
		pOrder->Price = 4000;
		pOrder->Qty = 1;
		pOrder->Type = OrderType::OrderType_Limit;
		pOrder->Side = OrderSide::OrderSide_Buy;

		pApi2->SendOrder(pOrder, 1, Out);

		printf("%s\r\n", Out);

		getchar();

		OrderIDType Out2 = { 0 };
		pApi2->CancelOrder(&Out,1,Out2);

		getchar();

		pApi1->Disconnect();
		pApi2->Disconnect();
	}

	return 0;
}



int main_2(int argc, char* argv[])
{
	CXSpiImpl* p = new CXSpiImpl();

	char DLLPath1[250] = "TongShi_Quote\\TongShi_Quote_x86.dll";

	ServerInfoField				m_ServerInfo1 = { 0 };
	UserInfoField				m_UserInfo = { 0 };

	strcpy(m_ServerInfo1.Address, "D:\\Scengine\\Stock.dll");
	//strcpy(m_ServerInfo1.Address, "D:\\YjStock\\Stock.dll");

	CXApi* pApi1 = CXApi::CreateApi(DLLPath1);
	if (pApi1)
	{
		if (!pApi1->Init())
		{
			printf("%s\r\n", pApi1->GetLastError());
			pApi1->Disconnect();
			return -1;
		}

		pApi1->RegisterSpi(p);
		pApi1->Connect("D:\\", &m_ServerInfo1, &m_UserInfo, 1);
		

		getchar();

		printf("退出");
		
		pApi1->Disconnect();

		printf("退出成功");
	}

	return 0;
}


int main_4(int argc, char* argv[])
{
	CXSpiImpl* p = new CXSpiImpl();
#if defined WINDOWS || _WIN32
	char DLLPath1[250] = "CTP_Quote\\CTP_Quote_x86.dll";
#else
	char DLLPath1[250] = "libQuantBox_CTP_Quote.so";
#endif

	ServerInfoField				m_ServerInfo1 = { 0 };
	UserInfoField				m_UserInfo = { 0 };

	strcpy(m_ServerInfo1.BrokerID, "0272");
	strcpy(m_ServerInfo1.Address, "tcp://180.168.146.181:10210");
	m_ServerInfo1.TopicId = 100;// femas这个地方一定不能省
	strcpy(m_ServerInfo1.ExtInfoChar128, "tcp://*:5555");//这需要对Femas进行改造
	
	strcpy(m_UserInfo.UserID, "00049");
	strcpy(m_UserInfo.Password, "123456");

	CXApi* pApi1 = CXApi::CreateApi(DLLPath1);
	if (pApi1)
	{
		if (!pApi1->Init())
		{
			printf("%s\r\n", pApi1->GetLastError());
			pApi1->Disconnect();
			return -1;
		}

		//p->m_pApi = pApi1;
		pApi1->RegisterSpi(p);

#if defined WINDOWS || _WIN32
		pApi1->Connect("D:\\", &m_ServerInfo1, &m_UserInfo, 1);
#else
		pApi1->Connect("./", &m_ServerInfo1, &m_UserInfo, 1);
#endif

		do
		{
			int c = getchar();
			if (c == 'q')
				break;
		} while (true);

		pApi1->Disconnect();
	}

	return 0;
}

int main_122(int argc, char* argv[])
{
	CXSpiImpl* p = new CXSpiImpl();
#if defined WINDOWS || _WIN32
	char DLLPath1[250] = "C:\\Program Files\\SmartQuant Ltd\\OpenQuant 2014\\XAPI\\x86\\Tdx\\Tdx_Trade_x86.dl";
#else
	char DLLPath1[250] = "libQuantBox_CTP_Quote.so";
#endif

	ServerInfoField				m_ServerInfo1 = { 0 };
	UserInfoField				m_UserInfo = { 0 };

	strcpy(m_ServerInfo1.Address, "D:\\new_hbzq_qq\\Login.lua");
	strcpy(m_ServerInfo1.ExtInfoChar128, "D:\\new_hbzq_qq\\");

	strcpy(m_UserInfo.UserID, "05000000");
	strcpy(m_UserInfo.Password, "4");

	CXApi* pApi1 = CXApi::CreateApi(DLLPath1);
	if (pApi1)
	{
		if (!pApi1->Init())
		{
			printf("%s\r\n", pApi1->GetLastError());
			pApi1->Disconnect();
			return -1;
		}

		pApi1->RegisterSpi(p);

#if defined WINDOWS || _WIN32
		pApi1->Connect("D:\\", &m_ServerInfo1, &m_UserInfo, 1);
#else
		pApi1->Connect("./", &m_ServerInfo1, &m_UserInfo, 1);
#endif

		getchar();

		pApi1->Disconnect();

		getchar();

		do
		{
			int c = getchar();
			if (c == 'q')
				break;
		} while (true);

		pApi1->Disconnect();
	}

	return 0;
}

int main(int argc, char* argv[])
{
	CXSpiImpl* p = new CXSpiImpl();
#if defined WINDOWS || _WIN32
	char DLLPath1[250] = "C:\\Program Files\\SmartQuant Ltd\\OpenQuant 2014\\XAPI\\x86\\CTP\\CTP_Quote_x86.dll";
	char DLLPath2[250] = "C:\\Program Files\\SmartQuant Ltd\\OpenQuant 2014\\XAPI\\x86\\CTP\\CTP_Trade_x86.dll";
#else
	char DLLPath1[250] = "libQuantBox_CTP_Quote.so";
	char DLLPath2[250] = "libQuantBox_CTP_Trade.so";
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
		pApi1->Connect("D:\\", &m_ServerInfo1, &m_UserInfo, 1);
		printf("已经执行完Connect\n");

		pApi2->RegisterSpi(p);
		pApi2->Connect("D:\\", &m_ServerInfo2, &m_UserInfo, 1);
		printf("已经执行完Connect\n");
		getchar();

		pApi1->Disconnect();
		pApi2->Disconnect();
		printf("已经执行完Disconnect");
		//getchar();
	}

	return 0;
}
