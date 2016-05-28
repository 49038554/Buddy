#ifndef BUY_H
#define BUY_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//����
class Buy : public Buffer
{
public:
	//�������
	int					m_itemId;		//����id
	int					m_count;		//����
	unsigned int		m_SellerId;		//����Id��0ϵͳ
	//��Ӧ����

public:
	Buy();
	virtual ~Buy();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //BUY_H
