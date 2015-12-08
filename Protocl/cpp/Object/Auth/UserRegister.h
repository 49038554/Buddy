#ifndef __USER_REGISTER_H__
#define __USER_REGISTER_H__

#include "../../Buffer.h"
#include <string>

namespace msg {

// 注册
class UserRegister : public Buffer
{
public:
	// 请求参数
	AccountType::AccountType m_accountType;   // 账号类型：1键注册/手机注册
	std::string              m_account;       // 账号:imei/手机号
	std::string              m_pwd;           // 密码md5(明文)

	// 回应参数
	unsigned int			m_userId; // 用户id(失败时候不填写)

public:
	UserRegister(void);
	virtual ~UserRegister(void);
	bool Build(bool isResult = false); //构造报文，isResult=true构造回应报文，发送
	bool Parse(void);                  //解析解析，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif // __USER_REGISTER_H__