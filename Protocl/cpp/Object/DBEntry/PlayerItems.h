#ifndef PLAYERITEMS_H
#define PLAYERITEMS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//玩家物品
class PlayerItems : public Buffer
{
public:
	//回应参数
	std::vector<data::PLAYER_ITEM>	m_items;//最大200种

public:
	PlayerItems();
	virtual ~PlayerItems();
	bool Build( bool isResult = false );//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PLAYERITEMS_H
