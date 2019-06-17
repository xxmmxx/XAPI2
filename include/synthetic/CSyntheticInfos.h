#pragma once

#include <list>
#include <unordered_map>

using namespace std;

// �ϳɺ�Լ������Ϣ
// ���ڿ��Դ������ļ���ֱ�ӽ�������Ҫ�ĸ�ʽ
// ����ķ���ӳ�����Ҳûɶʵ���ô���������
class CSyntheticInfos
{
public:
	CSyntheticInfos();
	~CSyntheticInfos();

	void Add(const char* product, const char* instrument, double weight = 0);
	unordered_map<string, double> GetByProduct(const char* product);
	unordered_map<string, double> GetByInstrument(const char* instrument);

	list<string> GetProducts();
private:
	void Add_Product(const char* product, const char* instrument, double weight);
	void Add_Instrument(const char* instrument, const char* product, double weight);

private:
	unordered_map<string, unordered_map<string,double>> m_map_by_product;
	unordered_map<string, unordered_map<string, double>> m_map_by_instrument;

	list<string> m_list_product;
};

