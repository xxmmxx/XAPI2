function OnTdConnectionStatus(sender,arg)

import XAPI.*;

global td;

disp('TD')
% �������ӻر�
disp(arg.status);

if(arg.size1>0)
    disp(Extensions_GBK.Text(arg.userLogin));

end
