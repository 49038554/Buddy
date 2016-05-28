#ifndef DELHOUSE_H
#define DELHOUSE_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//地图技能：瓦割，拆房子
class DelHouse : public Buffer
{
public:
	//请求参数
	unsigned int	m_buildId;		//建筑id

	//回应参数

public:
	DelHouse();
	virtual ~DelHouse();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //DELHOUSE_H
