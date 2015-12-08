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
 * 封装的数据库连接节点，控制Socket的连接关闭，以及发送和接收数据等业务层的封装
 */
class DBConnect
{
public:
	DBConnect(void);
	~DBConnect(void);

	// 创建用户
	bool CreateUser(msg::UserRegister& msg);
	// 重设用户密码
	bool ResetUserPwd(msg::ResetPassword& msg);
	// 绑定手机号码
	bool BindingPhone(msg::BindingPhone& msg);

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

/**
 * 数据库查询类(线程安全类，不涉及业务)
 */
class DBInterface
{
public:
	// ID城市映射数据
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

	// 设置服务器连接信息(IP, PORT)
	void SetServerInfo(const std::string& strIP, int nPort);
	// 获取服务器连接信息(IP, PORT)
	void GetServerInfo(std::string& strIP, int& nPort);

	// 创建用户
	bool CreateUser(msg::UserRegister& msg);
	// 重设用户密码
	bool ResetUserPwd(msg::ResetPassword& msg);
	// 绑定手机号码
	bool BindingPhone(msg::BindingPhone& msg);

	// 获取Id城市映射数据
	bool GetIpMap(std::vector<IPMAP> &ipmap);

private:
	/**
	 * 线程安全的相关封装操作
	 */
	DBConnect* Alloc(void);
	void Free(DBConnect* pDBNode);

private:
	std::string             m_strIP;         // IP地址
	int                     m_nPort;         // 端口号
	mdk::Mutex              m_pMutex;        // 锁
	std::vector<DBConnect*> m_pSockets;
};

#endif // __DB_INTERFACE_H__