#ifndef TREEPLANT_H
#define TREEPLANT_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//绿化
class TreePlant : public Buffer
{
public:
	//请求参数
	int		m_houseId;		//据点id

	//回应参数
	int		m_treeId;		//树id

public:
	TreePlant();
	virtual ~TreePlant();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //TREEPLANT_H
