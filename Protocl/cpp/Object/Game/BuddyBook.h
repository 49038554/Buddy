#ifndef BUDDYBOOK_H
#define BUDDYBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>
#include <map>

namespace data
{
	typedef struct BUDDY
	{
		short		number;//全国编号
		std::string	name;//名字
		std::string	descript;//描述,最大60byte
		char		race1;//属性1
		char		race2;//属性2
		char		talent1;//特性1
		char		talent2;//特性2
		char		talent3;//梦特性
		short		itemId;//专属物品，0没有专属物品
		short		hitPoint;//血
		short		physicalA;//攻
		short		physicalD;//防
		short		specialA;//特攻
		short		specialD;//特防
		short		speed;//速度
		std::map<unsigned short, bool> skills;//最大30个
		char		rare;	//出现率 1~99
		char		tame;	//驯化率 1~99
		short		upMumber;//进化后编号，0无进化态
	}BUDDY;
}

namespace msg
{

//巴迪图鉴
class BuddyBook : public Buffer
{
public:
	//回应参数
	unsigned short					m_buddyVersion;
	std::vector<data::BUDDY>		m_buddys;//最大20个

public:
	BuddyBook();
	virtual ~BuddyBook();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //BUDDYBOOK_H
