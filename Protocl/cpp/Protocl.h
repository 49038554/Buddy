#pragma once

#define MAX_MSG_SIZE 10240	//������󳤶�
#define BYTE_ORDER	net::smallOrder	//�ֽ���
//ģ�鶨��
namespace Moudle
{
	enum Moudle
	{
		all = 0,			//����ģ��
		Operation = 1,		//��άģ��
		TcpEntry = 2,		//Tcp���
		Auth = 3,			//��֤ģ��
		Notify = 4,			//�㲥ģ��
		DBEntry = 5,		//��������
		Client = 6,			//�ͻ���
	};
}

//����Id
namespace MsgId
{
	enum MsgId
	{
		//����ģ��ͨ��
		connectAuth = 1001,//������֤
		//��֤ģ��
		userRegister = 1001,//ע��
		userLogin = 1002,//��¼
		userRelogin = 1003,//�ظ���¼
		userLogout = 1004,//�ǳ�
		resetPassword = 1005,//��������
		bindingPhone = 1006,//���ֻ���
		//��άģ��
		getCluster = 1001,//ȡ��Ⱥ��Ϣ
		cluster = 2001,//��Ⱥ��Ϣ
		getRoomCfg = 1002,//ȡ��������
		roomCfg = 2002,//��������
		//��������
		updateCoin = 1001,//�޸������
		updateItemCount = 1002,//�޸ĵ�������
		updateRoomBacklist = 1003,// �����ض�����ĺ�������Ϣ
		updateRoomDisableList = 1004,// �����ض�����Ľ����б���Ϣ
		updateRoomManagerList = 1005,// �����ض�����Ĺ���Ա�б���Ϣ
		//֪ͨģ��
		event = 1001,//ת���¼������죬���񣬽��ԣ��뿪����==

		//���ݷ�������ݻ�֪ͨ
		itemListState = 2001,//��Ʒ�б�ʼ/����
		itemInfo = 2002,//������Ʒ��Ϣ
		roomInfo = 2003,//ֱ������Ϣ
		lookerListState = 2004,//ֱ��������б�ʼ/����
		lookerPart = 2005,//ֱ��������б�Ƭ��
	};
}

// �ն�����
namespace ClientType
{
	enum ClientType
	{
		web     = 0,  // web
		flash   = 1,  // flash
		android = 2,  // android
		iphone  = 3,  // iphone
	};
}

// �˺�����
namespace AccountType
{
	enum AccountType
	{
		imei  = 0,
		mobile = 1,
	};
}

namespace NetLine
{
	enum NetLine
	{
		unknow = 0,	//δ֪��·
		CTCC = 1,	//�й�����  chinaTelecom  
		CMCC = 2,	//�й��ƶ�ͨ�� chinamobile
		CUCC = 3,	//�й���ͨͨѶ chinaunicom     
	};
}

namespace ErrorCode
{
	enum ErrorCode
	{
		// ע��ģ��
		EC_UserExist        = 101,// �û��Ѿ����ڣ�
		 
		// ��½ģ��
		EC_TokenError       = 111,// Token����
		EC_AccountNotExist  = 112,// �˺Ų����ڣ�
		EC_PwdError         = 113,// ��½�������

		// ����ģ��
		EC_IsBacklist       = 121,// �û��Ǻ�����
		EC_RoomNotExist     = 122,// ���䲻����
		EC_RoomFrozen       = 123,// ���䱻����
		EC_CoinNotEnough    = 124,// ���������
	};
}

namespace ResultCode
{
	enum ResultCode
	{
		Success = 0,				//�ɹ�
		FormatInvalid = 1,			//�Ƿ���ʽ����Ҫ�Ͽ�Client����
		MsgInvalid = 2,				//�Ƿ����ģ���Ҫ�Ͽ�Client����
		Refuse = 3,					//�ܾ�
		DBError = 4,				//���ݿ���󣬲����ݿ�����Ƿ�����
		SvrError = 5,				//������󣬲�����ԭ��
	};
};

//��Ʒ������
namespace ItemBigType
{
	enum ItemBigType
	{
		all = 0,//ȫ��
		gift = 1,//����
		commodity = 2,//��Ʒ
		luckGift = 3,//��������
	};
}