#ifndef PETS_H
#define PETS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <map>
#include <vector>

namespace msg
{

//宠物数据
class Pets : public Buffer
{
public:
	//回应参数
	std::vector<data::PET> m_pets;//最大100只
public:
	Pets();
	virtual ~Pets();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PETS_H
