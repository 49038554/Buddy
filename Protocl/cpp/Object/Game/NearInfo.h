#ifndef NEARINFO_H
#define NEARINFO_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//ʹ����Ʒ
class NearInfo : public Buffer
{
public:
	//�������
	char			m_type;		//��Ϣ����

	//��Ӧ����

public:
	NearInfo();
	virtual ~NearInfo();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //NEARINFO_H
