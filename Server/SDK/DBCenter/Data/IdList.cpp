#include "IdList.h"

namespace Cache 
{

IdList::IdList(void)
{
	m_buffer = new unsigned char[sizeof(int)+sizeof(int)*10000];
	Bind(m_buffer, sizeof(int)+sizeof(int)*10000);          // 绑定一个缓冲区，在此之上做转换
	SetByteOrder(net::smallOrder); // 设置字节序
}

IdList::~IdList(void)
{
	// body
	delete[]m_buffer;
}

bool IdList::Build(void)
{
	Clear();
	int count = m_ids.size();
	if (! AddData(count)) return false;
	int i = 0;
	for ( i = 0; i < count; i++ )
	{
		if (! AddData(m_ids[i])) return false;
	}

	return true;
}

bool IdList::Parse(void)
{
	m_ids.clear();
	if ( -1 == Size() ) return false;
	int count;
	if (! GetData(count)) return false;
	if ( count > 10000 ) return false;
	int i = 0;
	int userId;
	for ( i = 0; i < count; i++ )
	{
		if (! GetData(userId)) return false;
		m_ids.push_back(userId);
	}

	return true;
}

}