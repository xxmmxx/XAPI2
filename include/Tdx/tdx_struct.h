﻿#pragma once

#ifndef _TDX_STRUCT_H_
#define _TDX_STRUCT_H_

#define COL_EACH_ROW	(64) //每行多少例，相当重要

// 字段信息,通达信内部定义的，非自定义
struct FieldInfo_STRUCT
{
	short FieldID;		// 字段ID
	char a;				// 只有a和1两种情况
	char b;				// 怎么一直都是2？
	char Len;			// 好像目前看到最大的是32
	char d;
	char e;
	char FieldName[25];	// 字段名字
};

// 错误
struct Error_STRUCT
{
	int ErrType;
	int ErrCode;
	char ErrInfo[256];

	void* Client;	// 多账号
};

// 请求
// 响应，有两种类型
// 一个请求只返回一行数据
// 一个请求返回多行数据
struct RequestResponse_STRUCT
{
	//请求类型
	int	requestType;
	int requestID;
	// 多行内容
	char pContent[1024];
	// 请求大小
	int size;

	void* Client;	// 多账号
	char khh[64];

	// 表头
	FieldInfo_STRUCT** ppFieldInfo;
	// 错误
	Error_STRUCT* pErr;
	// 多行内容
	char** ppResults;

	void* pSend;
	// API内部使用的字段，请不要动
	void* pUserData_Private;
	// API外部使用的字段，用来方便开发
	void* pUserData_Public;
	// API外部使用的字段，用来方便开发
	void* pUserData_Public2;
};

// 委托
struct Order_STRUCT
{
	char KHH[32];	// 120_客户号
	char ZJZH[32];	// 121_资金帐号
	char GDDM[32];	// 123_股东代码
	char ZQDM[32];	// 140_证券代码
	int ZHLB;		// 125_帐号类别
	int RZRQBS;		// 281_融资融券标识
	double Price;	// 145_委托价格
	double Qty;		// 144_委托数量
	long MMBZ;		// 130_买卖标志
	long WTFS;		// 166_委托方式
	int CZBZ;		// 113_操作标志

	char ZHLB_[2];	// 125_帐号类别
	// 下完单后用来回填
	char WTBH[32];	// 146_委托编号

	int	requestType;	// 请求类型

	void* Client;	// 多账号
};

struct CancelOrder_STRUCT
{
	char KHH[32];	// 120_客户号
	char ZJZH[32];	// 121_资金帐号
	char ZHLB[32];	// 125_帐号类别
	char GDDM[32];	// 123_股东代码
	char ZQDM[32];	// 140_证券代码
	char WTBH[32];	// 146_委托编号
	char JYSDM[32];	// 100_交易所代码
	char XWDM[32];	// 173_席位代码
	// 港股需要委托价格
	char WTJG[32];	// 145_委托价格
	char WTSL[32];  // 144_委托数量

	void* Client;	// 多账号
};

struct ReqQueryData_STRUCT
{
	char KHH[32];	// 120_客户号
	char ZJZH[32];	// 121_资金帐号
	char GDDM[32];	// 123_股东代码
	char KSRQ[32];
	char ZZRQ[32];
	char ZQDM[32];
	int RZRQBS;		// 281_融资融券标识

	bool bAll;

	void* Client;	// 多账号
	//请求类型
	int	requestType;
};

// 股东列表
struct GDLB_STRUCT
{
	char GDDM[32];	// 123_股东代码
	char GDMC[32];	// 124_股东名称
	char ZHLB[32];	// 125_帐号类别
	char ZJZH[32];	// 121_资金帐号
	char XWDM[32];	// 173_席位代码
	char RZRQBS[32];	// 281_融资融券标识
	char YMTZH[32];	// 5925_一码通账号
	char BLXX[32];	// 1213_保留信息
	char JB[32];	// 1233_句柄

	int ZHLB_;		// 125_帐号类别
	int RZRQBS_;	// 281_融资融券标识

	void* Client;	// 多账号
};

// 为了处理银河证券没有资金账号，只有句柄的问题
// 149和101都会返回资金账号，但149不一定会出现
struct REQ148_STRUCT
{
	char CPZH[32];	// 120_产品账号
	char ZHLB[32];	// 125_帐号类别
	char FJYZHBZ[32];	// 1224_非交易帐号标志
	char RZRQBS[32];	// 281_融资融券标识
	char BLXX[32];	// 1213_保留信息

	void* Client;	// 多账号
};

// 当日委托/历史委托=委托列表
struct WTLB_STRUCT
{
	char KHH[32];	// 120_客户号
	char ZJZH[32];	// 121_资金帐号
	char WTRQ[32];	// 142_委托日期
	char WTSJ[32];	// 143_委托时间
	char GDDM[32];  // 123_股东代码
	char ZQDM[32];  // 140_证券代码
	char ZQMC[32];  // 141_证券名称
	char MMBZ[32];  // 130_买卖标志	*由于很多特别的指令都是3,只能想法解析中文
	char WTLB[32];  // 131_委托类别	*
	char JYSDM[32];	// 100_交易所代码
	char WTJG[32];  // 145_委托价格
	char WTSL[32];  // 144_委托数量
	char CJJG[32];  // 153_成交价格
	char CJSL[32];  // 152_成交数量
	char CDSL[32];  // 162_撤单数量
	char WTBH[32];  // 146_委托编号
	char WTFS[32];  // 166_委托方式	*有些版本没有此字段，只能想法解析中文
	char BJFS[32];  // 194_报价方式	*
	char ZTSM[32];  // 147_状态说明
	char DJZJ[32];  // 161_冻结资金
	char BLXX[32];	// 1213_保留信息
	char JB[32];	// 1233_句柄

	//char WTJE[32];  // 148_委托金额
	//char CJJE[32];	// 154_成交金额
	char ZHLB[32];	// 125_帐号类别
	

	int WTRQ_;
	int WTSJ_;
	int MMBZ_;
	int WTLB_;
	int JYSDM_;
	double WTJG_;
	int WTSL_;
	double CJJG_;
	int CJSL_;
	int CDSL_;
	double DJZJ_;
	int ZTSM_;
	int BJFS_;
	int WTFS_;

	//double WTJE_;
	//double CJJE_;
	int ZHLB_;

	void* Client;	// 多账号
};

// 成交列表
struct CJLB_STRUCT
{
	char CJRQ[32];	// 150_成交日期
	char CJSJ[32];	// 151_成交时间
	char GDDM[32];	// 123_股东代码
	char ZQDM[32];	// 140_证券代码
	char ZQMC[32];	// 141_证券名称
	char MMBZ[32];	// 130_买卖标志
	char WTLB[32];	// 131_委托类别
	char CJJG[32];	// 153_成交价格
	char CJSL[32];	// 152_成交数量
	char FSJE[32];	// 303_发生金额
	char SYJE[32];	// 304_剩余金额
	char YJ[32];	// 206_佣金
	char YHS[32];	// 210_印花税
	char GHF[32];	// 207_过户费
	char CJF[32];	// 208_成交费
	char CJBH[32];	// 155_成交编号
	char CDBZ[32];	// 167_撤单标志
	char WTBH[32];	// 146_委托编号

	int CJRQ_;
	int CJSJ_;
	int MMBZ_;
	int WTLB_;
	double CJJG_;
	int CJSL_;
	double FSJE_;
	double SYJE_;
	double YJ_;
	double YHS_;
	double GHF_;
	double CJF_;
	int CDBZ_;

	void* Client;	// 多账号
};


// 股份列表
struct GFLB_STRUCT
{
	char ZQDM[32];  // 140_证券代码
	char ZQMC[32];  // 141_证券名称
	char ZQSL[32];  // 200_证券数量
	char KMSL[32];  // 201_可卖数量
	char TBCBJ[32];  // 202_摊簿成本价
	char DQJ[32];  // 949_当前价
	char ZXSZ[32];  // 205_最新市值
	char TBFDYK[32];  // 204_摊簿浮动盈亏
	char SXYK[32];  // 232_实现盈亏
	char CKYKBL[32];  // 230_参考盈亏比例(%)
	char DJSL[32];  // 160_冻结数量
	char GDDM[32];  // 123_股东代码
	char JYSDM[32]; // 100_交易所代码
	char JYSMC[32];  // 101_交易所名称
	char BLXX[32];	// 1213_保留信息
	char JB[32];	// 1233_句柄

	char DQSL[32];  // 5712_当前数量
	char SJSL[32];  // 5508_实际数量

	int ZQSL_;
	int KMSL_;
	double TBCBJ_;
	double DQJ_;
	double ZXSZ_;
	double DJSL_;
	double DQSL_;
	double SJSL_;

	void* Client;	// 多账号
};

// 资金余额
struct ZJYE_STRUCT
{
	char BZ[32];		// 132_币种
	char ZJYE[32];		// 300_资金余额
	char KYZJ[32];		// 301_可用资金
	char ZZC[32];		// 310_总资产
	char KQZJ[32];		// 302_可取资金
	char ZJZH[32];		// 121_资金帐号
	char ZXSZ[32];		// 205_最新市值

	double ZJYE_;
	double KYZJ_;
	double ZZC_;
	double KQZJ_;
	double ZXSZ_;

	void* Client;	// 多账号
};

struct HQ_STRUCT
{
	char ZQDM[32];		// 140_证券代码
	char ZQMC[32];		// 141_证券名称
	char ZSJ[32];		// 946_昨收价
	char JKJ[32];		// 945_今开价
	char GZLX[32];		// 948_国债利息
	char DQJ[32];		// 949_当前价
	char BidPrice1[32];		// 910_买一价
	char BidPrice2[32];		// 911_买二价
	char BidPrice3[32];		// 912_买三价
	char BidPrice4[32];		// 913_买四价
	char BidPrice5[32];		// 914_买五价
	char BidSize1[32];		// 900_买一量
	char BidSize2[32];		// 901_买二量
	char BidSize3[32];		// 902_买三量
	char BidSize4[32];		// 903_买四量
	char BidSize5[32];		// 904_买五量
	char AskPrice1[32];		// 930_卖一价
	char AskPrice2[32];		// 931_卖二价
	char AskPrice3[32];		// 932_卖三价
	char AskPrice4[32];		// 933_卖四价
	char AskPrice5[32];		// 934_卖五价
	char AskSize1[32];		// 920_卖一量
	char AskSize2[32];		// 921_卖二量
	char AskSize3[32];		// 922_卖三量
	char AskSize4[32];		// 923_卖四量
	char AskSize5[32];		// 924_卖五量
	char JYSDM[32];		// 100_交易所代码
	char ZXJYGS[32];		// 187_最小交易股数
	char ZXMRBDJW[32];		// 226_最小买入变动价位
	char ZXMCBDJW[32];		// 227_最小卖出变动价位
	char ZHLB[32];		// 125_帐号类别
	char BZ[32];		// 132_币种
	char GZBS[32];		// 958_国债标识
	char BLXX[32];		// 1213_保留信息
	char ZTJG[32];		// 940_涨停价格
	char DTJG[32];		// 941_跌停价格

	double ZSJ_;		// 946_昨收价
	double JKJ_;		// 945_今开价
	double GZLX_;		// 948_国债利息
	double DQJ_;		// 949_当前价
	double BidPrice1_;		// 910_买一价
	double BidPrice2_;		// 911_买二价
	double BidPrice3_;		// 912_买三价
	double BidPrice4_;		// 913_买四价
	double BidPrice5_;		// 914_买五价
	int BidSize1_;		// 900_买一量
	int BidSize2_;		// 901_买二量
	int BidSize3_;		// 902_买三量
	int BidSize4_;		// 903_买四量
	int BidSize5_;		// 904_买五量
	double AskPrice1_;		// 930_卖一价
	double AskPrice2_;		// 931_卖二价
	double AskPrice3_;		// 932_卖三价
	double AskPrice4_;		// 933_卖四价
	double AskPrice5_;		// 934_卖五价
	int AskSize1_;		// 920_卖一量
	int AskSize2_;		// 921_卖二量
	int AskSize3_;		// 922_卖三量
	int AskSize4_;		// 923_卖四量
	int AskSize5_;		// 924_卖五量

	int ZXJYGS_;		// 187_最小交易股数
	double ZXMRBDJW_;		// 226_最小买入变动价位
	double ZXMCBDJW_;		// 227_最小卖出变动价位
	double ZTJG_;		// 940_涨停价格
	double DTJG_;		// 941_跌停价格

	void* Client;	// 多账号
};

//
////////////////////////////////////////////////////////////////////////////
//// 以下的在整理完后要删除
//

//
//// 登录结果
//struct DLJG_STRUCT
//{
//	char COL1[128];
//	char COL2[128];
//	char COL3[128];
//
//	void* Client;	// 多账号
//};

////////////////////////////////////////////////////////////////////////////
// 打新相关结构体

// 新股配号查询 REQUEST_XGSG_XGPHCX 1116
struct XGPHCX_STRUCT
{
	char PHRQ[32];	// 170_配号日期
	char ZQDM[32];  // 140_证券代码
	char ZQMC[32];  // 141_证券名称
	char QSPH[32];  // 171_起始配号
	char PHSL[32];  // 172_配号数量
	char ZHLB[32];	// 125_帐号类别
	char BLXX[32];	// 1213_保留信息

	int PHRQ_;
	int QSPH_;
	int PHSL_;
	int ZHLB_;

	void* Client;	// 多账号
};

// 新股中签查询 REQUEST_XGSG_XGZQCX 1126
struct XGZQCX_STRUCT
{
	char CJRQ[32];	// 150_成交日期
	char ZQDM[32];  // 140_证券代码
	char ZQMC[32];  // 141_证券名称
	char CJJG[32];  // 153_成交价格
	char ZQS[32];  // 174_中签数
	char ZHLB[32];	// 125_帐号类别
	char BLXX[32];	// 1213_保留信息

	int CJRQ_;
	int CJJG_;
	int ZQS_;
	int ZHLB_;

	void* Client;	// 多账号
};

// 可申购新股查询 REQUEST_XGSG_KSGXGCX 1156
struct KSGXGCX_STRUCT
{
	char ZQDM[32];  // 140_证券代码
	char ZQMC[32];  // 141_证券名称
	char FXJG[32];  // 418_发行价格
	char ZDSL[32];  // 962_最低数量
	char ZGSL[32];  // 961_最高数量
	char WTSL[32];  // 144_委托数量
	char WTRQ[32];  // 142_委托日期
	char JYSDM[32];  // 100_交易所代码
	char JYSMC[32];  // 101_交易所名称
	char CZSJ[32];  // 50_(参数)操作数据
	char JB[32];	// 1233_句柄
	char BLXX[32];	// 1213_保留信息

	char SGSX[32];	// 5905_申购上限
	char WTJG[32];	// 145_委托价格
	char ZHLB[32];	// 125_帐号类别

	double FXJG_;
	double ZDSL_;
	double ZGSL_;
	double WTSL_;
	int WTRQ_;
	int JYSDM_;
	double SGSX_;
	double WTJG_;
	int ZHLB_;

	void* Client;	// 多账号
};

// 新股申购额度查询 REQUEST_XGSG_XGSGEDCX 1340
struct XGSGEDCX_STRUCT
{
	char ZJZH[32];  // 121_资金帐号
	char CS[32];  // 1227_参数
	char ZHLB[32];  // 125_帐号类别
	char GDDM[32];  // 123_股东代码
	char JYSMC[32];  // 101_交易所名称
	char XGSGED[32];  // 5679_新股申购额度
	char KSGED[32];  // 5801_可申购额度,光大证券
	char CZSJ[32];  // 50_(参数)操作数据
	char JB[32];	// 1233_句柄
	char BLXX[32];	// 1213_保留信息

	char SGED_961[32];		//	961_申购额度，在申万宏源中居然也是961
	char XGSGED_5564[32];	// 5564_新股申购额度
	char XGSGED_5718[32];	// 5718_新股申购额度
	char KHSZED_5759[32];	// 5759_客户市值额度
	char ZHED[32];			// 5772_账户额度


	double CS_;
	double XGSGED_;
	int ZHLB_;
	double SGED_961_;
	double XGSGED_5564_;
	double XGSGED_5718_;
	double KSGED_;
	double ZHED_;
	double KHSZED_5759_;

	void* Client;	// 多账号
};

// 新股申购待缴款查询 REQUEST_XGSG_XGSGDJKCX 1370
struct XGSGDJKCX_STRUCT
{
	char ZQDM[32];  // 140_证券代码
	char ZQS[32];  // 174_中签数
	char FQSL[32];  // 98_放弃数量
	char ZQJE[32];  // 1088_中签金额
	char QKZJ[32];  // 1091_缺口资金
	char GDDM[32];  // 123_股东代码
	char ZHLB[32];  // 125_帐号类别
	char CZSJ[32];  // 50_(参数)操作数据
	char JB[32];	// 1233_句柄
	char BLXX[32];	// 1213_保留信息

	double CS_;
	double XGSGED_;

	void* Client;	// 多账号
};

#endif


