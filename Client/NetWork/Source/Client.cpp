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

//////////////////////////////////////////////////////////////////////////
//DBEntry
#include "Protocl/cpp/Object/Game/SetupVersion.h"
#include "Protocl/cpp/Object/Game/RaceMap.h"
#include "Protocl/cpp/Object/Game/SkillBook.h"
#include "Protocl/cpp/Object/Game/ItemBook.h"
#include "Protocl/cpp/Object/Game/TalentBook.h"
#include "Protocl/cpp/Object/Game/BuddyBook.h"
#include "Protocl/cpp/Object/Game/BuddyMap.h"

#include "Protocl/cpp/Object/Game/GetPlayerData.h"
#include "Protocl/cpp/Object/Game/Pets.h"
#include "Protocl/cpp/Object/Game/PlayerItems.h"

#include "Protocl/cpp/Object/Game/BuildHouse.h"
#include "Protocl/cpp/Object/Game/TreePlant.h"
#include "Protocl/cpp/Object/Game/SyncPets.h"
#include "Protocl/cpp/Object/Game/SyncItem.h"
#include "Protocl/cpp/Object/Game/SyncCoin.h"

#include "mdk/File.h"


Client::Client(void)
{
	m_gameInitLoaded = LoadGameInit();
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

bool SaveRaceBook( mdk::File &db, std::map<unsigned char, std::string> &races )
{
	std::map<unsigned char, std::string>::iterator it = races.begin();
	unsigned char len = races.size();
	db.Write(&len, sizeof(char));
	unsigned char raceId;
	for ( ; it != races.end(); it++ )
	{
		raceId = it->first;
		db.Write(&raceId, 1);
		len = it->second.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(it->second.c_str()), len);
	}
	db.Close();

	return true;
}

int LoadRaceBook( mdk::File &db, std::map<unsigned char, std::string> &races )
{
	std::map<unsigned char, std::string>::iterator it = races.begin();
	unsigned char len = 0;
	unsigned char count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(char)) ) return 1;
	if ( 0 >= count || 17 < count ) return 2;
	unsigned char raceId;
	int i = 0;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&raceId, 1) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		races[raceId] = std::string(buf, len);
	}
	db.Close();

	return 0;
}

bool SaveItemBook( mdk::File &db, std::vector<data::ITEM> &items )
{
	data::ITEM *pItem;
	data::EFFECT *pEffect;
	char len;
	short count = items.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	for ( i = 0; i < items.size(); i++ )
	{
		pItem = &items[i];

		db.Write(&pItem->id, sizeof(short));
		len = pItem->name.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(pItem->name.c_str()), len);
		db.Write(&pItem->coin, sizeof(int));
		len = pItem->descript.size();
		db.Write(&len, sizeof(char));
		db.Write(&pItem->descript, len);

		len = pItem->effects.size();
		db.Write(&len, sizeof(char));
		int j = 0;
		for ( j = 0; j < pItem->effects.size(); j++ )
		{
			pEffect = &pItem->effects[j];

			db.Write(&pEffect->id, sizeof(char));
			db.Write(&pEffect->step, sizeof(char));
			db.Write(&pEffect->probability, sizeof(char));
			db.Write(&pEffect->agent, sizeof(char));
		}

	}
	db.Close();

	return true;
}

int LoadItemBook( mdk::File &db, std::vector<data::ITEM> &items )
{
	items.clear();
	data::ITEM info;
	data::EFFECT effect;
	char len;
	short count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;
	int i = 0;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.id, sizeof(short)) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		info.name = std::string(buf, len);
		if ( mdk::File::success != db.Read(&info.coin, sizeof(int)) ) return 7;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 8;
		if ( 60 < len || 0 >= len ) return 9;
		if ( mdk::File::success != db.Read(&buf, len) ) return 10;
		info.descript = std::string(buf, len);

		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 11;
		if ( 5 < len || 0 > len ) return 12;
		int j = 0;
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&effect.id, sizeof(char)) ) return 13;
			if ( mdk::File::success != db.Read(&effect.step, sizeof(char)) ) return 14;
			if ( mdk::File::success != db.Read(&effect.probability, sizeof(char)) ) return 15;
			if ( mdk::File::success != db.Read(&effect.agent, sizeof(char)) ) return 16;
			info.effects.push_back(effect);
		}

		items.push_back(info);
	}
	db.Close();

	return 0;
}

bool SaveTalentBook(mdk::File &db, std::vector<data::TALENT> &talents)
{
	data::TALENT *pTalent;
	data::EFFECT *pEffect;
	char len;
	short count = talents.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	for ( i = 0; i < talents.size(); i++ )
	{
		pTalent = &talents[i];

		db.Write(&pTalent->id, sizeof(short));
		len = pTalent->name.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(pTalent->name.c_str()), len);
		len = pTalent->descript.size();
		db.Write(&len, sizeof(char));
		db.Write(&pTalent->descript, len);

		len = pTalent->effects.size();
		db.Write(&len, sizeof(char));
		int j = 0;
		for ( j = 0; j < pTalent->effects.size(); j++ )
		{
			pEffect = &pTalent->effects[j];

			db.Write(&pEffect->id, sizeof(char));
			db.Write(&pEffect->step, sizeof(char));
			db.Write(&pEffect->probability, sizeof(char));
			db.Write(&pEffect->agent, sizeof(char));
		}

	}
	db.Close();

	return true;
}

int LoadTalentBook(mdk::File &db, std::vector<data::TALENT> &talents)
{
	talents.clear();
	data::TALENT info;
	data::EFFECT effect;
	char len;
	short count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;
	int i = 0;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.id, sizeof(short)) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		info.name = std::string(buf, len);
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 7;
		if ( 60 < len || 0 >= len ) return 8;
		if ( mdk::File::success != db.Read(&buf, len) ) return 9;
		info.descript = std::string(buf, len);

		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 10;
		if ( 5 < len || 0 >= len ) return 11;
		int j = 0;
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&effect.id, sizeof(char)) ) return 12;
			if ( mdk::File::success != db.Read(&effect.step, sizeof(char)) ) return 13;
			if ( mdk::File::success != db.Read(&effect.probability, sizeof(char)) ) return 14;
			if ( mdk::File::success != db.Read(&effect.agent, sizeof(char)) ) return 15;
			info.effects.push_back(effect);
		}
		talents.push_back(info);
	}
	db.Close();

	return 0;
}

bool SaveSkillBook(mdk::File &db, std::vector<data::SKILL> &skills)
{
	data::SKILL *pSkill;
	data::EFFECT *pEffect;
	char len;
	short count = skills.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	char valChar;
	for ( i = 0; i < skills.size(); i++ )
	{
		pSkill = &skills[i];

		db.Write(&pSkill->id, sizeof(short));
		len = pSkill->name.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(pSkill->name.c_str()), len);
		len = pSkill->descript.size();
		db.Write(&len, sizeof(char));
		db.Write(&pSkill->descript, len);
		db.Write(&pSkill->race, sizeof(char));
		db.Write(&pSkill->power, sizeof(short));
		db.Write(&pSkill->type, sizeof(char));
		db.Write(&pSkill->hitRate, sizeof(char));
		valChar = pSkill->isMapSkill?1:0;
		db.Write(&valChar, sizeof(char));

		len = pSkill->effects.size();
		db.Write(&len, sizeof(char));
		int j = 0;
		for ( j = 0; j < pSkill->effects.size(); j++ )
		{
			pEffect = &pSkill->effects[j];

			db.Write(&pEffect->id, sizeof(char));
			db.Write(&pEffect->step, sizeof(char));
			db.Write(&pEffect->probability, sizeof(char));
			db.Write(&pEffect->agent, sizeof(char));
		}

	}
	db.Close();

	return true;
}

int LoadSkillBook(mdk::File &db, std::vector<data::SKILL> &skills)
{
	skills.clear();
	data::SKILL info;
	data::EFFECT effect;
	char len;
	short count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;
	int i = 0;
	char valChar;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.id, sizeof(short)) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		info.name = std::string(buf, len);
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 7;
		if ( 60 < len || 0 >= len ) return 8;
		if ( mdk::File::success != db.Read(&buf, len) ) return 8;
		info.descript = std::string(buf, len);
		if ( mdk::File::success != db.Read(&info.race, sizeof(char)) ) return 10;
		if ( mdk::File::success != db.Read(&info.power, sizeof(short)) ) return 11;
		if ( mdk::File::success != db.Read(&info.type, sizeof(char)) ) return 12;
		if ( mdk::File::success != db.Read(&info.hitRate, sizeof(char)) ) return 13;
		if ( mdk::File::success != db.Read(&valChar, sizeof(char)) ) return 14;
		info.isMapSkill = (0 == valChar?false:true);

		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 15;
		if ( 5 < len || 0 > len ) return 16;
		int j = 0;
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&effect.id, sizeof(char)) ) return 17;
			if ( mdk::File::success != db.Read(&effect.step, sizeof(char)) ) return 18;
			if ( mdk::File::success != db.Read(&effect.probability, sizeof(char)) ) return 19;
			if ( mdk::File::success != db.Read(&effect.agent, sizeof(char)) ) return 20;

			info.effects.push_back(effect);
		}

		skills.push_back(info);
	}
	db.Close();

	return 0;
}

bool SaveBuddyBook(mdk::File &db, std::vector<data::BUDDY> &buddys)
{
	data::BUDDY *pBuddy;
	char len;
	short count = buddys.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	for ( i = 0; i < buddys.size(); i++ )
	{
		pBuddy = &buddys[i];

		db.Write(&pBuddy->number, sizeof(short));
		len = pBuddy->name.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(pBuddy->name.c_str()), len);
		len = pBuddy->descript.size();
		db.Write(&len, sizeof(char));
		db.Write(&pBuddy->descript, len);
		db.Write(&pBuddy->race1, sizeof(char));
		db.Write(&pBuddy->race2, sizeof(char));
		db.Write(&pBuddy->talent1, sizeof(char));
		db.Write(&pBuddy->talent2, sizeof(char));
		db.Write(&pBuddy->talent3, sizeof(char));
		db.Write(&pBuddy->itemId, sizeof(short));
		db.Write(&pBuddy->hitPoint, sizeof(short));
		db.Write(&pBuddy->physicalA, sizeof(short));
		db.Write(&pBuddy->physicalD, sizeof(short));
		db.Write(&pBuddy->specialA, sizeof(short));
		db.Write(&pBuddy->specialD, sizeof(short));
		db.Write(&pBuddy->speed, sizeof(short));
		db.Write(&pBuddy->rare, sizeof(char));
		db.Write(&pBuddy->tame, sizeof(char));

		len = pBuddy->skills.size();
		db.Write(&len, sizeof(char));
		short valShort;
		char valChar;
		std::map<unsigned short, bool>::iterator it = pBuddy->skills.begin();
		for ( ; it != pBuddy->skills.end(); it++ )
		{
			valShort = it->first;
			valChar = it->second?1:0;
			db.Write(&valShort, sizeof(short));
			db.Write(&valChar, sizeof(char));
		}

		len = pBuddy->upBuddys.size();
		db.Write(&len, sizeof(char));
		int j = 0;
		for ( j = 0; j < pBuddy->upBuddys.size(); j++ )
		{
			valShort = pBuddy->upBuddys[j];
			db.Write(&valShort, sizeof(short));
		}

	}
	db.Close();

	return true;
}

int LoadBuddyBook(mdk::File &db, std::vector<data::BUDDY> &buddys)
{
	buddys.clear();
	data::BUDDY info;
	char len;
	short count = buddys.size();
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;

	int i = 0;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.number, sizeof(short)) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		info.name = std::string(buf, len);
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 7;
		if ( 60 < len || 0 >= len ) return 8;
		if ( mdk::File::success != db.Read(buf, len) ) return 9;
		info.descript = std::string(buf, len);
		if ( mdk::File::success != db.Read(&info.race1, sizeof(char)) ) return 10;
		if ( mdk::File::success != db.Read(&info.race2, sizeof(char)) ) return 11;
		if ( mdk::File::success != db.Read(&info.talent1, sizeof(char)) ) return 12;
		if ( mdk::File::success != db.Read(&info.talent2, sizeof(char)) ) return 13;
		if ( mdk::File::success != db.Read(&info.talent3, sizeof(char)) ) return 14;
		if ( mdk::File::success != db.Read(&info.itemId, sizeof(short)) ) return 15;
		if ( mdk::File::success != db.Read(&info.hitPoint, sizeof(short)) ) return 16;
		if ( mdk::File::success != db.Read(&info.physicalA, sizeof(short)) ) return 17;
		if ( mdk::File::success != db.Read(&info.physicalD, sizeof(short)) ) return 18;
		if ( mdk::File::success != db.Read(&info.specialA, sizeof(short)) ) return 19;
		if ( mdk::File::success != db.Read(&info.specialD, sizeof(short)) ) return 20;
		if ( mdk::File::success != db.Read(&info.speed, sizeof(short)) ) return 21;
		if ( mdk::File::success != db.Read(&info.rare, sizeof(char)) ) return 22;
		if ( mdk::File::success != db.Read(&info.tame, sizeof(char)) ) return 23;

		len = info.skills.size();
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 24;
		if ( 80 < len || 0 >= len ) return 25;
		short valShort;
		char valChar;
		int j = 0;
		for ( ; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&valShort, sizeof(short)) ) return 26;
			if ( mdk::File::success != db.Read(&valChar, sizeof(char)) ) return 27;
			info.skills[valShort] = (0 == valChar?false:true); 
		}

		len = info.upBuddys.size();
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 28;
		if ( 10 < len || 0 > len) return 29;
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&valShort, sizeof(short)) ) return 30;
			info.upBuddys.push_back(valShort);
		}
		buddys.push_back(info);
	}
	db.Close();

	return 0;
}

bool SaveBuddyMap(mdk::File &db, std::vector<data::BUDDY_MAP> &buddyMaps)
{
	data::BUDDY_MAP *pBuddyMap;
	unsigned char len;
	short count = buddyMaps.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	char valChar;
	short valShort;
	for ( i = 0; i < buddyMaps.size(); i++ )
	{
		pBuddyMap = &buddyMaps[i];

		db.Write(&pBuddyMap->id, sizeof(int));
		db.Write(&pBuddyMap->shape, sizeof(char));
		db.Write(&pBuddyMap->x, sizeof(double));//latitude
		db.Write(&pBuddyMap->y, sizeof(double));//longitude
		db.Write(&pBuddyMap->radius, sizeof(int));
		db.Write(&pBuddyMap->right, sizeof(double));//latitude
		db.Write(&pBuddyMap->bottom, sizeof(double));//longitude
		db.Write(&pBuddyMap->city, sizeof(int));
		valChar = pBuddyMap->spot?1:0;
		db.Write(&valChar, sizeof(char));

		len = pBuddyMap->buddys.size();
		db.Write(&len, sizeof(char));
		int j = 0;
		for ( j = 0; j < pBuddyMap->buddys.size(); j++ )
		{
			valShort = pBuddyMap->buddys[j];
			db.Write(&valShort, sizeof(short));
		}

	}
	db.Close();

	return true;
}

int LoadBuddyMap(mdk::File &db, std::vector<data::BUDDY_MAP> &buddyMaps)
{
	buddyMaps.clear();
	data::BUDDY_MAP info;
	unsigned char len;
	short count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;

	int i = 0;
	char valChar;
	short valShort;
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.id, sizeof(int)) ) return 3;
		if ( mdk::File::success != db.Read(&info.shape, sizeof(char)) ) return 4;
		if ( mdk::File::success != db.Read(&info.x, sizeof(double)) ) return 5;//latitude
		if ( mdk::File::success != db.Read(&info.y, sizeof(double)) ) return 6;//longitude
		if ( mdk::File::success != db.Read(&info.radius, sizeof(int)) ) return 7;
		if ( mdk::File::success != db.Read(&info.right, sizeof(double)) ) return 9;//latitude
		if ( mdk::File::success != db.Read(&info.bottom, sizeof(double)) ) return 10;//longitude
		if ( mdk::File::success != db.Read(&info.city, sizeof(int)) ) return 11;
		if ( mdk::File::success != db.Read(&valChar, sizeof(char)) ) return 12;
		info.spot = (0 == valChar?false:true);
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 13;
		if ( 100 < len || 0 >= len) return 14;
		int j = 0;
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&valShort, sizeof(short)) ) return 15;
			info.buddys.push_back(valShort);
		}
		buddyMaps.push_back(info);
	}
	db.Close();

	return 0;
}

bool Client::LoadGameInit()
{
	mdk::File raceFile("D:/data", "raceBook");
	mdk::File itemFile("D:/data", "itemBook");
	mdk::File talentFile("D:/data", "talentBook");
	mdk::File skillFile("D:/data", "skillBook");
	mdk::File buddyFile("D:/data", "buddyBook");
	mdk::File mapFile("D:/data", "buddyMap");

	if ( mdk::File::success != raceFile.Open(mdk::File::read, mdk::File::assii) ) return false;
	if ( mdk::File::success != itemFile.Open(mdk::File::read, mdk::File::assii) ) return false;
	if ( mdk::File::success != talentFile.Open(mdk::File::read, mdk::File::assii) ) return false;
	if ( mdk::File::success != skillFile.Open(mdk::File::read, mdk::File::assii) ) return false;
	if ( mdk::File::success != buddyFile.Open(mdk::File::read, mdk::File::assii) ) return false;
	if ( mdk::File::success != mapFile.Open(mdk::File::read, mdk::File::assii) ) return false;
	int ret = LoadRaceBook(raceFile, m_races);//
	if ( 0 != ret ) return false;
	ret = LoadItemBook(itemFile, m_items);//
	if ( 0 != ret ) return false;
	ret = LoadTalentBook(talentFile, m_talents);//
	if ( 0 != ret ) return false;
	ret = LoadSkillBook(skillFile, m_skills);//
	if ( 0 != ret ) return false;
	ret = LoadBuddyBook(buddyFile, m_buddys);//
	if ( 0 != ret ) return false;
	ret = LoadBuddyMap(mapFile, m_buddyMaps);//
	if ( 0 != ret ) return false;

	return true;
}

bool Client::SaveGameInit()
{
	mdk::File raceFile("D:/data", "raceBook");
	mdk::File itemFile("D:/data", "itemBook");
	mdk::File talentFile("D:/data", "talentBook");
	mdk::File skillFile("D:/data", "skillBook");
	mdk::File buddyFile("D:/data", "buddyBook");
	mdk::File mapFile("D:/data", "buddyMap");

	if ( mdk::File::success != raceFile.Open(mdk::File::write, mdk::File::assii) ) return false;
	if ( mdk::File::success != itemFile.Open(mdk::File::write, mdk::File::assii) ) return false;
	if ( mdk::File::success != talentFile.Open(mdk::File::write, mdk::File::assii) ) return false;
	if ( mdk::File::success != skillFile.Open(mdk::File::write, mdk::File::assii) ) return false;
	if ( mdk::File::success != buddyFile.Open(mdk::File::write, mdk::File::assii) ) return false;
	if ( mdk::File::success != mapFile.Open(mdk::File::write, mdk::File::assii) ) return false;
	SaveRaceBook(raceFile, m_races);//
	SaveItemBook(itemFile, m_items);//
	SaveTalentBook(talentFile, m_talents);//
	SaveSkillBook(skillFile, m_skills);//
	SaveBuddyBook(buddyFile, m_buddys);//
	SaveBuddyMap(mapFile, m_buddyMaps);//

	return true;
}
