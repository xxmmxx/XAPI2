function OnRtnOrder(varargin)

event_struct = varargin{1,end-1};
order = event_struct.order;

% ��ӡ����
t = object_2_table(order);
disp(t);

end
