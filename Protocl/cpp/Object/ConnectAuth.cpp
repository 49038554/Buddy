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
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData((char)m_moudleId) ) return false;//模块Id
	if ( !AddData(m_nodeId) ) return false;//结点Id

	return true;
}

bool ConnectAuth::Parse()
{
	if ( !Buffer::Parse() ) return false;
	if ( IsResult() ) return false;

	//请求参数
	char moudleId;
	if ( !GetData(moudleId) ) return false;//模块Id
	m_moudleId = (Moudle::Moudle)moudleId;
	if ( !GetData(m_nodeId) ) return false;//结点Id

	return true;
}

}
