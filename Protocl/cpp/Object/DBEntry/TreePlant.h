#ifndef TREEPLANT_H
#define TREEPLANT_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//�̻�
class TreePlant : public Buffer
{
public:
	//�������
	int		m_houseId;		//�ݵ�id

	//��Ӧ����
	int		m_treeId;		//��id

public:
	TreePlant();
	virtual ~TreePlant();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //TREEPLANT_H