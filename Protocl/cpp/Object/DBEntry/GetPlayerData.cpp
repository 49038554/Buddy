#include "GetPlayerData.h"

namespace msg
{

GetPlayerData::GetPlayerData()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

GetPlayerData::~GetPlayerData()
{
}

bool GetPlayerData::Build(bool isResult)
{
	SetId(MsgId::getPlayerData, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	//��Ӧ����

	return true;
}

bool GetPlayerData::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	//��Ӧ����

	return true;
}

}
