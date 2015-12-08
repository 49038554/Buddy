#pragma once

#include "mysql/MySqlClient.h"
#include "mdk/ConfigFile.h"
#include <vector>
#include <string>

/*
	数据库集群类（非线程安全）
	参考MysqlCluster.cfg，在应用程序配置文件中添加集群配置
	将应用程序的配置文件对象传递给Init()
*/
class MysqlCluster
{
public:
	MysqlCluster();
	virtual ~MysqlCluster(void);
	bool Init( mdk::ConfigFile &cfg );

	// 轮询结点
	MySqlClient* Node(const std::string& dbName);
	// 用户账号，通过特定的算法，选节点
	MySqlClient* Node(const std::string& dbName, const std::string& account);
	// 用户ID%结点数量，选节点
	MySqlClient* Node(const std::string& dbName, int userId);

private:
	std::map<std::string, std::vector<MySqlClient*> >	m_dbCluster;//map<数据库名，mysql集群>
	MySqlClient m_mysql;
};

