%% ��ѯ�ֲ�
function tbl = ReqQrySettlementInfo(td,date,sec)

SetWait(0);

% ��ѯ
global settlementInfo_table;
settlementInfo_table = [];
td.NewQuery();
td.SetQuery('DateStart',date);
td.ReqQuery('ReqQrySettlementInfo');

% ��Ҫѭ���ȴ�����
Wait(sec);

% ����
tbl = settlementInfo_table;

end