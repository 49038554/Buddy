#ifndef SYNCPLAYER_H
#define SYNCPLAYER_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//同步正能量
class SyncPlayer : public Buffer
{
public:
	//请求参数
	data::PLAYER	m_player;

	//回应参数

public:
	SyncPlayer();
	virtual ~SyncPlayer();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SYNCCOIN_H
