#ifndef ITEMBOOK_H
#define ITEMBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>

namespace data
{
	typedef struct ITEM
	{
		short			id;
		std::string		name;
		int				coin;//��ֵ
		std::string		descript;//���60byte
	}ITEM;
}

namespace msg
{

//��Ʒͼ��
class ItemBook : public Buffer
{
public:
	//��Ӧ����
	unsigned short					m_itemVersion;
	std::vector<data::ITEM>			m_items;//���50��

public:
	ItemBook();
	virtual ~ItemBook();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //ITEMBOOK_H
