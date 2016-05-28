#ifndef CATCHSTAR_H
#define CATCHSTAR_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//地图技能：摘星术
class CatchStar : public Buffer
{
public:
	//请求参数
	//回应参数
	int					m_itemId;		//道具id

public:
	CatchStar();
	virtual ~CatchStar();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CATCHSTAR_H
