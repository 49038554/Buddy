#include "User.h"
#include <cstdio>

namespace Cache {

User::User()
	: id(-1)
	, pwd("NULL")
	, randKey("NULL")
	, nickName("NULL")      
	, headIco("NULL")
	, sex(true)     
	, coin(0)
	, bindImei("NULL")
	, bindMobile("NULL")
{
	Bind(m_buffer, 1024);          // 绑定一个缓冲区，在此之上做转换
	SetByteOrder(net::smallOrder); // 设置字节序
}

User::~User()
{
	// body
}

bool User::Build()
{
	StartAdd();
	if (! AddData(id)) return false;
	if (! AddData(pwd)) return false;
	if (! AddData(randKey)) return false;
	if (! AddData(nickName)) return false;
	if (! AddData(headIco)) return false;
	if (! AddData(sex)) return false;
	if (! AddData(coin)) return false;

	if (! AddData(bindImei)) return false;
	if (! AddData(bindMobile)) return false;

	return true;
}

bool User::Parse()
{
	StartGet();
	if ( -1 == Size() ) return false;

	if (! GetData(id)) return false;
	if (! GetData(pwd)) return false;
	if (! GetData(randKey)) return false;
	if (! GetData(nickName)) return false;
	if (! GetData(headIco)) return false;
	if (! GetData(sex)) return false;
	if (! GetData(coin)) return false;

	if (! GetData(bindImei)) return false;
	if (! GetData(bindMobile)) return false;

	return true;
}

std::string User::ToString()
{
	char info[2048];
	sprintf ( info, "User:\n" );
	// 数据库数据
	sprintf ( info, "%s\tid = %d\n", info, id );             // 用户ID
	sprintf ( info, "%s\tpwd = %s\n", info, pwd.c_str() );            // 密码
	sprintf ( info, "%s\trandKey = %s\n", info, randKey.c_str() );        // 随机key pwd == md5(md5(用户传递pwd)+randKey)登录成功
	sprintf ( info, "%s\tnickname = %s\n", info, nickName.c_str() );       // 昵称
	sprintf ( info, "%s\tpicURL = %s\n", info, headIco.c_str() );         // 头像URL
	sprintf ( info, "%s\tsex = %s\n", info, sex?"男":"女" );            // 性别true = 男 false = 女
	sprintf ( info, "%s\tcoin = %d\n", info, coin );           // 柚元（由RMB充值产生）

	// 内存数据

	return std::string(info);
}

}
