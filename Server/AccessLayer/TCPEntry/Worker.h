#pragma once

#include "frame/netserver/NetServer.h"
#include "frame/netserver/HostData.h"
#include "mdk/Logger.h"
#include "mdk/ConfigFile.h"
#include "mdk/Lock.h"
#include <vector>
#include <string>
#include "Protocl/cpp/Buffer.h"
#include "DBCenter/CacheInterface.h"
#include "Interface/ClusterMgr/cpp/ClusterMgr.h"
#include "common/HostCluster.h"

class Worker : public mdk::NetServer
{
	friend int main(int argc, char* argv[]);
public:
	Worker(void);
	virtual ~Worker(void);

	virtual void OnConnect(mdk::NetHost &host);
	virtual void OnCloseConnect(mdk::NetHost &host);
	virtual void OnMsg(mdk::NetHost &host);

	//回应报文
	bool OnServerMsg( msg::Buffer &buffer );
	//通用报文
	bool OnAllMod(mdk::NetHost &host, msg::Buffer &buffer);
	//Auth请求
	bool OnAuth(mdk::NetHost &host, msg::Buffer &buffer);
	//消息队列通知
	bool OnNotify(mdk::NetHost &host, msg::Buffer &buffer);

	//连接认证
	bool OnConnectAuth(mdk::NetHost &host, msg::Buffer &buffer);

	//////////////////////////////////////////////////////////////////////////
	//认证模块
	//用户注册
	bool OnUserRegister(mdk::NetHost &host, msg::Buffer &buffer);
	//用户登录
	bool OnUserLogin(mdk::NetHost &host, msg::Buffer &buffer);

private:
	//填充由Tcp服务器填充的参数
	void FillTCPParam( mdk::NetHost &host, msg::Buffer &buffer );
	//m_client增删查操作
	void SaveConnect(mdk::NetHost &host);//保存连接
	bool DelConnect(mdk::int64 connectId);//删除连接
	bool DelConnect(mdk::int64 connectId, mdk::NetHost &host);//删除连接，并返回被删除连接的host
	bool GetConnect(mdk::int64 connectId, mdk::NetHost &host);//取得连接
	//m_user增删查操作
	mdk::NetHost AddUser( mdk::NetHost &host);//返回旧的登录连接
	bool DelUser( mdk::uint32 userId );
	bool GetUser(mdk::uint32 userId, mdk::NetHost& host);

	//用户登出
	bool UserLogout(mdk::NetHost &host);
	//选择Auth结点,randNode = true随机选择一个结点
	bool SelectAuthNode( const std::string &accout, mdk::NetHost &host, bool randNode = true );
	bool SelectAuthNode( mdk::uint32 userId, mdk::NetHost &host );
	void SetHostRecv(mdk::NetHost &host, bool enable);//设置host是否可以接收消息中心转发消息


private:
	mdk::Logger		m_log;
	mdk::ConfigFile	m_cfg;
	mdk::int32		m_nodeId;//结点编号
	ClusterMgr		m_cluster;//集群
	mdk::int32		m_authSvrCount;//配置的Auth数量
	mdk::int32		m_snsSvrCount;//配置的sns数量
	//结点信息
	class ConnectInfo : public mdk::HostData
	{
	public:
		mdk::int32			nodeId;//结点id
		Moudle::Moudle		type;//结点类型
		NetLine::NetLine	lineType;//运营商线路
		std::string			ip;//地址
		mdk::int32			port;//端口
	public:
		ConnectInfo(void){}
		virtual ~ConnectInfo(){}
	};

	//用户数据
	class User : public ConnectInfo
	{
	public:
		mdk::uint32					id;//用户id
		std::string					token;//登录token
		ClientType::ClientType		clientType;//终端类型
		AccountType::AccountType	accountType;//账号类型，（回应失败不填写）
		std::string					account;//账号	账号/手机/qq号码/email（回应失败不填写）

	public:
		User(void) : id(0) {}
		virtual ~User(){}
	};
	std::map<mdk::uint64,mdk::NetHost>	m_client;//未登录client<hostId,host>
	mdk::Mutex							m_lockClient;
	std::map<mdk::int32,mdk::NetHost>	m_user;//已登录client<userId,host>
	mdk::Mutex							m_lockUser;


	CacheInterface	m_cache;//缓存
	HostCluster		m_authCluster;//认证服务集群
	HostCluster		m_snsCluster;//SNS服务集群
	HostCluster		m_notifyCluster;//notify服务集群
};

