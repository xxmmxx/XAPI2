function OnRspQryInstrument(varargin)

% Ԥ����
event_struct = varargin{1,end-1};
instrument = event_struct.instrument;
bIsLast = event_struct.bIsLast;

% �������ݣ�Ӧ���������������ϣ�Ȼ��ͳһ���д�ӡ
global instrument_table;
t = object_2_table(instrument);
instrument_table = [instrument_table;t];

% ���һ����¼�����Է�����
SetWait(bIsLast);

end
