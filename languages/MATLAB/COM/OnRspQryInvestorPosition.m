function OnRspQryInvestorPosition(varargin)

% Ԥ����
event_struct = varargin{1,end-1};
position = event_struct.position;
bIsLast = event_struct.bIsLast;

% �������ݣ�Ӧ���������������ϣ�Ȼ��ͳһ���д�ӡ
global position_table;
t = object_2_table(position);
position_table = [position_table;t];

% ���һ����¼�����Է�����
SetWait(bIsLast);

end
