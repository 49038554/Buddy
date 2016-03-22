#ifndef BUDDY_DATA_H
#define BUDDY_DATA_H

#include <string>
#include <vector>
#include <map>

namespace Race
{
	enum Race
	{
		unknow = 0,
		pu = 1	,//普
		huo = 2	,//火
		shui = 3	,//水
		dian = 4	,//电
		bing = 5	,//冰
		gui = 6	,//鬼
		e = 7	,//恶
		chao = 8	,//超
		longx = 9	,//龙
		fei = 10	,//飞
		chong = 11	,//虫
		cao = 12	,//草
		di = 13	,//地
		yan = 14	,//岩
		gang = 15	,//钢
		dou = 16	,//斗
		du = 17	,//毒
	};
}

namespace data
{

	//特效
	typedef struct EFFECT
	{
		unsigned char id;	//id
		char step;			//触发时机
		char probability;	//机率
		char agent;			//作用对象
	}EFFECT;

	//物品
	typedef struct ITEM
	{
		short			id;
		std::string		name;
		int				coin;//价值
		std::vector<EFFECT>	effects;//特效,最多5个
		std::string		descript;//最大60byte
	}ITEM;

	//特性
	typedef struct TALENT
	{
		short			id;
		std::string		name;
		std::vector<EFFECT>	effects;//特效,最多5个
		std::string		descript;//最大60byte
	}TALENT;

	//技能
	typedef struct SKILL
	{
		short				id;
		std::string			name;
		char				race;//属性
		short				power;//威力0~300
		char				type;//1物理，2变化，3特殊
		char				hitRate;//命中率30~101, 101必中技
		std::vector<EFFECT>	effects;//特效,最多5个
		bool				isMapSkill;//是地图技能
		std::string			descript;//最大60byte
	}SKILL;

	//巴迪兽
	typedef struct BUDDY
	{
		short		number;//全国编号
		std::string	name;//名字
		std::string	descript;//描述,最大60byte
		char		race1;//属性1
		char		race2;//属性2
		char		talent1;//特性1
		char		talent2;//特性2
		char		talent3;//梦特性
		short		itemId;//专属物品，0没有专属物品
		short		hitPoint;//血
		short		physicalA;//攻
		short		physicalD;//防
		short		specialA;//特攻
		short		specialD;//特防
		short		speed;//速度
		std::map<unsigned short, bool> skills;//最大80个
		char		rare;	//出现率 1~99
		char		tame;	//驯化率 1~99
		std::vector<short>		upBuddys;//进化后编号最多10种进化
	}BUDDY;

	//巴迪兽分布
	typedef struct BUDDY_MAP
	{
		int					id;
		std::vector<short>	buddys;//最大100
		char				shape;//形状：0圆形，1矩形
		double				x;//latitude
		double				y;//longitude
		int					radius;
		double				right;//latitude
		double				bottom;//longitude
		int					city;//城市
		bool				spot;//景区
	}BUDDY_MAP;

	typedef struct PLAYER
	{
		unsigned int					playerId;
		int								coin;
		char							petCount;
		short							pet[6];
		time_t							lastLuckTime;
		short							luckCoin;
		bool							synced;
	}PLAYER;

	typedef struct PET
	{
		int		id;
		short	number;
		char	talent;//特性
		char	nature;//性格
		short	skill1;
		short	skill2;
		short	skill3;
		short	skill4;
		short	itemId;
		short	HP;//血
		short	WG;//攻
		short	WF;//防
		short	TG;//特攻
		short	TF;//特防
		short	SD;//速度
		char	HPHealthy;//血先天体质
		char	WGHealthy;//攻先天体质
		char	WFHealthy;//防先天体质
		char	TGHealthy;//特攻先天体质
		char	TFHealthy;//特防先天体质
		char	SDHealthy;//速度先天体质
		unsigned char	HPMuscle;//血后天修炼
		unsigned char	WGMuscle;//攻后天修炼
		unsigned char	WFMuscle;//防后天修炼
		unsigned char	TGMuscle;//特攻后天修炼
		unsigned char	TFMuscle;//特防后天修炼
		unsigned char	SDMuscle;//速度后天修炼
		std::vector<char>	race;//17个属性的恶魔果实是否吃过

		bool	synced;
		char	state;//状态
		short	curHP;//当前血量
	}PET;

	typedef struct PLAYER_ITEM
	{
		short			id;
		int				count;

		bool			synced;
	}PLAYER_ITEM;

}

data::ITEM* Item(short id, std::vector<data::ITEM> &items);
data::TALENT* Talent(short id, std::vector<data::TALENT> &talents);
data::SKILL* Skill(short id, std::vector<data::SKILL> &skills);
data::BUDDY* Buddy(short number, std::vector<data::BUDDY> &buddys);
data::BUDDY* Buddy(const std::string name, std::vector<data::BUDDY> &buddys);
data::BUDDY_MAP* BuddyMap(int id, std::vector<data::BUDDY_MAP> &BuddyMaps);

data::PLAYER_ITEM* PlayerItem(short id, std::vector<data::PLAYER_ITEM> &items);
data::PET* Pet(int id, std::vector<data::PET> &pets);

double GetNatureCal(int nature, const std::string &attr);

#endif //BUDDY_DATA_H