#ifndef CITYBUILDINIT_H
#define CITYBUILDINIT_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//开始一处城市建设
class CityBuildInit : public Buffer
{
public:
	//请求参数
	unsigned char	m_timeLimited;	//建造期限天，最大200天
	std::string		m_longitude;	//经度
	std::string		m_latitude;		//纬度
	std::string		m_address;		//地址
	int				m_radius;		//半径，最小1000米

	//回应参数
	unsigned int	m_buildId;		//建筑id

public:
	CityBuildInit();
	virtual ~CityBuildInit();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //CITYBUILDINIT_H
