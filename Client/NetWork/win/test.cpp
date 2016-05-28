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

	mdk::Console cmd("ご主人は入ります", 256);
	cmd.Start((mdk::FuntionPointer)OnCommand);
	cmd.WaitStop();

	return 0;
}

void Helper();
void PrintPets(Battle::WARRIOR *pPlayer, Game *pGame);
void PrintWarrior(Battle::WARRIOR *pPlayer, Game *pGame);
void PrintUI(int battleId);
bool ShowLog(int battleId);

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
		int battleId = g_cli.CreateBattle();
		if ( 0 == battleId )
		{
			printf( "战斗初始化错误\n" );
			return NULL;
		}
		printf( "战斗(%d)开始： %s VS %s\n", battleId,
			g_cli.Fighter(battleId, true)->name.c_str(),
			g_cli.Fighter(battleId, false)->name.c_str() );
		ShowLog(battleId);
		PrintUI(battleId);
	}
	else if ( "ack" == cmd[0] )
	{
		if ( 3 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		int uId = 1;
		if ( 4 == cmd.size() ) uId = 2;
		int battleId = atoi(cmd[1].c_str());
		int oId = atoi(cmd[2].c_str());
		const char *ret = NULL;
		data::RAND_PARAM rp;
		if ( 1 == uId ) 
		{
			g_cli.CreateRP(battleId, true, rp);
			ret = g_cli.Ready(battleId, Battle::attack, oId, rp);
		}
		else 
		{
			g_cli.CreateRP(battleId, false, rp);
			ret = g_cli.SheReady(battleId, Battle::attack, oId, rp);
		}
		if ( NULL != ret ) printf( "%s\n", ret );
		if ( ShowLog(battleId) )
		{
			PrintWarrior(g_cli.Fighter(battleId, true), g_cli.GetGame());
			PrintWarrior(g_cli.Fighter(battleId, false), g_cli.GetGame());
		}
	}
	else if ( "change" == cmd[0] )
	{
		if ( 3 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		int uId = 1;
		if ( 4 == cmd.size() ) uId = 2;
		int battleId = atoi(cmd[1].c_str());
		int oId = atoi(cmd[2].c_str());
		const char *ret = NULL;
		data::RAND_PARAM rp;
		if ( 1 == uId ) 
		{
			g_cli.CreateRP(battleId, true, rp);
			ret = g_cli.Ready(battleId, Battle::change, oId, rp);
		}
		else 
		{
			g_cli.CreateRP(battleId, false, rp);
			ret = g_cli.SheReady(battleId, Battle::change, oId, rp);
		}

		if ( NULL != ret ) printf( "%s\n", ret );
		if ( ShowLog(battleId) )
		{
			PrintWarrior(g_cli.Fighter(battleId, true), g_cli.GetGame());
			PrintWarrior(g_cli.Fighter(battleId, false), g_cli.GetGame());
		}
	}
	else if ( "send" == cmd[0] )
	{
		if ( 3 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		int uId = 1;
		if ( 4 == cmd.size() ) uId = 2;
		int battleId = atoi(cmd[1].c_str());
		int oId = atoi(cmd[2].c_str());
		const char *ret = NULL;
		if ( 1 == uId ) ret = g_cli.ChangePet(battleId, oId);
		else ret = g_cli.SheChangePet(battleId, oId);
		if ( NULL != ret ) printf( "%s\n", ret );
		ShowLog(battleId);
		PrintWarrior(g_cli.Fighter(battleId, true), g_cli.GetGame());
		PrintWarrior(g_cli.Fighter(battleId, false), g_cli.GetGame());
	}
	else if ( "buse" == cmd[0] )
	{
		if ( 3 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		int uId = 1;
		if ( 4 == cmd.size() ) uId = 2;
		int battleId = atoi(cmd[1].c_str());
		int itemId = atoi(cmd[2].c_str());
		const char *ret = NULL;
		data::RAND_PARAM rp;
		if ( 1 == uId ) 
		{
			g_cli.CreateRP(battleId, true, rp);
			ret = g_cli.Ready(battleId, Battle::useItem, itemId, rp);
		}
		else 
		{
			g_cli.CreateRP(battleId, false, rp);
			ret = g_cli.SheReady(battleId, Battle::useItem, itemId, rp);
		}
		if ( NULL != ret ) printf( "%s\n", ret );
		ShowLog(battleId);
		PrintWarrior(g_cli.Fighter(battleId, true), g_cli.GetGame());
		PrintWarrior(g_cli.Fighter(battleId, false), g_cli.GetGame());
	}
	else if ( "pets" == cmd[0] )
	{
		if ( 2 > cmd.size() ) 
		{
			Helper();
			return NULL;
		}
		bool me = true;
		if ( 3 == cmd.size() ) me = false;
		int battleId = atoi(cmd[1].c_str());
		PrintPets(g_cli.Fighter(battleId, me), g_cli.GetGame());
	}
	else if ( "load" == cmd[0] )
	{
		int battleId = g_cli.LoadBattle();
		ShowLog(battleId);
		PrintWarrior(g_cli.Fighter(battleId, true), g_cli.GetGame());
		PrintWarrior(g_cli.Fighter(battleId, false), g_cli.GetGame());
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
	printf( "\t\tbattle\n" );
	printf( "\t\tack battleId skillId [she]\n" );
	printf( "\t\tchange battleId petId [she]\n" );
	printf( "\t\tsend battleId petId [she]\n" );
	printf( "\t\tbuse battleId itemId count\n" );
	printf( "\t\tpets battleId [she]\n" );
	printf( "\t\tload\n" );
}

void PrintWarrior(Battle::WARRIOR *pPlayer, Game *pGame)
{
	data::PET *pPet = pPlayer->pCurPet;
	if ( NULL == pPet ) pPet = &pPlayer->pets[0];
	printf( "%s\t出场宠物\n\t%s\tid:%d\t(%s)(%s) %s HP:%d%%(%d) 速度:%d\n",
		pPlayer->name.c_str(),
		pPet->nick.c_str(), pPet->id, pPlayer->pBuddy->name.c_str(),
		pPlayer->pItem->name.c_str(),
		0 >= pPet->curHP?"休克":StateDes(pPet->state), pPet->curHP*100/pPet->HP, pPet->curHP, pPet->SD );
	printf( "\t" );
	data::SKILL *pSkill = Skill(pPet->skill1, pGame->SkillBook());
	if ( NULL == pSkill ) printf("非法技能 (%d), ", pPet->skill1);
	else printf( "%s (%d), ", pSkill->name.c_str(), pPet->skill1 );
	pSkill = Skill(pPet->skill2, pGame->SkillBook());
	if ( NULL == pSkill ) printf("非法技能 (%d), ", pPet->skill2);
	else printf( "%s (%d), ", pSkill->name.c_str(), pPet->skill2 );
	pSkill = Skill(pPet->skill3, pGame->SkillBook());
	if ( NULL == pSkill ) printf("非法技能 (%d), ", pPet->skill3);
	else printf( "%s (%d), ", pSkill->name.c_str(), pPet->skill3 );
	pSkill = Skill(pPet->skill4, pGame->SkillBook());
	if ( NULL == pSkill ) printf("非法技能 (%d) ", pPet->skill4);
	else printf( "%s (%d) ", pSkill->name.c_str(), pPet->skill4 );
	printf( "\n\n" );
}

void PrintPets(Battle::WARRIOR *pPlayer, Game *pGame)
{
	printf( "%s\t宠物列表\n", pPlayer->name.c_str() );
	int i = 0;
	data::BUDDY *pBuddy;
	for ( i = 0; i < pPlayer->pets.size(); i++ )
	{
		pBuddy = Buddy(pPlayer->pets[i].number, pGame->BuddyBook());
		if ( NULL == pBuddy )
		{
			printf( "\t不存在的宠物%s\tid:%d\t(%d)\n", pPlayer->pets[i].nick.c_str(), pPlayer->pets[i].number );
			continue;
		}
		printf( "\t%s\tid:%d\t(%s)\n", pPlayer->pets[i].nick.c_str(),
			pPlayer->pets[i].id,
			pBuddy->name.c_str() );
	}
	printf( "\n\n" );
}

void PrintUI(int battleId)
{
	PrintPets(g_cli.Fighter(battleId, true), g_cli.GetGame());
	PrintPets(g_cli.Fighter(battleId, false), g_cli.GetGame());

	if ( 0 < g_cli.Fighter(battleId, true)->pCurPet->curHP )
	{
		PrintWarrior(g_cli.Fighter(battleId, true), g_cli.GetGame());
	}
	if ( 0 < g_cli.Fighter(battleId, false)->pCurPet->curHP )
	{
		PrintWarrior(g_cli.Fighter(battleId, false), g_cli.GetGame());
	}
}

bool ShowLog(int battleId)
{
	std::vector<std::string> log;
	if ( !g_cli.Log(battleId, log) ) return false;

	int i = 0;
	for ( i = 0; i < log.size(); i++ )
	{
		printf( "%s\n", log[i].c_str() );
	}

	return true;
}
