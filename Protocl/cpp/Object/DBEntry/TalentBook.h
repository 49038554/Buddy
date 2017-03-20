#ifndef TALENTBOOK_H
#define TALENTBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>

namespace msg
{

//特性图鉴
class TalentBook : public Buffer
{
public:
	//回应参数
	std::vector<data::TALENT>		m_talents;//最大50个

public:
	TalentBook ();
	virtual ~TalentBook ();
	bool Build( bool isResult = false );//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //TALENTBOOK_H
