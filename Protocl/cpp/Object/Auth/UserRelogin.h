#ifndef __USER_RELOGIN_H__
#define __USER_RELOGIN_H__

#include "../../Buffer.h"
#include <string>

namespace msg {

// 登录
class UserRelogin : public Buffer
{
public:
	// 请求参数

	// 回应参数
	std::string		m_position;//登录位置

public:
	UserRelogin(void);
	virtual ~UserRelogin(void);
	bool Build(bool isResult = false);  // 构造报文，isResult=true构造回应报文，发送
	bool Parse(void);                   // 解析解析，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif // __USER_RELOGIN_H__