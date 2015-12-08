#ifndef __ID_LIST_H__
#define __ID_LIST_H__

#include "Protocl/cpp/base/Stream.h"
#include <vector>

namespace Cache 
{

//id�б����1����
class IdList : public net::Stream
{
public:
	std::vector<int>	m_ids;

public:
	IdList(void);
	virtual ~IdList(void);

	bool Build(void);  // ����
	bool Parse(void);  // ����

private:
	unsigned char *m_buffer;
};

}

#endif // __ID_LIST_H__