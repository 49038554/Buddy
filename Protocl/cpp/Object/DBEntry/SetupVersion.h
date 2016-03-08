#ifndef SETUPVERSION_H
#define SETUPVERSION_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//��װ�汾,��ͼ���ݣ��͵�ͼ������Ʒͼ��������ͼ��
class SetupVersion : public Buffer
{
public:
	//�������
	unsigned short	m_dataVersion;
	//��Ӧ����

public:
	SetupVersion();
	virtual ~SetupVersion();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //SETUPINIT_H