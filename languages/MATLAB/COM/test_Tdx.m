%% ����
td = Tdx_Connect();

% ��Ҫѭ���ȴ�����
SetWait(0);
Wait();
return

%% һ��Ҫ�ȴ���¼�ɹ�����
positions = ReqQryInvestorPosition(td,10)
orders = ReqQryOrder(td,10)
return;

BuyLimit(td,'600000', 100, 16.4);

td.CancelOrder('20161024:A499180558:1166')

td.Subscribe('600000','')
td.Subscribe('000001','')
