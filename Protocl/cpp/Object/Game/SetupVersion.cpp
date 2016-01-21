#include "SetupVersion.h"


namespace msg
{

SetupVersion::SetupVersion()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
	m_raceVersion = 0;
	m_skillVersion = 0;
	m_itemVersion = 0;
	m_buddyVersion = 0;
	m_lbsVersion = 0;
}

SetupVersion::~SetupVersion()
{
}

bool SetupVersion::Build(bool isResult)
{
	SetId(MsgId::setupVersion, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	//�������
	if ( !AddData(m_raceVersion) ) return false;
	if ( !AddData(m_skillVersion) ) return false;
	if ( !AddData(m_itemVersion) ) return false;
	if ( !AddData(m_buddyVersion) ) return false;
	if ( !AddData(m_lbsVersion) ) return false;

	//��Ӧ����

	return true;
}

bool SetupVersion::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_raceVersion) ) return false;
	if ( !GetData(m_skillVersion) ) return false;
	if ( !GetData(m_itemVersion) ) return false;
	if ( !GetData(m_buddyVersion) ) return false;
	if ( !GetData(m_lbsVersion) ) return false;

	//��Ӧ����

	return true;
}

}
