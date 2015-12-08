#pragma once

#include <string>
#include <vector>
#include "Protocl/cpp/Buffer.h"

class SyncClient
{
public:
	SyncClient();
	~SyncClient();

	//设置服务器
	void SetSvr( const std::string &ip, int port );

	//设置连接模式长连接还是短连接，默认使用长连接
	void SetConnectMode( bool shortConnect );

	enum ErrorCode
	{
		SUCESS			= 0,		//成功
		CONNECT_FAILED	= 1,		//连接失败
		NET_ERROR		= 2,		//网络错误
		SVR_ERROR		= 3,		//服务器端错误
		EMPTY_DATA		= 4,		//空数据
	};

protected:
	bool Connect();
	//长连接模式下Close不会执行，要强制执行请设置force = true
	void Close(bool force = false);
	bool RecvData( msg::Buffer &msg );
	/*
		功能：调用库函数send发送数据
		参数：
			lpBuf	const void*	[In]	发送的数据
			nBufLen	int		[In]	数据长度
			nFlags	int		[In]	An indicator specifying the way in which the call is made
		返回值：成功实际发送的字节数，失败返回-1
	*/
	int Send( const void* lpBuf, int nBufLen, int nFlags = 0 );
private:
	std::string		m_ip;
	int				m_port;
	bool			m_isShortConnect;
	void			*m_client;
	bool			m_isConnected;	
};
