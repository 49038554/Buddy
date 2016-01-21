#ifndef USEITEM_H
#define USEITEM_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//ʹ����Ʒ
class UseItem : public Buffer
{
public:
	//�������
	short			m_itemId;		//��Ʒid
	unsigned int	m_objectId;		//����id

	//��Ӧ����

public:
	UseItem();
	virtual ~UseItem();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //USEITEM_H
