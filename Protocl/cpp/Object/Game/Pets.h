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
		char	nature;//性格
		short	skill1;
		short	skill2;
		short	skill3;
		short	skill4;
		short	itemId;
		short	HP;//血
		short	WG;//攻
		short	WF;//防
		short	TG;//特攻
		short	TF;//特防
		short	SD;//速度
		char	HPHealthy;//血先天体质
		char	WGHealthy;//攻先天体质
		char	WFHealthy;//防先天体质
		char	TGHealthy;//特攻先天体质
		char	TFHealthy;//特防先天体质
		char	SDHealthy;//速度先天体质
		unsigned char	HPMuscle;//血后天修炼
		unsigned char	WGMuscle;//攻后天修炼
		unsigned char	WFMuscle;//防后天修炼
		unsigned char	TGMuscle;//特攻后天修炼
		unsigned char	TFMuscle;//特防后天修炼
		unsigned char	SDMuscle;//速度后天修炼
		std::vector<char>	race;//17个属性的恶魔果实是否吃过
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
