#include "Dekaron.h"

namespace msg
{

Dekaron::Dekaron()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

Dekaron::~Dekaron()
{
}

bool Dekaron::Build(bool isResult)
{
	SetId(MsgId::dekaron, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_battleId) ) return false;//ս��Id Game������ͳһ����
	if ( !AddData(m_playerId) ) return false;//����ս���id
	if ( !AddData(m_nick) ) return false;		// ��ս���ǳ�
	if ( !AddData(m_pet.size()) ) return false;		// ��ս���ǳ�

	int i = 0;
	for ( i = 0; i < m_pet.size(); i++ )
	{
		if ( !AddData(m_pet[i].id) ) return false;//����Id
		if ( !AddData(m_pet[i].number) ) return false;//�͵ϱ��
		if ( !AddData(m_pet[i].talent) ) return false;//����
		if ( !AddData(m_pet[i].nature) ) return false;//�Ը�
		if ( !AddData(m_pet[i].skill1) ) return false;//����1
		if ( !AddData(m_pet[i].skill2) ) return false;//����2
		if ( !AddData(m_pet[i].skill3) ) return false;//����3
		if ( !AddData(m_pet[i].skill4) ) return false;//����4
		if ( !AddData(m_pet[i].itemId) ) return false;//��Ʒid
		if ( !AddData(m_pet[i].HP) ) return false;//Ѫ
		if ( !AddData(m_pet[i].WG) ) return false;//��
		if ( !AddData(m_pet[i].WF) ) return false;//��
		if ( !AddData(m_pet[i].TG) ) return false;//�ع�
		if ( !AddData(m_pet[i].TF) ) return false;//�ط�
		if ( !AddData(m_pet[i].SD) ) return false;//�ٶ�
		if ( !AddData(m_pet[i].nick) ) return false;//�ǳ�
	}
	
	//��Ӧ����

	return true;
}

bool Dekaron::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_battleId) ) return false;//ս��Id Game������ͳһ����
	if ( !GetData(m_playerId) ) return false;//����ս���id
	if ( !GetData(m_nick) ) return false;		// ��ս���ǳ�
	int count = 0;
	if ( !GetData(count) ) return false;		// ��ս���ǳ�
	if ( 0 >= count || 6 < count ) return false;

	int i = 0;
	data::PET info;
	info.state = 0;//״̬
	info.sleepRound = 0;//����ʣ��غ�
	info.frozenRound = 0;//����ʣ��غ�
	for ( i = 0; i < count; i++ )
	{
		if ( !GetData(info.id) ) return false;//����Id
		if ( !GetData(info.number) ) return false;//�͵ϱ��
		if ( !GetData(info.talent) ) return false;//����
		if ( !GetData(info.nature) ) return false;//�Ը�
		if ( !GetData(info.skill1) ) return false;//����1
		if ( !GetData(info.skill2) ) return false;//����2
		if ( !GetData(info.skill3) ) return false;//����3
		if ( !GetData(info.skill4) ) return false;//����4
		if ( !GetData(info.itemId) ) return false;//��Ʒid
		if ( !GetData(info.HP) ) return false;//Ѫ
		if ( !GetData(info.WG) ) return false;//��
		if ( !GetData(info.WF) ) return false;//��
		if ( !GetData(info.TG) ) return false;//�ع�
		if ( !GetData(info.TF) ) return false;//�ط�
		if ( !GetData(info.SD) ) return false;//�ٶ�
		if ( !GetData(info.nick) ) return false;//�ǳ�
		info.curHP = info.HP;//��ǰѪ��
		m_pet.push_back(info);
	}

	//��Ӧ����

	return true;
}

}
