#include "Undekaron.h"

namespace msg
{

Undekaron::Undekaron()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

Undekaron::~Undekaron()
{
}

bool Undekaron::Build(bool isResult)
{
	SetId(MsgId::undekaron, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_playerId) ) return false;//����ս���id
	
	//��Ӧ����

	return true;
}

bool Undekaron::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_playerId) ) return false;//����ս���id

	//��Ӧ����

	return true;
}

}
