function OnTdConnectionStatus(varargin)

% varargin��ÿ��cell����˼��ο�doc�е�COM Event Handlers
event_struct = varargin{1,end-1};
sender = event_struct.sender;
status = event_struct.status_String;
userLogin = event_struct.userLogin;

t = object_2_table(userLogin);
disp(t);
disp(status);
disp(t.Text{1});

SetWait(strcmp(status,'Done'));

end