#ifndef DELHOUSE_H
#define DELHOUSE_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//��ͼ���ܣ��߸����
class DelHouse : public Buffer
{
public:
	//�������
	unsigned int	m_buildId;		//����id

	//��Ӧ����

public:
	DelHouse();
	virtual ~DelHouse();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //DELHOUSE_H
