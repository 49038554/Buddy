#include "DBInterFace.h"

/**
 * DBConnect
 */
DBConnect::DBConnect(void)
	: m_pSocket(new net::Socket)
	, m_strIP("")
	, m_nPort(0)
	, m_bIsConnected(false)
{
}

DBConnect::~DBConnect(void)
{
	if (m_pSocket != NULL)
	{
		Close();
		delete m_pSocket;
		m_pSocket = NULL;
	}
}

bool DBConnect::CreateUser(msg::UserRegister& msg)
{
	return doLogic(msg);
}

bool DBConnect::ResetUserPwd(msg::ResetPassword& msg)
{
	return doLogic(msg);
}

bool DBConnect::BindingPhone(msg::BindingPhone& msg)
{
	return doLogic(msg);
}

void DBConnect::SetServerInfo(const std::string& strIP, int nPort)
{
	m_strIP = strIP;
	m_nPort = nPort;
}

void DBConnect::GetServerInfo(std::string& strIP, int& nPort)
{
	strIP = m_strIP;
	nPort = m_nPort;
}

bool DBConnect::Connect(void)
{
	if (m_bIsConnected) return true;
	if (! net::Socket::SocketInit()) return false;
	if (! m_pSocket->Init(net::Socket::tcp)) return false;
	if (! m_pSocket->Connect(m_strIP.c_str(), m_nPort)) return false;

	m_pSocket->SetSockMode(true);
	m_pSocket->SetNoDelay(true);

	m_bIsConnected = true;

	return true;
}

void DBConnect::Close(void)
{
	m_pSocket->Close();
	m_bIsConnected = false;
}

bool DBConnect::Send(msg::Buffer& msg)
{
	return (m_pSocket->Send(msg, msg.Size()) >= 0);
}

bool DBConnect::Receive(msg::Buffer& msg)
{
	msg.ReInit();

	if (m_pSocket->Receive(msg, msg.HeaderSize(), true, 3) < 0) return false;
	if (msg.Size() < 0) return false;
	if (m_pSocket->Receive(msg, msg.Size(), false, 3) < 0) return false;

	return true;
}

bool DBConnect::doLogic(msg::Buffer& msg)
{
	if (! Connect()) 
	{
		msg.m_code = ResultCode::SvrError;
		msg.m_reason = "数据中心服务器忙";
		msg.Build(true);
		return false;
	}
	if (! Send(msg))
	{
		Close();
		msg.m_code = ResultCode::SvrError;
		msg.m_reason = "数据中心服务器忙";
		msg.Build(true);
		return false;
	}
	if (! Receive(msg))
	{
		Close();
		msg.m_code = ResultCode::SvrError;
		msg.m_reason = "数据中心服务器忙";
		msg.Build(true);
		return false;
	}
	if ( !msg.Parse() )
	{
		Close();
		msg.m_code = ResultCode::SvrError;
		msg.m_reason = "数据中心服务回应非法报文";
		msg.Build(true);
		return false;
	}

	return true;
}

/**
 * DBInterface
 */
DBInterface::DBInterface(void)
	: m_strIP("")
	, m_nPort(0)
{
}

DBInterface::~DBInterface(void)
{
	for (int idx = 0; idx != m_pSockets.size(); ++idx)
	{
		delete m_pSockets[idx];
		m_pSockets[idx] = NULL;
	}
}

void DBInterface::SetServerInfo(const std::string& strIP, int nPort)
{
	m_strIP = strIP;
	m_nPort = nPort;
}

void DBInterface::GetServerInfo(std::string& strIP, int& nPort)
{
	strIP = m_strIP;
	nPort = m_nPort;
}

bool DBInterface::CreateUser(msg::UserRegister& msg)
{
	DBConnect* pDBNode = Alloc();
	if (NULL == pDBNode)
	{
		msg.m_code = ResultCode::SvrError;
		msg.m_reason = "DB线程安全对象初始化错误";
		msg.Build(true);
		return false;
	}

	if (! pDBNode->CreateUser(msg)) return false;

	Free(pDBNode);

	return true;
}

bool DBInterface::ResetUserPwd(msg::ResetPassword& msg)
{
	DBConnect* pDBNode = Alloc();
	if (NULL == pDBNode) 
	{
		msg.m_code = ResultCode::SvrError;
		msg.m_reason = "DB线程安全对象初始化错误";
		msg.Build(true);
		return false;
	}

	if (! pDBNode->ResetUserPwd(msg)) return false;

	Free(pDBNode);

	return true;
}

bool DBInterface::BindingPhone(msg::BindingPhone& msg)
{
	DBConnect* pDBNode = Alloc();
	if (NULL == pDBNode)
	{
		msg.m_code = ResultCode::SvrError;
		msg.m_reason = "DB线程安全对象初始化错误";
		msg.Build(true);
		return false;
	}

	if (! pDBNode->BindingPhone(msg)) return false;

	Free(pDBNode);

	return true;
}

DBConnect* DBInterface::Alloc(void)
{
	mdk::AutoLock lock(&m_pMutex);

	if (m_pSockets.empty())
	{
		DBConnect* pDBNode = new DBConnect;
		pDBNode->SetServerInfo(m_strIP, m_nPort);
		return pDBNode;
	}
	else
	{
		DBConnect* pDBNode = m_pSockets.back();
		m_pSockets.pop_back();
		return pDBNode;
	}
}

void DBInterface::Free(DBConnect* pDBNode)
{
	mdk::AutoLock lock(&m_pMutex);
	m_pSockets.push_back(pDBNode);
}
