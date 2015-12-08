#pragma once

#include "mysql/MySqlClient.h"
#include "mdk/ConfigFile.h"
#include <vector>
#include <string>

/*
	���ݿ⼯Ⱥ�ࣨ���̰߳�ȫ��
	�ο�MysqlCluster.cfg����Ӧ�ó��������ļ�����Ӽ�Ⱥ����
	��Ӧ�ó���������ļ����󴫵ݸ�Init()
*/
class MysqlCluster
{
public:
	MysqlCluster();
	virtual ~MysqlCluster(void);
	bool Init( mdk::ConfigFile &cfg );

	// ��ѯ���
	MySqlClient* Node(const std::string& dbName);
	// �û��˺ţ�ͨ���ض����㷨��ѡ�ڵ�
	MySqlClient* Node(const std::string& dbName, const std::string& account);
	// �û�ID%���������ѡ�ڵ�
	MySqlClient* Node(const std::string& dbName, int userId);

private:
	std::map<std::string, std::vector<MySqlClient*> >	m_dbCluster;//map<���ݿ�����mysql��Ⱥ>
	MySqlClient m_mysql;
};

