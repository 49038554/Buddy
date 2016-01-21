#ifndef SETUPVERSION_H
#define SETUPVERSION_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//安装版本,地图数据，巴迪图鉴，物品图鉴，技能图鉴
class SetupVersion : public Buffer
{
public:
	//请求参数
	unsigned short	m_raceVersion;
	unsigned short	m_skillVersion;
	unsigned short	m_itemVersion;
	unsigned short	m_buddyVersion;
	unsigned short	m_lbsVersion;
	//回应参数

public:
	SetupVersion();
	virtual ~SetupVersion();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //SETUPINIT_H