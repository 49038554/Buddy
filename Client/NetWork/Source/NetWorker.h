#ifndef NET_WORKER_H
#define NET_WORKER_H

#include "mdk/Thread.h"
#include "mdk/Signal.h"
#include "mdk/Lock.h"

#include "Protocl/cpp/base/Socket.h"
#include "Protocl/cpp/Buffer.h"
#include <string>
#include <map>
#include <cstdlib>
#include <cstdio>
#include "NetTask.h"

//网络业务处理
class NetWorker
{
public:
	NetWorker(void);
	virtual ~NetWorker(void);

	bool Connect( int svrType, const std::string &ip, int port );
	bool Start();
	bool Stop();
	void* RemoteCall TMain(void *param);

protected:
	virtual void Main();
	virtual void OnConnect(int svrType, net::Socket &svr);
	virtual void OnClose(int svrType);
	virtual void OnMsg(int svrType, net::Socket &svr, msg::Buffer &buffer);
	int Svr(int svrType);
	void Close(int svrType);
	NetTask* CreateTask();
	void ReleaseTask(NetTask *pTask);
private:
	bool			m_running;
	mdk::Thread		m_mainThread;
	mdk::Signal		m_sigExit;		//退出信号
	net::Socket		m_svr;			//服务器连接
	typedef struct SVR
	{
		net::Socket		svr;
		std::string		svrIp;
		int				svrPort;
		bool			isConnected;
	}SVR;
	std::map<int, SVR>	m_services;//服务器连接map<服务器类型，socket>

	std::vector<NetTask*>	m_taskPool;
	mdk::Mutex			m_lockPool;
};


#endif//NET_WORKER_H