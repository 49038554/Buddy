#ifndef __WORKER_H__
#define __WORKER_H__

#include "mdk/Logger.h"
#include "mdk/ConfigFile.h"
#include "frame/netserver/STNetHost.h"
#include "frame/netserver/STNetServer.h"
#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "DBCenter/CacheInterface.h"
#include "common/MysqlCluster.h"
#include "Interface/ClusterMgr/cpp/ClusterMgr.h"
#include "Protocl/cpp/Object/SNS/AddBuddy.h"
#include "Protocl/cpp/Object/SNS/DelBuddy.h"
#include "Protocl/cpp/Object/SNS/SetUserData.h"


/**
 * Worker
 */
class Worker : public mdk::STNetServer
{
public:
	Worker(void);
	virtual ~Worker(void);
	
protected:
	/**
	 * 功能：消息处理函数
	 * 说明：该处理会过滤掉除了要处理之外的消息，并记录Log
	 */
	virtual void OnMsg(mdk::STNetHost& host);
	void OnAuth(mdk::STNetHost &host, msg::Buffer &buffer);//认证模块消息响应
	void OnSNS(mdk::STNetHost &host, msg::Buffer &buffer);//社交模块消息响应
	void OnGame(mdk::STNetHost &host, msg::Buffer &buffer);//游戏模酷开消息响应
	//////////////////////////////////////////////////////////////////////////
	// 认证服模块
	//////////////////////////////////////////////////////////////////////////
	// 用户注册消息
	bool OnUserRegister(mdk::STNetHost& host, msg::Buffer& buffer);
	// 用户重设密码
	bool OnResetPassword(mdk::STNetHost& host, msg::Buffer& buffer);
	// 用户绑定手机号码
	bool OnBindingPhone(mdk::STNetHost& host, msg::Buffer& buffer);
	//////////////////////////////////////////////////////////////////////////
	//SNS模块
	//添加伙伴
	void OnAddBuddy(mdk::STNetHost &host, msg::Buffer &buffer);
	void OnDelBuddy(mdk::STNetHost &host, msg::Buffer &buffer);
	//设置用户数据
	void OnSetUserData(mdk::STNetHost &host, msg::Buffer &buffer);
	//////////////////////////////////////////////////////////////////////////
	//游戏模块
	//安装版本数据
	bool Worker::OnSetupVersion(mdk::STNetHost &host, msg::Buffer &buffer);


private:
	friend int main(int argc, char* argv[]);
	bool InitId( const std::string &fileName );
	bool CreateId( mdk::uint32 &id, bool now = true );
	void SaveId( bool now );

	// 创建用户，并根据注册协议的内容，填充用户信息
	bool CreateUser(msg::UserRegister& userRegister, Cache::User& user);
	// 创建一个用户ID，用来创建用户时，生成全局唯一的userId
	bool CreateUserId(int& userId);
	// 创建一个32个字节的randKey，用来创建用户时，填充user结构中的randKey字段
	void RandKey(std::string& randKey);
	// 创建一个用户的昵称，用来创建用户时进行默认填充
	void RandNickname(std::string& nickname, int userId);
	bool SetBuddy(mdk::uint32 userid, mdk::uint32 buddyId, msg::AddBuddy &msg);
	bool DelBuddy(mdk::uint32 userId, mdk::uint32 buddyId, msg::DelBuddy &msg);
	bool SetUserData(msg::SetUserData &msg);

private:
	mdk::Logger     m_log;
	mdk::ConfigFile m_cfg;
	mdk::uint16 m_nodeId;
	mdk::uint32 m_searialNo;
	mdk::ConfigFile *m_pCfg;

	CacheInterface  m_cache;
	MysqlCluster    m_mySQLCluster;
	ClusterMgr      m_cluster;
};

#endif // __WORKER_H__