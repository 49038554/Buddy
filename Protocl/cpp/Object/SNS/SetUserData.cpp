#include "SetUserData.h"


namespace msg
{

SetUserData::SetUserData()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
	m_userId = 0;
	m_pwdOpt = false;			
	m_pwd = "";					
	m_nickNameOpt = false;		
	m_nickName = "";			
	m_faceOpt = false;			
	m_face = "";				
	m_sexOpt = false;			
	m_sex = -1;					
	m_coinOpt = false;			
	m_coin = -1;					
	m_bindImeiOpt = false;		
	m_bindImei = "";			
	m_bindMobileOpt = false;	
	m_bindMobile = "";			
}

SetUserData::~SetUserData()
{
}

bool SetUserData::Build(bool isResult)
{
	SetId(MsgId::setUserData, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_userId) ) return false;//�û�Id	SNS������д
	if ( "" != m_pwd ) m_pwdOpt = true;
	if ( !AddData(m_pwdOpt) ) return false;//���뱻����
	if ( m_pwdOpt )	if ( !AddData(m_pwd) ) return false;//����

	if ( "" != m_nickName ) m_nickNameOpt = true;
	if ( !AddData(m_nickNameOpt) ) return false;//�ǳƱ�����
	if ( m_nickNameOpt )	if ( !AddData(m_nickName) ) return false;//�ǳ�

	if ( "" != m_face ) m_faceOpt = true;
	if ( !AddData(m_faceOpt) ) return false;//ͷ��URL������
	if ( m_faceOpt )	if ( !AddData(m_face) ) return false;//ͷ��URL

	if ( -1 != m_sex ) m_sexOpt = true;
	if ( !AddData(m_sexOpt) ) return false;//�Ա�����
	if ( m_sexOpt )	if ( !AddData((char)(0 == m_sex?0:1)) ) return false;//�Ա�

	if ( -1 != m_coin ) m_coinOpt = true;
	if ( !AddData(m_coinOpt) ) return false;//����ң���RMB��ֵ������������
	if ( m_coinOpt )	if ( !AddData(m_coin) ) return false;//����ң���RMB��ֵ������

	if ( "" != m_bindImei ) m_bindImeiOpt = true;
	if ( !AddData(m_bindImeiOpt) ) return false;//�˺ţ�Imei������
	if ( m_bindImeiOpt )	if ( !AddData(m_bindImei) ) return false;//�˺ţ�Imei

	if ( "" != m_bindMobile ) m_bindMobileOpt = true;
	if ( !AddData(m_bindMobileOpt) ) return false;//�˺ţ��ֻ����뱻����
	if ( m_bindMobileOpt )	if ( !AddData(m_bindMobile) ) return false;//�˺ţ��ֻ�����

	return true;
}

bool SetUserData::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_userId) ) return false;//�û�Id	SNS������д

	if ( !GetData(m_pwdOpt) ) return false;//���뱻����
	if ( m_pwdOpt )	if ( !GetData(m_pwd) ) return false;//����

	if ( !GetData(m_nickNameOpt) ) return false;//�ǳƱ�����
	if ( m_nickNameOpt )	if ( !GetData(m_nickName) ) return false;//�ǳ�

	if ( !GetData(m_faceOpt) ) return false;//ͷ��URL������
	if ( m_faceOpt )	if ( !GetData(m_face) ) return false;//ͷ��URL

	if ( !GetData(m_sexOpt) ) return false;//�Ա�����
	if ( m_sexOpt )	if ( !GetData(m_sex) ) return false;//�Ա�

	if ( !GetData(m_coinOpt) ) return false;//����ң���RMB��ֵ������������
	if ( m_coinOpt )	if ( !GetData(m_coin) ) return false;//����ң���RMB��ֵ������

	if ( !GetData(m_bindImeiOpt) ) return false;//�˺ţ�Imei������
	if ( m_bindImeiOpt )	if ( !GetData(m_bindImei) ) return false;//�˺ţ�Imei

	if ( !GetData(m_bindMobileOpt) ) return false;//�˺ţ��ֻ����뱻����
	if ( m_bindMobileOpt )	if ( !GetData(m_bindMobile) ) return false;//�˺ţ��ֻ�����

	return true;
}

}
