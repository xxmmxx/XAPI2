function OnRtnOrder(sender,arg)

import XAPI.*;

% ί�лر�
global orders;

% ��ӡ����
disp(arg.order.InstrumentID);
disp(arg.order.Status);
disp(arg.order.ExecType);
disp(Extensions_GBK.Text(arg.order));

disp(arg.order.LocalID);
disp(arg.order.ID);

end
