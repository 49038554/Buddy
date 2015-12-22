#include "DBCenter.h"

//将一个整数存储到字节流buf，按照小端字节序(低位在前，高位在后)
static void itomemSmall( unsigned char *buf, uint64 value, int size )
{
	int move = (size-1) * 8;
	int del = (size-1) * 8;
	int i = 0;
	for ( i = 0; i < size; i++ )
	{
		buf[i] = (char)((value << move) >> del);
		move -= 8;
	}
}

//将buf字节流专程整数，按照小端字节序(低位在前，高位在后)
static uint64 memtoiSmall( unsigned char *buf, int size )
{
	uint64 value = 0;
	int i = 0;
	value += buf[size - 1];
	for ( i = size - 2; 0 <= i; i-- )
	{
		value <<= 8;
		value += buf[i];
	}
	return value;
}


/**
 * DBCenter
 */
DBCenter::DBCenter(void)
	: m_pSocket(new net::Socket)
	, m_strIP("")
	, m_nPort(0)
	, m_bIsConnected(false)
{
}

DBCenter::~DBCenter(void)
{
	if (m_pSocket != NULL)
	{
		Close();
		delete m_pSocket;
		m_pSocket = NULL;
	}
}

bool DBCenter::CreateUser(msg::UserRegister& msg)
{
	return doLogic(msg);
}

bool DBCenter::ResetUserPwd(msg::ResetPassword& msg)
{
	return doLogic(msg);
}

bool DBCenter::BindingPhone(msg::BindingPhone& msg)
{
	return doLogic(msg);
}

void DBCenter::SetServerInfo(const std::string& strIP, int nPort)
{
	m_strIP = strIP;
	m_nPort = nPort;
}

void DBCenter::GetServerInfo(std::string& strIP, int& nPort)
{
	strIP = m_strIP;
	nPort = m_nPort;
}

bool DBCenter::Connect(void)
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

void DBCenter::Close(void)
{
	m_pSocket->Close();
	m_bIsConnected = false;
}

bool DBCenter::Send(msg::Buffer& msg)
{
	return (m_pSocket->Send(msg, msg.Size()) >= 0);
}

bool DBCenter::Receive(msg::Buffer& msg)
{
	msg.ReInit();

	if (m_pSocket->Receive(msg, msg.HeaderSize(), true, 3) < 0) return false;
	if (msg.Size() < 0) return false;
	if (m_pSocket->Receive(msg, msg.Size(), false, 3) < 0) return false;

	return true;
}

bool DBCenter::doLogic(msg::Buffer& msg)
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

bool DBCenter::AddBuddy(msg::AddBuddy& msg)
{
	return doLogic(msg);
}

bool DBCenter::DelBuddy(msg::DelBuddy& msg)
{
	return doLogic(msg);
}

bool DBCenter::SetUserData(msg::SetUserData& msg)
{
	return doLogic(msg);
}

