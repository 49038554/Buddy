#include "TreePlant.h"

namespace msg
{

TreePlant::TreePlant()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

TreePlant::~TreePlant()
{
}

bool TreePlant::Build(bool isResult)
{
	SetId(MsgId::treePlant, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_longitude) ) return false;
	if ( !AddData(m_latitude) ) return false;
	if ( !AddData(m_address) ) return false;

	//回应参数
	if ( !isResult || ResultCode::Success != m_code ) return true;
	if ( !AddData(m_treeId) ) return false;

	return true;
}

bool TreePlant::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_longitude) ) return false;
	if ( !GetData(m_latitude) ) return false;
	if ( !GetData(m_address) ) return false;

	//回应参数
	if ( !IsResult() || ResultCode::Success != m_code ) return true;
	if ( !GetData(m_treeId) ) return false;

	return true;
}

}
