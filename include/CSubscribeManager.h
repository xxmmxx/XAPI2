#pragma once

#include <set>
#include <string>
#include <vector>
#include <map>

class CSubscribeManager1;
class CSubscribeManager2;
class CSyntheticConfig;
class CSyntheticManager;
class CSyntheticCalculateFactory;

using namespace std;

class CSubscribeManager
{
public:
	CSubscribeManager(CSyntheticConfig* pConfig, CSyntheticManager* pManager, CSyntheticCalculateFactory* pCalculateFactory);
	~CSubscribeManager();

	// ���ģ�������ҪAPI���ĵĺ�Լ
	set<string> Subscribe(set<string> instruments, string exchange);
	// ȡ�����ģ�������ҪAPIȡ���ĺ�Լ
	set<string> Unsubscribe(set<string> instruments, string exchange);

	// setת���ַ��������ڷ�������
	string Set2String(set<string> instruments, const char* seps = ";");

	// ���ַ���ת��api���õĸ�ʽ
	char* String2Buf(const char* str, vector<char*>& vct, const char* seps = ";");
	// �ַ���ת��set�����ں���Ķ��ļ���
	set<string> String2Set(const char* str, const char* seps = ";", int before = 1, const char* prefix = "");

	set<string> GetParentInstruments(string exchange);

	void Clear();

	CSubscribeManager1* GetManager1(string exchange);
	CSubscribeManager2* GetManager2(string exchange);

	set<string> GetExchanges();

private:
	map<string, CSubscribeManager1*> m_map_pSubscribeManager1;
	map<string, CSubscribeManager2*> m_map_pSubscribeManager2;

	CSyntheticConfig*			m_pConfig;
	CSyntheticManager*			m_pManager;

	CSyntheticCalculateFactory* m_pCalculateFactory;
};

