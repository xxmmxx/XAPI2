REM 2 ��ѯ�ֲ�
REM 32 �ֲֻر���10��
REM 12 д��Ŀ��ֲ��ļ�
REM 13 �ϲ������ļ�
REM 7 ѭ���µ�
python.exe test_ctp_api.py --input=2;32;12;13;7  >log/%date:~0,4%%date:~5,2%%date:~8,2%.log
REM ��������һ�����ʼ��ͳ���
python mail.py --username=123 --password=456 --from=123@qq.com --to=123@qq.com;456@qq.com --log=log --bat=%~f0
