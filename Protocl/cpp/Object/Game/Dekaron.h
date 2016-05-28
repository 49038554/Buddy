#ifndef DEKARON_H
#define DEKARON_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//挑战
class Dekaron : public Buffer
{
public:
	//请求参数
	int						m_battleId;	// 战斗Id Game服务器统一设置
	unsigned int			m_playerId;	// 被挑战玩家id
	std::string				m_nick;		// 挑战者昵称
	std::vector<data::PET>	m_pet;		// 挑战者宠物
	//回应参数

public:
	Dekaron();
	virtual ~Dekaron();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //DEKARON_H
