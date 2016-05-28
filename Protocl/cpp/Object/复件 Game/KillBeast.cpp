#include "KillBeast.h"


namespace msg
{

KillBeast::KillBeast()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

KillBeast::~KillBeast()
{
}

bool KillBeast::Build(bool isResult)
{
	char count = m_petIds.size();
	if ( 6 < count ) return false;

	SetId(MsgId::killBeast, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_killPetNo) ) return false;
	if ( !AddData(count) ) return false;
	std::map<unsigned int, unsigned int>::iterator it = m_petIds.begin();
	for ( ; it != m_petIds.end(); it++ )
	{
		if ( !AddData(it->second) ) return false;
	}
	
	return true;
}

bool KillBeast::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_killPetNo) ) return false;
	m_petIds.clear();
	char count;
	if ( !GetData(count) ) return false;
	if ( count > 6 ) return false;

	int i = 0;
	unsigned int petId;
	for ( ; i < count; i++ )
	{
		if ( !GetData(petId) ) return false;
		m_petIds[petId] = petId;
	}

	return true;
}

}
