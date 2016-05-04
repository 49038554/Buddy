#include "SyncPets.h"

namespace msg
{

SyncPets::SyncPets()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

SyncPets::~SyncPets()
{
}

bool SyncPets::Build(bool isResult)
{
	if ( m_pets.size() > 100 )
	{
		return false;
	}
	SetId(MsgId::syncPets, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//��Ӧ����
	if ( !AddData((unsigned char)m_pets.size()) )
	{
		return false;
	}
	int i = 0;
	for ( i = 0; i < m_pets.size(); i++ )
	{
		if ( !AddData(m_pets[i].id) )
		{
			return false;
		}
		if ( !AddData(m_pets[i].number) )
		{
			return false;
		}
		if ( !AddData(m_pets[i].talent) )//����
		{
			return false;
		}
		if ( !AddData(m_pets[i].skill1) )
		{
			return false;
		}
		if ( !AddData(m_pets[i].skill2) )
		{
			return false;
		}
		if ( !AddData(m_pets[i].skill3) )
		{
			return false;
		}
		if ( !AddData(m_pets[i].skill4) )
		{
			return false;
		}
		if ( !AddData(m_pets[i].nature) )//�Ը�
		{
			return false;
		}
		if ( !AddData(m_pets[i].SDHealthy) )//�ٶ�
		{
			return false;
		}
		if ( !AddData(m_pets[i].HPMuscle) )//Ѫ
		{
			return false;
		}
		if ( !AddData(m_pets[i].WGMuscle) )//��
		{
			return false;
		}
		if ( !AddData(m_pets[i].WFMuscle) )//��
		{
			return false;
		}
		if ( !AddData(m_pets[i].TGMuscle) )//�ع�
		{
			return false;
		}
		if ( !AddData(m_pets[i].TFMuscle) )//�ط�
		{
			return false;
		}
		if ( !AddData(m_pets[i].SDMuscle) )//�ٶ�
		{
			return false;
		}
		if ( m_pets[i].race.size() > 17 )
		{
			return false;
		}
		if ( !AddData((unsigned char)m_pets[i].race.size()) )
		{
			return false;
		}
		int j = 0;
		for ( j = 0; j < m_pets[i].race.size(); j++ )
		{
			if ( !AddData(m_pets[i].race[j]) )
			{
				return false;
			}
		}
		if ( !AddData(m_pets[i].itemId) )
		{
			return false;
		}

		if ( !AddData(m_pets[i].synced) )
		{
			return false;
		}
	}

	return true;
}

bool SyncPets::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//��Ӧ����
	unsigned char count;
	if ( !GetData(count) )
	{
		return false;
	}
	if ( count > 100 ) 
	{
		return false;
	}
	int i = 0;
	data::PET pet;
	for ( i = 0; i < count; i++ )
	{
		if ( !GetData(pet.id) )
		{
			return false;
		}
		if ( !GetData(pet.number) )
		{
			return false;
		}
		if ( !GetData(pet.talent) )//����
		{
			return false;
		}
		if ( !GetData(pet.skill1) )
		{
			return false;
		}
		if ( !GetData(pet.skill2) )
		{
			return false;
		}
		if ( !GetData(pet.skill3) )
		{
			return false;
		}
		if ( !GetData(pet.skill4) )
		{
			return false;
		}
		if ( !GetData(pet.nature) )//�Ը�
		{
			return false;
		}
		if ( !GetData(pet.SDHealthy) )//�ٶ�
		{
			return false;
		}
		if ( !GetData(pet.HPMuscle) )//Ѫ
		{
			return false;
		}
		if ( !GetData(pet.WGMuscle) )//��
		{
			return false;
		}
		if ( !GetData(pet.WFMuscle) )//��
		{
			return false;
		}
		if ( !GetData(pet.TGMuscle) )//�ع�
		{
			return false;
		}
		if ( !GetData(pet.TFMuscle) )//�ط�
		{
			return false;
		}
		if ( !GetData(pet.SDMuscle) )//�ٶ�
		{
			return false;
		}
		unsigned char raceCount;
		if ( !GetData(raceCount) )
		{
			return false;
		}
		if ( raceCount > 17 )
		{
			return false;
		}
		int j = 0;
		char race;
		pet.race.clear();
		for ( j = 0; j < raceCount; j++ )
		{
			if ( !GetData(race) )
			{
				return false;
			}
			pet.race.push_back(race);
		}
		if ( !GetData(pet.itemId) )
		{
			return false;
		}

		if ( !GetData(pet.synced) )
		{
			return false;
		}
		m_pets.push_back(pet);
	}


	return true;
}

}
