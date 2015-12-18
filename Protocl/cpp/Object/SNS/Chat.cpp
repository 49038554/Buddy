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
	if ( !AddData(m_senderId) ) return false;//发言人Id	//SNS服务填写
	if ( !AddData(m_chatType) ) return false;//聊天类型，单聊，群聊，...
	if ( !AddData(m_recverId) ) return false;//接收方Id

	return true;
}

bool Chat::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_senderId) ) return false;//发言人Id	//SNS服务填写
	if ( !GetData(m_chatType) ) return false;//聊天类型，单聊，群聊，...
	if ( !GetData(m_recverId) ) return false;//接收方Id

	return true;
}

}
