#include "TreePlant.h"

namespace msg
{

TreePlant::TreePlant()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

TreePlant::~TreePlant()
{
}

bool TreePlant::Build(bool isResult)
{
	SetId(MsgId::treePlant, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_houseId) ) return false;

	//��Ӧ����
	if ( !isResult || ResultCode::success != m_code ) return true;
	if ( !AddData(m_treeId) ) return false;

	return true;
}

bool TreePlant::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_houseId) ) return false;

	//��Ӧ����
	if ( !IsResult() || ResultCode::success != m_code ) return true;
	if ( !GetData(m_treeId) ) return false;

	return true;
}

}
