#include "BuddyBook.h"


namespace msg
{

BuddyBook::BuddyBook()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

BuddyBook::~BuddyBook()
{
}

bool BuddyBook::Build()
{
	unsigned char count = m_buddys.size();
	if ( 0 >= count || count > 20 ) 
	{
		return false;
	}
	SetId(MsgId::buddyBook, true);
	if ( !FillTransmitParam() ) //Tcp������д����
	{
		return false;
	}

	//��Ӧ����
	if ( !AddData(count) ) 
	{
		return false;
	}
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData(m_buddys[i].number) ) //ȫ�����
		{
			return false;
		}
		if ( !AddData(m_buddys[i].name) ) //����
		{
			return false;
		}
		if ( !AddData(m_buddys[i].descript) ) //����
		{
			return false;
		}
		if ( !AddData(m_buddys[i].race1) ) //����1
		{
			return false;
		}
		if ( !AddData(m_buddys[i].race2) ) //����2
		{
			return false;
		}
		if ( !AddData(m_buddys[i].talent1) ) //����1
		{
			return false;
		}
		if ( !AddData(m_buddys[i].talent2) ) //����2
		{
			return false;
		}
		if ( !AddData(m_buddys[i].talent3) ) //������
		{
			return false;
		}
		if ( !AddData(m_buddys[i].itemId) ) //ר����Ʒ��0û��ר����Ʒ
		{
			return false;
		}
		if ( !AddData(m_buddys[i].hitPoint) ) //Ѫ
		{
			return false;
		}
		if ( !AddData(m_buddys[i].physicalA) ) //��
		{
			return false;
		}
		if ( !AddData(m_buddys[i].physicalD) ) //��
		{
			return false;
		}
		if ( !AddData(m_buddys[i].specialA) ) //�ع�
		{
			return false;
		}
		if ( !AddData(m_buddys[i].specialD) ) //�ط�
		{
			return false;
		}
		if ( !AddData(m_buddys[i].speed) ) //�ٶ�
		{
			return false;
		}
		if ( !AddData(m_buddys[i].rare) ) 	//������ 1~99
		{
			return false;
		}
		if ( !AddData(m_buddys[i].tame) ) 	//ѱ���� 1~99
		{
			return false;
		}
		//������Ϣ
		unsigned char count = m_buddys[i].upBuddys.size();
		if ( 0 > count || count > 10 ) 
		{
			return false;
		}
		if ( !AddData(count) ) 
		{
			return false;
		}
		int j = 0;
		for ( j = 0; j < count; j++ )
		{
			if ( !AddData(m_buddys[i].upBuddys[j]) ) 
			{
				return false;
			}
		}

		//������Ϣ
		unsigned char skillCount = m_buddys[i].skills.size();
		if ( 0 >= skillCount || skillCount > 80 ) 
		{
			return false;
		}
		if ( !AddData(skillCount) ) 
		{
			return false;
		}
		std::map<unsigned short, bool>::iterator it = m_buddys[i].skills.begin();
		for ( ; it != m_buddys[i].skills.end(); it++ ) 
		{
			if ( !AddData(it->first) ) 
			{
				return false;
			}
			if ( !AddData((bool)it->second) ) 
			{
				return false;
			}
		}
	}


	return true;
}

bool BuddyBook::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//��Ӧ����
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || count > 20 ) return false;

	int i = 0;
	data::BUDDY info;
	m_buddys.clear();
	for ( ; i < count; i++ )
	{
		if ( !GetData(info.number) ) return false;//ȫ�����
		if ( !GetData(info.name) ) return false;//����
		if ( !GetData(info.descript) ) return false;//����
		if ( !GetData(info.race1) ) return false;//����1
		if ( !GetData(info.race2) ) return false;//����2
		if ( !GetData(info.talent1) ) return false;//����1
		if ( !GetData(info.talent2) ) return false;//����2
		if ( !GetData(info.talent3) ) return false;//������
		if ( !GetData(info.itemId) ) return false;//ר����Ʒ��0û��ר����Ʒ
		if ( !GetData(info.hitPoint) ) return false;//Ѫ
		if ( !GetData(info.physicalA) ) return false;//��
		if ( !GetData(info.physicalD) ) return false;//��
		if ( !GetData(info.specialA) ) return false;//�ع�
		if ( !GetData(info.specialD) ) return false;//�ط�
		if ( !GetData(info.speed) ) return false;//�ٶ�
		if ( !GetData(info.rare) ) return false;	//������ 1~99
		if ( !GetData(info.tame) ) return false;	//ѱ���� 1~99
		//������Ϣ
		unsigned char count;
		if ( !GetData(count) ) return false;
		if ( 0 > count || count > 10 ) return false;
		int j = 0;
		short upMember;
		info.upBuddys.clear();
		for ( j = 0; j < count; j++ )
		{
			if ( !GetData(upMember) ) return false;
			info.upBuddys.push_back(upMember);
		}

		//������Ϣ
		unsigned char skillCount = 0;
		if ( !GetData(skillCount) ) return false;
		if ( 0 >= skillCount || skillCount > 80 ) return false;
		short skillId;
		bool isInitSkill;
		info.skills.clear();
		for ( j = 0; j < skillCount; j++ ) 
		{
			if ( !GetData(skillId) ) return false;
			if ( !GetData(isInitSkill) ) return false;
			info.skills[skillId] = isInitSkill;
		}
		m_buddys.push_back(info);
	}

	return true;
}

}
