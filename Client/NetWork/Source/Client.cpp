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
#include "Protocl/cpp/Object/SNS/DelBuddy.h"
#include "Protocl/cpp/Object/SNS/GetBuddys.h"
#include "Protocl/cpp/Object/SNS/Buddys.h"
#include "Protocl/cpp/Object/SNS/Chat.h"
#include "Protocl/cpp/Object/SNS/SetUserData.h"
#include "Protocl/cpp/Object/SNS/GetUserData.h"
#include "Protocl/cpp/Object/SNS/UserData.h"

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
	if ( Moudle::DBEntry == buffer.MoudleId() ) OnSNS(buffer);
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

void Client::ClientInfo()
{
	system( "cls" );
	if ( !m_user.logined )	
	{
		printf( "请登录\n" );
		return;
	}
	printf( "用户%u\n", m_user.id );
	printf( "\t%s登录%s\n", m_user.isMobileLogin?"手机":"imei", m_user.account.c_str());
	printf( "\t密码%s\n", m_user.pwd.c_str() );
	printf( "\t小伙伴%d个:\n", m_user.buddys.size() );
	std::map<mdk::uint32, BUDDY_DATA>::iterator it;
	for ( it = m_user.buddys.begin(); it != m_user.buddys.end(); it++ )
	{
		printf( "\t\t%s(%u) face(%s)\n", it->second.nickName.c_str(), it->second.id, it->second.face.c_str() );
	}
	printf( "\n" );
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
	ClientInfo();
	printf( "user(%u)已登录\n", msg.m_userId );
	GetEvent();
	GameInit();
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
	ClientInfo();
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
	ClientInfo();
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
	m_user.pwd = m_user.newPwd;
	ClientInfo();
	printf( "修改密码成功,新密码：%s\n", m_user.pwd.c_str() );

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
		if ( msg.m_accepted ) 
		{
			m_user.buddys[msg.m_userId].id = msg.m_userId;
			m_user.buddys[msg.m_userId].nickName = msg.m_nickName;
			m_user.buddys[msg.m_userId].face = "---";
			ClientInfo();
			printf( "%d同意与你成为小伙伴\n", msg.m_userId );
		}
		else printf( "%d拒绝与你成为小伙伴:%s\n", msg.m_userId, msg.m_msg.c_str() );
		return;
	}

	if ( ResultCode::Success != msg.m_code )
	{
		printf( "添加小伙伴失败：%s\n", msg.m_reason.c_str() );
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
	printf( "%d请求添加你为小伙伴\n", msg.m_userId );
	
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

	if ( accept ) 
	{
		ClientInfo();
		printf( "接受\n" );
	}
	else printf( "拒绝\n" );

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

bool Client::DelBuddy( unsigned int buddyId )
{
	if ( !m_user.logined ) return false;
	if ( m_user.buddys.end() == m_user.buddys.find(buddyId) )
	{
		printf("错误：用户(%d)不是小伙伴，不能与之绝交\n", buddyId);
		return false;
	}

	printf( "user(%u)与小伙伴(%u)绝交...\n", m_user.id, buddyId );
	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::DelBuddy msg;
	msg.m_buddyId = buddyId;
	msg.Build();
	svr.Send(msg, msg.Size());

	return true;
}

void Client::OnDelBuddy(msg::Buffer &buffer)
{
	msg::DelBuddy msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( m_user.id == msg.m_buddyId )
	{
		if ( m_user.buddys.end() == m_user.buddys.find(msg.m_userId) )
		{
			printf("用户(%d)已与你绝交\n", msg.m_userId);
			return;
		}
		BUDDY_DATA buddy = m_user.buddys[msg.m_buddyId];
		m_user.buddys.erase(msg.m_buddyId);
		ClientInfo();
		printf( "%s(%d)已与你绝交\n", buddy.nickName.c_str(), buddy.id );
	}
	else
	{
		BUDDY_DATA buddy = m_user.buddys[msg.m_buddyId];
		m_user.buddys.erase(msg.m_buddyId);
		ClientInfo();
		printf( "你已与%s(%d)绝交\n", buddy.nickName.c_str(), buddy.id );
	}

	return;
}

bool Client::GetBuddys()
{
	if ( !m_user.logined ) return false;

	printf( "user(%u)取小伙伴...\n", m_user.id );
	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::GetBuddys msg;
	msg.Build();
	svr.Send(msg, msg.Size());

	return true;
}

void Client::OnBuddys(msg::Buffer &buffer)
{
	msg::Buddys msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( m_user.id != msg.m_userId )
	{
		printf( "小伙伴列表不属于自己\n" );
		return;
	}
	if ( msg::Buddys::buddyList == msg.m_type )
	{
		BUDDY_DATA buddy;
		int i = 0;
		for ( i = 0; i < msg.m_buddys.size(); i++ )
		{
			buddy.id = msg.m_buddys[i].id;
			buddy.nickName = msg.m_buddys[i].nickName;
			buddy.face = msg.m_buddys[i].face;
			m_user.buddys[buddy.id] = buddy;
		}
		ClientInfo();
	}
	printf( "刷新小伙伴列表\n" );

	return;
}

static char* RecvType(msg::Chat::RecvType recvType)
{
	if ( msg::Chat::RecvType::buddy == recvType ) return "小伙伴";
	if ( msg::Chat::RecvType::buddys == recvType ) return "所有小伙伴";
	if ( msg::Chat::RecvType::group == recvType ) return "分组";
	return "";
}

bool Client::Chat(unsigned int recverId, unsigned char recvType, const std::string &talk)
{
	if ( !m_user.logined ) return false;

	printf( "user(%u)发聊天消息到%s(%u)...\n", m_user.id, RecvType((msg::Chat::RecvType)recvType), recverId );
	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::Chat msg;
	msg.m_recvType = (msg::Chat::RecvType)recvType;//接收方类型
	msg.m_recverId = recverId;//接收方Id
	msg.m_talk = talk;
	if ( !msg.Build() ) return false;
	svr.Send(msg, msg.Size());

	return true;
}

void Client::OnChat(msg::Buffer &buffer)
{
	msg::Chat msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( msg.IsResult() )
	{
		if ( m_user.id != msg.m_senderId )
		{
			printf( "不是自己发的聊天\n" );
			return;
		}
		if ( msg::Chat::buddy != msg.m_recvType ) return;
		if ( ResultCode::Refuse != msg.m_code ) return;
		printf( "你不是对方的小伙伴，不能与用户(%u)聊天\n", msg.m_recverId );
		return;
	}
	if ( msg::Chat::buddy == msg.m_recvType )
	{
		printf( "%s(%d)对你说：%s\n", msg.m_senderName.c_str(), msg.m_senderId, msg.m_talk.c_str() );
		return;
	}
	if ( msg::Chat::buddys == msg.m_recvType )
	{
		printf( "%s(%d)：%s\n", msg.m_senderName.c_str(), msg.m_senderId, msg.m_talk.c_str() );
		return;
	}
	if ( msg::Chat::group == msg.m_recvType )
	{
		printf( "%s(%d)在群(%u)里说：%s\n", msg.m_senderName.c_str(), 
			msg.m_senderId, msg.m_talk.c_str(), msg.m_recverId );
		return;
	}

	return;
}

bool Client::SetUserData(unsigned int userId)
{
	if ( !m_user.logined ) return false;

	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::SetUserData msg;
	msg.m_userId = userId;
	msg.m_face = "face";				// 头像URL
	msg.m_sex = 0;				// 性别true = 男 false = 女
	if ( !msg.Build() ) return false;
	svr.Send(msg, msg.Size());

	return true;
}

bool Client::GetUserData(unsigned int userId)
{
	if ( !m_user.logined ) return false;

	printf( "user(%u)查询用户(%u)...\n", m_user.id, userId );
	int sock = Svr(TcpSvr);
	if ( -1 == sock ) return false;
	net::Socket svr;
	svr.Attach(sock);
	msg::GetUserData msg;
	msg.m_userId = userId;
	if ( !msg.Build() ) return false;
	svr.Send(msg, msg.Size());

	return true;
}

void Client::OnUserData(msg::Buffer &buffer)
{
	msg::UserData msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() || !msg.IsResult() ) return;
	printf( "User:id(%u) 昵称(%s) 头像(%s) 性别(%s) 币(%d) 绑定Imei(%s) 绑定手机号(%s)", 
		msg.m_userId, msg.m_nickName.c_str(), msg.m_face.c_str(), 
		msg.m_sex?"男":"女", msg.m_coin, msg.m_bindImei.c_str(), msg.m_bindMobile.c_str() );
}

void Client::OnSNS(msg::Buffer &buffer)
{
	switch ( buffer.Id() )
	{
	case MsgId::addBuddy :
		OnAddBuddy(buffer);
		break;
	case MsgId::delBuddy :
		OnDelBuddy(buffer);
		break;
	case MsgId::buddys :
		OnBuddys(buffer);
		break;
	case MsgId::chat :
		OnChat(buffer);
		break;
	case MsgId::userData :
		OnUserData(buffer);
		break;
	default:
		break;
	}
}

bool Client::GameInit()
{
	return true;
}
