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
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_senderId) ) return false;//发言人Id	SNS服务填写
	if ( !AddData(m_senderName) ) return false;//发言人昵称	SNS服务填写
	if ( !AddData(m_senderFace) ) return false;//发言人头像	SNS服务填写
	if ( !AddData((unsigned char)m_recvType) ) return false;//聊天类型，单聊，群聊，...
	if ( !AddData(m_recverId) ) return false;//接收方Id
	if ( !AddData(m_talk) ) return false;//聊天内容
	
	return true;
}

bool Chat::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_senderId) ) return false;//发言人Id	SNS服务填写
	if ( !GetData(m_senderName) ) return false;//发言人昵称	SNS服务填写
	if ( !GetData(m_senderFace) ) return false;//发言人头像	SNS服务填写
	unsigned char val;
	if ( !GetData(val) ) return false;//聊天类型，单聊，群聊，...
	m_recvType = (RecvType)val;
	if ( !GetData(m_recverId) ) return false;//接收方Id
	if ( !GetData(m_talk) ) return false;//聊天内容

	return true;
}

}
