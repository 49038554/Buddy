#ifndef USER_DATA_H
#define USER_DATA_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
ȡ����б�
*/
class UserData : public Buffer
{
public:
	//�������
	unsigned int	m_userId;			// �û�Id
	std::string		m_nickName;			// �ǳ�
	std::string		m_face;				// ͷ��URL
	bool			m_sex;				// �Ա�true = �� false = Ů
	int				m_coin;				// ����ң���RMB��ֵ������

	// �ڴ�����
	std::string		m_bindImei;			// �˺ţ�Imei
	std::string		m_bindMobile;		// �˺ţ��ֻ�����
	//��Ӧ����

public:
	UserData();
	virtual ~UserData();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //USER_DATA_H
