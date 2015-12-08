#include "FastMemoryPool.h"
#include "mdk/mapi.h"
#include "mdk/atom.h"

namespace mdk
{

FastMemoryPool::FastMemoryPool(int objectSize, int count)
{
	if ( 0 < objectSize % 4 ) m_objectSize = objectSize + (4 - objectSize % 4);
	else m_objectSize = objectSize;
	m_objectSize = objectSize;
	m_expandCount = count;
	m_poolHeader = NULL;
	m_header = NULL;

	m_ownerThreadId = -1;
	m_threadCount = 0;
}

FastMemoryPool::FastMemoryPool(int objectSize, int count, uint64 ownerThreadId)
{
	if ( 0 < objectSize % 4 ) m_objectSize = objectSize + (4 - objectSize % 4);
	else m_objectSize = objectSize;
	m_expandCount = count;
	m_poolHeader = NULL;
	m_header = NULL;

	m_ownerThreadId = CurThreadId();
	m_threadCount = 0;
}

FastMemoryPool::~FastMemoryPool()
{
	POOL *pPool = NULL;
	while ( NULL != m_poolHeader )
	{
		pPool = m_poolHeader;
		m_poolHeader = m_poolHeader->next;
		delete[]pPool->buffer;
		delete pPool;
	}

	int i = 0;
	for ( i = 0; i < m_threadCount; i++ )
	{
		delete m_poolForThread[i];
	}
}

void FastMemoryPool::Expand()
{
	POOL *pPool = new POOL;
	unsigned char *buffer = new unsigned char[(sizeof(MEMORY) + m_objectSize) * m_expandCount];
	if ( NULL == buffer || NULL == pPool ) mdk_assert(false);//ϵͳ�ڴ�����

	pPool->freeCount = m_expandCount;
	pPool->buffer = buffer;
	pPool->next = m_poolHeader;
	m_poolHeader = pPool;

	int i = 0;
	int pos = 0;
	MEMORY *pMemory = (MEMORY*)buffer;
	pMemory->pPool = pPool;
	pMemory->isAlloced = 0;
	pMemory->buffer = &buffer[sizeof(MEMORY) + pos];
	pMemory->pThis = this;
	pMemory->next = NULL;
	pos += sizeof(MEMORY) + m_objectSize;

	for ( i = 1; i < m_expandCount; i++ )
	{
		pMemory->next = (MEMORY*)&buffer[pos];
		pMemory = pMemory->next;
		pMemory->pPool = pPool;
		pMemory->isAlloced = 0;
		pMemory->buffer = &buffer[sizeof(MEMORY) + pos];
		pMemory->pThis = this;
		pMemory->next = NULL;
		pos += sizeof(MEMORY) + m_objectSize;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	m_header = (MEMORY*)buffer;
}

void FastMemoryPool::UniteFree()
{
	POOL *pPool = m_poolHeader;
	MEMORY *pMemory = NULL;
	int i = 0;
	int pos = 0;
	for ( ; NULL != pPool; pPool = pPool->next )
	{
		if ( pPool->freeCount == 0 ) continue;
		i = 0;
		pos = 0;
		for ( i = 0; i < m_expandCount; i++ )
		{
			pMemory = (MEMORY*)&pPool->buffer[pos];
			if ( 0 == pMemory->isAlloced )
			{
				pMemory->next = m_header;
				m_header = pMemory;
			}
			pos += sizeof(MEMORY) + m_objectSize;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	if ( NULL == m_header ) 
	{
		Expand();
	}
	mdk_assert( NULL != m_header );
}

void* FastMemoryPool::Alloc()
{
	uint64 tid = CurThreadId();
	int index = 0;
	FastMemoryPool *pPool = NULL;
	for ( index = 0; index < m_threadCount; index++ )
	{
		if ( tid == m_poolIndex[index] ) 
		{
			pPool = m_poolForThread[index];
			break;
		}
	}
	if ( NULL == pPool )
	{
		pPool = new FastMemoryPool( m_objectSize, m_expandCount, tid );
		if ( NULL == pPool ) mdk_assert(false);//ϵͳ�ڴ�����

		index = AtomAdd(&m_threadCount, 1);
		m_poolIndex[index] = tid;
		m_poolForThread[index] = pPool;
	}
	mdk_assert( tid == pPool->m_ownerThreadId );//һ���ֲ������̱�����ȷ�������ط�����������ÿ��ǲ�������
	return pPool->AllocMethod();
}

void* FastMemoryPool::AllocMethod()
{
	if ( NULL == m_header ) 
	{
		UniteFree();
	}
	MEMORY *pMemory = m_header;
	m_header = m_header->next;

 	mdk_assert( 0 == pMemory->isAlloced );
	mdk_assert( NULL != pMemory->buffer );
	pMemory->next = NULL;

	AtomAdd(&pMemory->pPool->freeCount, -1);
	pMemory->isAlloced = 1;

	return pMemory->buffer;
}

void FastMemoryPool::Free( void *pObject )
{
	if ( NULL == pObject ) return;
	MEMORY *pMemory = (MEMORY*)((char*)pObject - sizeof(MEMORY));
	if ( 0 != ((char*)pMemory - (char*)pMemory->pPool->buffer) % (sizeof(MEMORY) + pMemory->pThis->m_objectSize) )//�����û��Բ������ڴ�ص��ڴ����Free
	{
		mdk_assert(false);
	}
	mdk_assert( NULL == pMemory->next );
	if ( 1 != AtomAdd(&pMemory->isAlloced, -1) )//�����û�δ��Ե�Alloc()/Free()����
	{
		mdk_assert(false);
	}
	AtomAdd(&pMemory->pPool->freeCount, 1);
}

}
