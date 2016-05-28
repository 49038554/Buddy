#ifndef ROUNDREADY_H
#define ROUNDREADY_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//回合准备
class RoundReady : public Buffer
{
public:
	//请求参数
	int					m_battleId;		//战斗Id
	char				m_action;		//动作
	int					m_gameObjId;		//对象id
	data::RAND_PARAM	m_rp;			//随机数

	//回应参数

public:
	RoundReady();
	virtual ~RoundReady();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //ROUNDREADY_H
