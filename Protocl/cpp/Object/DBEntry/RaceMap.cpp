#include "RaceMap.h"

namespace msg
{

RaceMap::RaceMap()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

RaceMap::~RaceMap()
{
}

bool RaceMap::Build()
{
	if ( 0 >= m_races.size() ) 
	{
		return false;
	}
	SetId(MsgId::raceMap, true);
	if ( !FillTransmitParam() ) //Tcp服务填写参数
	{
		return false;
	}

	//回应参数
	if ( !AddData((char)m_races.size()) ) 
	{
		return false;
	}
	std::map<unsigned char, std::string>::iterator it = m_races.begin();
	for ( ; it != m_races.end(); it++ )
	{
		if ( !AddData(it->first) ) //id
		{
			return false;
		}
		if ( !AddData(it->second) ) //名字
		{
			return false;
		}
	}


	return true;
}

bool RaceMap::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//回应参数
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count ) return false;
	unsigned char id;
	std::string name;
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !GetData(id) ) return false;
		if ( !GetData(name) ) return false;
		m_races[id] = name;
	}

	return true;
}

}
