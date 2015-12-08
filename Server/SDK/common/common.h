#pragma once

/*
	ִ���ַ���ָ��
	���շָ���������comminds���������ַ�����
	�����ַ�������ItemCallBack���Զ��崦��

	comminds һ���ַ���
	delimiter	�ָ���
	ItemCallBack �������ַ����ص����������ItemCallBack����false,������ֹͣ����
	pParam	���ݸ�ItemCallBack�Ĳ���

*/
bool StrCommonds( const char *comminds, char *delimiter, void *pParam, bool (*ItemCallback)(void* pParam, const char *item) );

//��������
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
