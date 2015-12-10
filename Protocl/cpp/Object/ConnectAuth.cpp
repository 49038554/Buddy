#include "ConnectAuth.h"

namespace msg
{

ConnectAuth::ConnectAuth()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::all);
}

ConnectAuth::~ConnectAuth()
{
}

bool ConnectAuth::Build()
{
	SetId(MsgId::connectAuth, false);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData((char)m_nodeType) ) return false;//�������
	if ( !AddData(m_nodeId) ) return false;//���Id
	return true;
}

bool ConnectAuth::Parse()
{
	if ( !Buffer::Parse() ) return false;
	if ( IsResult() ) return false;

	//�������
	char moudleId;
	if ( !GetData(moudleId) ) return false;//�������
	m_nodeType = (Moudle::Moudle)moudleId;
	if ( !GetData(m_nodeId) ) return false;//���Id

	return true;
}

}
