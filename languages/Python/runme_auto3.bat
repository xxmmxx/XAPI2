REM 11 �ϲ��ֲ�
REM 7 ѭ���µ�
python.exe test_ctp_api.py --input=11;22;32 >log/%date:~0,4%%date:~5,2%%date:~8,2%.log
REM ��������һ�����ʼ��ͳ���
python mail.py --username=123 --password=456 --from=123@qq.com --to=456@qq.com --log=log --bat=%~f0