#include "CityBuilding.h"

namespace msg
{

CityBuilding::CityBuilding()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

CityBuilding::~CityBuilding()
{
}

bool CityBuilding::Build( bool isResult )
{
	SetId(MsgId::cityBuilding, true);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_buildId) ) return false;
	if ( !AddData(m_name) ) return false;

	//��Ӧ����
	if ( !isResult || ResultCode::Success != m_code ) return true;
	if ( !AddData(m_needCoin) ) return false;
	if ( !AddData(m_payCoin) ) return false;

	return true;
}

bool CityBuilding::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_buildId) ) return false;
	if ( !GetData(m_name) ) return false;

	//��Ӧ����
	if ( !IsResult() || ResultCode::Success != m_code ) return true;
	if ( !GetData(m_needCoin) ) return false;
	if ( !GetData(m_payCoin) ) return false;

	return true;
}

}
