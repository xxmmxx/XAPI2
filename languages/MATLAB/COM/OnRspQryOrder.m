function OnRspQryOrder(varargin)

% Ԥ����
event_struct = varargin{1,end-1};
order = event_struct.order;
bIsLast = event_struct.bIsLast;

% �������ݣ�Ӧ���������������ϣ�Ȼ��ͳһ���д�ӡ
global order_table;
t = object_2_table(order);
order_table = [order_table;t];

% ���һ����¼�����Է�����
SetWait(bIsLast);

end
