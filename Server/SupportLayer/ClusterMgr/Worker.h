#pragma once

#include "mdk_ex/ServerEx.h"
#include "mdk/Logger.h"
#include "mdk/ConfigFile.h"
#include "Protocl/cpp/Buffer.h"

class Worker : public mdk::ServerEx
{
	friend int main(int argc, char* argv[]);
public:
	Worker(void);
	virtual ~Worker(void);
	
	virtual void OnConnect(mdk::NetHost &host);
	virtual void OnCloseConnect(mdk::NetHost &host);
	virtual void OnMsg(mdk::NetHost &host);

protected:
	bool OnGetCluster(mdk::NetHost &host, msg::Buffer &buffer);

private:
	mdk::Logger		m_log;
	mdk::ConfigFile	m_cfg;
};

