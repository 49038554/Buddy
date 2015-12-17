#ifndef PROTOCL_H
#define PROTOCL_H

#define MAX_MSG_SIZE 10240	//报文最大长度
#define BYTE_ORDER	net::smallOrder	//字节序
//模块定义
namespace Moudle
{
	enum Moudle
	{
		all = 0,			//所有模块
		Operation = 1,		//运维模块
		TcpEntry = 2,		//Tcp入口
		Auth = 3,			//认证模块
		Notify = 4,			//广播模块
		DBEntry = 5,		//数据中心
		Client = 6,			//客户端
		SNS = 7,			//社交模块
	};
}

//报文Id
namespace MsgId
{
	enum MsgId
	{
		//运维模块
		getCluster = 1001,//取集群信息
		cluster = 2001,//集群信息
		//所有模块通用
		connectAuth = 1001,//连接认证
		//认证模块
		userRegister = 1001,//注册
		userLogin = 1002,//登录
		userRelogin = 1003,//重复登录
		userLogout = 1004,//登出
		resetPassword = 1005,//重置密码
		bindingPhone = 1006,//绑定手机号
		//SNS模块
		addBuddy = 1001,	//添加伙伴
		//通知模块
		event = 1001,//转发事件
		getEvent = 1002,//读取事件
		//数据中心
	};
}

// 终端类型
namespace ClientType
{
	enum ClientType
	{
		web     = 0,  // web
		flash   = 1,  // flash
		android = 2,  // android
		iphone  = 3,  // iphone
	};
}

// 账号类型
namespace AccountType
{
	enum AccountType
	{
		imei  = 0,
		mobile = 1,
	};
}

namespace NetLine
{
	enum NetLine
	{
		unknow = 0,	//未知线路
		CTCC = 1,	//中国电信  chinaTelecom  
		CMCC = 2,	//中国移动通信 chinamobile
		CUCC = 3,	//中国联通通讯 chinaunicom     
	};
}

namespace ErrorCode
{
	enum ErrorCode
	{
		// 注册模块
		EC_UserExist        = 101,// 用户已经存在！
		 
		// 登陆模块
		EC_TokenError       = 111,// Token错误！
		EC_AccountNotExist  = 112,// 账号不存在！
		EC_PwdError         = 113,// 登陆密码错误！
		EC_ReLogin			= 114,// 重复登陆！

	};
}

namespace ResultCode
{
	enum ResultCode
	{
		Success = 0,				//成功
		FormatInvalid = 1,			//非法格式，需要断开Client连接
		MsgInvalid = 2,				//非法报文，需要断开Client连接
		Refuse = 3,					//拒绝
		DBError = 4,				//数据库错误，查数据库服务是否正常
		SvrError = 5,				//服务错误，查服务端原因
	};
};

//商品大类型
namespace ItemBigType
{
	enum ItemBigType
	{
		all = 0,//全部
		gift = 1,//礼物
		commodity = 2,//商品
		luckGift = 3,//幸运礼物
	};
}

#endif //PROTOCL_H