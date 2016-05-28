#include "RoundReady.h"

namespace msg
{

RoundReady::RoundReady()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

RoundReady::~RoundReady()
{
}

bool RoundReady::Build(bool isResult)
{
	SetId(MsgId::roundReady, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_battleId) ) return false;		//ս��id
	if ( !AddData((char)m_action) ) return false;		//����
	if ( !AddData(m_gameObjId) ) return false;		//����id
	//�����
	if ( !AddData(m_rp.miss) ) return false;//���������
	if ( !AddData(m_rp.sePro) ) return false;//������Ч�����
	if ( !AddData(m_rp.iePro) ) return false;//��Ʒ��Ч�����
	if ( !AddData(m_rp.tePro) ) return false;//������Ч�����
	if ( !AddData(m_rp.luanWu) ) return false;//����غ���
	if ( !AddData(m_rp.sleepRound) ) return false;//˯�߻غ������
	if ( !AddData(m_rp.frozenRound) ) return false;//�����غ������
	if ( !AddData(m_rp.dian) ) return false;//��������
	char val = m_rp.luan?1:0;
	if ( !AddData(val) ) return false;//���������
	if ( !AddData(m_rp.hurt) ) return false;//�˺������217~255
	if ( !AddData(m_rp.speed) ) return false;//�ٶ������
	if ( !AddData(m_rp.fouceChange) ) return false;//ǿ�ƻ��ϵ�?ֻPet�������1~5
	
	//��Ӧ����

	return true;
}

bool RoundReady::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_battleId) ) return false;		//ս��Id
	char val;
	if ( !GetData(val) ) return false;		//����
	m_action = val;
	if ( !GetData(m_gameObjId) ) return false;		//����id
	//�����
	if ( !GetData(m_rp.miss) ) return false;//���������
	if ( !GetData(m_rp.sePro) ) return false;//������Ч�����
	if ( !GetData(m_rp.iePro) ) return false;//��Ʒ��Ч�����
	if ( !GetData(m_rp.tePro) ) return false;//������Ч�����
	if ( !GetData(m_rp.luanWu) ) return false;//����غ���
	if ( !GetData(m_rp.sleepRound) ) return false;//˯�߻غ������
	if ( !GetData(m_rp.frozenRound) ) return false;//�����غ������
	if ( !GetData(m_rp.dian) ) return false;//��������
	if ( !GetData(val) ) return false;//���������
	m_rp.luan = 1 == val?true:false;
	if ( !GetData(m_rp.hurt) ) return false;//�˺������217~255
	if ( !GetData(m_rp.speed) ) return false;//�ٶ������
	if ( !GetData(m_rp.fouceChange) ) return false;//ǿ�ƻ��ϵ�?ֻPet�������1~5

	//��Ӧ����

	return true;
}

}
