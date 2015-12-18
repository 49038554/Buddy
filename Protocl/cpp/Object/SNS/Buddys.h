#ifndef BUDDYS_H
#define BUDDYS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

/*
伙伴列表
最多100人
*/
class Buddys : public Buffer
{
public:
	enum ListType
	{
		buddyList = 0,//伙伴列表
	};
	typedef struct BUDDY
	{
		unsigned int	id;
		std::string		face;
		std::string		nickName;
	}BUDDY;
	//请求参数
	ListType			m_type;		//列表类型
	unsigned int		m_userId;	//用户Id
	std::vector<BUDDY>	m_buddys;	//列表

	//回应参数

public:
	Buddys();
	virtual ~Buddys();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}

#endif //BUDDYS_H