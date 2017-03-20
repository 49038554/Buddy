#pragma once

#include "Socket.h"
#include "../Buffer.h"
#include <string>
#include <map>
#include <cstdlib>
#include <cstdio>

namespace net
{

class Client 
{
public:
	Client(void);
	virtual ~Client(void);

	void SetService(const std::string &ip, int port );
	/*
		检查服务是否就绪
		对于未就绪的服务，5分种内不再检查服务状态，直接认为是服务未就绪
	*/
	bool IsReady();
	bool IsEnd();//数据接收完成
	void Close();

protected:
	ResultCode::ResultCode Query(msg::Buffer *msg, int millSecond);
	ResultCode::ResultCode Send(char *msg, int size);
	ResultCode::ResultCode ReadMsg(msg::Buffer *msg, int millSecond);

private:
	bool Connect();


private:
	bool			isConnected;
	net::Socket		m_svr;			//服务器连接
	std::string		svrIp;
	int				svrPort;
	time_t			m_lastConnectFailed;//最后一次连接失败时间
};

}
