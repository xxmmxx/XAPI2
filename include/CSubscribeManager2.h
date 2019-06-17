#pragma once

#include <map>
#include <set>
#include <mutex>

using namespace std;

class CSubscribeManager2
{
public:
	CSubscribeManager2();
	~CSubscribeManager2();

	// ���ģ�������ҪAPI���ĵĺ�Լ
	set<string> Subscribe(string sym, set<string> ss);
	// ȡ�����ģ�������ҪAPIȡ���ĺ�Լ
	set<string> Unsubscribe(string sym);
	// �õ������еĺ�Լ
	set<string> GetChildenInstruments();
	// �õ��ϳɵĺ�Լ
	set<string> GetParentInstruments();
	// ����
	void Clear();

private:
	// ��Ҫ��¼���Ĵ���
	//unordered_map<string, int> m_map;
	mutex					   m_cs;
	map<string, set<string>> m_maps;
};

