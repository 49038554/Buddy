// TestClient.cpp : 定义控制台应用程序的入口点。
//

#include "Protocl/cpp/base/Socket.h"
#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "Protocl/cpp/Object/Auth/UserLogin.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/UserRelogin.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "Protocl/cpp/Object/Auth/UserLogout.h"
#include "Protocl/cpp/Object/SNS/AddBuddy.h"
#include "Protocl/cpp/Object/Notify/Event.h"
#include "Protocl/cpp/Object/Notify/GetEvent.h"

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
		/*
			test0005 16777221
			test0006 16777222
		*/
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
		{
			msg::GetEvent msg;
			msg.Build();
			c.Send(msg, msg.Size());
			msg::Event e;
			msg::AddBuddy ab;
			e.ReInit();
			c.Receive(e, e.HeaderSize(), true);
			int n = e.Size();
			c.Receive(e, e.Size());
			ret = e.Parse();
			ab.ReInit();
			memcpy( ab, e.m_msg, e.m_msg.Size() );
			ret = ab.Parse();


			e.ReInit();
			e.m_msg.ReInit();
			c.Receive(e, e.HeaderSize(), true);
			n = e.Size();
			c.Receive(e, e.Size());
			ret = e.Parse();
			ab.ReInit();
			memcpy( ab, e.m_msg, e.m_msg.Size() );
			ret = ab.Parse();

			e.ReInit();
			e.m_msg.ReInit();
			c.Receive(e, e.HeaderSize(), true);
			n = e.Size();
			c.Receive(e, e.Size());
			ret = e.Parse();
			ab.ReInit();
			memcpy( ab, e.m_msg, e.m_msg.Size() );
			ret = ab.Parse();
		}

		msg::AddBuddy ab;
		ab.m_step = 0;
		ab.m_msg = "test";
		ab.m_buddyId = 16777221;//test0005
		ab.Build();
		c.Send(ab, ab.Size());
		c.Send(ab, ab.Size());
		c.Send(ab, ab.Size());
		msg::Event n;
		c.Receive(n, 1024);
		ret = n.Parse();
		ab.ReInit();
		memcpy( ab, n.m_msg, n.m_msg.Size() );
		ret = ab.Parse();
		ret = false;
	}

	return 0;
}

