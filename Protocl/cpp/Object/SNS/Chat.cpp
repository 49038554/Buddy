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
	if ( !AddData(m_senderId) ) return false;//������Id	//SNS������д
	if ( !AddData(m_chatType) ) return false;//�������ͣ����ģ�Ⱥ�ģ�...
	if ( !AddData(m_recverId) ) return false;//���շ�Id

	return true;
}

bool Chat::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_senderId) ) return false;//������Id	//SNS������д
	if ( !GetData(m_chatType) ) return false;//�������ͣ����ģ�Ⱥ�ģ�...
	if ( !GetData(m_recverId) ) return false;//���շ�Id

	return true;
}

}
