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
		/*
			test0001 16777223
			test0002 16777224
		*/
		msg::UserRegister msg;
		msg.m_accountType = AccountType::mobile;
		msg.m_account = "test0001";
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
		msg.m_account = "test0001";
		msg.m_pwd = md5.HashString("111111", 6);
		msg.Build();
		c.Send(msg, msg.Size());
		msg.ReInit();
		c.Receive(msg, 1024);
		ret = msg.Parse();
	}
	{
		msg::AddBuddy ab;
		ab.m_step = 0;
		ab.m_msg = "test";
		ab.m_buddyId = 16777224;//test0002
		ab.Build();
		c.Send(ab, ab.Size());
	}
	msg::Buffer buffer;
	while ( true )
	{
		buffer.ReInit();
		c.Receive(buffer, buffer.HeaderSize(), true);
		c.Receive(buffer, buffer.Size());
		if ( !buffer.Parse() ) return 0;
		if ( Moudle::SNS != buffer.MoudleId() ) continue;

		if ( MsgId::addBuddy == buffer.Id() ) 
		{
			msg::AddBuddy msg;
			memcpy(msg, buffer, buffer.Size());
			if ( !msg.Parse() )
			{
				return 0;
			}
			if ( !msg.m_accepted )
			{
				msg.m_code = ResultCode::Success;
				msg.m_step = 0;
				msg.m_buddyId = msg.m_userId;
				msg.Build();
				c.Send(msg, msg.Size());
			}
		}
	}

	return 0;
}

