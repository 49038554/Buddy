#ifndef CLIENT_H
#define CLIENT_H

#include "NetWorker.h"

class Client : public NetWorker
{
public:
	Client(void);
	virtual ~Client(void);
	enum SvrType
	{
		TcpSvr = 0,
	};

public:
	//////////////////////////////////////////////////////////////////////////
	//Auth
	bool Register(bool isMobile, const std::string &account, const std::string &pwd);
	void OnRegister(msg::Buffer &buffer);
	bool Login(bool isMobile, const std::string &account, const std::string &pwd);
	void OnLogin(msg::Buffer &buffer);
	bool BindPhone(const std::string &moblie);
	void OnBindPhone(msg::Buffer &buffer);
	void OnRelogin(msg::Buffer &buffer);
	bool SetPassword(const std::string pwd);
	void OnResetPassword(msg::Buffer &buffer);

	//////////////////////////////////////////////////////////////////////////
	//SNS
	bool AddBuddy(unsigned int buddyId, const std::string &talk);
	bool AcceptBuddy(unsigned int buddyId, const std::string &talk, bool accept);
	void OnAddBuddy(msg::Buffer &buffer);
	bool GetEvent();

protected:
	virtual void Main();
	virtual void OnConnect(int svrType, net::Socket &svr);
	virtual void OnClose(int svrType);
	virtual void OnMsg(int svrType, net::Socket &svr, msg::Buffer &buffer);

	void OnAuth(msg::Buffer &buffer);
	void OnSNS(msg::Buffer &buffer);

private:
	typedef struct USER_DATA
	{
		bool			logined;
		unsigned int	id;
		bool			isMobileLogin;
		std::string		account;
		std::string		pwd;
		std::string		newPwd;
	}USER_DATA;

	USER_DATA	m_user;
};

#endif //CLIENT_H