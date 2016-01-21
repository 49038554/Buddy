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
	unsigned short	m_raceVersion;
	unsigned short	m_skillVersion;
	unsigned short	m_itemVersion;
	unsigned short	m_buddyVersion;
	unsigned short	m_lbsVersion;
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