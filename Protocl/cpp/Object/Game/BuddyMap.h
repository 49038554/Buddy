#ifndef BUDDYMAP_H
#define BUDDYMAP_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace data
{
	typedef struct BUDDY_MAP
	{
		int					id;
		std::vector<short>	buddys;//最大100
		char				shape;//形状：0圆形，1矩形
		double				x;//latitude
		double				y;//longitude
		int					radius;
		double				right;//latitude
		double				bottom;//longitude
		int					city;//城市
		bool				spot;//景区
	}BUDDY_MAP;

}

namespace msg
{

//巴迪分布图
class BuddyMap : public Buffer
{
public:
	//回应参数
	unsigned short					m_lbsVersion;
	std::vector<data::BUDDY_MAP>	m_buddyMaps;//最大10

public:
	BuddyMap();
	virtual ~BuddyMap();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //BUDDYMAP_H
