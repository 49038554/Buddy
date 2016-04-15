#ifndef __DB_CENTER_H__
#define __DB_CENTER_H__

#include "Protocl/cpp/base/Socket.h"
#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "Protocl/cpp/Object/SNS/AddBuddy.h"
#include "Protocl/cpp/Object/SNS/DelBuddy.h"
#include "Protocl/cpp/Object/SNS/SetUserData.h"
#include "mdk/Lock.h"

#include "Protocl/cpp/Object/DBEntry/SkillBook.h"
#include "Protocl/cpp/Object/DBEntry/ItemBook.h"
#include "Protocl/cpp/Object/DBEntry/BuddyBook.h"
#include "Protocl/cpp/Object/DBEntry/BuddyMap.h"

#include <vector>
#include <string>
#include <map>

//DBCenter����client��(���̰߳�ȫ)
class DBCenter
{
public:
	DBCenter(void);
	~DBCenter(void);

	// �����û�
	bool CreateUser(msg::UserRegister& msg);
	// �����û�����
	bool ResetUserPwd(msg::ResetPassword& msg);
	// ���ֻ�����
	bool BindingPhone(msg::BindingPhone& msg);
	// ���С���
	bool AddBuddy(msg::AddBuddy& msg);
	// ɾ��С��飨˫��
	bool DelBuddy(msg::DelBuddy& msg);
	// �����û�����
	bool SetUserData(msg::SetUserData& msg);

	//��ȡ��װ����
	bool GetGameSetupData( 
		unsigned short &raceVersion, std::map<unsigned char, std::string> &races,
		unsigned short &skillVersion, std::vector<data::SKILL> &skills,
		unsigned short &itemVersion, std::vector<data::ITEM> &items,
		unsigned short &buddyVersion, std::vector<data::BUDDY> &buddys,
		unsigned short &lbsVersion, std::vector<data::BUDDY_MAP> &buddyMaps
		);

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

#endif // __DB_CENTER_H__