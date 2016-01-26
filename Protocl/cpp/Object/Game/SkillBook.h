#ifndef SKILLBOOK_H
#define SKILLBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>

namespace data
{
	typedef struct SKILL
	{
		short				id;
		std::string			name;
		char				race;//属性
		short				power;//威力0~300
		char				type;//1物理，2变化，3特殊
		char				hitRate;//命中率30~101, 101必中技
		std::vector<char>	effects;//特效,最多5个
		bool				isMapSkill;//是地图技能
		char				priority;//先手级别0~6
		std::string			descript;//最大60byte
	}SKILL;
}

namespace msg
{

//物品图鉴
class SkillBook : public Buffer
{
public:
	//回应参数
	unsigned short					m_skillVersion;
	std::vector<data::SKILL>		m_skills;//最大50个

public:
	SkillBook();
	virtual ~SkillBook();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //SKILLBOOK_H
