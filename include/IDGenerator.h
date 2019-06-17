#pragma once

#include <atomic>

#include "ApiStruct.h"

using namespace std;

class CIDGenerator
{
public:
	CIDGenerator();
	~CIDGenerator();

	// ����̫���ˣ���Ҫ�Զ��ضϣ�Ŀǰ�ƻ����ܳ���32
	void SetPrefix(const char* prefix);

	const char* GetIDString();
	unsigned int GetID();
	// ��֤���첻�ظ�
	unsigned int GetTimeID();

	const unsigned int sec_per_year = 86400 * 365;
private:
	atomic<unsigned int> m_id;
	OrderIDType m_IDString;
	OrderIDType m_Prefix;

};

