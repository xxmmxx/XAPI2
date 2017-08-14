%% ����
function td = Tdx_Connect()

% ֻ��ʵ����һ��
td = actxserver('XAPI.COM');
td.SetLibPath(fullfile(cd,'XAPI\x86\Tdx\Tdx_Trade_x86.dll'));
td.SetLibPath('C:\Program Files\SmartQuant Ltd\OpenQuant 2014\XAPI\x86\Tdx\Tdx_Trade_x86.dll');

td.SetServerInfo('ExtInfoChar128','D:\new_gjzq_v6\');
td.SetServerInfo('Address','D:\new_gjzq_v6\Login.lua');

td.SetUserInfo('UserID','890626');
td.SetUserInfo('Password','123');

registerevent(td,{'OnConnectionStatus' @OnTdConnectionStatus});
registerevent(td,{'OnRtnOrder' @OnRtnOrder});
registerevent(td,{'OnRspQryOrder' @OnRspQryOrder});
registerevent(td,{'OnRspQryInvestorPosition' @OnRspQryInvestorPosition});
registerevent(td,{'OnRtnDepthMarketData' @OnRtnDepthMarketData});

td.Connect();

end

