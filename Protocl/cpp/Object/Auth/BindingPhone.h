#ifndef __BINDING_PHONE_H__
#define __BINDING_PHONE_H__

#include "../../Buffer.h"
#include <string>

namespace msg {

// 绑定手机号
class BindingPhone : public Buffer
{
public:
	// 请求参数
	std::string m_phone;    // 手机号

	// 回应参数
	// body

public:
	BindingPhone(void);
	virtual ~BindingPhone(void);
	bool Build(bool isResult = false); // 构造报文，isResult=true构造回应报文，发送
	bool Parse(void);                  // 解析解析，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif // __BINDING_PHONE_H__