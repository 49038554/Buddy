#ifndef PROTOCL_H
#define PROTOCL_H

#include "common/BuddyData.h"

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
		Game = 8,			//游戏模块
		LBS = 9,			//Lbs模块
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
		delBuddy = 1002,	//删除伙伴
		getBuddys = 1003,	//取伙伴
		buddys = 2001,	//伙伴
		chat = 1004,	//聊天
		setUserData = 1005,//设置用户资料
		getUserData = 1006,//取用户资料
		userData = 2002,//用户资料
		//通知模块
		event = 1001,//转发事件
		getEvent = 1002,//读取事件

		//游戏模块
		dekaron = 1001,//挑战
		undekaron = 1002,//挑战取消
		challenge = 1003,//应战
		roundReady = 1004,//回合准备
		sendPet = 1005,//放出宠物
		battleResult = 1006,//战斗结果

		//数据中心
		setupVersion = 1001,//安装版本
		raceMap = 2001,//属性表
		skillBook = 2002,//技能图鉴
		itemBook = 2003,//物品图鉴
		talentBook = 2004,//特性图鉴
		buddyBook = 2005,//巴迪图鉴
		buddyMap = 2006,//巴迪分布地图
		getPlayerData = 1002,//取玩家数据
		player = 2007,//玩家数据
		pets = 2008,//宠物数据
		playerItems = 2009,//玩家物品
		syncItem = 1005,//同步物品
		syncPlayer = 1006,//同步玩家
		syncPets = 1007,//同步宠物，只添加不减少

		buildHouse = 1003,//地图技能：秘密力量，造房子
		treePlant = 1004,//种树

// 		putPet = 1010,//放置宠物
// 		nearInfo = 1011,//周边信息
// 		createShop = 1012,//开店
// 		cityBuildInit = 1016,//开始城市建设
// 		cityBuilding = 1017,//为城市建设添砖加瓦
// 		delHouse = 1018,//地图技能：碎石踢，拆房子

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
		success = 0,				//成功
// 		FormatInvalid = 1,			//非法格式，需要断开Client连接
// 		MsgInvalid = 2,				//非法报文，需要断开Client连接
// 		Refuse = 3,					//拒绝
// 		DBError = 4,				//数据库错误，查数据库服务是否正常
// 		SvrError = 5,				//服务错误，查服务端原因

		paramError = 1,//参数错误
		netError = 2,//网络错误
		msgError = 3,//报文错误
		refuse = 4,//拒绝
		svrBug = 5,//服务存在bug
	};
};


#endif //PROTOCL_H