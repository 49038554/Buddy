#ifndef SKILLBOOK_H
#define SKILLBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>

namespace msg
{

//物品图鉴
class SkillBook : public Buffer
{
public:
	//回应参数
	std::vector<data::SKILL>		m_skills;//最大50个

public:
	SkillBook();
	virtual ~SkillBook();
	bool Build( bool isResult = false );//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //SKILLBOOK_H
