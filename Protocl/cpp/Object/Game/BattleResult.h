#ifndef BATTLERESULT_H
#define BATTLERESULT_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//购买
class BattleResult : public Buffer
{
public:
	//请求参数
	int					m_winnerId;		//胜方id
	int					m_loserId;		//负方id

	//回应参数

public:
	BattleResult();
	virtual ~BattleResult();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //BATTLERESULT_H
