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
#include "Source/Client.h"

#include "Console.h"

Client g_cli;

char* OnCommand(std::vector<std::string> *cmd);

int main(int argc, char* argv[])
{
	g_cli.Connect(Client::TcpSvr, "127.0.0.1", 6601);
	g_cli.Start();

	mdk::Console cmd("Buddy", 256);
	cmd.Start((mdk::FuntionPointer)OnCommand);
	cmd.WaitStop();

	return 0;
}

void Helper();
char* OnCommand(std::vector<std::string> *param)
{
	std::vector<std::string> &cmd = *param;

	if ( "register" == cmd[0] )
	{
		if ( 3 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		bool isMobile = false;
		if ( 3 < cmd.size() && "true" == cmd[3] ) isMobile = true;
		g_cli.Register(isMobile, cmd[1], cmd[2]);
	}
	else if ( "login" == cmd[0] )
	{
		if ( 3 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		bool isMobile = false;
		if ( 3 < cmd.size() && "true" == cmd[3] ) isMobile = true;
		g_cli.Login(isMobile, cmd[1], cmd[2]);
	}
	else if ( "bind" == cmd[0] )
	{
		if ( 2 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		g_cli.BindPhone( cmd[1] );
	}
	else if ( "setPwd" == cmd[0] )
	{
		if ( 3 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		std::string talk = "play";
		if ( cmd[1] != cmd[2] ) return "2次密码不一致";
		g_cli.SetPassword( cmd[1] );
	}
	else if ( "addBuddy" == cmd[0] )
	{
		if ( 2 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		std::string talk = "play";
		if ( 2 < cmd.size() ) talk = cmd[2];
		unsigned int bid;
		sscanf(cmd[1].c_str(), "%u", &bid);
		if ( 0 >= bid )
		{
			Helper();
			return NULL;
		}
		g_cli.AddBuddy( bid, talk );
	}
	else if ( "acceptBuddy" == cmd[0] )
	{
		if ( 2 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		bool accept = false;
		if ( 2 < cmd.size() ) accept = "true" == cmd[2]?true:false;
		std::string talk = "play";
		if ( 3 < cmd.size() ) talk = cmd[3];
		unsigned int bid;
		sscanf(cmd[1].c_str(), "%u", &bid);
		if ( 0 >= bid )
		{
			Helper();
			return NULL;
		}
		g_cli.AcceptBuddy( bid, talk, accept );
	}
	else if ( "delBuddy" == cmd[0] )
	{
		if ( 2 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		unsigned int bid;
		sscanf(cmd[1].c_str(), "%u", &bid);
		if ( 0 >= bid )
		{
			Helper();
			return NULL;
		}
		g_cli.DelBuddy( bid );
	}
	else if ( "getBuddys" == cmd[0] )
	{
		if ( 1 != cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		g_cli.GetBuddys();
	}
	else if ( "chat" == cmd[0] )
	{
		if ( 3 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		unsigned int bid;
		sscanf(cmd[1].c_str(), "%u", &bid);
		if ( 0 >= bid )
		{
			Helper();
			return NULL;
		}
		unsigned char recvType = 0;
		unsigned int val;
		if ( 3 < cmd.size() ) 
		{
			sscanf(cmd[3].c_str(), "%u", &val);
			if ( 0 > recvType || 2 < recvType )
			{
				Helper();
				return NULL;
			}
			recvType = val;
		}
		g_cli.Chat(bid, recvType, cmd[2]);
	}
	else if ( "getUser" == cmd[0] )
	{
		if ( 2 != cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		unsigned int uid;
		sscanf(cmd[1].c_str(), "%u", &uid);
		if ( 0 >= uid )
		{
			Helper();
			return NULL;
		}
		g_cli.GetUserData(uid);
	}
	else if ( "luck" == cmd[0] )
	{
		if ( 1 != cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		std::string ret = g_cli.TestLuck();
		if ( "" != ret ) printf( "%s\n", ret.c_str() );
	}
	else if ( "use" == cmd[0] )
	{
		if ( 3 != cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		short id;
		sscanf(cmd[1].c_str(), "%hd", &id);
		int count;
		sscanf(cmd[2].c_str(), "%d", &count);
		std::string ret = g_cli.UseItem(id, count);
		if ( "" != ret ) printf( "%s\n", ret.c_str() );
	}
	else if ( "buy" == cmd[0] )
	{
		if ( 3 != cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		short id;
		sscanf(cmd[1].c_str(), "%hd", &id);
		int count;
		sscanf(cmd[2].c_str(), "%d", &count);
		std::string ret = g_cli.Buy(id, count);
		if ( "" != ret ) printf( "%s\n", ret.c_str() );
	}
	else if ( "eat" == cmd[0] )
	{
		if ( 3 != cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		short id;
		sscanf(cmd[1].c_str(), "%hd", &id);
		int count;
		sscanf(cmd[2].c_str(), "%d", &count);
		std::string ret = g_cli.Devour(id, count);
		if ( "" != ret ) printf( "%s\n", ret.c_str() );
	}
	else if ( "battle" == cmd[0] )
	{
		if ( 1 != cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		int id = g_cli.CreateBattle();
		printf( "战斗(%d)开始\n", id );
	}
	else Helper();

	return NULL;
}

void Helper()
{
	printf( "cmd fromat:\n" );
	printf( "\t\tregister username pwd [true/false]\n" );
	printf( "\t\tlogin username pwd [true/false]\n" );
	printf( "\t\tbind phone\n" );
	printf( "\t\tsetPwd pwd1 pwd2\n" );
	printf( "\t\taddBuddy buddyId [talk]\n" );
	printf( "\t\tacceptBuddy buddyId [talk] [true/false]\n" );
	printf( "\t\tdelBuddy buddyId\n" );
	printf( "\t\tgetBuddys buddyId\n" );
	printf( "\t\tchat buddyId talk [recvType]\n" );
	printf( "\t\tgetUser userId\n" );
	printf( "\t\tluck itemId count\n" );
	printf( "\t\tuse itemId count\n" );
	printf( "\t\tbuy itemId count\n" );
	printf( "\t\teat itemId count\n" );
}
