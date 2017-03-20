#pragma once
#include "../../Buffer.h"

namespace msg
{

//取集群信息
class GetCluster : public Buffer
{
public:
	//请求参数
	Moudle::Moudle	m_moudle;//要取的服务的类型
	//回应参数

public:
	GetCluster();
	virtual ~GetCluster();
	bool Build( bool isResult = false );//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
