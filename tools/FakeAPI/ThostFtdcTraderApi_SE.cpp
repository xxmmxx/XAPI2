﻿#include "stdafx.h"

#define ISLIB
#define LIB_TRADER_API_EXPORT

#include <stdio.h>
#include "../../include/CTP_SE/ThostFtdcTraderApi.h"

class MyClassCTP_SE :public CThostFtdcTraderApi
{
public:
	///创建TraderApi
	///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
	///@return 创建出的UserApi
	static CThostFtdcTraderApi *CreateFtdcTraderApi(const char *pszFlowPath = "");

	///获取API的版本信息
	///@retrun 获取到的版本号
	static const char *GetApiVersion() { return 0; }

	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	virtual void Release() {}

	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	virtual void Init()
	{
		m_pSpi->OnFrontConnected();
	}

	///等待接口线程结束运行
	///@return 线程退出代码
	virtual int Join() { return 0; }

	///获取当前交易日
	///@retrun 获取到的交易日
	///@remark 只有登录成功后,才能得到正确的交易日
	virtual const char *GetTradingDay() { return 0; }

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(char *pszFrontAddress)
	{
		char buf[512] = { 0 };
		sprintf(buf, "RegisterFront\n\n"
			"pszFrontAddress:%s\n"
			"已经复制到剪贴板",
			pszFrontAddress);

		ShowMessageBox(buf);
	}

	///注册名字服务器网络地址
	///@param pszNsAddress：名字服务器网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:12001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”12001”代表服务器端口号。
	///@remark RegisterNameServer优先于RegisterFront
	virtual void RegisterNameServer(char *pszNsAddress) {}

	///注册名字服务器用户信息
	///@param pFensUserInfo：用户信息。
	virtual void RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo) {}

	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CThostFtdcTraderSpi *pSpi)
	{
		m_pSpi = pSpi;
	}

	///订阅私有流。
	///@param nResumeType 私有流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后私有流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	virtual void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) {}

	///订阅公共流。
	///@param nResumeType 公共流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	virtual void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) {}

	///客户端认证请求
	virtual int ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID)
	{
		char buf[512] = { 0 };
		sprintf(buf, "ReqAuthenticate\n\n"
			"BrokerID:%s\n"
			"UserProductInfo:%s\n"
			"AuthCode:%s\n"
			"AppID:%s\n"
			"已经复制到剪贴板",
			pReqAuthenticateField->BrokerID,
			pReqAuthenticateField->UserProductInfo,
			pReqAuthenticateField->AuthCode,
			pReqAuthenticateField->AppID);

		ShowMessageBox(buf);

		return 0;
	}

	///注册用户终端信息，用于中继服务器多连接模式
///需要在终端认证成功后，用户登录前调用该接口
	virtual int RegisterUserSystemInfo(CThostFtdcUserSystemInfoField *pUserSystemInfo)
	{
		char buf[512] = { 0 };
		sprintf(buf, "RegisterUserSystemInfo\n\n"
			"BrokerID:%s\n"
			"UserID:%s\n"
			"ClientSystemInfoLen:%d\n"
			"ClientSystemInfo:%s\n"
			"ClientPublicIP:%s\n"
			"ClientIPPort:%d\n"
			"ClientLoginTime:%s\n"
			"ClientAppID:%s\n"
			"已经复制到剪贴板",
			pUserSystemInfo->BrokerID,
			pUserSystemInfo->UserID,
			pUserSystemInfo->ClientSystemInfoLen,
			pUserSystemInfo->ClientSystemInfo,
			pUserSystemInfo->ClientPublicIP,
			pUserSystemInfo->ClientIPPort,
			pUserSystemInfo->ClientLoginTime,
			pUserSystemInfo->ClientAppID
		);

		ShowMessageBox(buf);

		return 0;
	}

	///上报用户终端信息，用于中继服务器操作员登录模式
	///操作员登录后，可以多次调用该接口上报客户信息
	virtual int SubmitUserSystemInfo(CThostFtdcUserSystemInfoField *pUserSystemInfo)
	{ 
		char buf[512] = { 0 };
		sprintf(buf, "SubmitUserSystemInfo\n\n"
			"BrokerID:%s\n"
			"UserID:%s\n"
			"ClientSystemInfoLen:%d\n"
			"ClientSystemInfo:%s\n"
			"ClientPublicIP:%s\n"
			"ClientIPPort:%d\n"
			"ClientLoginTime:%s\n"
			"ClientAppID:%s\n"
			"已经复制到剪贴板",
			pUserSystemInfo->BrokerID,
			pUserSystemInfo->UserID,
			pUserSystemInfo->ClientSystemInfoLen,
			pUserSystemInfo->ClientSystemInfo,
			pUserSystemInfo->ClientPublicIP,
			pUserSystemInfo->ClientIPPort,
			pUserSystemInfo->ClientLoginTime,
			pUserSystemInfo->ClientAppID
		);

		ShowMessageBox(buf);

		return 0;
	}

	///用户登录请求
	virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
	{
		char buf[512] = { 0 };
		sprintf(buf, "ReqUserLogin\n\n"
			"BrokerID:%s\n"
			"UserID:%s\n"
			"Password:%s\n"
			"UserProductInfo:%s\n"
			"InterfaceProductInfo:%s\n"
			"ProtocolInfo:%s\n"
			"MacAddress:%s\n"
			"OneTimePassword:%s\n"
			"ClientIPAddress:%s\n"
			"LoginRemark:%s\n"
			"ClientIPPort:%d\n"
			"已经复制到剪贴板",
			pReqUserLoginField->BrokerID,
			pReqUserLoginField->UserID,
			pReqUserLoginField->Password,
			pReqUserLoginField->UserProductInfo,
			pReqUserLoginField->InterfaceProductInfo,
			pReqUserLoginField->ProtocolInfo,
			pReqUserLoginField->MacAddress,
			pReqUserLoginField->OneTimePassword,
			pReqUserLoginField->ClientIPAddress,
			pReqUserLoginField->LoginRemark,
			pReqUserLoginField->ClientIPPort
		);

		ShowMessageBox(buf);

		return 0;
	}

	///登出请求
	virtual int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) { return 0; }

	///用户口令更新请求
	virtual int ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) { return 0; }

	///资金账户口令更新请求
	virtual int ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) { return 0; }

	///登录请求2
	virtual int ReqUserLogin2(CThostFtdcReqUserLoginField *pReqUserLogin, int nRequestID)
	{
		char buf[512] = { 0 };
		sprintf(buf, "ReqUserLogin2\n\n"
			"BrokerID:%s\n"
			"UserID:%s\n"
			"Password:%s\n"
			"UserProductInfo:%s\n"
			"InterfaceProductInfo:%s\n"
			"ProtocolInfo:%s\n"
			"MacAddress:%s\n"
			"OneTimePassword:%s\n"
			"ClientIPAddress:%s\n"
			"LoginRemark:%s\n"
			"ClientIPPort:%d\n"
			"已经复制到剪贴板",
			pReqUserLogin->BrokerID,
			pReqUserLogin->UserID,
			pReqUserLogin->Password,
			pReqUserLogin->UserProductInfo,
			pReqUserLogin->InterfaceProductInfo,
			pReqUserLogin->ProtocolInfo,
			pReqUserLogin->MacAddress,
			pReqUserLogin->OneTimePassword,
			pReqUserLogin->ClientIPAddress,
			pReqUserLogin->LoginRemark,
			pReqUserLogin->ClientIPPort
		);

		ShowMessageBox(buf);

		return 0;
	}

	///用户口令更新请求2
	virtual int ReqUserPasswordUpdate2(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) { return 0; }

	///查询用户当前支持的认证模式
	virtual int ReqUserAuthMethod(CThostFtdcReqUserAuthMethodField *pReqUserAuthMethod, int nRequestID) { return 0; }

	///用户发出获取图形验证码请求
	virtual int ReqGenUserCaptcha(CThostFtdcReqGenUserCaptchaField *pReqGenUserCaptcha, int nRequestID) { return 0; }

	///用户发出获取短信验证码请求
	virtual int ReqGenUserText(CThostFtdcReqGenUserTextField *pReqGenUserText, int nRequestID) { return 0; }

	///用户发出带有图片验证码的登陆请求
	virtual int ReqUserLoginWithCaptcha(CThostFtdcReqUserLoginWithCaptchaField *pReqUserLoginWithCaptcha, int nRequestID) { return 0; }

	///用户发出带有短信验证码的登陆请求
	virtual int ReqUserLoginWithText(CThostFtdcReqUserLoginWithTextField *pReqUserLoginWithText, int nRequestID) { return 0; }

	///用户发出带有动态口令的登陆请求
	virtual int ReqUserLoginWithOTP(CThostFtdcReqUserLoginWithOTPField *pReqUserLoginWithOTP, int nRequestID) { return 0; }

	///报单录入请求
	virtual int ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder, int nRequestID) { return 0; }

	///预埋单录入请求
	virtual int ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID) { return 0; }

	///预埋撤单录入请求
	virtual int ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID) { return 0; }

	///报单操作请求
	virtual int ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID) { return 0; }

	///查询最大报单数量请求
	virtual int ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID) { return 0; }

	///投资者结算结果确认
	virtual int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID) { return 0; }

	///请求删除预埋单
	virtual int ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID) { return 0; }

	///请求删除预埋撤单
	virtual int ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID) { return 0; }

	///执行宣告录入请求
	virtual int ReqExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, int nRequestID) { return 0; }

	///执行宣告操作请求
	virtual int ReqExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, int nRequestID) { return 0; }

	///询价录入请求
	virtual int ReqForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, int nRequestID) { return 0; }

	///报价录入请求
	virtual int ReqQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, int nRequestID) { return 0; }

	///报价操作请求
	virtual int ReqQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, int nRequestID) { return 0; }

	///批量报单操作请求
	virtual int ReqBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, int nRequestID) { return 0; }

	///期权自对冲录入请求
	virtual int ReqOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, int nRequestID) { return 0; }

	///期权自对冲操作请求
	virtual int ReqOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField *pInputOptionSelfCloseAction, int nRequestID) { return 0; }

	///申请组合录入请求
	virtual int ReqCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, int nRequestID) { return 0; }

	///请求查询报单
	virtual int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID) { return 0; }

	///请求查询成交
	virtual int ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID) { return 0; }

	///请求查询投资者持仓
	virtual int ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID) { return 0; }

	///请求查询资金账户
	virtual int ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) { return 0; }

	///请求查询投资者
	virtual int ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID) { return 0; }

	///请求查询交易编码
	virtual int ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID) { return 0; }

	///请求查询合约保证金率
	virtual int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID) { return 0; }

	///请求查询合约手续费率
	virtual int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID) { return 0; }

	///请求查询交易所
	virtual int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID) { return 0; }

	///请求查询产品
	virtual int ReqQryProduct(CThostFtdcQryProductField *pQryProduct, int nRequestID) { return 0; }

	///请求查询合约
	virtual int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID) { return 0; }

	///请求查询行情
	virtual int ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID) { return 0; }

	///请求查询投资者结算结果
	virtual int ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID) { return 0; }

	///请求查询转帐银行
	virtual int ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID) { return 0; }

	///请求查询投资者持仓明细
	virtual int ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID) { return 0; }

	///请求查询客户通知
	virtual int ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID) { return 0; }

	///请求查询结算信息确认
	virtual int ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID) { return 0; }

	///请求查询投资者持仓明细
	virtual int ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID) { return 0; }

	///请求查询保证金监管系统经纪公司资金账户密钥
	virtual int ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID) { return 0; }

	///请求查询仓单折抵信息
	virtual int ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID) { return 0; }

	///请求查询投资者品种/跨品种保证金
	virtual int ReqQryInvestorProductGroupMargin(CThostFtdcQryInvestorProductGroupMarginField *pQryInvestorProductGroupMargin, int nRequestID) { return 0; }

	///请求查询交易所保证金率
	virtual int ReqQryExchangeMarginRate(CThostFtdcQryExchangeMarginRateField *pQryExchangeMarginRate, int nRequestID) { return 0; }

	///请求查询交易所调整保证金率
	virtual int ReqQryExchangeMarginRateAdjust(CThostFtdcQryExchangeMarginRateAdjustField *pQryExchangeMarginRateAdjust, int nRequestID) { return 0; }

	///请求查询汇率
	virtual int ReqQryExchangeRate(CThostFtdcQryExchangeRateField *pQryExchangeRate, int nRequestID) { return 0; }

	///请求查询二级代理操作员银期权限
	virtual int ReqQrySecAgentACIDMap(CThostFtdcQrySecAgentACIDMapField *pQrySecAgentACIDMap, int nRequestID) { return 0; }

	///请求查询产品报价汇率
	virtual int ReqQryProductExchRate(CThostFtdcQryProductExchRateField *pQryProductExchRate, int nRequestID) { return 0; }

	///请求查询产品组
	virtual int ReqQryProductGroup(CThostFtdcQryProductGroupField *pQryProductGroup, int nRequestID) { return 0; }

	///请求查询做市商合约手续费率
	virtual int ReqQryMMInstrumentCommissionRate(CThostFtdcQryMMInstrumentCommissionRateField *pQryMMInstrumentCommissionRate, int nRequestID) { return 0; }

	///请求查询做市商期权合约手续费
	virtual int ReqQryMMOptionInstrCommRate(CThostFtdcQryMMOptionInstrCommRateField *pQryMMOptionInstrCommRate, int nRequestID) { return 0; }

	///请求查询报单手续费
	virtual int ReqQryInstrumentOrderCommRate(CThostFtdcQryInstrumentOrderCommRateField *pQryInstrumentOrderCommRate, int nRequestID) { return 0; }

	///请求查询资金账户
	virtual int ReqQrySecAgentTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) { return 0; }

	///请求查询二级代理商资金校验模式
	virtual int ReqQrySecAgentCheckMode(CThostFtdcQrySecAgentCheckModeField *pQrySecAgentCheckMode, int nRequestID) { return 0; }

	///请求查询二级代理商信息
	virtual int ReqQrySecAgentTradeInfo(CThostFtdcQrySecAgentTradeInfoField *pQrySecAgentTradeInfo, int nRequestID) { return 0; }

	///请求查询期权交易成本
	virtual int ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField *pQryOptionInstrTradeCost, int nRequestID) { return 0; }

	///请求查询期权合约手续费
	virtual int ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField *pQryOptionInstrCommRate, int nRequestID) { return 0; }

	///请求查询执行宣告
	virtual int ReqQryExecOrder(CThostFtdcQryExecOrderField *pQryExecOrder, int nRequestID) { return 0; }

	///请求查询询价
	virtual int ReqQryForQuote(CThostFtdcQryForQuoteField *pQryForQuote, int nRequestID) { return 0; }

	///请求查询报价
	virtual int ReqQryQuote(CThostFtdcQryQuoteField *pQryQuote, int nRequestID) { return 0; }

	///请求查询期权自对冲
	virtual int ReqQryOptionSelfClose(CThostFtdcQryOptionSelfCloseField *pQryOptionSelfClose, int nRequestID) { return 0; }

	///请求查询投资单元
	virtual int ReqQryInvestUnit(CThostFtdcQryInvestUnitField *pQryInvestUnit, int nRequestID) { return 0; }

	///请求查询组合合约安全系数
	virtual int ReqQryCombInstrumentGuard(CThostFtdcQryCombInstrumentGuardField *pQryCombInstrumentGuard, int nRequestID) { return 0; }

	///请求查询申请组合
	virtual int ReqQryCombAction(CThostFtdcQryCombActionField *pQryCombAction, int nRequestID) { return 0; }

	///请求查询转帐流水
	virtual int ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID) { return 0; }

	///请求查询银期签约关系
	virtual int ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID) { return 0; }

	///请求查询签约银行
	virtual int ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID) { return 0; }

	///请求查询预埋单
	virtual int ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID) { return 0; }

	///请求查询预埋撤单
	virtual int ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID) { return 0; }

	///请求查询交易通知
	virtual int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID) { return 0; }

	///请求查询经纪公司交易参数
	virtual int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID) { return 0; }

	///请求查询经纪公司交易算法
	virtual int ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID) { return 0; }

	///请求查询监控中心用户令牌
	virtual int ReqQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, int nRequestID) { return 0; }

	///期货发起银行资金转期货请求
	virtual int ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) { return 0; }

	///期货发起期货资金转银行请求
	virtual int ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) { return 0; }

	///期货发起查询银行余额请求
	virtual int ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) { return 0; }

private:
	CThostFtdcTraderSpi *m_pSpi;
};

CThostFtdcTraderApi *CThostFtdcTraderApi::CreateFtdcTraderApi(const char *pszFlowPath)
{
	return new MyClassCTP_SE();
}

