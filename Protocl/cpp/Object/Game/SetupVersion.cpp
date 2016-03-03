#include "SetupVersion.h"


namespace msg
{

SetupVersion::SetupVersion()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
	m_dataVersion = 0;
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
	if ( !AddData(m_dataVersion) ) return false;

	//��Ӧ����

	return true;
}

bool SetupVersion::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_dataVersion) ) return false;

	//��Ӧ����

	return true;
}

}
