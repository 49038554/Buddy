#ifndef BUILDHOUSE_H
#define BUILDHOUSE_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//��ͼ���ܣ���ǽ��������
class BuildHouse : public Buffer
{
public:
	//�������
	std::string		m_name;			//����
	std::string		m_longitude;	//����
	std::string		m_latitude;		//γ��
	std::string		m_address;		//��ַ
	int				m_radius;		//�뾶����С100��
	int				m_coin;			//��Ҫ���������

	//��Ӧ����
	int	m_buildId;		//����id

public:
	BuildHouse();
	virtual ~BuildHouse();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //BUILDHOUSE_H
