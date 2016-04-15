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

//DBCenter访问client类(非线程安全)
class DBCenter
{
public:
	DBCenter(void);
	~DBCenter(void);

	// 创建用户
	bool CreateUser(msg::UserRegister& msg);
	// 重设用户密码
	bool ResetUserPwd(msg::ResetPassword& msg);
	// 绑定手机号码
	bool BindingPhone(msg::BindingPhone& msg);
	// 添加小伙伴
	bool AddBuddy(msg::AddBuddy& msg);
	// 删除小伙伴（双向）
	bool DelBuddy(msg::DelBuddy& msg);
	// 设置用户数据
	bool SetUserData(msg::SetUserData& msg);

	//获取安装数据
	bool GetGameSetupData( 
		unsigned short &raceVersion, std::map<unsigned char, std::string> &races,
		unsigned short &skillVersion, std::vector<data::SKILL> &skills,
		unsigned short &itemVersion, std::vector<data::ITEM> &items,
		unsigned short &buddyVersion, std::vector<data::BUDDY> &buddys,
		unsigned short &lbsVersion, std::vector<data::BUDDY_MAP> &buddyMaps
		);

	// 设置服务器连接信息(IP, PORT)
	void SetServerInfo(const std::string& strIP, int nPort);
	// 获取服务器连接信息(IP, PORT)
	void GetServerInfo(std::string& strIP, int& nPort);

	// 连接服务器
	bool Connect(void);
	// 断开服务器
	void Close(void);

	// 发送数据
	bool Send(msg::Buffer& msg);
	// 接收数据
	bool Receive(msg::Buffer& msg);

	// 连接，发送，接收整合
	bool doLogic(msg::Buffer& msg);

private:
	net::Socket* m_pSocket;       // Socket对象
	std::string  m_strIP;         // IP地址
	int          m_nPort;         // 端口号
	bool         m_bIsConnected;  // 是否已经连接
};

#endif // __DB_CENTER_H__