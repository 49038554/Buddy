#include "Client.h"

#include "mdk/mapi.h"
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
#include "Protocl/cpp/Object/DBEntry/SetupVersion.h"
#include "Protocl/cpp/Object/DBEntry/RaceMap.h"
#include "Protocl/cpp/Object/DBEntry/SkillBook.h"
#include "Protocl/cpp/Object/DBEntry/ItemBook.h"
#include "Protocl/cpp/Object/DBEntry/TalentBook.h"
#include "Protocl/cpp/Object/DBEntry/BuddyBook.h"
#include "Protocl/cpp/Object/DBEntry/BuddyMap.h"

#include "Protocl/cpp/Object/DBEntry/Player.h"
#include "Protocl/cpp/Object/DBEntry/GetPlayerData.h"
#include "Protocl/cpp/Object/DBEntry/Pets.h"
#include "Protocl/cpp/Object/DBEntry/PlayerItems.h"

#include "Protocl/cpp/Object/DBEntry/BuildHouse.h"
#include "Protocl/cpp/Object/DBEntry/TreePlant.h"
#include "Protocl/cpp/Object/DBEntry/SyncPets.h"
#include "Protocl/cpp/Object/DBEntry/SyncItem.h"
#include "Protocl/cpp/Object/DBEntry/SyncPlayer.h"


Client::Client(void)
{
	m_palyerDataLoaded = LoadGame();
	m_mapId = 5;
	PetFactory pf;
	pf.SetGame(&m_game);
// 	data::PET pet = pf.Pet("悬浮魔偶");
	pf.SetSkill(m_pets[4], "大爆炸", "撒菱", "晴天", "求雨");
// 	m_pets[4].skill1 = pet.skill1;
// 	m_pets[4].skill2 = pet.skill2;
// 	m_pets[4].skill3 = pet.skill3;
// 	m_pets[4].skill4 = pet.skill4;
//	pf.AddItem(m_pets[0], "专爱头巾");
	;
}

Client::~Client(void)
{
}

void Client::Main()
{
	if ( !m_tcpEntry.IsClosed() )
	{
		time_t curTime = time(NULL);
		if ( !m_game.IsInit() && curTime - m_lastQueryTime > 3 )
		{
			Close(Client::TcpSvr);
		}
	}

	if ( !m_game.IsInit() || !m_palyerDataLoaded ) return;
	if ( Client::idle == m_state )
	{
//		data::BUDDY *pBuddy= m_game.Encounter(m_mapId);
	}
}

void Client::OnConnect(int svrType, net::Socket &svr)
{
	if ( Client::TcpSvr == svrType )
	{
		m_tcpEntry = svr;
		msg::SetupVersion msg;
		msg.m_dataVersion = m_game.IsInit()?m_game.Version():0;
		msg.Build();
		m_tcpEntry.Send(msg, msg.Size());
		m_lastQueryTime = time(NULL);
	}
}

void Client::OnClose(int svrType)
{
	m_user.logined = false;
}

void Client::OnMsg(int svrType, net::Socket &svr, msg::Buffer &buffer)
{
	if ( Moudle::Auth == buffer.MoudleId() ) OnAuth(buffer);
	if ( Moudle::SNS == buffer.MoudleId() ) OnSNS(buffer);
	if ( Moudle::DBEntry == buffer.MoudleId() ) OnDBEntry(buffer);
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
	if ( m_player.playerId != m_user.id )
	{
		if ( !GameSaved() ) 
		{
			//发出通知
			return;
		}
		m_palyerDataLoaded = false;
		memset(&m_player, 0, sizeof(data::PLAYER));
		m_player.synced = true;
		m_items.clear();
		m_pets.clear();
	}
	m_player.nick = msg.m_nick;
	if ( !m_palyerDataLoaded )//下载
	{
		msg::GetPlayerData msg;
		msg.Build();
		m_tcpEntry.Send(msg, msg.Size());
	}
	SyncGame();
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

void Client::OnRaceMap(msg::Buffer &buffer)
{
	msg::RaceMap msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	m_game.SetRaceMap(msg.m_races);
}

void Client::OnItemBook(msg::Buffer &buffer)
{
	msg::ItemBook msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	m_game.SetItemBook(msg.m_items);
}

void Client::OnTalentBook(msg::Buffer &buffer)
{
	msg::TalentBook msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	m_game.SetTalentBook(msg.m_talents);
}

void Client::OnSkillBook(msg::Buffer &buffer)
{
	msg::SkillBook msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	m_game.SetSkillBook(msg.m_skills);
}

void Client::OnBuddyBook(msg::Buffer &buffer)
{
	msg::BuddyBook msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	m_game.SetBuddyBook(msg.m_buddys);
}

void Client::OnBuddyMap(msg::Buffer &buffer)
{
	msg::BuddyMap msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	m_game.SetBuddyMap(msg.m_buddyMaps);
}

void Client::OnSetupVersion(msg::Buffer &buffer)
{
	msg::SetupVersion msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( ResultCode::Success != msg.m_code ) return;
	m_game.OnSetupVersion(msg.m_dataVersion);
}

void Client::OnDBEntry(msg::Buffer &buffer)
{
	switch ( buffer.Id() )
	{
	case MsgId::raceMap :
		OnRaceMap(buffer);
		break;
	case MsgId::itemBook :
		OnItemBook(buffer);
		break;
	case MsgId::talentBook :
		OnTalentBook(buffer);
		break;
	case MsgId::skillBook :
		OnSkillBook(buffer);
		break;
	case MsgId::buddyBook :
		OnBuddyBook(buffer);
		break;
	case MsgId::buddyMap :
		OnBuddyMap(buffer);
		break;
	case MsgId::setupVersion :
		OnSetupVersion(buffer);
		break;
	case MsgId::player :
		OnPlayer(buffer);
		break;
	case MsgId::playerItems :
		OnPlayerItems(buffer);
		break;
	case MsgId::pets :
		OnPets(buffer);
		break;
	case MsgId::getPlayerData :
		OnGetPlayerData(buffer);
		break;
	case MsgId::syncPlayer :
		OnSyncPlayer(buffer);
		break;
	case MsgId::syncItem :
		OnSyncItem(buffer);
		break;
	case MsgId::syncPets :
		OnSyncPets(buffer);
		break;
	default:
		break;
	}
}

int Client::LoadItems(mdk::File &db, std::vector<data::PLAYER_ITEM> &items)
{
	data::PLAYER_ITEM info;
	short count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( count > 500 || count < 0 ) return 2;
	int i = 0; 
	char varChar;
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.id, sizeof(short)) ) return 3;
		if ( mdk::File::success != db.Read(&info.count, sizeof(int)) ) return 4;
		if ( mdk::File::success != db.Read(&varChar, sizeof(char)) ) return 5;
		info.synced = (0 == varChar?false:true);
		items.push_back(info);
	}

	return 0;
}

bool Client::SaveItems(mdk::File &db, std::vector<data::PLAYER_ITEM> &items)
{
	data::PLAYER_ITEM *pInfo;
	short count = items.size();
	if ( count > 500 ) return false;
	db.Write(&count, sizeof(short));
	int i = 0; 
	char varChar;
	for ( i = 0; i < items.size(); i++ )
	{
		pInfo = &items[i];
		db.Write(&pInfo->id, sizeof(short));
		db.Write(&pInfo->count, sizeof(int));
		varChar = pInfo->synced?1:0;
		db.Write(&varChar, sizeof(char));
	}

	return true;
}

int Client::LoadPets(mdk::File &db, std::vector<data::PET> &pets)
{
	data::PET info;
	int count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(int)) ) return 1;
	if ( count <= 0 ) return 2;

	int i = 0;
	char varChar;
	for ( i = 0; i < count; i++ )
	{
		info.nick = "";
		if ( mdk::File::success != db.Read(&info.id, sizeof(int)) ) return 3;
		if ( mdk::File::success != db.Read(&info.number, sizeof(short)) ) return 4;
		if ( mdk::File::success != db.Read(&info.talent, sizeof(char)) ) return 5;
		if ( mdk::File::success != db.Read(&info.nature, sizeof(char)) ) return 6;
		if ( mdk::File::success != db.Read(&info.HP, sizeof(short)) ) return 12;
		if ( mdk::File::success != db.Read(&info.WG, sizeof(short)) ) return 13;
		if ( mdk::File::success != db.Read(&info.WF, sizeof(short)) ) return 14;
		if ( mdk::File::success != db.Read(&info.TG, sizeof(short)) ) return 15;
		if ( mdk::File::success != db.Read(&info.TF, sizeof(short)) ) return 16;
		if ( mdk::File::success != db.Read(&info.SD, sizeof(short)) ) return 17;
		if ( mdk::File::success != db.Read(&info.HPHealthy, sizeof(char)) ) return 18;
		if ( mdk::File::success != db.Read(&info.WGHealthy, sizeof(char)) ) return 19;
		if ( mdk::File::success != db.Read(&info.WFHealthy, sizeof(char)) ) return 20;
		if ( mdk::File::success != db.Read(&info.TGHealthy, sizeof(char)) ) return 21;
		if ( mdk::File::success != db.Read(&info.TFHealthy, sizeof(char)) ) return 22;
		if ( mdk::File::success != db.Read(&info.SDHealthy, sizeof(char)) ) return 23;
		if ( mdk::File::success != db.Read(&info.HPMuscle, sizeof(char)) ) return 24;
		if ( mdk::File::success != db.Read(&info.WGMuscle, sizeof(char)) ) return 25;
		if ( mdk::File::success != db.Read(&info.WFMuscle, sizeof(char)) ) return 26;
		if ( mdk::File::success != db.Read(&info.TGMuscle, sizeof(char)) ) return 27;
		if ( mdk::File::success != db.Read(&info.TFMuscle, sizeof(char)) ) return 28;
		if ( mdk::File::success != db.Read(&info.SDMuscle, sizeof(char)) ) return 29;

		char len = 0;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 30;
		if ( len > 17 || len < 0 ) return 31;
		int j = 0;
		for ( j = 0; j < info.race.size(); j++ )
		{
			if ( mdk::File::success != db.Read(&varChar, sizeof(char)) ) return 32;
			info.race.push_back(varChar);
		}

		if ( mdk::File::success != db.Read(&varChar, sizeof(char)) ) return 33;
		info.synced = (0 == varChar?false:true);
		char nickLen = 0;
		if ( mdk::File::success != db.Read(&nickLen, sizeof(char)) ) return 41;
		if ( 0 > nickLen || 20 < nickLen ) return 42;
		if ( 0 != nickLen )
		{
			char nick[256];
			if ( mdk::File::success != db.Read(nick, nickLen) ) return 43;
			nick[nickLen] = 0;
			info.nick = std::string(nick);
		}
		if ( mdk::File::success != db.Read(&info.curHP, sizeof(short)) ) return 34;
		if ( mdk::File::success != db.Read(&info.state, sizeof(char)) ) return 35;
		if ( mdk::File::success != db.Read(&info.skill1, sizeof(short)) ) return 36;
		if ( mdk::File::success != db.Read(&info.skill2, sizeof(short)) ) return 37;
		if ( mdk::File::success != db.Read(&info.skill3, sizeof(short)) ) return 38;
		if ( mdk::File::success != db.Read(&info.skill4, sizeof(short)) ) return 39;
		if ( mdk::File::success != db.Read(&info.itemId, sizeof(short)) ) return 40;
		if ( "" == info.nick )
		{
			data::BUDDY *pBuddy = Buddy(info.number, m_game.BuddyBook());
			if ( NULL == pBuddy ) continue;
			info.nick = pBuddy->name;
		}
		pets.push_back(info);
	}

	return 0;
}

bool Client::SavePets(mdk::File &db, std::vector<data::PET> &pets)
{
	data::PET *pInfo;
	int count = pets.size();
	if ( count <= 0 ) return false;

	db.Write(&count, sizeof(int));
	int i = 0; 
	char varChar;
	for ( i = 0; i < pets.size(); i++ )
	{
		pInfo = &pets[i];
		db.Write(&pInfo->id, sizeof(int));
		db.Write(&pInfo->number, sizeof(short));
		db.Write(&pInfo->talent, sizeof(char));
		db.Write(&pInfo->nature, sizeof(char));
		db.Write(&pInfo->HP, sizeof(short));
		db.Write(&pInfo->WG, sizeof(short));
		db.Write(&pInfo->WF, sizeof(short));
		db.Write(&pInfo->TG, sizeof(short));
		db.Write(&pInfo->TF, sizeof(short));
		db.Write(&pInfo->SD, sizeof(short));
		db.Write(&pInfo->HPHealthy, sizeof(char));
		db.Write(&pInfo->WGHealthy, sizeof(char));
		db.Write(&pInfo->WFHealthy, sizeof(char));
		db.Write(&pInfo->TGHealthy, sizeof(char));
		db.Write(&pInfo->TFHealthy, sizeof(char));
		db.Write(&pInfo->SDHealthy, sizeof(char));
		db.Write(&pInfo->HPMuscle, sizeof(char));
		db.Write(&pInfo->WGMuscle, sizeof(char));
		db.Write(&pInfo->WFMuscle, sizeof(char));
		db.Write(&pInfo->TGMuscle, sizeof(char));
		db.Write(&pInfo->TFMuscle, sizeof(char));
		db.Write(&pInfo->SDMuscle, sizeof(char));

		varChar = pInfo->race.size();
		db.Write(&varChar, sizeof(char));
		int j = 0;
		for ( j = 0; j < pInfo->race.size(); j++ )
		{
			varChar = pInfo->race[j];
			db.Write(&varChar, sizeof(char));
		}

		varChar = pInfo->synced?1:0;
		db.Write(&varChar, sizeof(char));
		if ( "" == pInfo->nick )
		{
			data::BUDDY *pBuddy = Buddy(pInfo->number, m_game.BuddyBook());
			if ( NULL == pBuddy ) pInfo->nick = "未知巴迪";
			else pInfo->nick = pBuddy->name;
		}
		char nickLen = pInfo->nick.size();
		if ( nickLen > 17 || nickLen < 0 ) nickLen = 0;
		db.Write(&nickLen, sizeof(char));
		if ( 0 != nickLen )
		{
			db.Write((void*)(pInfo->nick.c_str()), nickLen);
		}
		db.Write(&pInfo->curHP, sizeof(short));
		db.Write(&pInfo->state, sizeof(char));
		db.Write(&pInfo->skill1, sizeof(short));
		db.Write(&pInfo->skill2, sizeof(short));
		db.Write(&pInfo->skill3, sizeof(short));
		db.Write(&pInfo->skill4, sizeof(short));
		db.Write(&pInfo->itemId, sizeof(short));
	}

	return true;
}

bool Client::SaveGame()
{
	mdk::File db("D:/data", "player.db");

	if ( mdk::File::success != db.Open(mdk::File::write, mdk::File::assii) ) return false;

	db.Write(&m_player.playerId, sizeof(unsigned int));
	db.Write(&m_player.coin, sizeof(int));
	db.Write(&m_player.petCount, sizeof(char));
	db.Write(&m_player.pet[6], sizeof(short));
	db.Write(&m_player.lastLuckTime, sizeof(time_t));
	db.Write(&m_player.luckCoin, sizeof(short));
	db.Write(&m_player.synced, sizeof(bool));
	short len = m_player.nick.size();
	db.Write(&len, sizeof(short));
	db.Write((void*)m_player.nick.c_str(), len);

	SaveItems(db, m_items);//
	SavePets(db, m_pets);//

	return true;
}

bool Client::LoadGame()
{
	mdk::File db("D:/data", "player.db");

	if ( mdk::File::success != db.Open(mdk::File::read, mdk::File::assii) ) return false;
	if ( mdk::File::success != db.Read(&m_player.playerId, sizeof(unsigned int)) ) return false;
	if ( mdk::File::success != db.Read(&m_player.coin, sizeof(int)) ) return false;
	if ( mdk::File::success != db.Read(&m_player.petCount, sizeof(char)) ) return false;
	if ( mdk::File::success != db.Read(&m_player.pet[6], sizeof(short)) ) return false;
	if ( mdk::File::success != db.Read(&m_player.lastLuckTime, sizeof(time_t)) ) return false;
	if ( mdk::File::success != db.Read(&m_player.luckCoin, sizeof(short)) ) return false;
	if ( mdk::File::success != db.Read(&m_player.synced, sizeof(bool)) ) return false;
	short len = 0;
	if ( mdk::File::success != db.Read(&len, sizeof(short)) ) return false;
	char buf[256];
	if ( mdk::File::success != db.Read(buf, len) ) return false;
	buf[len] = 0;
	m_player.nick = buf;
	int ret = LoadItems(db, m_items);//
	if ( 0 != ret ) return false;
	ret = LoadPets(db, m_pets);//
	if ( 0 != ret ) return false;

	return true;
}

void Client::OnPlayer(msg::Buffer &buffer)
{
	msg::Player msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	time_t t = m_player.lastLuckTime;
	std::string nick = m_player.nick;
	m_player = msg.m_player;
	m_player.nick = nick;
	m_player.lastLuckTime = t;
	m_player.synced = true;
}

void Client::OnPlayerItems(msg::Buffer &buffer)
{
	msg::PlayerItems msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;

	int i = 0;
	data::PLAYER_ITEM *pInfo;
	for ( i = 0; i < msg.m_items.size(); i++ )
	{
		pInfo = PlayerItem(msg.m_items[i].id, m_items);
		if ( NULL == pInfo ) m_items.push_back(msg.m_items[i]);
		else *pInfo = msg.m_items[i];
	}
}

void Client::OnPets(msg::Buffer &buffer)
{
	msg::Pets msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;

	int i = 0;
	data::PET *pInfo;
	data::BUDDY *pBuddy;
	for ( i = 0; i < msg.m_pets.size(); i++ )
	{
		pBuddy = Buddy(msg.m_pets[i].number, m_game.BuddyBook());
		if ( NULL == pBuddy ) continue;

		pInfo = Pet(msg.m_pets[i].id, m_pets);
		if ( NULL == pInfo ) m_pets.push_back(msg.m_pets[i]);
		else *pInfo = msg.m_pets[i];
		pInfo = Pet(msg.m_pets[i].id, m_pets);
		pInfo->curHP = pInfo->HP = (pBuddy->hitPoint * 2 + pInfo->HPMuscle/4 + pInfo->HPHealthy) + 100 + 10;//血
		pInfo->WG = ((pBuddy->physicalA * 2 + pInfo->WGMuscle/4 + pInfo->WGHealthy) + 5) * GetNatureCal(pInfo->nature, "WG");//攻
		pInfo->WF = ((pBuddy->physicalD * 2 + pInfo->WFMuscle/4 + pInfo->WFHealthy) + 5) * GetNatureCal(pInfo->nature, "WF");//防
		pInfo->TG = ((pBuddy->specialA * 2 + pInfo->TFMuscle/4 + pInfo->TGHealthy) + 5) * GetNatureCal(pInfo->nature, "TG");//特攻
		pInfo->TF = ((pBuddy->specialD * 2 + pInfo->TFMuscle/4 + pInfo->TFHealthy) + 5) * GetNatureCal(pInfo->nature, "TF");//特防
		pInfo->SD = ((pBuddy->speed * 2 + pInfo->SDMuscle/4 + pInfo->SDHealthy) + 5) * GetNatureCal(pInfo->nature, "SD");//速度
		pInfo->state = 0;
		pInfo->itemId = 0;
		pInfo->skill1 = 0;
		pInfo->skill2 = 0;
		pInfo->skill3 = 0;
		pInfo->skill4 = 0;
		std::map<unsigned short, bool>::iterator it;
		for ( it = pBuddy->skills.begin(); it != pBuddy->skills.end(); it++ )
		{
			if ( !it->second ) continue;
			if ( 0 == pInfo->skill1 ) 
			{
				pInfo->skill1 = it->first;
				continue;
			}
			else if ( 0 == pInfo->skill2 ) 
			{
				pInfo->skill2 = it->first;
				continue;
			}
			else if ( 0 == pInfo->skill3 ) 
			{
				pInfo->skill3 = it->first;
				continue;
			}
			else if ( 0 == pInfo->skill4 ) 
			{
				pInfo->skill4 = it->first;
				continue;
			}
			else break;
		}
	}
}

void Client::OnGetPlayerData(msg::Buffer &buffer)
{
	msg::GetPlayerData msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;

	if ( ResultCode::Success != msg.m_code ) return;
	SaveGame();
	m_palyerDataLoaded = true;
	//发出通知
}

bool Client::GameSaved()
{
	if ( !m_palyerDataLoaded ) return true;
	if ( !m_player.synced ) return false;
	int i = 0;
	for ( i = 0; i < m_pets.size(); i++ )
	{
		if ( !m_pets[i].synced ) return false;
	}
	for ( i = 0; i < m_items.size(); i++ )
	{
		if ( !m_items[i].synced ) return false;
	}

	return true;
}

void Client::SyncGame()
{
	if ( m_tcpEntry.IsClosed() ) return;
	if ( GameSaved() ) return;

	if ( !m_player.synced )
	{
		msg::SyncPlayer msg;
		msg.m_player = m_player;
		msg.Build();
		m_tcpEntry.Send(msg, msg.Size());
	}
	if ( 0 < m_items.size() )
	{
		msg::SyncItem msg;
		int i = 0;
		for ( i = 0; i < m_items.size(); i++ )
		{
			if ( m_items[i].synced ) continue;
			msg.m_items.push_back(m_items[i]);
			if ( msg.m_items.size() == 500 )
			{
				msg.Build();
				m_tcpEntry.Send(msg, msg.Size());
				msg.m_items.clear();
			}
		}
		if ( 0 < msg.m_items.size() )
		{
			msg.Build();
			m_tcpEntry.Send(msg, msg.Size());
		}
	}
	if ( false )
	{
		msg::SyncPets msg;
		int i = 0;
		for ( i = 0; i < m_pets.size(); i++ )
		{
			if ( m_pets[i].synced ) continue;
			msg.m_pets.push_back(m_pets[i]);
			if ( msg.m_pets.size() == 100 )
			{
				msg.Build();
				m_tcpEntry.Send(msg, msg.Size());
				msg.m_pets.clear();
			}
		}
		if ( msg.m_pets.size() > 0 )
		{
			msg.Build();
			m_tcpEntry.Send(msg, msg.Size());
		}
	}
}

void Client::IOCoin( int count )
{
	m_player.coin += count;
	m_player.synced = false;
}

void Client::IOItem( short itemId, int count )
{
	data::PLAYER_ITEM *pInfo = PlayerItem(itemId, m_items);
	if ( NULL == pInfo )
	{
		data::PLAYER_ITEM info;
		info.id = itemId;
		info.count = count;
		info.synced = false;
		m_items.push_back(info);
	}
	else 
	{
		pInfo->count += count;
		pInfo->synced = false;
	}
}

std::string Client::TestLuck()
{
	time_t today = mdk::mdk_Date();
	if ( today > m_player.lastLuckTime ) 
	{
		m_player.luckCoin = 0;
	}
	if ( m_player.luckCoin >= 1000 ) return "今日机会已经用完";

	short itemId;
	std::string result = m_game.TestLuck(m_player.luckCoin, itemId);
	IOItem(itemId, 1);
	SaveGame();
	m_player.lastLuckTime = time(NULL);

	return result;
}

std::string Client::UseItem( short itemId, int count )
{
	if ( 0 >= count ) return "参数错误";

	data::PLAYER_ITEM *pInfo = PlayerItem(itemId, m_items);
	if ( NULL == pInfo || pInfo->count < count ) return "数量不足";
	IOItem(itemId, count * -1);
	SaveGame();

	return "";
}

std::string Client::Buy( short itemId, int count )
{
	int coin;
	std::string result = m_game.Buy(itemId, count, coin);
	if ( "" != result ) return result;
	if ( coin > m_player.coin ) return "正能量不足";

	IOCoin(coin * -1);
	IOItem(itemId, count);
	SaveGame();

	return "";
}

std::string Client::Devour( short itemId, int count )
{
	std::string result = UseItem(itemId, count);
	if ( "" != result ) return result;

	int coin;
	result = m_game.Devour(itemId, count, coin);
	if ( "" != result ) return result;

	IOCoin(coin);
	SaveGame();

	return "";
}

void Client::OnSyncPlayer(msg::Buffer &buffer)
{
	msg::SyncPlayer msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;

	if ( ResultCode::Success != msg.m_code ) return;
	m_player.synced = true;
	SaveGame();
}

void Client::OnSyncItem(msg::Buffer &buffer)
{
	msg::SyncItem msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( ResultCode::Success != msg.m_code ) return;

	int i = 0;
	data::PLAYER_ITEM *pInfo;
	for ( i = 0; i < msg.m_items.size(); i++ )
	{
		if ( !msg.m_items[i].synced ) continue;
		pInfo = PlayerItem(msg.m_items[i].id, m_items);
		if ( NULL == pInfo )
		{
			m_items.push_back(msg.m_items[i]);
		}
		else
		{
			pInfo->synced = true;
		}
	}
	SaveGame();
}

void Client::OnSyncPets(msg::Buffer &buffer)
{
	msg::SyncPets msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) return;
	if ( ResultCode::Success != msg.m_code ) return;

	int i = 0;
	data::PET *pInfo;
	for ( i = 0; i < msg.m_pets.size(); i++ )
	{
		if ( !msg.m_pets[i].synced ) continue;
		pInfo = Pet(msg.m_pets[i].id, m_pets);
		if ( NULL == pInfo )
		{
			m_pets.push_back(msg.m_pets[i]);
		}
		else
		{
			pInfo->synced = true;
		}
	}
	SaveGame();
}

void Client::SetLBS(int mapId)
{
	m_mapId = mapId;
}

int Client::CreateBattle( unsigned int shePlayerId, const std::string &enemyName,
	std::vector<data::PET> &she)
{
	return m_game.CreateBattle(m_player.playerId, m_player.nick, m_pets, shePlayerId, enemyName, she);
}

int Client::CreateBattle()
{
	return m_game.CreateBattle(m_player.playerId, m_player.nick, m_pets);
}

bool Client::Log( int battleId, std::vector<std::string> &log )
{
	return m_game.Log(battleId, log);
}

const char* Client::Ready(int battleId, Battle::Action act, short objectId)
{
	Battle::RAND_PARAM rp;
	const char *ret = m_game.CheckReady(battleId, true, act, objectId, rp);
	if ( NULL != ret ) return ret;
	m_game.Ready(battleId, true, act, objectId, rp);
	return NULL;
}

const char* Client::ChangePet(int battleId, short petId)
{
	const char *ret = m_game.ChangePet(battleId, true, petId);
	if ( NULL != ret ) return ret;
	return NULL;
}

const char* Client::SheReady(int battleId, Battle::Action act, short objectId)
{
	Battle::RAND_PARAM rp;
	const char *ret = m_game.CheckReady(battleId, false, act, objectId, rp);
	if ( NULL != ret ) return ret;
	m_game.Ready(battleId, false, act, objectId, rp);
	return NULL;
}

const char* Client::SheChangePet(int battleId, short petId)
{
	const char *ret = m_game.ChangePet(battleId, false, petId);
	if ( NULL != ret ) return ret;
	return NULL;
}

Game* Client::GetGame()
{
	return &m_game;
}

Battle* Client::GetBattle(int battleId)
{
	return m_game.GetBattle(battleId);
}

Battle::WARRIOR* Client::Fighter(int battleId, bool me)
{
	return m_game.Fighter(battleId, me);
}
