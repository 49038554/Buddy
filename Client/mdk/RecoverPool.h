// RecoverPool.h: interface for the RecoverPool class.
//
//	���ճ�
//	1�η����ڴ棬���Ϊ2��MAX_SIZE_POWER���ݣ���4k
//	����4k�Ķ�����ֱ��ʹ��ϵͳ�ķ�����
//
//	һ�ֽ�Լ�ڴ���ڴ��
//	��Ҫ����
//		Ҫʹ���ڴ棬new������ֱ�ӷ������
//		�ͷ�ʱ��delete�����Ǳ��Ϊ���գ����´���ͬ��С���ڴ�����ʱʹ�ã�ѭ��ʹ��
//
//  �ڴ��
//		1��������n���С��ͬ���ڴ棬�ȴ�����
//		���һ��ʼ����10000�飬��ʵ��ֻ��ǰ��100���ڴ���ѭ�����ã��Ǿͺ��˷��ڴ���
//  ���ճ�
//      1������1���ڴ棬������գ�ѭ�����ã��������ǰ�������ڴ����룬��������1��
//		�ڴ�ʹ���ʱ��ڴ��С�Ķ�
//////////////////////////////////////////////////////////////////////

#if !defined RECOVERPOOL_H
#define RECOVERPOOL_H

#include "Lock.h"

#define POOL_SIZE (4)
#define MAX_SIZE_POWER (12)
class RecoverPool  
{
	enum BlockInfo
	{
		state = 4,//0δ���䣬��0�ѷ���
		index = 2,//�ڳ����±�
		poolAddr = 8,//�ص�ַ
	};
public:
	RecoverPool();
	virtual ~RecoverPool();
	
public:
	void* Alloc( unsigned int objectSize, int arraySize = 1 );
	void Free( void *pObject );
	void Release();
		
private:
	int GetPoolIndex(unsigned int &size);
	typedef struct POOL
	{
		unsigned char* block[POOL_SIZE];
		int useCount;
		POOL *next;
	}POOL;
	RecoverPool::POOL* GetPool( RecoverPool::POOL *&pPool );
	void *AllocFromPool( unsigned int size, RecoverPool::POOL *pPool );
	void ReleaseArray(RecoverPool::POOL *pPool);
private:
	POOL* m_poolMap[MAX_SIZE_POWER+1];//2��n�ݴ�С�Ŀ���ճ�����
	mdk::Mutex m_createArrayMutex;
	mdk::Mutex m_createBlockMutex;
};

#endif // !defined(RECOVERPOOL_H)
