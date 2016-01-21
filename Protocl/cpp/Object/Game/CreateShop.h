#ifndef CREATESHOP_H
#define CREATESHOP_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//开店
class CreateShop : public Buffer
{
public:
	//请求参数
	std::string		m_longitude;	//经度
	std::string		m_latitude;		//纬度
	std::string		m_address;		//地址

	//回应参数

public:
	CreateShop();
	virtual ~CreateShop();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CREATESHOP_H
