#ifndef TALENTBOOK_H
#define TALENTBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>

namespace data
{
	typedef struct TALENT
	{
		short			id;
		std::string		name;
		std::vector<EFFECT>	effects;//��Ч,���5��
		std::string		descript;//���60byte
	}TALENT;
}

namespace msg
{

//����ͼ��
class TalentBook : public Buffer
{
public:
	//��Ӧ����
	std::vector<data::TALENT>		m_talents;//���100��

public:
	TalentBook ();
	virtual ~TalentBook ();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //TALENTBOOK_H