REM 2 ��ѯ�ֲ�
REM 42 �ֲֻر���10��
REM 12 д��Ŀ��ֲ��ļ�
REM 13 �ϲ������ļ�
REM 7 ѭ���µ�
PATH d:\Users\Kan\Anaconda3;d:\Users\Kan\Anaconda3\Scripts;d:\Users\Kan\Anaconda3\Library\bin;%PATH%
set date_Ymd=%date:~0,4%%date:~5,2%%date:~8,2%
python.exe test_ctp_api.py --input=2;42;12;13;7 1>log/%date_Ymd%.log 2>&1
REM ��������һ�����ʼ��ͳ���
python mail.py --username=123456 --password=654321 --from=123456@qq.com --to=123456@qq.com;654321@qq.com --log=log --bat=%~f0
