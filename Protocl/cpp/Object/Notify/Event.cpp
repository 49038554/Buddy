#include "Event.h"


namespace msg
{

Event::Event()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Notify);
	m_holdTime = -1;
}

Event::~Event()
{
}

bool Event::Build()
{
	SetId(MsgId::event, false);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_eventId) ) return false;//事件Id Nofity填写
	if ( !AddData(m_senderId) ) return false;//发送者Id
	if ( !AddData(m_sender) ) return false;//发送者昵称
	if ( !AddData((char)m_recvType) ) return false;//接收者类型 业务服务器填写
	if ( !AddData(m_recverId) ) return false;//接收者Id  业务服务器填写
	if ( !AddData(m_holdTime) ) return false;//有效时间
	if ( !AddData(m_msg, m_msg.Size()) ) return false;//原始消息流

	return true;
}

bool Event::Parse()
{
	if ( !Buffer::Parse() ) return false;
	if ( IsResult() ) return false;
	//请求参数
	if ( !GetData(m_eventId) ) return false;//事件Id Nofity填写
	if ( !GetData(m_senderId) ) return false;//发送者Id
	if ( !GetData(m_sender) ) return false;//发送者昵称
	char val;
	if ( !GetData(val) ) return false;//接收者类型 业务服务器填写
	m_recvType = (Event::RecvType)val;
	if ( !GetData(m_recverId) ) return false;//接收者Id  业务服务器填写
	if ( !GetData(m_holdTime) ) return false;//有效时间
	int size = 0;
	if ( !GetData(m_msg, size) ) return false;//原始消息流
	if ( 0 > m_msg.Size() ) return false;
	if ( !m_msg.Parse() ) return false;

	return true;
}

}
