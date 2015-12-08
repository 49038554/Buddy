#include "SyncClient.h"
#include "Protocl/cpp/base/Socket.h"

SyncClient::SyncClient()
{
	m_client = new net::Socket();

	m_isConnected = false;
	m_isShortConnect = false;
}

SyncClient::~SyncClient()
{
	if ( NULL != m_client )
	{
		delete m_client;
		m_client = NULL;
	}
}

//设置服务器
void SyncClient::SetSvr( const std::string &ip, int port )
{
	m_ip = ip;
	m_port = port;
}

//设置连接模式长连接还是短连接，默认使用长连接
void SyncClient::SetConnectMode( bool shortConnect )
{
	m_isShortConnect = shortConnect;
}

bool SyncClient::Connect()
{
	if ( m_isConnected ) return true;
	if(!net::Socket::SocketInit())
	{
		return false;
	}

	net::Socket &client = *((net::Socket*)m_client);
	if ( !client.Init(net::Socket::tcp) ) return false;
	if ( !client.Connect(m_ip.c_str(), m_port) ) return false;
	client.SetSockMode(true);
	client.SetNoDelay(true);
	m_isConnected = true;

	return true;
}

void SyncClient::Close( bool force )
{
	if ( m_isShortConnect && !force ) return;

	net::Socket &client = *((net::Socket*)m_client);
	client.Close();
	m_isConnected = false;

	return;
}

bool SyncClient::RecvData( msg::Buffer &msg )
{
	net::Socket &client = *((net::Socket*)m_client);
	msg.ReInit();
	if  ( 0 > client.Receive(msg, msg.HeaderSize(), true, 3) )
	{
		Close(true);
		return false;
	}
	if ( 0 > msg.Size() )
	{
		Close(true);
		return false;
	}
	if ( 0 > client.Receive(msg, msg.Size(), false, 3) )
	{
		Close(true);
		return false;
	}

	return true;
}

/*
	功能：调用库函数send发送数据
	参数：
		lpBuf	const void*	[In]	发送的数据
		nBufLen	int		[In]	数据长度
		nFlags	int		[In]	An indicator specifying the way in which the call is made
	返回值：成功实际发送的字节数，失败返回-1
*/
int SyncClient::Send( const void* lpBuf, int nBufLen, int nFlags )
{
	net::Socket &client = *((net::Socket*)m_client);
	return client.Send(lpBuf, nBufLen, nFlags);
}
