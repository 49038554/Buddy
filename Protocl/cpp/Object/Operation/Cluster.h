#pragma once
#include "../../Buffer.h"
#include <map>
#include <vector>
#include <string>

namespace msg
{

//��Ⱥ��Ϣ����
class Cluster : public Buffer
{
public:
	typedef struct NODE
	{
		int nodeId;		//���Id
		std::string ip;
		int port;
	}NODE;
	//map<ģ��, map<��·, vector<���>>>
	std::map<Moudle::Moudle, std::map<NetLine::NetLine, std::vector<NODE> > > m_cluster;

public:
	Cluster();
	virtual ~Cluster();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
