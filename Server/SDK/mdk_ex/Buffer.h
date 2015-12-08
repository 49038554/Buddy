#ifndef BUFFER_H
#define BUFFER_H
#include "ShareObject.h"
#include "FastMemoryPool.h"
#include "mdk/Lock.h"
namespace mdk
{
	/*
		��������
	*/
#define EXPAND_SIZE 8192	//ÿ�����ݴ�С
	class Buffer : public ShareObject
	{
	public:
		Buffer();
		Buffer(bool usePool);
		virtual ~Buffer();

		void AddData( unsigned char *pData, int32 size );	//��������
		bool GetData( unsigned char *pData, int32 size, bool checkData = false );	//ȡ����
		bool Seek( int32 size );//������ɾ����size byte���ȵ�����
		int32 Size();
		void Clear();

	private:
		static FastMemoryPool ms_pool;//�����NULL,������ڴ�ط���
		typedef struct LIST_NODE
		{
			unsigned char pData[EXPAND_SIZE];	//����
			int32 size;					//���ݳ���
			int32 pos;					//�Ѵ������ݵ�λ��
			LIST_NODE *pNext;			//��һ������
		}LIST_NODE;

		LIST_NODE *m_pHeader;
		LIST_NODE *m_pTail;

		int32	m_sumSize;			//�������н�������ܳ���
		bool	m_usePool;			//ʹ���ڴ�ش�������
		mdk::Mutex m_lock;			//�̰߳�ȫ��
	};

}

#endif //BUFFER_H
