function OnRspQrySettlementInfo(varargin)

% Ԥ����
event_struct = varargin{1,end-1};
settlementInfo = event_struct.settlementInfo;
bIsLast = event_struct.bIsLast;

% �������ݣ�Ӧ���������������ϣ�Ȼ��ͳһ���д�ӡ
global settlementInfo_table;
settlementInfo_table = settlementInfo;

% ���һ����¼�����Է�����
SetWait(bIsLast);

end
