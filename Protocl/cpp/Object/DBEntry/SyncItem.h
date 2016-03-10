#ifndef SYNCITEM_H
#define SYNCITEM_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//同步物品
class SyncItem : public Buffer
{
public:
	typedef struct ITEM
	{
		short		m_itemId;	//物品id
		bool		m_successed;//成功/失败
		int			m_count;	//数量
		int			m_countInDB;//同步后数据库中数量
	}ITEM;
	//请求参数
	std::vector<ITEM>	m_items;//同步物品,最大500

	//回应参数
	int					m_coin;//作弊罚款

public:
	SyncItem();
	virtual ~SyncItem();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SYNCITEM_H
