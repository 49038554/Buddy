#include "CatchStar.h"

namespace msg
{

CatchStar::CatchStar()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

CatchStar::~CatchStar()
{
}

bool CatchStar::Build(bool isResult)
{
	SetId(MsgId::catchStar, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	
	//��Ӧ����
	if ( !isResult || ResultCode::Success != m_code ) return true;
	if ( !AddData(m_itemId) ) return false;

	return true;
}

bool CatchStar::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������

	//��Ӧ����
	if ( !IsResult() || ResultCode::Success != m_code ) return true;
	if ( !GetData(m_itemId) ) return false;

	return true;
}

}
