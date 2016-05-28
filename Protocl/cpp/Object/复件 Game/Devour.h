#ifndef DEVOUR_H
#define DEVOUR_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//��ͼ���ܣ�����
class Devour : public Buffer
{
public:
	//�������
	int				m_itemId;		//����id

	//��Ӧ����
	int				m_coin;			//�õ����������

public:
	Devour();
	virtual ~Devour();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //DEVOUR_H
