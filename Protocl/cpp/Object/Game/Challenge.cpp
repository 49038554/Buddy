#include "Challenge.h"

namespace msg
{

Challenge::Challenge()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

Challenge::~Challenge()
{
}

bool Challenge::Build(bool isResult)
{
	SetId(MsgId::challenge, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_battleId) ) return false;	// ս��Id ��Dekaron����
	if ( !AddData(m_playerId) ) return false;	// ��ս���id
	char val = m_accepted?1:0;
	if ( !AddData(val) ) return false;// ������ս
	if ( !m_accepted ) return true;
	if ( !AddData(m_dNick) ) return false;		// ��ս���ǳ�

	//��ս�߳���
	if ( !AddData(m_dPet.size()) ) return false;
	int i = 0;
	for ( i = 0; i < m_dPet.size(); i++ )
	{
		if ( !AddData(m_dPet[i].id) ) return false;//����Id
		if ( !AddData(m_dPet[i].number) ) return false;//�͵ϱ��
		if ( !AddData(m_dPet[i].talent) ) return false;//����
		if ( !AddData(m_dPet[i].nature) ) return false;//�Ը�
		if ( !AddData(m_dPet[i].skill1) ) return false;//����1
		if ( !AddData(m_dPet[i].skill2) ) return false;//����2
		if ( !AddData(m_dPet[i].skill3) ) return false;//����3
		if ( !AddData(m_dPet[i].skill4) ) return false;//����4
		if ( !AddData(m_dPet[i].itemId) ) return false;//��Ʒid
		if ( !AddData(m_dPet[i].HP) ) return false;//Ѫ
		if ( !AddData(m_dPet[i].WG) ) return false;//��
		if ( !AddData(m_dPet[i].WF) ) return false;//��
		if ( !AddData(m_dPet[i].TG) ) return false;//�ع�
		if ( !AddData(m_dPet[i].TF) ) return false;//�ط�
		if ( !AddData(m_dPet[i].SD) ) return false;//�ٶ�
		if ( !AddData(m_dPet[i].nick) ) return false;//�ǳ�
	}
	if ( !AddData(m_cNick) ) return false;		// Ӧս���ǳ�
	// Ӧս�߳���
	if ( !AddData(m_cPet.size()) ) return false;
	for ( i = 0; i < m_cPet.size(); i++ )
	{
		if ( !AddData(m_cPet[i].id) ) return false;//����Id
		if ( !AddData(m_cPet[i].number) ) return false;//�͵ϱ��
		if ( !AddData(m_cPet[i].talent) ) return false;//����
		if ( !AddData(m_cPet[i].nature) ) return false;//�Ը�
		if ( !AddData(m_cPet[i].skill1) ) return false;//����1
		if ( !AddData(m_cPet[i].skill2) ) return false;//����2
		if ( !AddData(m_cPet[i].skill3) ) return false;//����3
		if ( !AddData(m_cPet[i].skill4) ) return false;//����4
		if ( !AddData(m_cPet[i].itemId) ) return false;//��Ʒid
		if ( !AddData(m_cPet[i].HP) ) return false;//Ѫ
		if ( !AddData(m_cPet[i].WG) ) return false;//��
		if ( !AddData(m_cPet[i].WF) ) return false;//��
		if ( !AddData(m_cPet[i].TG) ) return false;//�ع�
		if ( !AddData(m_cPet[i].TF) ) return false;//�ط�
		if ( !AddData(m_cPet[i].SD) ) return false;//�ٶ�
		if ( !AddData(m_cPet[i].nick) ) return false;//�ǳ�
	}
	
	//��Ӧ����

	return true;
}

bool Challenge::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_battleId) ) return false;// ս��Id ��Dekaron����
	if ( !GetData(m_playerId) ) return false;// ��ս���id
	char val;
	if ( !GetData(val) ) return false;// ������ս
	m_accepted = 1 == val?true:false;
	if ( !m_accepted ) return true;
	if ( !GetData(m_dNick) ) return false;		// ��ս���ǳ�

	//��ս�߳���
	data::PET info;
	info.state = 0;//״̬
	info.sleepRound = 0;//����ʣ��غ�
	info.frozenRound = 0;//����ʣ��غ�

	int count = 0;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || 6 < count ) return false;

	int i = 0;
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
		m_dPet.push_back(info);
	}

	if ( !GetData(m_cNick) ) return false;		// Ӧս���ǳ�
	// Ӧս�߳���
	if ( !GetData(count) ) return false;
	if ( 0 >= count || 6 < count ) return false;

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
		m_cPet.push_back(info);
	}

	//��Ӧ����

	return true;
}

}
