function tbl = ReqQryTradingAccount(td, sec)

SetWait(0);

% ��ѯ
global account_table;
account_table = [];
td.NewQuery();
td.ReqQuery('ReqQryTradingAccount');

% ��Ҫѭ���ȴ�����
Wait(sec);

% ����
tbl = account_table;

end