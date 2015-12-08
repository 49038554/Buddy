#include <cstring>
#include "Stream.h"

#ifdef WIN32
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#endif

//将一个整数存储到字节流buf，按照大端字节序(网络序，高位在前，低位在后)
static void itomemBig( unsigned char *buf, uint64 value, int size )
{
	int move = (size-1) * 8;
	int del = (size-1) * 8;
	int i = size - 1 ;
	for ( ; i >= 0; i-- )
	{
		buf[i] = (char)((value << move) >> del);
		move -= 8;
	}
}

//将buf字节流专程整数，按照大端字节序(网络序，高位在前，低位在后)
static uint64 memtoiBig( unsigned char *buf, int size )
{
	uint64 value = 0;
	int i = 0;
	value += buf[i];
	for ( i = 1; i < size; i++ )
	{
		value <<= 8;
		value += buf[i];
	}
	return value;
}

//将一个整数存储到字节流buf，按照小端字节序(低位在前，高位在后)
static void itomemSmall( unsigned char *buf, uint64 value, int size )
{
	int move = (size-1) * 8;
	int del = (size-1) * 8;
	int i = 0;
	for ( i = 0; i < size; i++ )
	{
		buf[i] = (char)((value << move) >> del);
		move -= 8;
	}
}

//将buf字节流专程整数，按照小端字节序(低位在前，高位在后)
static uint64 memtoiSmall( unsigned char *buf, int size )
{
	uint64 value = 0;
	int i = 0;
	value += buf[size - 1];
	for ( i = size - 2; 0 <= i; i-- )
	{
		value <<= 8;
		value += buf[i];
	}
	return value;
}


namespace net
{

Stream::Stream()
:m_buffer(NULL), m_space(0), m_size(4), m_pos(4), m_byteOrder(bigOrder)
{
}

Stream::~Stream()
{
}

void Stream::Bind(unsigned char *pBuffer, unsigned int space)
{
	m_buffer = pBuffer;
	m_space = space;
	Clear();
}

void Stream::SetByteOrder( ByteOrder order )
{
	m_byteOrder = order;
}

Stream::operator unsigned char*()
{
	I2Stream(m_buffer, m_size, sizeof(int));
	return m_buffer;
}

Stream::operator char*()
{
	I2Stream(m_buffer, m_size, sizeof(int));
	return (char*)m_buffer;
}

Stream::operator void*()
{
	I2Stream(m_buffer, m_size, sizeof(int));
	return m_buffer;
}

void Stream::Clear()
{
	m_size = 4;
	m_pos = 4;
	if ( NULL != m_buffer )
	{
		I2Stream(m_buffer, -1, sizeof(int));
	}
}

int Stream::Size()
{
	if ( 4 >= m_size )
	{
		m_size = Stream2I(m_buffer, sizeof(int));
		if ( 0 > m_size || m_size > m_space )
		{
			Clear();
			return -1;
		}
	}

	return m_size;
}

bool Stream::AddData(bool value)
{
	if ( m_size + sizeof(char) > m_space ) return false;
	m_buffer[m_size] = value;
	m_size += sizeof(char);

	return true;
}

bool Stream::AddData( char value )
{
	if ( m_size + sizeof(char) > m_space ) return false;
	m_buffer[m_size] = value;
	m_size += sizeof(char);

	return true;
}

bool Stream::AddData( short value )
{
	if ( m_size + sizeof(short) > m_space ) return false;
	I2Stream(&m_buffer[m_size], value, sizeof(short));
	m_size += sizeof(short);
	
	return true;
}

bool Stream::AddData( int value )
{
	if ( m_size + sizeof(int) > m_space ) return false;
	I2Stream(&m_buffer[m_size], value, sizeof(int));
	m_size += sizeof(int);

	return true;
}

bool Stream::AddData( int64 value )
{
	if ( m_size + sizeof(int64) > m_space ) return false;
	I2Stream(&m_buffer[m_size], value, sizeof(int64));
	m_size += sizeof(int64);

	return true;
}

bool Stream::AddData( const void* stream, int size )
{
	if ( !AddData( size ) ) return false;
	if ( m_size + size > m_space ) 
	{
		m_size -= sizeof(int);
		return false;
	}
	memcpy( &m_buffer[m_size], stream, size );
	m_size += size;
	
	return true;
}

bool Stream::AddData( const std::string &stream )
{
	if ( 0 == stream.size() ) return false;
	if ( !AddData( (int)(stream.size() + 1) ) ) return false;
	if ( m_size + stream.size() + 1 > m_space ) 
	{
		m_size -= sizeof(int);
		return false;
	}
	memcpy( &m_buffer[m_size], stream.data(), stream.size() );
	m_size += stream.size();
	m_buffer[m_size] = '\0';
	m_size += 1;

	return true;
}

bool Stream::AddData(float value)
{
	if ( m_size + sizeof(float) > m_space ) return false;
	memcpy( &m_buffer[m_size], &value, sizeof(float) );
	m_size += sizeof(float);

	return true;
}

bool Stream::AddData(double value)
{
	if ( m_size + sizeof(double) > m_space ) return false;
	memcpy( &m_buffer[m_size], &value, sizeof(double) );
	m_size += sizeof(double);

	return true;
}

bool Stream::GetData(bool &value)
{
	if ( m_pos + sizeof(char) > m_size ) return false;
	value = m_buffer[m_pos];
	m_pos += sizeof(char);

	return true;
}

bool Stream::GetData( char &value )
{
	if ( m_pos + sizeof(char) > m_size ) return false;
	value = m_buffer[m_pos];
	m_pos += sizeof(char);

	return true;
}

bool Stream::GetData( unsigned char &value )
{
	if ( m_pos + sizeof(char) > m_size ) return false;
	value = m_buffer[m_pos];
	m_pos += sizeof(char);

	return true;
}

bool Stream::GetData( short &value )
{
	if ( m_pos + sizeof(short) > m_size ) return false;
	value = Stream2I(&m_buffer[m_pos], sizeof(short));
	m_pos += sizeof(short);

	return true;
}

bool Stream::GetData( unsigned short &value )
{
	if ( m_pos + sizeof(short) > m_size ) return false;
	value = Stream2I(&m_buffer[m_pos], sizeof(short));
	m_pos += sizeof(short);

	return true;
}

bool Stream::GetData( int &value )
{
	if ( m_pos + sizeof(int) > m_size ) return false;
	value = Stream2I(&m_buffer[m_pos], sizeof(int));
	m_pos += sizeof(int);

	return true;
}

bool Stream::GetData( unsigned int &value )
{
	if ( m_pos + sizeof(int) > m_size ) return false;
	value = Stream2I(&m_buffer[m_pos], sizeof(int));
	m_pos += sizeof(int);

	return true;
}

bool Stream::GetData( int64 &value )
{
	if ( m_pos + sizeof(int64) > m_size ) return false;
	value = Stream2I(&m_buffer[m_pos], sizeof(int64));
	m_pos += sizeof(int64);

	return true;
}

bool Stream::GetData( uint64 &value )
{
	if ( m_pos + sizeof(int64) > m_size ) return false;
	value = Stream2I(&m_buffer[m_pos], sizeof(int64));
	m_pos += sizeof(int64);

	return true;
}

bool Stream::GetData(void* stream, int &size)
{
	if ( !GetData( size ) ) return false;
	if ( 0 >= size || m_pos + size > m_size ) 
	{
		m_pos -= sizeof(int);
		return false;
	}
	memcpy( stream, &m_buffer[m_pos], size );
	m_pos += size;
	
	return true;
}

bool Stream::GetData(std::string &str)
{
	int size = 0;
	str = "";
	void *stream = GetPointer(size);
	if ( NULL == stream ) return false;
	if ( 0 < size )
	{
		str = std::string((char*)stream, size - 1);
	}

	return true;
}

void* Stream::GetPointer( int &size )
{
	if ( !GetData( size ) ) return NULL;
	if ( 0 >= size || m_pos + size > m_size ) 
	{
		m_pos -= sizeof(int);
		return NULL;
	}
	void *stream = &m_buffer[m_pos];
	m_pos += size;
	
	return stream;
}

bool Stream::GetData(float &value)
{
	if ( m_pos + sizeof(float) > m_size ) return false;
	memcpy( &value, &m_buffer[m_pos], sizeof(float) );
	m_pos += sizeof(float);

	return true;
}

bool Stream::GetData(double &value)
{
	if ( m_pos + sizeof(double) > m_size ) return false;
	memcpy( &value, &m_buffer[m_pos], sizeof(double) );
	m_pos += sizeof(double);

	return true;
}

//将一个整数存储到字节流buf，按照byteOrder字节序(0网络序，1大端，2小端)
void Stream::I2Stream( unsigned char *buf, uint64 value, int size )
{
	if ( netOrder == m_byteOrder )
	{
		if ( size > 8 ) return;
		int high = value >> 32;
		int low = value;

		if ( 4 < size )
		{
			int netOrder = htonl(high);		
			memcpy(buf, &netOrder, sizeof(int));
			netOrder = htonl(low);
			memcpy(&buf[sizeof(int)], &netOrder, sizeof(int));
		}
		else 
		{
			int netOrder = htonl(low);
			netOrder = netOrder >> (4 - size) * 8;
			memcpy(buf, &netOrder, size);
		}
	}
	else if ( bigOrder == m_byteOrder )itomemBig(buf, value, size);
	else itomemSmall(buf, value, size);
}

//将buf字节流专程整数，按照byteOrder字节序(0网络序，1大端，2小端)
uint64 Stream::Stream2I( unsigned char *buf, int size )
{
	if ( netOrder == m_byteOrder )
	{
		if ( size > 8 ) return 0;

		uint64 value = 0;
		if ( 4 < size )
		{
			unsigned int high = ntohl(*((unsigned int*)buf) );
			unsigned int low = ntohl(*((unsigned int*)&buf[sizeof(int)]) );
			value = high;
			value = value << 32;
			value += low;
		}
		else 
		{
			char stream[4];
			int i = 0;
			for ( i = 0; i < 4 - size; i++ ) stream[i] = 0;
			int j = 0;
			for ( ; i < 4; i++ ) stream[i] = buf[j++];
			unsigned int low = ntohl(*((unsigned int*)stream) );
			value = low;
		}

		return value;
	}
	else if ( bigOrder == m_byteOrder ) return memtoiBig(buf, size);
	else return memtoiSmall(buf, size);
}

}
