#ifndef __USER_H__
#define __USER_H__

#include "Protocl/cpp/base/Stream.h"

namespace Cache {

// �����û���Ϣ�������ݿ�user_info�Ľṹ�屣��һ��
class User : public net::Stream
{
public:
	// ���ݿ�����
	unsigned int	id;             // �û�ID
	std::string		pwd;            // ����
	std::string		randKey;        // ���key pwd == md5(md5(�û�����pwd)+randKey)��¼�ɹ�
	std::string		nickName;       // �ǳ�
	std::string		headIco;         // ͷ��URL
	bool			sex;            // �Ա�true = �� false = Ů
	int				coin;           // ֤��������RMB��ֵ������

	// �ڴ�����
	std::string		bindImei;		//�˺ţ�Imei
	std::string		bindMobile;		//�˺ţ��ֻ�����
public:
	User(void);
	virtual ~User(void);

	bool Build(void);  // ����
	bool Parse(void);  // ����
	std::string ToString();//ת���ַ���
private:
	unsigned char m_buffer[1024];
};

}

#endif // __USER_H__