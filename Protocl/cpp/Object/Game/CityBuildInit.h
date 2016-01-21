#ifndef CITYBUILDINIT_H
#define CITYBUILDINIT_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//��ʼһ�����н���
class CityBuildInit : public Buffer
{
public:
	//�������
	unsigned char	m_timeLimited;	//���������죬���200��
	std::string		m_longitude;	//����
	std::string		m_latitude;		//γ��
	std::string		m_address;		//��ַ
	int				m_radius;		//�뾶����С1000��

	//��Ӧ����
	unsigned int	m_buildId;		//����id

public:
	CityBuildInit();
	virtual ~CityBuildInit();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //CITYBUILDINIT_H
