function OnMdConnectionStatus(varargin)

% varargin��ÿ��cell����˼��ο�doc�е�COM Event Handlers
event_struct = varargin{1,end-1};
sender = event_struct.sender;
status = event_struct.status_String;

disp(status);

if strcmp(status,'Done')
	sender.Subscribe('IF1608;IF1609;IF1612;ag1609','');
end

end