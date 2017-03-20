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
		�������Ƿ����
		����δ�����ķ���5�����ڲ��ټ�����״̬��ֱ����Ϊ�Ƿ���δ����
	*/
	bool IsReady();
	bool IsEnd();//���ݽ������
	void Close();

protected:
	ResultCode::ResultCode Query(msg::Buffer *msg, int millSecond);
	ResultCode::ResultCode Send(char *msg, int size);
	ResultCode::ResultCode ReadMsg(msg::Buffer *msg, int millSecond);

private:
	bool Connect();


private:
	bool			isConnected;
	net::Socket		m_svr;			//����������
	std::string		svrIp;
	int				svrPort;
	time_t			m_lastConnectFailed;//���һ������ʧ��ʱ��
};

}
