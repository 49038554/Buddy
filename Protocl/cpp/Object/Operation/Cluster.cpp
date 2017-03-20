#include "Cluster.h"


namespace msg
{

Cluster::Cluster()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Operation);
}


Cluster::~Cluster()
{
}

bool Cluster::Build( bool isResult)
{
	SetId(MsgId::cluster, false);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	std::map<Moudle::Moudle, std::map<NetLine::NetLine, std::vector<NODE> > >::iterator itMoudle;
	bool isEnd = false;
	for ( itMoudle = m_cluster.begin(); itMoudle != m_cluster.end(); itMoudle++ )
	{
		std::map<NetLine::NetLine, std::vector<NODE> > &netLine = itMoudle->second;
		if ( 0 >= netLine.size() ) return false;

		std::map<NetLine::NetLine, std::vector<NODE> >::iterator itLine = netLine.begin();
		for ( ; itLine != netLine.end(); itLine++ )
		{
			std::vector<NODE> &nodes = itLine->second;
			if ( 0 >= nodes.size() ) return false;

			std::vector<NODE>::iterator itNode = nodes.begin();
			for ( ; itNode != nodes.end(); itNode++ )
			{
				if ( !AddData(isEnd) ) return false;//�������
				if ( !AddData((char)itMoudle->first) ) return false;//ģ��
				if ( !AddData((char)itLine->first) ) return false;//��·
				if ( !AddData(itNode->nodeId) ) return false;
				if ( !AddData(itNode->ip) ) return false;
				if ( !AddData(itNode->port) ) return false;
			}
		}
	}
	isEnd = true;
	if ( !AddData(isEnd) ) return false;//�������

	return true;
}

bool Cluster::Parse()
{
	m_cluster.clear();
 	if ( !Buffer::Parse() ) return false;
	if ( IsResult() ) return false;
	bool isEnd = false;
	NODE node;
	char val;
	while ( !isEnd )
	{
		if ( !GetData(isEnd) ) return false;//�������
		if ( isEnd ) break;
		if ( !GetData(val) ) return false;//ģ��Id
		Moudle::Moudle moudleId = (Moudle::Moudle)val;
		if ( !GetData(val) ) return false;//��·Id
		NetLine::NetLine lineId = (NetLine::NetLine)val;
		if ( !GetData(node.nodeId) ) return false;//���Id
		if ( !GetData(node.ip) ) return false;//IP
		if ( !GetData(node.port) ) return false;//�˿�
		m_cluster[moudleId][lineId].push_back(node);
	}

	return true;
}

}
