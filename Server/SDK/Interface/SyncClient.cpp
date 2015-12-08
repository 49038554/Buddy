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

//���÷�����
void SyncClient::SetSvr( const std::string &ip, int port )
{
	m_ip = ip;
	m_port = port;
}

//��������ģʽ�����ӻ��Ƕ����ӣ�Ĭ��ʹ�ó�����
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
	���ܣ����ÿ⺯��send��������
	������
		lpBuf	const void*	[In]	���͵�����
		nBufLen	int		[In]	���ݳ���
		nFlags	int		[In]	An indicator specifying the way in which the call is made
	����ֵ���ɹ�ʵ�ʷ��͵��ֽ�����ʧ�ܷ���-1
*/
int SyncClient::Send( const void* lpBuf, int nBufLen, int nFlags )
{
	net::Socket &client = *((net::Socket*)m_client);
	return client.Send(lpBuf, nBufLen, nFlags);
}
