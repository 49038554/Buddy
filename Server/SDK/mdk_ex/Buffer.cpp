#include "Buffer.h"
#include <cstdio>
#include <cstring>

namespace mdk
{

FastMemoryPool Buffer::ms_pool(sizeof(mdk::Buffer::LIST_NODE), 100);

Buffer::Buffer()
{
	m_usePool = true;
	m_pHeader = m_pTail = NULL;
	m_sumSize = 0;
}

Buffer::Buffer(bool usePool)
{
	m_usePool = usePool;
	m_pHeader = m_pTail = NULL;
	m_sumSize = 0;
}

Buffer::~Buffer()
{
}

void Buffer::Clear()
{
	LIST_NODE *pDelete = NULL;
	while ( NULL != m_pHeader )
	{
		pDelete = m_pHeader = m_pHeader->pNext;
		if ( m_usePool ) Buffer::ms_pool.Free(pDelete);
		else delete pDelete;
	}
}

int32 Buffer::Size()
{
	return m_sumSize;
}

void Buffer::AddData( unsigned char *pData, int32 size )
{
	mdk::AutoLock lock(&m_lock);
	if ( NULL == m_pHeader )
	{
		//���ӽ��
		if ( m_usePool ) m_pHeader = (LIST_NODE*)Buffer::ms_pool.Alloc();
		else m_pHeader = new LIST_NODE;
		if ( NULL == m_pHeader )
		{
			return;
		}
		m_pTail = m_pHeader;
		m_pHeader->pNext = NULL;
		m_pHeader->pos = 0;
		m_pHeader->size = 0;
	}

	int32 saveSize = 0;
	int32 pos = 0;
	while ( size > 0 )
	{
		//��ʣ�����ݴ�������β
		if ( m_pTail->size + size <= EXPAND_SIZE )//������������
		{
			memcpy( &m_pTail->pData[m_pTail->size], &pData[pos], size );
			m_pTail->size += size;
			pos += size;
			m_sumSize += size;
			break;
		}
		//�Ƚ�ĩβ�����������¼ʣ�����ݳ���
		saveSize = EXPAND_SIZE - m_pTail->size;
		memcpy( &m_pTail->pData[m_pTail->size], &pData[pos], saveSize );
		m_pTail->size += saveSize;
		size -= saveSize;
		pos += saveSize;
		m_sumSize += saveSize;

		//���ӽ��
		if ( m_usePool ) m_pTail->pNext = (LIST_NODE*)Buffer::ms_pool.Alloc();
		else m_pTail->pNext = new LIST_NODE;
		if ( NULL == m_pTail->pNext )
		{
			return;
		}
		m_pTail = m_pTail->pNext;
		m_pTail->pNext = NULL;
		m_pTail->pos = 0;
		m_pTail->size = 0;
	}
}

bool Buffer::GetData( unsigned char *pData, int32 size, bool checkData )
{
	mdk::AutoLock lock(&m_lock);
	if ( m_sumSize < size ) return false;
	if ( !checkData ) m_sumSize -= size;

	int32 getSize = 0;
	int pos = 0;
	LIST_NODE *pHeader = m_pHeader;
	while ( true )
	{
		//��ʣ������ȡ��
		if ( pHeader->size - pHeader->pos >= size )//���ô���һ�����ȡ����
		{
			memcpy( &pData[pos], &pHeader->pData[pHeader->pos], size );
			if ( !checkData ) pHeader->pos += size;
			break;
		}
		//�Ƚ���ͷʣ�����ݳ���ȫ������
		getSize = pHeader->size - pHeader->pos;
		memcpy( &pData[pos], &pHeader->pData[pHeader->pos], getSize );
		if ( !checkData ) pHeader->pos += getSize;
		pos += getSize;
		size -= getSize;

		pHeader = pHeader->pNext;
		if ( !checkData )
		{
			if ( m_usePool ) Buffer::ms_pool.Free(m_pHeader);
			else delete m_pHeader;
			m_pHeader = pHeader;
		}
	}

	return true;
}

bool Buffer::Seek( int32 size )
{
	mdk::AutoLock lock(&m_lock);
	if ( m_sumSize < size ) return false;
	m_sumSize -= size;//��������

	int32 seekSize = 0;//��������
	LIST_NODE *pHeader = m_pHeader;
	while ( true )
	{
		if ( pHeader->size - pHeader->pos >= size ) //��������<��ǰ�ڵ�ʣ�����ݣ��ڽڵ�������
		{
			pHeader->pos += size;
			break;
		}
		//��������>��ǰ�ڵ�ʣ�����ݣ����������ڵ�ʣ������
		seekSize = pHeader->size - pHeader->pos;//ʣ������
		pHeader->pos += seekSize;//����
		size -= seekSize;

		pHeader = pHeader->pNext;
		if ( m_usePool ) Buffer::ms_pool.Free(m_pHeader);
		else delete m_pHeader;
		m_pHeader = pHeader;
	}

	return true;
}

}
