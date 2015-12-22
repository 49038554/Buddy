#ifndef SET_USER_DATA_H
#define SET_USER_DATA_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
设置用户信息
*/
class SetUserData : public Buffer
{
public:
	//请求参数
	unsigned int	m_userId;			// 用户Id		SNS服务填写
	bool			m_pwdOpt;			// 密码被设置
	std::string		m_pwd;				// 密码
	bool			m_nickNameOpt;		// 昵称被设置
	std::string		m_nickName;			// 昵称
	bool			m_faceOpt;			// 头像URL被设置
	std::string		m_face;				// 头像URL
	bool			m_sexOpt;			// 性别被设置
	char			m_sex;				// 性别true = 男 false = 女
	bool			m_coinOpt;			// 虚拟币（由RMB充值产生）被设置
	int				m_coin;				// 虚拟币（由RMB充值产生）

	// 内存数据
	bool			m_bindImeiOpt;		// 账号：Imei被设置
	std::string		m_bindImei;			// 账号：Imei
	bool			m_bindMobileOpt;	// 账号：手机号码被设置
	std::string		m_bindMobile;		// 账号：手机号码
	//回应参数

public:
	SetUserData();
	virtual ~SetUserData();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //SET_USER_DATA_H
