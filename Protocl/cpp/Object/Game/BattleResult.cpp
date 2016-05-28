#include "BattleResult.h"

namespace msg
{

BattleResult::BattleResult()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

BattleResult::~BattleResult()
{
}

bool BattleResult::Build(bool isResult)
{
	SetId(MsgId::battleResult, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_winnerId) ) return false;		//ʤ��id
	if ( !AddData(m_loserId) ) return false;		//����id
	
	//��Ӧ����

	return true;
}

bool BattleResult::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_winnerId) ) return false;		//ʤ��id
	if ( !GetData(m_loserId) ) return false;		//����id

	//��Ӧ����

	return true;
}

}
