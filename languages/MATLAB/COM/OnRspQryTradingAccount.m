function OnRspQryTradingAccount(varargin)

% Ԥ����
event_struct = varargin{1,end-1};
account = event_struct.account;
bIsLast = event_struct.bIsLast;

% �������ݣ�Ӧ���������������ϣ�Ȼ��ͳһ���д�ӡ
global account_table;
t = object_2_table(account);
account_table = [account_table;t];

% ���һ����¼�����Է�����
SetWait(bIsLast);

end
