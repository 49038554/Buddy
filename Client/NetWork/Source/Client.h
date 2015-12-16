#ifndef CLIENT_H
#define CLIENT_H

#include "NetWorker.h"

class Client : public NetWorker
{
public:
	Client(void);
	virtual ~Client(void);

	virtual void OnConnect(int svrType, net::Socket &svr);
	virtual void OnClose(int svrType);
	virtual void OnMsg(int svrType, net::Socket &svr, msg::Buffer &buffer);
};

#endif //CLIENT_H