#ifndef PETS_H
#define PETS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <map>
#include <vector>

namespace data
{
	typedef struct PET
	{
		int		id;
		short	number;
		char	talent;//特性
		short	skill1;
		short	skill2;
		short	skill3;
		short	skill4;
		char	nature;//性格
		char	HPHealthy;//血
		char	WGHealthy;//攻
		char	WFHealthy;//防
		char	TGHealthy;//特攻
		char	TFHealthy;//特防
		char	SDHealthy;//速度
		unsigned char	HPMuscle;//血
		unsigned char	WGMuscle;//攻
		unsigned char	WFMuscle;//防
		unsigned char	TGMuscle;//特攻
		unsigned char	TFMuscle;//特防
		unsigned char	SDMuscle;//速度
		std::vector<char>	race;//17个属性的恶魔果实是否吃过
		short	itemId;
	}PET;
}

namespace msg
{

//宠物数据
class Pets : public Buffer
{
public:
	//回应参数
	std::vector<data::PET> m_pets;//最大100只
public:
	Pets();
	virtual ~Pets();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PETS_H
