#include "NearInfo.h"

namespace msg
{

NearInfo::NearInfo()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

NearInfo::~NearInfo()
{
}

bool NearInfo::Build(bool isResult)
{
	SetId(MsgId::nearInfo, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_type) ) return false;

	//��Ӧ����

	return true;
}

bool NearInfo::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_type) ) return false;

	//��Ӧ����

	return true;
}

}
