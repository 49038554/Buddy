#ifndef TAMEBEAST_H
#define TAMEBEAST_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//驯服野怪
class TameBeast : public Buffer
{
public:
	//请求参数
	unsigned int	m_petId;		//宠物id
	short			m_buddyNo;		//巴迪兽全国编号
	std::string		m_nickname;		//昵称
	std::string		m_longitude;	//经度
	std::string		m_latitude;		//纬度
	std::string		m_address;		//地址
	//回应参数

public:
	TameBeast();
	virtual ~TameBeast();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}

#endif //TAMEBEAST_H