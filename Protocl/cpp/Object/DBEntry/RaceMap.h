#ifndef RACEMAP_H
#define RACEMAP_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>
#include <map>

namespace msg
{

//������ӳ��
class RaceMap : public Buffer
{
public:
	//��Ӧ����
	std::map<unsigned char, std::string>		m_races;

public:
	RaceMap();
	virtual ~RaceMap();
	bool Build( bool isResult = false );//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //RACEMAP_H
