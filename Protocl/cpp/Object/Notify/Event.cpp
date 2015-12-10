#include "Event.h"


namespace msg
{

Event::Event()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Notify);
}

Event::~Event()
{
}

bool Event::Build()
{
	SetId(MsgId::event, false);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_eventId) ) return false;//�¼�Id Nofity��д
	if ( !AddData(m_senderId) ) return false;//������Id
	if ( !AddData(m_sender) ) return false;//�������ǳ�
	if ( !AddData(m_holdTime) ) return false;//��Чʱ��
	if ( !AddData(m_msg, m_msg.Size()) ) return false;//ԭʼ��Ϣ��

	return true;
}

bool Event::Parse()
{
	if ( !Buffer::Parse() ) return false;
	if ( IsResult() ) return false;
	//�������
	if ( !GetData(m_eventId) ) return false;//�¼�Id Nofity��д
	if ( !GetData(m_senderId) ) return false;//������Id
	if ( !GetData(m_sender) ) return false;//�������ǳ�
	if ( !GetData(m_holdTime) ) return false;//��Чʱ��
	int size = 0;
	if ( !GetData(m_msg, size) ) return false;//ԭʼ��Ϣ��
	if ( 0 > m_msg.Size() ) return false;
	if ( !m_msg.Parse() ) return false;

	return true;
}

}
