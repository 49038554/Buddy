#include "AddBuddy.h"


namespace msg
{

AddBuddy::AddBuddy()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
	m_userId = 0;
	m_step = 0;
	m_nickName = "NULL";
	m_msg = "Hey Buddy��һ�����";
	m_accepted = false;
}

AddBuddy::~AddBuddy()
{
}

bool AddBuddy::Build(bool isResult)
{
	SetId(MsgId::addBuddy, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	
	if ( !AddData(m_userId) ) return false;//�û�Id
	if ( !AddData(m_nickName) ) return false;//�ǳ�
	if ( !AddData(m_buddyId) ) return false;//���Id
	if ( !AddData(m_step) ) return false;//��ǰ����
	if ( !AddData(m_msg) ) return false;//��Ϣ
	else if ( 1 == m_step )
	{
		if ( !AddData((char)(m_accepted?1:0)) ) return false;//����
	}

	return true;
}

bool AddBuddy::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_userId) ) return false;//�û�Id
	if ( !GetData(m_nickName) ) return false;//�ǳ�
	if ( !GetData(m_buddyId) ) return false;//���Id
	if ( !GetData(m_step) ) return false;//��ǰ����
	if ( !GetData(m_msg) ) return false;//��Ϣ
	if ( 1 == m_step )
	{
		char val;
		if ( !GetData(val) ) return false;//����
		m_accepted = 1 == val?true:false;
	}

	return true;
}

}
