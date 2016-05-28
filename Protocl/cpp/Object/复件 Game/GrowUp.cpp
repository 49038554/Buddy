#include "GrowUp.h"


namespace msg
{

GrowUp::GrowUp()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

GrowUp::~GrowUp()
{
}

bool GrowUp::Build(bool isResult)
{
	SetId(MsgId::growUp, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_petId) ) return false;

	return true;
}

bool GrowUp::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_petId) ) return false;

	return true;
}

}
