#include "User.h"
#include <cstdio>

namespace Cache {

User::User(void)
	: id(-1)
	, pwd("NULL")
	, randKey("NULL")
	, nickName("NULL")      
	, signature("NULL")     
	, sex(true)     
	, coin(0)
	, headIco("NULL")
	, level(0)      
	, realName("NULL")
	, bindImei("NULL")
	, bindMobile("NULL")
{
	Bind(m_buffer, 1024);          // ��һ�����������ڴ�֮����ת��
	SetByteOrder(net::smallOrder); // �����ֽ���
}

User::~User(void)
{
	// body
}

bool User::Build(void)
{
	Clear();

	if (! AddData(id)) return false;
	if (! AddData(pwd)) return false;
	if (! AddData(randKey)) return false;
	if (! AddData(nickName)) return false;
	if (! AddData(signature)) return false;
	if (! AddData(sex)) return false;
	if (! AddData(coin)) return false;
	if (! AddData(headIco)) return false;
	if (! AddData(level)) return false;
	if (! AddData(realName)) return false;

	if (! AddData(bindImei)) return false;
	if (! AddData(bindMobile)) return false;

	return true;
}

bool User::Parse(void)
{
	if ( -1 == Size() ) return false;

	if (! GetData(id)) return false;
	if (! GetData(pwd)) return false;
	if (! GetData(randKey)) return false;
	if (! GetData(nickName)) return false;
	if (! GetData(signature)) return false;
	if (! GetData(sex)) return false;
	if (! GetData(coin)) return false;
	if (! GetData(headIco)) return false;
	if (! GetData(level)) return false;
	if (! GetData(realName)) return false;

	if (! GetData(bindImei)) return false;
	if (! GetData(bindMobile)) return false;

	return true;
}

std::string User::ToString()
{
	char info[2048];
	sprintf ( info, "User:\n" );
	// ���ݿ�����
	sprintf ( info, "%s\tid = %d\n", info, id );             // �û�ID
	sprintf ( info, "%s\tpwd = %s\n", info, pwd.c_str() );            // ����
	sprintf ( info, "%s\trandKey = %s\n", info, randKey.c_str() );        // ���key pwd == md5(md5(�û�����pwd)+randKey)��¼�ɹ�
	sprintf ( info, "%s\tnickname = %s\n", info, nickName.c_str() );       // �ǳ�
	sprintf ( info, "%s\tsignature = %s\n", info, signature.c_str() );      // ǩ��
	sprintf ( info, "%s\tsex = %s\n", info, sex?"��":"Ů" );            // �Ա�true = �� false = Ů
	sprintf ( info, "%s\tcoin = %d\n", info, coin );           // ��Ԫ����RMB��ֵ������
	sprintf ( info, "%s\tpicURL = %s\n", info, headIco.c_str() );         // ͷ��URL
	sprintf ( info, "%s\tlevel = %d\n", info, level );          // �ȼ�
	sprintf ( info, "%s\trealName = %s\n", info, realName.c_str() );       // ��ʵ����

	// �ڴ�����

	return std::string(info);
}

}
