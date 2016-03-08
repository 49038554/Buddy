#ifndef BUILDHOUSE_H
#define BUILDHOUSE_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//地图技能：光墙，建房子
class BuildHouse : public Buffer
{
public:
	//请求参数
	std::string		m_name;			//名字
	std::string		m_longitude;	//经度
	std::string		m_latitude;		//纬度
	std::string		m_address;		//地址
	int				m_radius;		//半径，最小100米
	int				m_coin;			//需要虚拟币数量

	//回应参数
	int	m_buildId;		//建筑id

public:
	BuildHouse();
	virtual ~BuildHouse();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //BUILDHOUSE_H
