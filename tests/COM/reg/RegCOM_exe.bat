@echo off
echo �˹������XAPI��COM���ע�ᡣ
echo ���Ҽ����Թ���Ա��ʽ���С�������Ȩ�޲���
echo ������64λע����32λע����¶����COM���
echo ������������ڵ����з�ʽ�������ˣ�����Щϵͳ�����޷���ȷѡ���ǽ����ڻ��ǽ����⣬������Ҫclear_InprocServer32.reg���ʹ��
@echo on
cd %~dp0
%~d0
x64\RegAsm.exe XAPI_CSharp.exe /nologo /codebase /tlb
x86\RegAsm.exe XAPI_CSharp.exe /nologo /codebase /tlb
pause