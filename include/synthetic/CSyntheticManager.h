#pragma once

#include <map>
#include <set>
#include <list>
#include <mutex>

#include "CSyntheticMarketData.h"

class CSyntheticManager
{
public:
	CSyntheticManager();
	~CSyntheticManager();

	// ����ԭ�������Upate��IF1802ͬʱ����IF000��IF888�����У����Ծ�����ǰ����
	CSyntheticMarketData* Register(const char* product, set<string> emits, map<string,double> legs, CSyntheticCalculate* pCal);
	set<string> UnRegister(const char* product);

	// �ȸ��ݺ�Լ�����º�Լ�ڴ���
	// ע�⣬���ص��ڴ��0��λ�Ǳ��λ
	char* Update(const char* instrument, void* pData, int size);
	
	set<CSyntheticMarketData*> GetByInstrument(const char* instrument);

private:
	// ��¼�Ķ��ٸ��ϳɺ�Լ�������
	map<string, CSyntheticMarketData*>	m_map_by_product;
	// ֻ��¼��ԭʼ������buf��ÿ���������ж��ָ��ָ����
	map<string, char*>	m_map_by_instrument;
	// ���������ֻ�����������Ӧ������
	map<string, set<CSyntheticMarketData*>>	m_map_set_by_instrument;

	mutex							m_cs;
};

