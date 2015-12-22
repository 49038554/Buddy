#ifndef SET_USER_DATA_H
#define SET_USER_DATA_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
�����û���Ϣ
*/
class SetUserData : public Buffer
{
public:
	//�������
	unsigned int	m_userId;			// �û�Id		SNS������д
	bool			m_pwdOpt;			// ���뱻����
	std::string		m_pwd;				// ����
	bool			m_nickNameOpt;		// �ǳƱ�����
	std::string		m_nickName;			// �ǳ�
	bool			m_faceOpt;			// ͷ��URL������
	std::string		m_face;				// ͷ��URL
	bool			m_sexOpt;			// �Ա�����
	char			m_sex;				// �Ա�true = �� false = Ů
	bool			m_coinOpt;			// ����ң���RMB��ֵ������������
	int				m_coin;				// ����ң���RMB��ֵ������

	// �ڴ�����
	bool			m_bindImeiOpt;		// �˺ţ�Imei������
	std::string		m_bindImei;			// �˺ţ�Imei
	bool			m_bindMobileOpt;	// �˺ţ��ֻ����뱻����
	std::string		m_bindMobile;		// �˺ţ��ֻ�����
	//��Ӧ����

public:
	SetUserData();
	virtual ~SetUserData();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //SET_USER_DATA_H
