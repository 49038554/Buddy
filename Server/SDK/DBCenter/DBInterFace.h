#ifndef __DB_INTERFACE_H__
#define __DB_INTERFACE_H__

#include "Protocl/cpp/base/Socket.h"
#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "mdk/Lock.h"

#include <vector>
#include <string>
#include <map>
/**
 * DBConnect
 * ��װ�����ݿ����ӽڵ㣬����Socket�����ӹرգ��Լ����ͺͽ������ݵ�ҵ���ķ�װ
 */
class DBConnect
{
public:
	DBConnect(void);
	~DBConnect(void);

	// �����û�
	bool CreateUser(msg::UserRegister& msg);
	// �����û�����
	bool ResetUserPwd(msg::ResetPassword& msg);
	// ���ֻ�����
	bool BindingPhone(msg::BindingPhone& msg);

	// ���÷�����������Ϣ(IP, PORT)
	void SetServerInfo(const std::string& strIP, int nPort);
	// ��ȡ������������Ϣ(IP, PORT)
	void GetServerInfo(std::string& strIP, int& nPort);

	// ���ӷ�����
	bool Connect(void);
	// �Ͽ�������
	void Close(void);

	// ��������
	bool Send(msg::Buffer& msg);
	// ��������
	bool Receive(msg::Buffer& msg);

	// ���ӣ����ͣ���������
	bool doLogic(msg::Buffer& msg);

private:
	net::Socket* m_pSocket;       // Socket����
	std::string  m_strIP;         // IP��ַ
	int          m_nPort;         // �˿ں�
	bool         m_bIsConnected;  // �Ƿ��Ѿ�����
};

/**
 * ���ݿ��ѯ��(�̰߳�ȫ�࣬���漰ҵ��)
 */
class DBInterface
{
public:
	// ID����ӳ������
	typedef struct IPMAP_
	{
		int         start;
		int         end;
		std::string city;
		std::string location;
	}IPMAP;

public:
	DBInterface(void);
	~DBInterface(void);

	// ���÷�����������Ϣ(IP, PORT)
	void SetServerInfo(const std::string& strIP, int nPort);
	// ��ȡ������������Ϣ(IP, PORT)
	void GetServerInfo(std::string& strIP, int& nPort);

	// �����û�
	bool CreateUser(msg::UserRegister& msg);
	// �����û�����
	bool ResetUserPwd(msg::ResetPassword& msg);
	// ���ֻ�����
	bool BindingPhone(msg::BindingPhone& msg);

	// ��ȡId����ӳ������
	bool GetIpMap(std::vector<IPMAP> &ipmap);

private:
	/**
	 * �̰߳�ȫ����ط�װ����
	 */
	DBConnect* Alloc(void);
	void Free(DBConnect* pDBNode);

private:
	std::string             m_strIP;         // IP��ַ
	int                     m_nPort;         // �˿ں�
	mdk::Mutex              m_pMutex;        // ��
	std::vector<DBConnect*> m_pSockets;
};

#endif // __DB_INTERFACE_H__