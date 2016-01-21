#ifndef DEVOUR_H
#define DEVOUR_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//地图技能：吞噬
class Devour : public Buffer
{
public:
	//请求参数
	int				m_itemId;		//道具id

	//回应参数
	int				m_coin;			//得到虚拟币数量

public:
	Devour();
	virtual ~Devour();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //DEVOUR_H
