// TestClient.cpp : 定义控制台应用程序的入口点。
//

#include "Protocl/cpp/base/Socket.h"
#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "Protocl/cpp/Object/Auth/UserLogin.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/UserRelogin.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "Protocl/cpp/Object/Auth/UserLogout.h"

#include "common/MD5Helper.h"

int main(int argc, char* argv[])
{
	net::Socket::SocketInit();
	net::Socket c;
	bool b = c.Init(net::Socket::tcp);
	b = c.Connect("192.168.11.109", 6601);

	bool ret = false;
	MD5Helper md5;
	{
		msg::UserRegister msg;
		msg.m_accountType = AccountType::mobile;
		msg.m_account = "test0006";
		msg.m_pwd = md5.HashString("111111", 6);
		msg.Build();
		c.Send(msg, msg.Size());
		msg.ReInit();
		c.Receive(msg, 1024);
		ret = msg.Parse();
	}
	{
		msg::UserLogin msg;
		msg.m_clientType = ClientType::android;
		msg.m_accountType = AccountType::mobile;
		msg.m_account = "test0005";
		msg.m_pwd = md5.HashString("111111", 6);
		msg.Build();
		c.Send(msg, msg.Size());
		msg.ReInit();
		c.Receive(msg, 1024);
		ret = msg.Parse();
	}

	return 0;
}

