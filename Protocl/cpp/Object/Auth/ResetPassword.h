#ifndef __RESET_PASSWORD_H__
#define __RESET_PASSWORD_H__

#include "../../Buffer.h"
#include <string>

namespace msg {

// 密码重置
class ResetPassword : public Buffer
{
public:
	// 请求参数
	std::string m_pwd;      // 密码:md5(明文)

	// 回应参数
	// body

public:
	ResetPassword(void);
	virtual ~ResetPassword(void);
	bool Build(bool isResult = false); // 构造报文，isResult=true构造回应报文，发送
	bool Parse(void);                  // 解析解析，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif // __RESET_PASSWORD_H__