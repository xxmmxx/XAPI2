function OnMdConnectionStatus(sender,arg)

import XAPI.*;

global md;

disp('MD')
% �������ӻر�
disp(arg.status);

if(arg.size1>0)
    disp(Extensions_GBK.Text(arg.userLogin));

switch arg.status
    case XAPI.ConnectionStatus.Done
        % �������飬֧��";"�ָ�
        md.Subscribe('IF1602;IF1603;IF1606','');
end

end
