#ifndef __ID_LIST_H__
#define __ID_LIST_H__

#include "Protocl/cpp/base/Stream.h"
#include <vector>

namespace Cache 
{

//id列表最多1万人
class IdList : public net::Stream
{
public:
	std::vector<int>	m_ids;

public:
	IdList();
	virtual ~IdList();

	bool Build();  // 构造
	bool Parse();  // 解析

private:
	unsigned char *m_buffer;
};

}

#endif // __ID_LIST_H__