// RecoverPool.cpp: implementation of the RecoverPool class.
//
//////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cstring>
#include "RecoverPool.h"
#include "FixLengthInt.h"
#include "atom.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RecoverPool::RecoverPool()
{
	memset(m_poolMap, 0, sizeof(unsigned long*)*(MAX_SIZE_POWER+1));
}

RecoverPool::~RecoverPool()
{
	Release();
}

int RecoverPool::GetPoolIndex(unsigned int &size)
{
    unsigned int i = 1;
	int poolIdx = 0;
	for ( ; i < size; poolIdx++ ) i *= 2;
	if ( poolIdx > MAX_SIZE_POWER ) poolIdx = -1;

	return poolIdx;
}

RecoverPool::POOL *RecoverPool::GetPool( RecoverPool::POOL *&pPool )
{
	if ( NULL != pPool ) return pPool;

	mdk::AutoLock lock(&m_createArrayMutex);
	if ( NULL == pPool )
	{
		pPool = new POOL;
		if ( NULL == pPool ) return NULL;

		memset( pPool, 0, sizeof(POOL) );
	}
	return pPool;
}

void* RecoverPool::Alloc( unsigned int objectSize, int arraySize )
{
	POOL *pPool = NULL;
	
	unsigned int allocSize = objectSize*arraySize;
	//找到尺寸所属的数组
	int poolIdx = GetPoolIndex( objectSize );
	if ( -1 == poolIdx ) return NULL;
	pPool = GetPool(m_poolMap[poolIdx]);
	if ( NULL == pPool ) return NULL;
	
	//从数组中分配/从系统分配，放入数组
	void* pObject = NULL;
	for ( ; true; )
	{
		pObject = AllocFromPool( allocSize, pPool );
		if ( -1 == (long)pObject ) return NULL;
		if ( NULL != pObject ) return pObject;
		pPool = GetPool(pPool->next);
		if ( NULL == pPool ) return NULL;
	}
	
	return pObject;
}

void* RecoverPool::AllocFromPool( unsigned int size, POOL *pPool )
{
	if ( POOL_SIZE <= pPool->useCount ) return NULL;
	if ( POOL_SIZE <= mdk::AtomAdd( &pPool->useCount, 1) ) //使用计数+
	{
		mdk::AtomDec( &pPool->useCount, 1 );
		return NULL;
	}
	unsigned char *pBlock = NULL;
	unsigned short i = 0;
	for ( ; i < POOL_SIZE; i++ )
	{
		if ( NULL == pPool->block[i] )
		{
			//创建新Block
			mdk::AutoLock lock(&m_createBlockMutex);
			if ( NULL == pPool->block[i] )
			{
				pBlock = new unsigned char[state+index+poolAddr+size];
				if ( NULL == pBlock ) return (void*)-1;

				pBlock[0] = 1;
				pBlock[1] = 0;
				pBlock[2] = 0;
				pBlock[3] = 0;
				memcpy( &pBlock[state], &i, index );
				memcpy( &pBlock[state+index], &pPool, poolAddr );
				pPool->block[i] = pBlock;
				return (void*)&pBlock[state+index+poolAddr];
			}
		}

		pBlock = pPool->block[i];
		if ( 0 != pBlock [0] || 0 != pBlock [1] || 0 != pBlock [2] || 0 != pBlock [3] ) continue;
		if ( 0 < mdk::AtomAdd(&pBlock[0], 1) ) continue;
		return (void*)&pBlock[state+index+poolAddr];
	}
	return NULL;
}

void RecoverPool::Free( void *pObject )
{
	unsigned char *pBlock = (unsigned char*)pObject;
	pBlock = pBlock - (state+index+poolAddr);
	POOL *pPool;
	memcpy( &pPool, &pBlock[state+index], poolAddr );
	unsigned short idx = 0;
	memcpy( &idx, &pBlock[state], index );
	
	if (!( 0 <= idx && idx < POOL_SIZE ))assert( false );//非法游标
	if (!( pBlock == pPool->block[idx] ))assert( false );//非法地址
	if (!(0 != pBlock [0] || 0 != pBlock [1] || 0 != pBlock [2] || 0 != pBlock [3]))assert( false );//重复释放
	
	mdk::AtomSet( &pBlock[0], 0 );
	mdk::AtomDec( &pPool->useCount, 1 );//使用计数-
}

void RecoverPool::Release()
{
	POOL *pPool = NULL;
	int i = 0;
	for ( ; i < 33; i++ )
	{
		pPool = m_poolMap[i];
		ReleaseArray(pPool);
	}
}

void RecoverPool::ReleaseArray(RecoverPool::POOL *pPool)
{
	if ( NULL == pPool ) return;
	if ( NULL != pPool->next ) ReleaseArray(pPool->next);
	unsigned short i = 0;
	unsigned char* pBlock;
	for ( ; i < POOL_SIZE; i++ )
	{
		pBlock = pPool->block[i];
		if ( NULL == pBlock ) continue;
		delete[]pBlock;
		pPool->block[i] = NULL;
	}
	delete pPool;
	return;
}
