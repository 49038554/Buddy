#ifndef PICK_H
#define PICK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//摘果实
class Pick : public Buffer
{
public:
	//请求参数
	int					m_treeId;			//树id
	std::vector<short>	m_itemId;			//道具id

	//回应参数
	int					m_nextTime;			//下个果实成熟时间

public:
	Pick();
	virtual ~Pick();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PICK_H
