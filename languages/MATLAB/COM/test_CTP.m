td = CTP_Connect();

% ��Ҫѭ���ȴ�����
SetWait(0);
Wait(10);
return

%% һ��Ҫ�ȴ���¼�ɹ�����
positions = ReqQryInvestorPosition(td,10)
orders = ReqQryOrder(td,10)

settlementInfo = ReqQrySettlementInfo(td,20161219,10)
disp(settlementInfo.Content)

instruments = ReqQryInstrument(td,10)

account = ReqQryTradingAccount(td,10)

return;