#pragma once

#include <string>
#include <vector>
#include "Protocl/cpp/Buffer.h"

class SyncClient
{
public:
	SyncClient();
	~SyncClient();

	//���÷�����
	void SetSvr( const std::string &ip, int port );

	//��������ģʽ�����ӻ��Ƕ����ӣ�Ĭ��ʹ�ó�����
	void SetConnectMode( bool shortConnect );

	enum ErrorCode
	{
		SUCESS			= 0,		//�ɹ�
		CONNECT_FAILED	= 1,		//����ʧ��
		NET_ERROR		= 2,		//�������
		SVR_ERROR		= 3,		//�������˴���
		EMPTY_DATA		= 4,		//������
	};

protected:
	bool Connect();
	//������ģʽ��Close����ִ�У�Ҫǿ��ִ��������force = true
	void Close(bool force = false);
	bool RecvData( msg::Buffer &msg );
	/*
		���ܣ����ÿ⺯��send��������
		������
			lpBuf	const void*	[In]	���͵�����
			nBufLen	int		[In]	���ݳ���
			nFlags	int		[In]	An indicator specifying the way in which the call is made
		����ֵ���ɹ�ʵ�ʷ��͵��ֽ�����ʧ�ܷ���-1
	*/
	int Send( const void* lpBuf, int nBufLen, int nFlags = 0 );
private:
	std::string		m_ip;
	int				m_port;
	bool			m_isShortConnect;
	void			*m_client;
	bool			m_isConnected;	
};
