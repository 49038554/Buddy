#include "Chat.h"


namespace msg
{

Chat::Chat()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
}

Chat::~Chat()
{
}

bool Chat::Build(bool isResult)
{
	SetId(MsgId::chat, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_senderId) ) return false;//������Id	SNS������д
	if ( !AddData(m_senderName) ) return false;//�������ǳ�	SNS������д
	if ( !AddData(m_senderFace) ) return false;//������ͷ��	SNS������д
	if ( !AddData((unsigned char)m_recvType) ) return false;//�������ͣ����ģ�Ⱥ�ģ�...
	if ( !AddData(m_recverId) ) return false;//���շ�Id
	if ( !AddData(m_talk) ) return false;//��������
	
	return true;
}

bool Chat::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_senderId) ) return false;//������Id	SNS������д
	if ( !GetData(m_senderName) ) return false;//�������ǳ�	SNS������д
	if ( !GetData(m_senderFace) ) return false;//������ͷ��	SNS������д
	unsigned char val;
	if ( !GetData(val) ) return false;//�������ͣ����ģ�Ⱥ�ģ�...
	m_recvType = (RecvType)val;
	if ( !GetData(m_recverId) ) return false;//���շ�Id
	if ( !GetData(m_talk) ) return false;//��������

	return true;
}

}
