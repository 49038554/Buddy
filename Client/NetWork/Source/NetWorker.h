#ifndef NET_WORKER_H
#define NET_WORKER_H

#include "mdk/Thread.h"
#include "mdk/Signal.h"

#include "Protocl/cpp/base/Socket.h"
#include "Protocl/cpp/Buffer.h"
#include <string>
#include <map>
#include <cstdlib>
#include <cstdio>

//����ҵ����
class NetWorker
{
public:
	NetWorker(void);
	virtual ~NetWorker(void);

	bool Connect( int svrType, const std::string &ip, int port );
	bool Start();
	bool Stop();
	void* RemoteCall Main(void *param);

protected:
	virtual void OnConnect(int svrType, net::Socket &svr);
	virtual void OnClose(int svrType);
	virtual void OnMsg(int svrType, net::Socket &svr, msg::Buffer &buffer);
	int Svr(int svrType);

private:
	bool			m_running;
	mdk::Thread		m_mainThread;
	mdk::Signal		m_sigExit;		//�˳��ź�
	net::Socket		m_svr;			//����������
	typedef struct SVR
	{
		net::Socket		svr;
		std::string		svrIp;
		int				svrPort;
		bool			isConnected;
	}SVR;
	std::map<int, SVR>		m_services;//����������map<���������ͣ�socket>
};


#endif//NET_WORKER_H