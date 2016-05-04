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
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//回应参数
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
		if ( !AddData(m_pets[i].talent) )//特性
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
		if ( !AddData(m_pets[i].nature) )//性格
		{
			return false;
		}
		if ( !AddData(m_pets[i].SDHealthy) )//速度
		{
			return false;
		}
		if ( !AddData(m_pets[i].HPMuscle) )//血
		{
			return false;
		}
		if ( !AddData(m_pets[i].WGMuscle) )//攻
		{
			return false;
		}
		if ( !AddData(m_pets[i].WFMuscle) )//防
		{
			return false;
		}
		if ( !AddData(m_pets[i].TGMuscle) )//特攻
		{
			return false;
		}
		if ( !AddData(m_pets[i].TFMuscle) )//特防
		{
			return false;
		}
		if ( !AddData(m_pets[i].SDMuscle) )//速度
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
	//回应参数
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
		if ( !GetData(pet.talent) )//特性
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
		if ( !GetData(pet.nature) )//性格
		{
			return false;
		}
		if ( !GetData(pet.SDHealthy) )//速度
		{
			return false;
		}
		if ( !GetData(pet.HPMuscle) )//血
		{
			return false;
		}
		if ( !GetData(pet.WGMuscle) )//攻
		{
			return false;
		}
		if ( !GetData(pet.WFMuscle) )//防
		{
			return false;
		}
		if ( !GetData(pet.TGMuscle) )//特攻
		{
			return false;
		}
		if ( !GetData(pet.TFMuscle) )//特防
		{
			return false;
		}
		if ( !GetData(pet.SDMuscle) )//速度
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
