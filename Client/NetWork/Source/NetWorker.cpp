#include "NetWorker.h"
#include "mdk/mapi.h"

NetWorker::NetWorker(void)
{
	net::Socket::SocketInit();
	m_running = false;
}

NetWorker::~NetWorker(void)
{
}

bool NetWorker::Connect( int svrType, const std::string &ip, int port )
{
	if ( m_services.end() != m_services.find(svrType) ) return false;
	m_services[svrType].svrIp = ip;
	m_services[svrType].svrPort = port;
	m_services[svrType].isConnected = false;
	m_services[svrType].svr.Init(net::Socket::tcp);

	return true;
}

bool NetWorker::Start()
{
	m_running = true;
	m_mainThread.Run(mdk::Executor::Bind(&NetWorker::Main), this, NULL);
	m_mainThread.WaitStop();
	return false;
}

bool NetWorker::Stop()
{
	m_running = false;
	if ( !m_sigExit.Wait(3000) )
	{
		m_mainThread.Stop();
	}
	return false;
}

void* NetWorker::Main(void *param)
{
	msg::Buffer buffer;
	int list[65];
	int count;

	std::map<int, SVR>::iterator it;
	while ( m_running )
	{
		//�Զ����������������������
		count = 0;
		for ( it = m_services.begin(); it != m_services.end(); it++ )
		{
			if ( it->second.isConnected ) 
			{
				list[count++] = it->second.svr.GetSocket();
				continue;
			}
			if ( it->second.svr.Connect(it->second.svrIp.c_str(), it->second.svrPort) )
			{
				list[count++] = it->second.svr.GetSocket();
				it->second.isConnected = true;
				OnConnect(it->first, it->second.svr);
			}
		}
		//�ȴ�����
		net::Socket::WaitData( list, count, 5, 0 );

		//��������
		int i = 0;
		for ( ; i < count; i++ )
		{
			for ( it = m_services.begin(); it != m_services.end(); it++ )
			{
				if ( list[i] == it->second.svr.GetSocket() ) break;
			}
			if ( it == m_services.end() ) continue;

			int result = it->second.svr.Receive(buffer, buffer.HeaderSize(), true);
			if ( 0 > result )
			{
				it->second.isConnected = false;
				it->second.svr.Close();
				it->second.svr.Init(net::Socket::tcp);
				OnClose(it->first);
				continue;
			}
			if ( 0 == result ) continue;
			result = it->second.svr.Receive(buffer, buffer.Size());
			if ( 0 > result )
			{
				it->second.isConnected = false;
				it->second.svr.Close();
				it->second.svr.Init(net::Socket::tcp);
				OnClose(it->first);
				continue;
			}
			OnMsg(it->first, it->second.svr, buffer);
		}
	}
	m_sigExit.Notify();
	return NULL;
}

int NetWorker::Svr(int svrType)
{
	if ( m_services.end() == m_services.find(svrType) ) return -1;
	return m_services[svrType].svr.GetSocket();
}

void NetWorker::OnConnect(int svrType, net::Socket &svr)
{
	return;
}

void NetWorker::OnClose(int svrType)
{
	return;
}

void NetWorker::OnMsg(int svrType, net::Socket &svr, msg::Buffer &buffer)
{
	return;
}