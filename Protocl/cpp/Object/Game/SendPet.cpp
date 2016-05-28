#include "SendPet.h"

namespace msg
{

SendPet::SendPet()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

SendPet::~SendPet()
{
}

bool SendPet::Build(bool isResult)
{
	SetId(MsgId::sendPet, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_battleId) ) return false;		//ս��Id
	if ( !AddData(m_petId) ) return false;		//����id

	//��Ӧ����

	return true;
}

bool SendPet::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_battleId) ) return false;		//ս��Id
	if ( !GetData(m_petId) ) return false;		//����id

	//��Ӧ����

	return true;
}

}
