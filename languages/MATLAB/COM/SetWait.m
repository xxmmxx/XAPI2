% ��Ҫѭ���ȴ�����
function SetWait(n)

global wait_lock;
global timing_lock;

wait_lock = n;
timing_lock = now;

end