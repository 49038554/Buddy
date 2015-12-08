#ifndef FAST_MEMORY_POOL_H
#define FAST_MEMORY_POOL_H

#include "mdk/FixLengthInt.h"
//���֧��MAX_THREAD_COUNT���߳���FastMemoryPool�����ϲ���
#define MAX_THREAD_COUNT 5000
#define ToPool mdk::FastMemoryPool::Free

namespace mdk
{

class FastMemoryPool
{
	//�ڴ�أ�������ʽ
	typedef struct POOL
	{
		int				freeCount;	//�ɷ�������
		unsigned char	*buffer;	//n��(sizeof(MEMORY)+m_objectSize)�������ռ�
		POOL			*next;		//��һ���ڴ��
	}POOL;

	//���ڷ�����ڴ浥λ��������ʽ
	typedef struct MEMORY
	{
		POOL			*pPool;		//�����ڴ��
		unsigned char	*buffer;	//�����ȥ���ڴ�
		long			isAlloced;	//0δ���䣬1�ѷ��䣬����ֵ�쳣
		FastMemoryPool	*pThis;		//Ϊ��Free()�������Զ���Ϊstatic
		MEMORY			*next;		//��һ��������ڴ�
	}MEMORY;

public:
	FastMemoryPool(int objectSize, int count);
	~FastMemoryPool();
	void* Alloc();//�̰߳�ȫ�ķ���
	static void Free( void *pObject );//�̰߳�ȫ���ͷţ�static�����ȳ�Ա�������÷�ʽ������ʹ�����������󡢶���ָ�����

protected:
	void* AllocMethod();//���̰߳�ȫ�ķ���
	void Expand();//����
	/*
		�ϲ��ͷ�
			Free()ֻ�����ڴ���������ǣ���UniteFree()�ϲ���1���߳��л��գ���û���ڴ�ɻ���ʱ����Expand()����
	*/
	void UniteFree();

private:
	FastMemoryPool(){}
	FastMemoryPool( const FastMemoryPool &right ){}
	FastMemoryPool& operator = ( const FastMemoryPool &right ){ return *this; }

	int m_objectSize;//ÿ�η��䷵���ڴ�Ĵ�С
	int	m_expandCount;//ÿ���������ӵĴ�СΪm_objectSize���ڴ������
	MEMORY *m_header;//�������ڴ�������ͷ
	POOL *m_poolHeader;//�ڴ��������ͷ


	//lock freeʵ��
	FastMemoryPool(int objectSize, int count, uint64 ownerThreadId);//����һ��ֻ��(ID=ownerThreadId)���̷߳��ʵĵ��߳�FastMemoryPool����
	uint64 m_ownerThreadId;//������ʱ����ӵ��߳�ID

	int m_threadCount;//���ʱ�������߳��������֧��MAX_THREAD_COUNT���̲߳���
	FastMemoryPool* m_poolForThread[MAX_THREAD_COUNT];//m_threadCount���̣߳�ÿ���߳�1���ڴ�أ����̷߳���
	uint64 m_poolIndex[MAX_THREAD_COUNT];//m_threadCount���̵߳��߳�ID����m_poolForThread�±���ͬ
};

}

#endif//FAST_MEMORY_POOL_H
