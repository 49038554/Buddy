#ifndef RACEMAP_H
#define RACEMAP_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>
#include <map>

namespace msg
{

//属性名映射
class RaceMap : public Buffer
{
public:
	//回应参数
	std::map<unsigned char, std::string>		m_races;

public:
	RaceMap();
	virtual ~RaceMap();
	bool Build( bool isResult = false );//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //RACEMAP_H
