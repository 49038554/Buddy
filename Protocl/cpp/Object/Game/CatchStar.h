#ifndef CATCHSTAR_H
#define CATCHSTAR_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//��ͼ���ܣ�ժ����
class CatchStar : public Buffer
{
public:
	//�������
	//��Ӧ����
	int					m_itemId;		//����id

public:
	CatchStar();
	virtual ~CatchStar();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CATCHSTAR_H
