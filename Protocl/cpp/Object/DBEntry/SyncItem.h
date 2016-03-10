#ifndef SYNCITEM_H
#define SYNCITEM_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//ͬ����Ʒ
class SyncItem : public Buffer
{
public:
	typedef struct ITEM
	{
		short		m_itemId;	//��Ʒid
		bool		m_successed;//�ɹ�/ʧ��
		int			m_count;	//����
		int			m_countInDB;//ͬ�������ݿ�������
	}ITEM;
	//�������
	std::vector<ITEM>	m_items;//ͬ����Ʒ,���500

	//��Ӧ����
	int					m_coin;//���׷���

public:
	SyncItem();
	virtual ~SyncItem();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SYNCITEM_H
