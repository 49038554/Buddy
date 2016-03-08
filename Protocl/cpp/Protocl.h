#ifndef PROTOCL_H
#define PROTOCL_H

#include <vector>

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
		SNS = 7,			//�罻ģ��
		Game = 8,			//��Ϸģ��
		LBS = 9,			//Lbsģ��
	};
}

//����Id
namespace MsgId
{
	enum MsgId
	{
		//��άģ��
		getCluster = 1001,//ȡ��Ⱥ��Ϣ
		cluster = 2001,//��Ⱥ��Ϣ
		//����ģ��ͨ��
		connectAuth = 1001,//������֤
		//��֤ģ��
		userRegister = 1001,//ע��
		userLogin = 1002,//��¼
		userRelogin = 1003,//�ظ���¼
		userLogout = 1004,//�ǳ�
		resetPassword = 1005,//��������
		bindingPhone = 1006,//���ֻ���
		//SNSģ��
		addBuddy = 1001,	//��ӻ��
		delBuddy = 1002,	//ɾ�����
		getBuddys = 1003,	//ȡ���
		buddys = 2001,	//���
		chat = 1004,	//����
		setUserData = 1005,//�����û�����
		getUserData = 1006,//ȡ�û�����
		userData = 2002,//�û�����
		//֪ͨģ��
		event = 1001,//ת���¼�
		getEvent = 1002,//��ȡ�¼�
		//��������

		//��Ϸģ��
		setupVersion = 1001,//��װ�汾
		getPlayerData = 1002,//ȡ�������
		buildHouse = 1003,//��ͼ���ܣ������������췿��
		treePlant = 1004,//����
		syncItem = 1005,//ͬ����Ʒ
		syncCoin = 1006,//ͬ��������
		syncPets = 1007,//ͬ�����ֻ��Ӳ�����

// 		killBeast = 1006,//ɱ��Ұ��
// 		growUp = 1007,//����
// 		tameBeast = 1008,//ѱ��Ұ��

		putPet = 1010,//���ó���
		nearInfo = 1011,//�ܱ���Ϣ
		createShop = 1012,//����
		cityBuildInit = 1016,//��ʼ���н���
		cityBuilding = 1017,//Ϊ���н�����ש����
		delHouse = 1018,//��ͼ���ܣ���ʯ�ߣ�����

		raceMap = 2001,//���Ա�
		skillBook = 2002,//����ͼ��
		itemBook = 2003,//��Ʒͼ��
		talentBook = 2004,//����ͼ��
		buddyBook = 2005,//�͵�ͼ��
		buddyMap = 2006,//�͵Ϸֲ���ͼ
		pets = 2007,//��������
		playerItems = 2008,//�����Ʒ
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
		EC_ReLogin			= 114,// �ظ���½��

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

namespace data
{
	//��Ч
	typedef struct EFFECT
	{
		unsigned char id;	//id
		char step;			//����ʱ��
		char probability;	//����
		char agent;			//���ö���
	}EFFECT;

	typedef struct PET
	{
		int		id;
		bool	sync;
		short	number;
		char	talent;//����
		char	nature;//�Ը�
		short	skill1;
		short	skill2;
		short	skill3;
		short	skill4;
		short	itemId;
		short	HP;//Ѫ
		short	WG;//��
		short	WF;//��
		short	TG;//�ع�
		short	TF;//�ط�
		short	SD;//�ٶ�
		char	HPHealthy;//Ѫ��������
		char	WGHealthy;//����������
		char	WFHealthy;//����������
		char	TGHealthy;//�ع���������
		char	TFHealthy;//�ط���������
		char	SDHealthy;//�ٶ���������
		unsigned char	HPMuscle;//Ѫ��������
		unsigned char	WGMuscle;//����������
		unsigned char	WFMuscle;//����������
		unsigned char	TGMuscle;//�ع���������
		unsigned char	TFMuscle;//�ط���������
		unsigned char	SDMuscle;//�ٶȺ�������
		std::vector<char>	race;//17�����ԵĶ�ħ��ʵ�Ƿ�Թ�
	}PET;
}



#endif //PROTOCL_H