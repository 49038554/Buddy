#include "common.h"
#include <string>

bool StrCommonds( const char *src, char *delimiter, void *pParam, bool (*ItemCallback)(void* pParam, const char *item) )
{
	std::string stlSrc = src;
	std::string::size_type startPos = 0; 
	std::string::size_type pos = 0; 
	while ( startPos < stlSrc.size() )
	{
		pos = stlSrc.find( delimiter, startPos );
		if ( std::string::npos == pos ) pos = stlSrc.size();
		std::string item;
		item.assign( stlSrc.c_str(), startPos, pos - startPos );
		startPos = pos + 1;
		if ( !ItemCallback(pParam, item.c_str()) ) return false;
	}

	return true;
}

//将一个整数存储到字节流buf，按照小端字节序(低位在前，高位在后)
void itomemSmall( unsigned char *buf, uint64 value, int size )
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
uint64 memtoiSmall( unsigned char *buf, int size )
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
