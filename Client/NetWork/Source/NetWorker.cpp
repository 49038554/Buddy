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

void NetWorker::Main()
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
	m_mainThread.Run(mdk::Executor::Bind(&NetWorker::TMain), this, NULL);
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

void* NetWorker::TMain(void *param)
{
	msg::Buffer buffer;
	int list[65];
	int count;

	std::map<int, SVR>::iterator it;
	while ( m_running )
	{
		Main();
		//自动重连服务器，并加入监听
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
		//等待数据
		net::Socket::WaitData( list, count, 5, 0 );

		//处理数据
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
			if ( !buffer.Parse() ) continue;
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

void NetWorker::Close(int svrType)
{
	if ( m_services.end() == m_services.find(svrType) ) return;
	m_services[svrType].isConnected = false;
	m_services[svrType].svr.Close();
	m_services[svrType].svr.Init(net::Socket::tcp);
	OnClose(svrType);
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

NetTask* NetWorker::CreateTask(int size)
{
	int i = 2;
	for ( ; i < size; ) i *= 2;
	size = i;

	NetTask *pTask = NULL;
	mdk::AutoLock lock(&m_lockPool);
	std::vector<NetTask*>::iterator it = m_taskPool.begin();
	for ( ; it != m_taskPool.end(); it++)
	{
		if ( size > (*it)->spaceSize ) continue;

		pTask = *it;
		m_taskPool.erase(it);
		return pTask;
	}

	pTask = new NetTask();
	if ( NULL == pTask ) return NULL;
	pTask->spaceSize = size;
	pTask->pData = new char[pTask->spaceSize];
	if ( NULL == pTask->pData )
	{
		delete pTask;
		pTask = NULL;
	}
	return pTask;
}

void NetWorker::ReleaseTask(NetTask *pTask)
{
	mdk::AutoLock lock(&m_lockPool);
	if ( 100 <= m_taskPool.size() )
	{
		delete pTask;
		return;
	}
	m_taskPool.push_back(pTask);

	return;
}
