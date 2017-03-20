#ifndef SKILLBOOK_H
#define SKILLBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>

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
	bool Build( bool isResult = false );//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //SKILLBOOK_H
