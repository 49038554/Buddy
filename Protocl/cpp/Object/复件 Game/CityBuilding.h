#ifndef CITYBUILDING_H
#define CITYBUILDING_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//为城市建设添砖加瓦
class CityBuilding : public Buffer
{
public:
	//请求参数
	unsigned int	m_buildId;		//建筑id
	std::string		m_name;			//名字
	//回应参数
	int				m_needCoin;//需要提供的虚拟币数量
	int				m_payCoin;//自己提供的虚拟币数量

public:
	CityBuilding();
	virtual ~CityBuilding();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CITYBUILDING_H
