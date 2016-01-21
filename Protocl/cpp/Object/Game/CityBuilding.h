#ifndef CITYBUILDING_H
#define CITYBUILDING_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//Ϊ���н�����ש����
class CityBuilding : public Buffer
{
public:
	//�������
	unsigned int	m_buildId;		//����id
	std::string		m_name;			//����
	//��Ӧ����
	int				m_needCoin;//��Ҫ�ṩ�����������
	int				m_payCoin;//�Լ��ṩ�����������

public:
	CityBuilding();
	virtual ~CityBuilding();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CITYBUILDING_H
