#include "Client.h"

namespace net
{

Client::Client(void)
{
	net::Socket::SocketInit();
	svrIp = "";
	svrPort = 0;
	isConnected = false;
	m_lastConnectFailed = 0;
}

Client::~Client(void)
{
}

void Client::SetService(const std::string &ip, int port )
{
	svrIp = ip;
	svrPort = port;
}

bool Client::IsReady()
{
	if ( !isConnected )
	{
		if ( 300 > time(NULL) - m_lastConnectFailed ) return false;
	}
	if ( Connect() ) return true;
	m_lastConnectFailed = time(NULL);

	return false;
}

bool Client::Connect()
{
	if ( isConnected ) return true;

	if ( !m_svr.Init(net::Socket::tcp) ) return false;
	if ( !m_svr.Connect(svrIp.c_str(), svrPort) ) return false;
	m_svr.SetSockMode(true);
	m_svr.SetNoDelay(true);
	isConnected = true;

	return true;
}

void Client::Close()
{
	if ( !isConnected ) return;
	m_svr.Close();
	isConnected = false;
}

ResultCode::ResultCode Client::Query(msg::Buffer *msg, int millSecond)
{
	if ( !msg->Build() ) return ResultCode::paramError;
	ResultCode::ResultCode result = Send(*msg, msg->Size());
	if ( ResultCode::success != result ) return result;
	return ReadMsg(msg, millSecond);
}

ResultCode::ResultCode Client::Send(char *msg, int size)
{
	if ( !Connect() ) return ResultCode::netError;

	if ( 0 > m_svr.Send(msg, size) )
	{
		Close();//发送失败必须断开连接，以免下次请求时，接收到上次未接收的数据
		//由于心跳可能丢失，重试1次
		if ( !Connect() ) return ResultCode::netError;
		if ( 0 > m_svr.Send(msg, size) ) 
		{
			Close();//发送失败必须断开连接，以免下次请求时，接收到上次未接收的数据
			return ResultCode::netError;
		}
	}

	return ResultCode::success;
}

ResultCode::ResultCode Client::ReadMsg(msg::Buffer *msg, int millSecond)
{
	int ret;
	int size = msg->HeaderSize();
	char *buf = *msg;
	int pos = 0;
	while ( 0 < size )
	{
		ret = m_svr.Receive(&buf[pos], size, true, millSecond);
		if ( 0 > ret )
		{
			Close();//接收失败必须断开连接，以免下次请求时，接收到上次未接收的数据
			return ResultCode::netError;
		}
		size -= ret;
		pos += ret;
	}
	if ( !msg->ReadHeader() ) 
	{
		return ResultCode::msgError;
	}
	size = msg->Size();
	pos = 0;
	while ( 0 < size )
	{
		ret = m_svr.Receive(&buf[pos], size, false, millSecond);
		if ( 0 > ret )//接收失败必须断开连接，以免下次请求时，接收到上次未接收的数据
		{
			Close();
			return ResultCode::netError;
		}
		size -= ret;
		pos += ret;
	}

	if ( !msg->Parse() ) 
	{
		return ResultCode::msgError;
	}
 	if ( ResultCode::success != msg->m_code ) return msg->m_code;
	
	return ResultCode::success;
}

bool Client::IsEnd()
{
	char buf[256];
	int count = m_svr.Receive(buf, 1, true, 0, 1);
	if ( 1 == count ) return false;

	return true;
}

}