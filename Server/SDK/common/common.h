#pragma once

/*
	执行字符串指令
	按照分隔符解析出comminds中所有子字符串，
	将子字符串传递ItemCallBack做自定义处理

	comminds 一个字符串
	delimiter	分隔符
	ItemCallBack 处理子字符串回调函数，如果ItemCallBack返回false,将立刻停止解析
	pParam	传递给ItemCallBack的参数

*/
bool StrCommonds( const char *comminds, char *delimiter, void *pParam, bool (*ItemCallback)(void* pParam, const char *item) );

//插入排序
template<class T>
void Sort( T *order, int &count, T v, int (*cmp)(T&,T&) )
{
	int i = 0;
	T temp;
	for ( i = 0; i < count; i++ )
	{
		if ( 0 <= cmp(v, order[i]) ) continue;
		temp = order[i];
		order[i] = v;
		v = temp;
	}
	order[i] = v;
	count++;
}

#ifdef WIN32
typedef __int64				int64;
typedef unsigned __int64	uint64;
#else
#include <sys/types.h>
typedef int64_t				int64;
typedef u_int64_t			uint64;
#endif
#include <string>

void itomemSmall( unsigned char *buf, uint64 value, int size );
uint64 memtoiSmall( unsigned char *buf, int size );
