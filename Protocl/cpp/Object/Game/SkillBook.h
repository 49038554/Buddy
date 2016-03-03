#ifndef SKILLBOOK_H
#define SKILLBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>

namespace data
{
	//����
	typedef struct SKILL
	{
		short				id;
		std::string			name;
		char				race;//����
		short				power;//����0~300
		char				type;//1����2�仯��3����
		char				hitRate;//������30~101, 101���м�
		std::vector<EFFECT>	effects;//��Ч,���5��
		bool				isMapSkill;//�ǵ�ͼ����
		std::string			descript;//���60byte
	}SKILL;
}

namespace msg
{

//��Ʒͼ��
class SkillBook : public Buffer
{
public:
	//��Ӧ����
	std::vector<data::SKILL>		m_skills;//���50��

public:
	SkillBook();
	virtual ~SkillBook();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //SKILLBOOK_H
