REM 11 �ϲ��ֲ�
REM 7 ѭ���µ�
PATH d:\Users\Kan\Anaconda3;d:\Users\Kan\Anaconda3\Scripts;d:\Users\Kan\Anaconda3\Library\bin;%PATH%
set date_Ymd=%date:~0,4%%date:~5,2%%date:~8,2%
python.exe test_ctp_api.py --input=11;22;42 1>log/%date_Ymd%.log 2>&1
REM ��������һ�����ʼ��ͳ���
python mail.py --username=123456 --password= --from=123456@qq.com --to=123456@qq.com --log=log --bat=%~f0