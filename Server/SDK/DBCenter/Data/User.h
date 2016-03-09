#ifndef __USER_H__
#define __USER_H__

#include "Protocl/cpp/base/Stream.h"

namespace Cache {

// 缓存用户信息，跟数据库user_info的结构体保持一致
class User : public net::Stream
{
public:
	// 数据库数据
	unsigned int	id;             // 用户ID
	std::string		pwd;            // 密码
	std::string		randKey;        // 随机key pwd == md5(md5(用户传递pwd)+randKey)登录成功
	std::string		nickName;       // 昵称
	std::string		headIco;         // 头像URL
	bool			sex;            // 性别true = 男 false = 女
	int				coin;           // 证能量（由RMB充值产生）

	// 内存数据
	std::string		bindImei;		//账号：Imei
	std::string		bindMobile;		//账号：手机号码
public:
	User(void);
	virtual ~User(void);

	bool Build(void);  // 构造
	bool Parse(void);  // 解析
	std::string ToString();//转换字符串
private:
	unsigned char m_buffer[1024];
};

}

#endif // __USER_H__