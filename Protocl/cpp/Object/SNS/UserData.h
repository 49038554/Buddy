#ifndef USER_DATA_H
#define USER_DATA_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
取伙伴列表
*/
class UserData : public Buffer
{
public:
	//请求参数
	unsigned int	m_userId;			// 用户Id
	std::string		m_nickName;			// 昵称
	std::string		m_face;				// 头像URL
	bool			m_sex;				// 性别true = 男 false = 女
	int				m_coin;				// 虚拟币（由RMB充值产生）

	// 内存数据
	std::string		m_bindImei;			// 账号：Imei
	std::string		m_bindMobile;		// 账号：手机号码
	//回应参数

public:
	UserData();
	virtual ~UserData();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //USER_DATA_H
