#ifndef PLAYER_H
#define PLAYER_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//玩家数据
class Player : public Buffer
{
public:
	//回应参数
	int		m_coin;

public:
	Player();
	virtual ~Player();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PLAYER_H
