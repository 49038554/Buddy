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
	m_moudleId = -1;	//ģ��Id
	m_id = -1;		//Э���
	m_isResult = -1;	//�ǻ�Ӧ����
}

//����ͷ����
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

//���ĳ���,������ĳ��ȷǷ�����-1(����������ƻ���С����С����)
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

//ģ���
unsigned char Message::MoudleId()
{
	ReadHeader();

	return m_moudleId;
}

//����Э���
short Message::Id()
{
	ReadHeader();

	return m_id;
}

//�ǻ�Ӧ����
bool Message::IsResult()
{
	ReadHeader();

	return 0 == m_isResult?false:true;
}

//����id
void Message::SetId( short id, bool isResult )
{
	unsigned char moudleId = m_moudleId;
	ReInit();
	m_moudleId = moudleId;
	AddData(m_moudleId);
	AddData(id);
	AddData((char)(isResult?1:0));
}

//��ȡ����ͷ
void Message::ReadHeader()
{
	if ( -1 != m_moudleId && -1 != m_id && -1 != m_isResult ) return;

	m_pos = sizeof(int);
	GetData(m_moudleId);
	GetData(m_id);
	GetData(m_isResult);
}

}
