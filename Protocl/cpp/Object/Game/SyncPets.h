#ifndef SYNCPETS_H
#define SYNCPETS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <map>
#include <vector>

namespace msg
{

//添加宠物
class SyncPets : public Buffer
{
public:
	//请求参数
	std::vector<data::PET> m_pets;//最大100只，回应时data::PET.itemId < 0表示同步失败
	//回应参数
public:
	SyncPets();
	virtual ~SyncPets();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SYNCPETS_H
