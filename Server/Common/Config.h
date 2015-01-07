

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Base.h"


//Config
struct _CONFIG_INFO
{
	int32_t				m_SystemModel;
	ID_t			m_ServerID ;
	int32_t				m_ZoneSize ;
	float			m_DropParam;				//全局掉落修正参数
	float			m_ExpParam;					//全局经验修正参数
	int32_t				m_EquipmentDamagePoint;		//物品耐久度的微调值
	int32_t				m_RecoverTime ;
	int32_t				m_MaxMonster ;
	int32_t				m_MaxPet ;
	int32_t				m_PetBodyTime ;
	int32_t				m_PetHappinessInterval;		// 宠物快乐度下降1的时间间隔(毫少)
	int32_t				m_PetLifeInterval;			// 宠物寿命下降1的时间间隔(毫少)
	int32_t				m_PetCallUpHappiness;		// 宠物快乐度大于才可以召出
	uint32_t			m_PetPlacardTime;			// 发布一个宠物公告的持续时间
	int32_t				m_PetPlacardNeedLevel;		// 发布宠物公告的宠物所需级别
	int32_t				m_PetPlacardNeedHappiness;	// 发布宠物公告的宠物所需快乐度
	int32_t				m_PetPlacardNeedLife;		// 发布宠物公告的宠物所需寿命
	int32_t				m_DefaultRespawnTime ;
	int32_t				m_DropBoxRecycleTime;
	int32_t				m_TimeChangeInterval;		//时辰变更间隔
	int32_t				m_PositionRange ;
	int32_t				m_AIType ;
	int32_t				m_DisconnectTime ;
	CHAR			m_UserPath[_MAX_PATH] ;
	int32_t				m_MaxPortal ;
	int32_t				m_MaxSkillObj ;
	int32_t				m_DefaultBodyTime ;			//尸体存在时间
	int32_t				m_OutGhostTime ;			//释放灵魂时间
	float			m_CanGetExpRange ;			//杀死角色后可以得到EXP的范围
	int32_t				m_DefaultMoveSpeed ;
	int32_t				m_DefaultAttackSpeed ;
	int32_t				m_HumanVERecoverInterval;	// 精力与活力的恢复时间间隔(毫少)
	int32_t				m_WallowAge;
	int32_t				m_WallowStartTime;
	int32_t				m_WallowStartTime2;
	int32_t				m_WallowAwokeTime;
	int32_t				m_DefaultRefreshRate ;		//活力点刷新时间
	int32_t				m_DefaultCheckZoneRate ;	//刷新区域信息的时间间隔
	int32_t				m_DefaultRefuseScanTime;	//缺省怪物重生后不会扫描敌人的时间
	int32_t				m_MaxPlatform;
	int32_t				m_MaxSpecial;				// 技能OBJ的数量
	int32_t				m_MaxPlayerShopNum;			// 玩家商店的数量
	int32_t             m_MaxTimerCount;			//场景计时器数目
	int32_t				m_nMaxHumanUseTimer;		//Obj_Human可使用计时器数目
	int32_t				m_nAvailableFollowDist;		// 组队跟随的有效距离
	int32_t				m_nTimeForLoseFollow;		// 组队跟随超出有效距离多长时间后主动退出组队跟随(秒)
	int32_t				m_nFoundDurationHour;		// 创建周期
	int32_t				m_nDefaultMaxMemberCount;	// 默认帮会容纳帮众数
	int32_t				m_nResponseUserCount;		// 建立帮会需要的响应玩家数（含帮主）
	int32_t				m_nPasswdPoint;				// 多少点友好度需要输入二级密码
	int32_t				m_nPromptPoint;				// 多少点友好度需要提示上下线
	int32_t				m_nDeleteDelayTime;			// 多少小时以后再次强制解除则删除二级密码
	int32_t				m_nExpPoint;				// 获得多少经验可以提升友好度
	int32_t				m_nHashOnlineUserCount ;	//在线玩家的hash表大小
	int32_t				m_nHashMailUserCount ;		//拥有邮件的玩家hash表大小
	int32_t				m_nMaxOfflineUserCount ;	//登陆过游戏的玩家数量最大值
	float			m_fRespawnParam ;			//怪物刷新修正系数
	int32_t				m_KickUserTime ;			//不是正常游戏玩家的踢除倒计时间
	int32_t				m_nDefaultDamageFluctuation; // 缺省得伤害浮动

	int32_t				m_nMinGoodBadValue;			// 最小善恶值
	int32_t				m_nMaxGoodBadValue;			// 最大善恶值
	int32_t				m_nLevelNeeded;				// 获得善恶值的最低等级
	int32_t				m_nMemberLevel;				// 能给队长带来善恶值的队友最高等级
	float			m_fGoodBadRadius;			// 获得善恶值的半径
	int32_t				m_nBonusPerMember;			// 每个有效队友能增加的善恶值点数
	int32_t				m_nMaxBonus;				// 杀死一只怪能给队长带来的最大善恶值点数
	int32_t				m_nPenaltyWhenMemberDie;	// 有效队友的死亡减少队长的善恶值点数
	int32_t             m_nWorldChatItemIndex;

	int32_t				m_nOldRegionCount;			//旧区的个数

	_CONFIG_INFO( )
	{
		m_ServerID				= INVALID_ID ;
		m_ZoneSize				= 10 ;
		m_EquipmentDamagePoint	= 1000;
		m_RecoverTime			= 1000 ;
		m_MaxMonster			= 1000 ;
		m_MaxPet				= 1000 ;
		m_PetBodyTime			= 2000 ;
		m_PetHappinessInterval	= 600000;
		m_PetLifeInterval		= 30 * 60 * 1000;
		m_PetCallUpHappiness	= 60;
		m_PetPlacardTime		= 1000*60*30;
		m_DefaultRespawnTime	= 300000 ;
		m_DropBoxRecycleTime	= 300000 ;
		m_TimeChangeInterval	= 300000 ;
		m_PositionRange			= 10 ;
		m_AIType				= 1 ;
		m_DisconnectTime		= 0 ;
		memset( m_UserPath, 0, _MAX_PATH ) ;
		m_MaxPortal				= 0 ;
		m_MaxPlatform			= 0;
		m_MaxSpecial			= 1024;
		m_MaxPlayerShopNum		= 512;
		m_MaxSkillObj			= 1024;
		m_DefaultBodyTime		= 0 ;
		m_OutGhostTime			= 0 ;
		m_CanGetExpRange		= 30.f;
		m_DefaultMoveSpeed      = 2800 ;
		m_DefaultAttackSpeed    = 2000 ;
		m_HumanVERecoverInterval= 300000;
		m_DefaultRefreshRate    = 60000 ;
		m_DefaultCheckZoneRate  = 1000 ;
		m_nAvailableFollowDist	= 15;
		m_nTimeForLoseFollow	= 30;
		m_nFoundDurationHour	= 24;
		m_nDefaultMaxMemberCount= 160;
		m_nResponseUserCount	= 10;
		m_nPasswdPoint			= 0;
		m_nPromptPoint			= 0;
		m_nDeleteDelayTime		= 0;
		m_nExpPoint				= 0;
		m_nHashOnlineUserCount	= 1000 ;
		m_nHashMailUserCount	= 1000 ;
		m_nMaxOfflineUserCount	= 0 ;
		m_fRespawnParam			= 1.0f ;
		m_KickUserTime			= 300000 ;
		m_nDefaultDamageFluctuation = 10 ;

		m_nMinGoodBadValue		= 0;
		m_nMaxGoodBadValue		= 9999;
		m_nLevelNeeded			= 31;
		m_nMemberLevel			= 19;
		m_fGoodBadRadius		= 15.;
		m_nBonusPerMember		= 1;
		m_nMaxBonus				= 2;
		m_nPenaltyWhenMemberDie	= 10;

		m_nWorldChatItemIndex	= 0;

		m_nOldRegionCount		= 0;

	};
	~_CONFIG_INFO( )
	{

	};
};



class Config
{
public :
	Config( ) ;
	~Config( ) ;

	bool					Init( ) ;
	void					ReLoad( ) ;


	void					LoadConfigInfo( ) ;
	

public :
	_CONFIG_INFO			m_ConfigInfo ;

};


extern Config g_Config ;

#endif
