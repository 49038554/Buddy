// RecoverPool.h: interface for the RecoverPool class.
//
//	回收池
//	1次分配内存，最大为2的MAX_SIZE_POWER次幂，即4k
//	超过4k的对象，请直接使用系统的分配器
//
//	一种节约内存的内存池
//	主要机理：
//		要使用内存，new出来，直接放入池内
//		释放时不delete，而是标记为回收，供下次相同大小的内存申请时使用，循环使用
//
//  内存池
//		1次性申请n块大小相同的内存，等待分配
//		如果一开始申请10000块，而实际只是前面100块内存在循环利用，那就很浪费内存了
//  回收池
//      1次申请1块内存，用完回收，循环利用，如果回收前，又有内存申请，则再申请1块
//		内存使用率比内存池小的多
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
		state = 4,//0未分配，非0已分配
		index = 2,//在池中下标
		poolAddr = 8,//池地址
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
	POOL* m_poolMap[MAX_SIZE_POWER+1];//2的n幂大小的块回收池数组
	mdk::Mutex m_createArrayMutex;
	mdk::Mutex m_createBlockMutex;
};

#endif // !defined(RECOVERPOOL_H)
