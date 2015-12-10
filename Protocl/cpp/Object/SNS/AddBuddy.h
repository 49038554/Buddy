#pragma once
#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//添加伙伴
class AddBuddy : public Buffer
{
public:
	//请求参数
	unsigned int	m_buddyId;//伙伴Id
	unsigned int	m_userId;//用户Id	//SNS服务填写
	std::string		m_nickName;//昵称	//SNS服务填写
	char			m_step;//当前步骤
	//0 == m_step参数
	std::string		m_msg;//消息
	//1 == m_step参数
	bool			m_accepted;//接受
	//回应参数

public:
	AddBuddy();
	virtual ~AddBuddy();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
