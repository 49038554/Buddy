#ifndef PICK_H
#define PICK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//ժ��ʵ
class Pick : public Buffer
{
public:
	//�������
	int					m_treeId;			//��id
	std::vector<short>	m_itemId;			//����id

	//��Ӧ����
	int					m_nextTime;			//�¸���ʵ����ʱ��

public:
	Pick();
	virtual ~Pick();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PICK_H
