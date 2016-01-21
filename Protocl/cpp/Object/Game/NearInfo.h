#ifndef NEARINFO_H
#define NEARINFO_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//使用物品
class NearInfo : public Buffer
{
public:
	//请求参数
	char			m_type;		//信息类型

	//回应参数

public:
	NearInfo();
	virtual ~NearInfo();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //NEARINFO_H
