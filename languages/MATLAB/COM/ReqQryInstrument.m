%% ��ѯ�ֲ�
function tbl = ReqQryInstrument(td, sec)

SetWait(0);

% ��ѯ
global instrument_table;
instrument_table = [];
td.NewQuery();
td.ReqQuery('ReqQryInstrument');

% ��Ҫѭ���ȴ�����
Wait(sec);

% ����
tbl = instrument_table;

end