#ifndef SYNCCOIN_H
#define SYNCCOIN_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//同步正能量
class SyncCoin : public Buffer
{
public:
	//请求参数
	int			m_count;	//数量

	//回应参数

public:
	SyncCoin();
	virtual ~SyncCoin();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SYNCCOIN_H
