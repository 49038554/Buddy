#include "Client.h"

#include "common/MD5Helper.h"
#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "Protocl/cpp/Object/Auth/UserLogin.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/UserRelogin.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "Protocl/cpp/Object/Auth/UserLogout.h"

#include "Protocl/cpp/Object/Notify/GetEvent.h"

#include "Protocl/cpp/Object/SNS/AddBuddy.h"

Client::Client(void)
{
}

Client::~Client(void)
{
}

void Client::Main()
{
}

void Client::OnConnect(int svrType, net::Socket &svr)
{

}

void Client::OnClose(int svrType)
{
	m_user.logined = false;
}

void Client::OnMsg(int svrType, net::Socket &svr, msg::Buffer &buffer)
{
	if ( Moudle::Auth == buffer.MoudleId() ) OnAuth(buffer);
	if ( Moudle::SNS == buffer.MoudleId() ) OnSNS(buffer);
}

bool Client::Register(bool isMobile, const std::string &account, const std::string &pwd)
{
	if ( m_user.logined ) return false;

	m_user.isMobileLogin = isMobile;
	m_user.account = account;
	m_user.pwd = pwd;
	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::UserRegister msg;
	msg.m_accountType = isMobile?(AccountType::mobile):(AccountType::imei);
	msg.m_account = account;       
	MD5Helper md5;
	std::string md5Pwd = md5.HashString(pwd.c_str(), pwd.size());
	msg.m_pwd = md5Pwd;
	msg.Build();
	svr.Send(msg, msg.Size());
	
	return true;
}

void Client::OnRegister(msg::Buffer &buffer)
{
	msg::UserRegister msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( ResultCode::Success != msg.m_code )
	{
		printf( "注册失败：%s\n", msg.m_reason.c_str() );
		return;
	}
	printf( "注册成功:user(%u)\n", msg.m_userId );
	Login(m_user.isMobileLogin, m_user.account, m_user.pwd);
}

bool Client::Login(bool isMobile, const std::string &account, const std::string &pwd)
{
	if ( m_user.logined ) return false;

	printf( "正在登录...\n" );
	m_user.isMobileLogin = isMobile;
	m_user.account = account;
	m_user.pwd = pwd;
	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::UserLogin msg;
	msg.m_clientType = ClientType::android;
	msg.m_accountType = isMobile?(AccountType::mobile):(AccountType::imei);
	msg.m_account = account;
	MD5Helper md5;
	std::string md5Pwd = md5.HashString(pwd.c_str(), pwd.size());
	msg.m_pwd = md5Pwd;
	msg.Build();
	svr.Send(msg, msg.Size());

	return true;
}

void Client::OnLogin(msg::Buffer &buffer)
{
	msg::UserLogin msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( ResultCode::Success != msg.m_code )
	{
		printf( "登录失败：%s\n", msg.m_reason.c_str() );
		return;
	}
	m_user.id = msg.m_userId;
	m_user.logined = true;
	printf( "user(%u)已登录\n", msg.m_userId );
	GetEvent();
}

bool Client::BindPhone(const std::string &moblie)
{
	if ( !m_user.logined ) return false;

	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::BindingPhone msg;
	msg.m_phone = moblie;
	msg.Build();
	svr.Send(msg, msg.Size());

	return true;
}

void Client::OnBindPhone(msg::Buffer &buffer)
{
	msg::BindingPhone msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( ResultCode::Success != msg.m_code )
	{
		printf( "绑定失败：%s\n", msg.m_reason.c_str() );
		return;
	}
	printf( "user(%u)绑定手机%s\n", m_user.id, msg.m_phone.c_str() );
	return;
}

void Client::OnRelogin(msg::Buffer &buffer)
{
	msg::UserRelogin msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	printf( "user(%u)异地登录：登录位置%s\n", m_user.id, msg.m_position.c_str() );
	Close(TcpSvr);
	return;
}

bool Client::SetPassword(const std::string pwd)
{
	if ( !m_user.logined ) return false;

	m_user.newPwd = pwd;
	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::ResetPassword msg;
	MD5Helper md5;
	std::string md5Pwd = md5.HashString(pwd.c_str(), pwd.size());
	msg.m_pwd = md5Pwd;
	msg.Build();
	svr.Send(msg, msg.Size());

	return true;
}

void Client::OnResetPassword(msg::Buffer &buffer)
{
	msg::ResetPassword msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( ResultCode::Success != msg.m_code )
	{
		printf( "修改密码失败：%s\n", msg.m_reason.c_str() );
		return;
	}
	printf( "修改密码成功：%s\n" );
	m_user.pwd = m_user.newPwd;

	return;
}


void Client::OnAuth(msg::Buffer &buffer)
{
	switch ( buffer.Id() )
	{
	case MsgId::userRegister :
		OnRegister(buffer);
		break;
	case MsgId::userLogin :
		OnLogin(buffer);
		break;
	case MsgId::bindingPhone :
		OnBindPhone(buffer);
		break;
	case MsgId::userRelogin :
		OnRelogin(buffer);
		break;
	case MsgId::resetPassword :
		OnResetPassword(buffer);
		break;
	default :
		break;
	}
}

bool Client::AddBuddy(unsigned int buddyId, const std::string &talk)
{
	if ( !m_user.logined ) return false;

	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::AddBuddy msg;
	msg.m_buddyId = buddyId;//伙伴Id
	msg.m_step = 0;//当前步骤
	msg.m_msg = talk;//消息
	msg.Build();
	svr.Send(msg, msg.Size());

	return true;
}

void Client::OnAddBuddy(msg::Buffer &buffer)
{
	msg::AddBuddy msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( 1 == msg.m_step )
	{
		if ( msg.m_accepted ) printf( "%d同意与你成为好友\n", msg.m_userId );
		else printf( "%d拒绝与你成为好友:%s\n", msg.m_userId, msg.m_msg.c_str() );
		return;
	}

	if ( ResultCode::Success != msg.m_code )
	{
		printf( "添加好友失败：%s\n", msg.m_reason.c_str() );
		return;
	}
	if ( m_user.id == msg.m_userId )
	{
		printf( "错误：自己发给自己\n" );
		return;
	}
	if ( m_user.id != msg.m_buddyId )
	{
		printf( "错误：请求添加别人\n" );
		return;
	}
	printf( "%d请求添加你为好友\n", msg.m_userId );
	
	return;
}

bool Client::AcceptBuddy(unsigned int buddyId, const std::string &talk, bool accept)
{
	if ( !m_user.logined ) return false;

	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::AddBuddy msg;
	msg.m_buddyId = buddyId;//伙伴Id
	msg.m_step = 1;//当前步骤
	msg.m_accepted = accept;
	msg.m_msg = talk;//消息
	msg.Build();
	svr.Send(msg, msg.Size());

	return true;
}

bool Client::GetEvent()
{
	if ( !m_user.logined ) return false;

	printf( "user(%u) 查询离线消息...\n", m_user.id );
	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::GetEvent msg;
	msg.Build();
	svr.Send(msg, msg.Size());

	return true;
}

void Client::OnSNS(msg::Buffer &buffer)
{
	switch ( buffer.Id() )
	{
	case MsgId::addBuddy :
		OnAddBuddy(buffer);
		break;
	default:
		break;
	}
}

