#pragma once

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
	};
}

//报文Id
namespace MsgId
{
	enum MsgId
	{
		//所有模块通用
		connectAuth = 1001,//连接认证
		//认证模块
		userRegister = 1001,//注册
		userLogin = 1002,//登录
		userRelogin = 1003,//重复登录
		userLogout = 1004,//登出
		resetPassword = 1005,//重置密码
		bindingPhone = 1006,//绑定手机号
		//运维模块
		getCluster = 1001,//取集群信息
		cluster = 2001,//集群信息
		getRoomCfg = 1002,//取房间配置
		roomCfg = 2002,//房间配置
		//数据中心
		updateCoin = 1001,//修改虚拟币
		updateItemCount = 1002,//修改道具数量
		updateRoomBacklist = 1003,// 更新特定房间的黑名单信息
		updateRoomDisableList = 1004,// 更新特定房间的禁言列表信息
		updateRoomManagerList = 1005,// 更新特定房间的管理员列表信息
		//通知模块
		event = 1001,//转发事件，聊天，送礼，禁言，离开房间==

		//数据服务端数据或通知
		itemListState = 2001,//商品列表开始/结束
		itemInfo = 2002,//单个商品信息
		roomInfo = 2003,//直播间信息
		lookerListState = 2004,//直播间观众列表开始/结束
		lookerPart = 2005,//直播间观众列表片段
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

		// 互动模块
		EC_IsBacklist       = 121,// 用户是黑名单
		EC_RoomNotExist     = 122,// 房间不存在
		EC_RoomFrozen       = 123,// 房间被冻结
		EC_CoinNotEnough    = 124,// 虚拟币余额不足
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