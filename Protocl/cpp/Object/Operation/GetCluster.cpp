#include "GetCluster.h"


namespace msg
{

GetCluster::GetCluster()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Operation);
}


GetCluster::~GetCluster()
{
}

bool GetCluster::Build( bool isResult )
{
	SetId(MsgId::getCluster, false);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	if ( !AddData((char)m_moudle) ) return false;//ȡʲôģ��ķ���

	return true;
}

bool GetCluster::Parse()
{
 	if ( !Buffer::Parse() ) return false;
	if ( IsResult() ) return false;
	char val;
	if ( !GetData(val) ) return false;//ȡʲôģ��ķ���
	m_moudle = (Moudle::Moudle)val;

	return true;
}

}
