#pragma once

#include <map>
#include <set>
#include <mutex>
#include <vector>

using namespace std;

class CSubscribeManager1
{
public:
	CSubscribeManager1();
	~CSubscribeManager1();

	// ���ģ�������ҪAPI���ĵĺ�Լ
	set<string> Subscribe(set<string> ss);
	// ȡ�����ģ�������ҪAPIȡ���ĺ�Լ
	set<string> Unsubscribe(set<string> ss);

	// �õ������еĺ�Լ
	set<string> GetInstruments();
	// ����
	void Clear();


private:
	// ��Ҫ��¼���Ĵ���
	map<string, int> m_map;
	mutex					   m_cs;
};

