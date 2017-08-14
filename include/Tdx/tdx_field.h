﻿#pragma once

#ifndef _TDX_FIELD_H_
#define _TDX_FIELD_H_
/*
下面这张表目前是基于华宝证券个股期权客户端，不知道换券商或升级后是否会不一样
ID一样，中文名可能不一样
*/

#define FIELD_CZSJ			50 // 50_(参数)操作数据
#define FIELD_JYSDM			100 // 100_交易所代码
#define FIELD_JYSMC			101 // 101_交易所名称
#define FIELD_WTFS_110		110 // 110_委托方式
#define FIELD_CZBZ			113	//	113_操作标志
#define FIELD_ZDJY			116	//	116_指定交易
#define FIELD_KHH			120	//	120_客户号
#define FIELD_ZJZH			121	//	121_资金帐号
#define FIELD_KHMC			122	//	122_客户名称
#define FIELD_GDDM			123	//	123_股东代码
#define FIELD_GDMC			124	//	124_股东名称
#define FIELD_ZHLB			125	//	125_帐号类别
#define FIELD_KSRQ			126	//	126_开始日期
#define FIELD_ZZRQ			127	//	127_终止日期
#define FIELD_MMBZ			130	//	130_买卖标志
#define FIELD_WTLB			131	//	131_委托类别		华宝仿真显示131_买卖标志，华宝实盘显示131_委托类别
#define FIELD_BZ			132	//	132_币种
#define FIELD_JYDW			133	//	133_交易单位
#define FIELD_JYMM			134	//	134_交易密码
#define FIELD_ZQDM			140	//	140_证券代码
#define FIELD_ZQMC			141	//	141_证券名称
#define FIELD_WTRQ			142	//	142_委托日期
#define FIELD_WTSJ			143	//	143_委托时间
#define FIELD_WTSL			144	//	144_委托数量
#define FIELD_WTJG			145	//	145_委托价格
#define FIELD_WTBH			146	//	146_委托编号
#define FIELD_ZTSM			147	//	147_状态说明
#define FIELD_WTJE			148	//	148_委托金额
#define FIELD_FHXX			149	//	149_返回信息
#define FIELD_CJRQ			150	//	150_成交日期
#define FIELD_CJSJ			151	//	151_成交时间
#define FIELD_CJSL			152	//	152_成交数量
#define FIELD_CJJG			153	//	153_成交价格
#define FIELD_CJJE			154	//	154_成交金额
#define FIELD_CJBH			155	//	155_成交编号
#define FIELD_DJSL			160	//	160_冻结数量
#define FIELD_DJZJ			161	//	161_冻结资金
#define FIELD_CDSL			162	//	162_撤单数量
#define FIELD_164			164	//	164_
#define FIELD_165			165	//	165_
#define FIELD_WTFS			166	//	166_委托方式
#define FIELD_CDBZ			167	//	167_撤单标志
#define FIELD_168			168	//	?
#define FIELD_PHRQ			170	//	170_配号日期
#define FIELD_QSPH			171	//	171_起始配号
#define FIELD_PHSL			172	//	172_配号数量
#define FIELD_XWDM			173	//	173_席位代码
#define FIELD_ZXJYGS		187	//	187_最小交易股数
#define FIELD_BJFS			194	//	194_报价方式
#define FIELD_ZQSL			200	//	200_证券数量
#define FIELD_KMSL			201	//	201_可卖数量
#define FIELD_TBCBJ			202	//	202_摊簿成本价
#define FIELD_TBFDYK		204	//	204_摊簿浮动盈亏
#define FIELD_ZXSZ			205	//	205_最新市值
#define FIELD_YJ			206	//	206_佣金
#define FIELD_GHF			207	//	207_过户费
#define FIELD_CJF			208	//	208_成交费
#define FIELD_JSF			209	//	209_结算费
#define FIELD_YHS			210	//	210_印花税
#define FIELD_ZXMRBDJW		226	//	226_最小买入变动价位
#define FIELD_ZXMCBDJW		227	//	227_最小卖出变动价位
#define FIELD_CKYKBL		230	//	230_参考盈亏比例(%)
#define FIELD_SXYK			232	//	232_实现盈亏
#define FIELD_LX			234	//	234_类型
#define FIELD_ZHZT			238	//	238_帐户状态
#define FIELD_DFGDDM		244	//	244_对方股东代码
#define FIELD_DFZHLB		245	//	245_对方帐号类别
#define FIELD_RZRQBS		281	//	281_融资融券标识
#define FIELD_FJZH			294	//	294_附加账号
#define FIELD_ZJYE			300	//	300_资金余额
#define FIELD_KYZJ			301	//	301_可用资金
#define FIELD_KQZJ			302	//	302_可取资金
#define FIELD_FSJE			303	//	303_发生金额
#define FIELD_SYJE			304	//	304_剩余金额
#define FIELD_ZY			305	//	305_摘要
#define FIELD_ZZC_310		310	//	310_总资产
#define FIELD_FNLX			333	//	333_方案类型
#define FIELD_ZZC_350		350	//	350_总资产
#define FIELD_CPDM			391	//	391_产品代码
#define FIELD_FXJG			418	//	418_发行价格
#define FIELD_TJRDM			485	//	485_推荐人代码
#define FIELD_HYDM			510	//	510_合约代码
#define FIELD_HYMC			511	//	511_合约名称
#define FIELD_KPBZ			513	//	513_开平标志
#define FIELD_TBBZ			514	//	514_投保标志
#define FIELD_JGYF			540	//	540_交割月份
#define FIELD_BZJ			541	//	541_保证金
#define FIELD_ETFDM			601	//	601_ETF代码
#define FIELD_JYLX			603	//	603_交易类型
#define FIELD_RGSL			604	//	604_认购数量
#define FIELD_PCH			606	//	606_批次号
#define FIELD_FXD			700	//	700_风险度
#define FIELD_718			718	//	?
#define FIELD_ZGSL			961	//	961_最高数量
#define FIELD_SGED_961		961	//	961_申购额度，在申万宏源中居然也是961
#define FIELD_ZDSL			962	//	962_最低数量
#define FIELD_QSXH			1207	//	1207_起始序号
#define FIELD_BLXX			1213	//	1213_保留信息
#define FIELD_JCFXBZ		1223	//	1223_检查风险标志
#define FIELD_BZ_1217		1217	//	1217_备注	感觉是给当成备兑用了
#define FIELD_FJYZHBZ		1224	//	1224_非交易帐号标志
#define FIELD_CS			1227	//	1227_参数
#define FIELD_XSYS			1231	//	1231_显示颜色
#define FIELD_JB			1233	//	1233_句柄

#define FIELD_KHQZ			5250	// 5250_客户群组
#define FIELD_KHYYB			5251	// 5251_客户营业部
#define FIELD_YPXLH			5252	// 5252_硬盘序列号
#define FIELD_CPUID			5253	// 5253_CPUID
#define FIELD_XYJYBS		5254	// 5254_信用交易标识
#define FIELD_HHH			5255	// 5255_会话号
#define FIELD_KHDM			5256	// 5256_客户代码

#define FIELD_XGSGED_5564	5564	// 5564_新股申购额度
#define FIELD_WTLB_5565		5565	// 5565_委托类别
#define FIELD_JSHL			5599	// 5599_结算汇率

#define FIELD_JYF			5630	// 5630_交易费
#define FIELD_DSGF			5638	// 5638_代收规费
#define FIELD_GYWTBS		5639	// 5639_隔夜委托标识
#define FIELD_FXRQ_5653		5653	// 5653_发行日期
#define FIELD_ZZHBH			5673	// 5673_子账户编号
#define FIELD_JSJG			5674	// 5674_结算机构
#define FIELD_JSZH			5675	// 5675_结算账号
#define FIELD_DJRQ			5676	// 5676_登记日期
#define FIELD_XGSGED		5679	// 5679_新股申购额度
#define FIELD_KHSZED		5771	// 5771_客户市值额度
#define FIELD_ZHED			5772	// 5772_账户额度
#define FIELD_KSGED			5801	// 5801_可申购额度
#define FIELD_SGSX			5905	// 5905_申购上限
#define FIELD_FXRQ_5906		5906	// 5906_发行日期
#define FIELD_YMTZH			5925	// 5925_一码通账号
#define FIELD_MMFX			5935	// 5935_买卖方向
#define FIELD_BJFS_6027		6027	// 6027_报价方式


#define FIELD_BID_SIZE_1	900	// 900_买一量
#define FIELD_BID_SIZE_2	901	// 901_买二量
#define FIELD_BID_SIZE_3	902	// 902_买三量
#define FIELD_BID_SIZE_4	903	// 903_买四量
#define FIELD_BID_SIZE_5	904	// 904_买五量

#define FIELD_BID_PRICE_1	910	// 910_买一价
#define FIELD_BID_PRICE_2	911	// 911_买二价
#define FIELD_BID_PRICE_3	912	// 912_买三价
#define FIELD_BID_PRICE_4	913	// 913_买四价
#define FIELD_BID_PRICE_5	914	// 914_买五价

#define FIELD_ASK_SIZE_1	920	// 920_卖一量
#define FIELD_ASK_SIZE_2	921	// 921_卖二量
#define FIELD_ASK_SIZE_3	922	// 922_卖三量
#define FIELD_ASK_SIZE_4	923	// 923_卖四量
#define FIELD_ASK_SIZE_5	924	// 924_卖五量

#define FIELD_ASK_PRICE_1	930	// 930_卖一价
#define FIELD_ASK_PRICE_2	931	// 931_卖二价
#define FIELD_ASK_PRICE_3	932	// 932_卖三价
#define FIELD_ASK_PRICE_4	933	// 933_卖四价
#define FIELD_ASK_PRICE_5	934	// 934_卖五价

#define FIELD_ZTJG	940		// 940_涨停价格
#define FIELD_DTJG	941		// 941_跌停价格

#define FIELD_JKJ	945		// 945_今开价
#define FIELD_ZSJ	946		// 946_昨收价
#define FIELD_GZLX	948		// 948_国债利息
#define FIELD_DQJ	949		// 949_当前价
#define FIELD_QGTHQ	956		// 956_取柜台行情
#define FIELD_GZBS	958		// 958_国债标识




#endif