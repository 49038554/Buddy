#ifndef SYNCITEM_H
#define SYNCITEM_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//同步物品
class SyncItem : public Buffer
{
public:
	//请求参数
	std::vector<data::PLAYER_ITEM>	m_items;//同步物品,最大500

	//回应参数

public:
	SyncItem();
	virtual ~SyncItem();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SYNCITEM_H
