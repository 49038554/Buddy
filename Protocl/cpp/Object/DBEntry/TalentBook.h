#ifndef TALENTBOOK_H
#define TALENTBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>

namespace msg
{

//����ͼ��
class TalentBook : public Buffer
{
public:
	//��Ӧ����
	std::vector<data::TALENT>		m_talents;//���50��

public:
	TalentBook ();
	virtual ~TalentBook ();
	bool Build( bool isResult = false );//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif //TALENTBOOK_H
