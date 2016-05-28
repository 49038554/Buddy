#include "PutPet.h"

namespace msg
{

PutPet::PutPet()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

PutPet::~PutPet()
{
}

bool PutPet::Build(bool isResult)
{
	SetId(MsgId::putPet, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_petId) ) return false;
	if ( !AddData(m_toHouse) ) return false;
	if ( !AddData(m_houseId) ) return false;

	return true;
}

bool PutPet::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_petId) ) return false;
	if ( !GetData(m_toHouse) ) return false;
	if ( !GetData(m_houseId) ) return false;

	return true;
}

}
