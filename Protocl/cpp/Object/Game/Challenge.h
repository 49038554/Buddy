#ifndef CHALLENGE_H
#define CHALLENGE_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//应战
class Challenge : public Buffer
{
public:
	//请求参数
	int						m_battleId;	// 战斗Id 从Dekaron复制
	unsigned int			m_playerId;	// 挑战玩家id
	bool					m_accepted;	// 接受挑战
	std::string				m_dNick;	// 挑战者昵称
	std::vector<data::PET>	m_dPet;		// 挑战者宠物
	std::string				m_cNick;	// 应战者昵称
	std::vector<data::PET>	m_cPet;		// 应战者宠物
	//回应参数

public:
	Challenge();
	virtual ~Challenge();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CHALLENGE_H
