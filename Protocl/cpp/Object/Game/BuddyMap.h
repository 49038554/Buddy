#ifndef BUDDYMAP_H
#define BUDDYMAP_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace data
{
	typedef struct BUDDY_MAP
	{
		int					id;
		std::vector<short>	buddys;//���100
		char				shape;//��״��0Բ�Σ�1����
		double				x;//latitude
		double				y;//longitude
		int					radius;
		double				right;//latitude
		double				bottom;//longitude
		int					city;//����
		bool				spot;//����
	}BUDDY_MAP;

}

namespace msg
{

//�͵Ϸֲ�ͼ
class BuddyMap : public Buffer
{
public:
	//��Ӧ����
	unsigned short					m_lbsVersion;
	std::vector<data::BUDDY_MAP>	m_buddyMaps;//���10

public:
	BuddyMap();
	virtual ~BuddyMap();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //BUDDYMAP_H
