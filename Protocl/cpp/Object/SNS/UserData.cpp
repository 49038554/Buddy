#include "UserData.h"


namespace msg
{

UserData::UserData()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
	m_userId = 0;
}

UserData::~UserData()
{
}

bool UserData::Build()
{
	SetId(MsgId::userData, true);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	
	if ( !AddData(m_userId) ) return false;//�û�Id
	if ( !AddData(m_nickName) ) return false;//�ǳ�
	if ( !AddData(m_face) ) return false;//ͷ��URL
	if ( !AddData(m_sex) ) return false;//�Ա�true = �� false = Ů
	if ( !AddData(m_coin) ) return false;//����ң���RMB��ֵ������
	if ( !AddData(m_bindImei) ) return false;//�˺ţ�Imei
	if ( !AddData(m_bindMobile) ) return false;//�˺ţ��ֻ�����

	return true;
}

bool UserData::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_userId) ) return false;//�û�Id
	if ( !GetData(m_nickName) ) return false;//�ǳ�
	if ( !GetData(m_face) ) return false;//ͷ��URL
	if ( !GetData(m_sex) ) return false;//�Ա�true = �� false = Ů
	if ( !GetData(m_coin) ) return false;//����ң���RMB��ֵ������
	if ( !GetData(m_bindImei) ) return false;//�˺ţ�Imei
	if ( !GetData(m_bindMobile) ) return false;//�˺ţ��ֻ�����

	return true;
}

}
