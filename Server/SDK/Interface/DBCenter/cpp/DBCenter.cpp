#include "DBCenter.h"
#include "Protocl/cpp/Object/DBEntry/SetupVersion.h"
#include "Protocl/cpp/Object/DBEntry/RaceMap.h"

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
	if ( ResultCode::success != Query(&msg, 1) ) return false;
	return true;
}

bool DBCenter::ResetUserPwd(msg::ResetPassword& msg)
{
	if ( ResultCode::success != Query(&msg, 1) ) return false;
	return true;
}

bool DBCenter::BindingPhone(msg::BindingPhone& msg)
{
	if ( ResultCode::success != Query(&msg, 1) ) return false;
	return true;
}

bool DBCenter::AddBuddy(msg::AddBuddy& msg)
{
	if ( ResultCode::success != Query(&msg, 1) ) return false;
	return true;
}

bool DBCenter::DelBuddy(msg::DelBuddy& msg)
{
	if ( ResultCode::success != Query(&msg, 1) ) return false;
	return true;
}

bool DBCenter::SetUserData(msg::SetUserData& msg)
{
	if ( ResultCode::success != Query(&msg, 1) ) return false;
	return true;
}

bool DBCenter::GetGameSetupData( 
	unsigned short &dataVersion, 
	std::map<unsigned char, std::string> &races,
	std::vector<data::SKILL> &skills, std::vector<data::ITEM> &items,
	std::vector<data::BUDDY> &buddys, std::vector<data::BUDDY_MAP> &buddyMaps
	)
{
	dataVersion = 0;
	races.clear();
	items.clear();
	skills.clear();
	buddys.clear();
	buddyMaps.clear();

	msg::SetupVersion query;
	query.Build();
	if ( ResultCode::success != Send(query, query.Size()) )
	{
		Close();
		return false;
	}

	msg::Buffer buf;
	while ( true )
	{
		if ( ReadMsg(&buf, 10) )
		{
			Close();
			return false;
		}
		int i = 0;
		int count = 0;
		if ( MsgId::raceMap == buf.Id() )
		{
			msg::RaceMap reply;
			memcpy(reply, buf, buf.Size());
			if ( !reply.Parse() )
			{
				Close();
				return false;
			}
			races = reply.m_races;
		}
		else if ( MsgId::skillBook == buf.Id() )
		{
			msg::SkillBook reply;
			memcpy(reply, buf, buf.Size());
			if ( !reply.Parse() )
			{
				Close();
				return false;
			}
			count = reply.m_skills.size();
			for ( i = 0; i < count; i++ ) skills.push_back(reply.m_skills[i]);
		}
		else if ( MsgId::itemBook == buf.Id() )
		{
			msg::ItemBook reply;
			memcpy(reply, buf, buf.Size());
			if ( !reply.Parse() )
			{
				Close();
				return false;
			}
			count = reply.m_items.size();
			for ( i = 0; i < count; i++ ) items.push_back(reply.m_items[i]);
		}
		else if ( MsgId::buddyBook == buf.Id() )
		{
			msg::BuddyBook reply;
			memcpy(reply, buf, buf.Size());
			if ( !reply.Parse() )
			{
				Close();
				return false;
			}
			count = reply.m_buddys.size();
			for ( i = 0; i < count; i++ ) buddys.push_back(reply.m_buddys[i]);
		}
		else if ( MsgId::buddyMap == buf.Id() )
		{
			msg::BuddyMap reply;
			memcpy(reply, buf, buf.Size());
			if ( !reply.Parse() )
			{
				Close();
				return false;
			}
			count = reply.m_buddyMaps.size();
			for ( i = 0; i < count; i++ ) buddyMaps.push_back(reply.m_buddyMaps[i]);
		}
		else if ( MsgId::setupVersion == buf.Id() )
		{
			msg::SetupVersion reply;
			memcpy(reply, buf, buf.Size());
			if ( !reply.Parse() )
			{
				Close();
				return false;
			}
			if ( ResultCode::success != reply.m_code )
			{
				Close();
				return false;
			}
			dataVersion = reply.m_dataVersion;
			break;
		}
	}

	return true;
}
