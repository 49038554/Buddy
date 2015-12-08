#include "Message.h"
#include <cstdio>
#include <cstring>

namespace net
{

Message::Message(void)
{
	ReInit();
}


Message::~Message(void)
{
}

void Message::BindSetting( unsigned char *buffer, int maxSize, char	byteOrder, unsigned char moudleId )
{
	m_buffer = buffer;
	Bind(buffer, maxSize);
	SetByteOrder((ByteOrder)byteOrder);
	m_moudleId = moudleId;
}

void Message::ReInit()
{
	Clear();
	m_moudleId = -1;	//模块Id
	m_id = -1;		//协议号
	m_isResult = -1;	//是回应报文
}

//报文头长度
int Message::HeaderSize()
{
	return sizeof(int) + sizeof(char) + sizeof(short) + sizeof(char);
}	

Message::operator unsigned char*()
{
	return Stream::operator unsigned char*();
}

Message::operator char*()
{
	return Stream::operator char*();
}

Message::operator void*()
{
	return Stream::operator void*();
}

//报文长度,如果报文长度非法返回-1(超出最大限制或者小于最小长度)
int Message::Size()
{
	if ( sizeof(int) >= m_size )
	{
		Stream::Size();
		if ( sizeof(int) > m_size ) 
		{
			Clear();
			return -1;
		}
	}

	return m_size;
}

//模块号
unsigned char Message::MoudleId()
{
	ReadHeader();

	return m_moudleId;
}

//返回协议号
short Message::Id()
{
	ReadHeader();

	return m_id;
}

//是回应报文
bool Message::IsResult()
{
	ReadHeader();

	return 0 == m_isResult?false:true;
}

//设置id
void Message::SetId( short id, bool isResult )
{
	unsigned char moudleId = m_moudleId;
	ReInit();
	m_moudleId = moudleId;
	AddData(m_moudleId);
	AddData(id);
	AddData((char)(isResult?1:0));
}

//读取报文头
void Message::ReadHeader()
{
	if ( -1 != m_moudleId && -1 != m_id && -1 != m_isResult ) return;

	m_pos = sizeof(int);
	GetData(m_moudleId);
	GetData(m_id);
	GetData(m_isResult);
}

}
