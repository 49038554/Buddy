#ifndef CREATEPLAYER_H
#define CREATEPLAYER_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//交易
class CreatePlayer : public Buffer
{
public:
	//请求参数

	//回应参数

public:
	CreatePlayer();
	virtual ~CreatePlayer();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CREATEPLAYER_H
