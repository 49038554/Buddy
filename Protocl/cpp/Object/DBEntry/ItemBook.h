#ifndef ITEMBOOK_H
#define ITEMBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>

namespace msg
{

//��Ʒͼ��
class ItemBook : public Buffer
{
public:
	//��Ӧ����
	std::vector<data::ITEM>			m_items;//���50��

public:
	ItemBook();
	virtual ~ItemBook();
	bool Build( bool isResult = false );//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //ITEMBOOK_H
