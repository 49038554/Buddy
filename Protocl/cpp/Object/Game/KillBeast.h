#ifndef KILLBEAST_H
#define KILLBEAST_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <map>

namespace msg
{

//杀死野怪
class KillBeast : public Buffer
{
public:
	//请求参数
	short						m_killPetNo;			//被杀巴迪兽全国编号
	std::map<unsigned int, unsigned int>	m_petIds;	//出场巴迪兽Id
	//回应参数

public:
	KillBeast();
	virtual ~KillBeast();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //KILLBEAST_H
