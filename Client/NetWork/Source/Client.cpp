#include "Client.h"

#include "Protocl/cpp/Object/Notify/Event.h"
#include "Protocl/cpp/Object/Notify/GetEvent.h"

#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "Protocl/cpp/Object/Auth/UserLogin.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/UserRelogin.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "Protocl/cpp/Object/Auth/UserLogout.h"

#include "Protocl/cpp/Object/SNS/AddBuddy.h"

Client::Client(void)
{
}

Client::~Client(void)
{
}

void Client::OnConnect(int svrType, net::Socket &svr)
{

}

void Client::OnClose(int svrType)
{
}

void Client::OnMsg(int svrType, net::Socket &svr, msg::Buffer &buffer)
{

}
