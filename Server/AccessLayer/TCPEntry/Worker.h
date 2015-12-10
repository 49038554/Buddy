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

	//��Ӧ����
	bool OnServerMsg( msg::Buffer &buffer );
	//ͨ�ñ���
	bool OnAllMod(mdk::NetHost &host, msg::Buffer &buffer);
	//Auth����
	bool OnAuth(mdk::NetHost &host, msg::Buffer &buffer);
	//��Ϣ����֪ͨ
	bool OnNotify(mdk::NetHost &host, msg::Buffer &buffer);

	//������֤
	bool OnConnectAuth(mdk::NetHost &host, msg::Buffer &buffer);

	//////////////////////////////////////////////////////////////////////////
	//��֤ģ��
	//�û�ע��
	bool OnUserRegister(mdk::NetHost &host, msg::Buffer &buffer);
	//�û���¼
	bool OnUserLogin(mdk::NetHost &host, msg::Buffer &buffer);

private:
	//�����Tcp���������Ĳ���
	void FillTCPParam( mdk::NetHost &host, msg::Buffer &buffer );
	//m_client��ɾ�����
	void SaveConnect(mdk::NetHost &host);//��������
	bool DelConnect(mdk::int64 connectId);//ɾ������
	bool DelConnect(mdk::int64 connectId, mdk::NetHost &host);//ɾ�����ӣ������ر�ɾ�����ӵ�host
	bool GetConnect(mdk::int64 connectId, mdk::NetHost &host);//ȡ������
	//m_user��ɾ�����
	mdk::NetHost AddUser( mdk::NetHost &host);//���ؾɵĵ�¼����
	bool DelUser( mdk::uint32 userId );
	bool GetUser(mdk::uint32 userId, mdk::NetHost& host);

	//�û��ǳ�
	bool UserLogout(mdk::NetHost &host);
	//ѡ��Auth���,randNode = true���ѡ��һ�����
	bool SelectAuthNode( const std::string &accout, mdk::NetHost &host, bool randNode = true );
	bool SelectAuthNode( mdk::uint32 userId, mdk::NetHost &host );
	void SetHostRecv(mdk::NetHost &host, bool enable);//����host�Ƿ���Խ�����Ϣ����ת����Ϣ


private:
	mdk::Logger		m_log;
	mdk::ConfigFile	m_cfg;
	mdk::int32		m_nodeId;//�����
	ClusterMgr		m_cluster;//��Ⱥ
	mdk::int32		m_authSvrCount;//���õ�Auth����
	mdk::int32		m_snsSvrCount;//���õ�sns����
	//�����Ϣ
	class ConnectInfo : public mdk::HostData
	{
	public:
		mdk::int32			nodeId;//���id
		Moudle::Moudle		type;//�������
		NetLine::NetLine	lineType;//��Ӫ����·
		std::string			ip;//��ַ
		mdk::int32			port;//�˿�
	public:
		ConnectInfo(void){}
		virtual ~ConnectInfo(){}
	};

	//�û�����
	class User : public ConnectInfo
	{
	public:
		mdk::uint32					id;//�û�id
		std::string					token;//��¼token
		ClientType::ClientType		clientType;//�ն�����
		AccountType::AccountType	accountType;//�˺����ͣ�����Ӧʧ�ܲ���д��
		std::string					account;//�˺�	�˺�/�ֻ�/qq����/email����Ӧʧ�ܲ���д��

	public:
		User(void) : id(0) {}
		virtual ~User(){}
	};
	std::map<mdk::uint64,mdk::NetHost>	m_client;//δ��¼client<hostId,host>
	mdk::Mutex							m_lockClient;
	std::map<mdk::int32,mdk::NetHost>	m_user;//�ѵ�¼client<userId,host>
	mdk::Mutex							m_lockUser;


	CacheInterface	m_cache;//����
	HostCluster		m_authCluster;//��֤����Ⱥ
	HostCluster		m_snsCluster;//SNS����Ⱥ
	HostCluster		m_notifyCluster;//notify����Ⱥ
};

