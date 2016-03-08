#include "BuildHouse.h"

namespace msg
{

BuildHouse::BuildHouse()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

BuildHouse::~BuildHouse()
{
}

bool BuildHouse::Build(bool isResult)
{
	SetId(MsgId::buildHouse, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_name) ) return false;
	if ( !AddData(m_longitude) ) return false;
	if ( !AddData(m_latitude) ) return false;
	if ( !AddData(m_address) ) return false;
	if ( !AddData(m_radius) ) return false;
	if ( !AddData(m_coin) ) return false;
	
	//回应参数
	if ( !isResult || ResultCode::Success != m_code ) return true;
	if ( !AddData(m_buildId) ) return false;

	return true;
}

bool BuildHouse::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_name) ) return false;
	if ( !GetData(m_longitude) ) return false;
	if ( !GetData(m_latitude) ) return false;
	if ( !GetData(m_address) ) return false;
	if ( !GetData(m_radius) ) return false;
	if ( !GetData(m_coin) ) return false;
	
	//回应参数
	if ( !IsResult() || ResultCode::Success != m_code ) return true;
	if ( !GetData(m_buildId) ) return false;

	return true;
}

}
