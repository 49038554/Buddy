#include "CityBuildInit.h"


namespace msg
{

CityBuildInit::CityBuildInit()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

CityBuildInit::~CityBuildInit()
{
}

bool CityBuildInit::Build(bool isResult)
{
	SetId(MsgId::cityBuildInit, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_timeLimited) ) return false;
	if ( !AddData(m_longitude) ) return false;
	if ( !AddData(m_latitude) ) return false;
	if ( !AddData(m_address) ) return false;
	if ( !AddData(m_radius) ) return false;

	//��Ӧ����
	if ( !isResult || ResultCode::Success != m_code ) return true;
	if ( !AddData(m_buildId) ) return false;

	return true;
}

bool CityBuildInit::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_timeLimited) ) return false;
	if ( !GetData(m_longitude) ) return false;
	if ( !GetData(m_latitude) ) return false;
	if ( !GetData(m_address) ) return false;
	if ( !GetData(m_radius) ) return false;

	//��Ӧ����
	if ( !IsResult() || ResultCode::Success != m_code ) return true;
	if ( !GetData(m_buildId) ) return false;

	return true;
}

}
