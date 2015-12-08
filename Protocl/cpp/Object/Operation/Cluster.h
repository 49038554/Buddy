#pragma once
#include "../../Buffer.h"
#include <map>
#include <vector>
#include <string>

namespace msg
{

//集群信息数据
class Cluster : public Buffer
{
public:
	typedef struct NODE
	{
		int nodeId;		//结点Id
		std::string ip;
		int port;
	}NODE;
	//map<模块, map<线路, vector<结点>>>
	std::map<Moudle::Moudle, std::map<NetLine::NetLine, std::vector<NODE> > > m_cluster;

public:
	Cluster();
	virtual ~Cluster();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
