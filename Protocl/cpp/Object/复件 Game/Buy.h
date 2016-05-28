#ifndef BUY_H
#define BUY_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//购买
class Buy : public Buffer
{
public:
	//请求参数
	int					m_itemId;		//道具id
	int					m_count;		//数量
	unsigned int		m_SellerId;		//卖家Id，0系统
	//回应参数

public:
	Buy();
	virtual ~Buy();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //BUY_H
