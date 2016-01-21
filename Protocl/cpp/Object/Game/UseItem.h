#ifndef USEITEM_H
#define USEITEM_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//使用物品
class UseItem : public Buffer
{
public:
	//请求参数
	short			m_itemId;		//物品id
	unsigned int	m_objectId;		//对象id

	//回应参数

public:
	UseItem();
	virtual ~UseItem();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //USEITEM_H
