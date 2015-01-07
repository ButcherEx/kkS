

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Base.h"


//Config
struct _CONFIG_INFO
{
	int32_t				m_SystemModel;
	ID_t			m_ServerID ;
	int32_t				m_ZoneSize ;
	float			m_DropParam;				//ȫ�ֵ�����������
	float			m_ExpParam;					//ȫ�־�����������
	int32_t				m_EquipmentDamagePoint;		//��Ʒ�;öȵ�΢��ֵ
	int32_t				m_RecoverTime ;
	int32_t				m_MaxMonster ;
	int32_t				m_MaxPet ;
	int32_t				m_PetBodyTime ;
	int32_t				m_PetHappinessInterval;		// ������ֶ��½�1��ʱ����(����)
	int32_t				m_PetLifeInterval;			// ���������½�1��ʱ����(����)
	int32_t				m_PetCallUpHappiness;		// ������ֶȴ��ڲſ����ٳ�
	uint32_t			m_PetPlacardTime;			// ����һ�����﹫��ĳ���ʱ��
	int32_t				m_PetPlacardNeedLevel;		// �������﹫��ĳ������輶��
	int32_t				m_PetPlacardNeedHappiness;	// �������﹫��ĳ���������ֶ�
	int32_t				m_PetPlacardNeedLife;		// �������﹫��ĳ�����������
	int32_t				m_DefaultRespawnTime ;
	int32_t				m_DropBoxRecycleTime;
	int32_t				m_TimeChangeInterval;		//ʱ��������
	int32_t				m_PositionRange ;
	int32_t				m_AIType ;
	int32_t				m_DisconnectTime ;
	CHAR			m_UserPath[_MAX_PATH] ;
	int32_t				m_MaxPortal ;
	int32_t				m_MaxSkillObj ;
	int32_t				m_DefaultBodyTime ;			//ʬ�����ʱ��
	int32_t				m_OutGhostTime ;			//�ͷ����ʱ��
	float			m_CanGetExpRange ;			//ɱ����ɫ����Եõ�EXP�ķ�Χ
	int32_t				m_DefaultMoveSpeed ;
	int32_t				m_DefaultAttackSpeed ;
	int32_t				m_HumanVERecoverInterval;	// ����������Ļָ�ʱ����(����)
	int32_t				m_WallowAge;
	int32_t				m_WallowStartTime;
	int32_t				m_WallowStartTime2;
	int32_t				m_WallowAwokeTime;
	int32_t				m_DefaultRefreshRate ;		//������ˢ��ʱ��
	int32_t				m_DefaultCheckZoneRate ;	//ˢ��������Ϣ��ʱ����
	int32_t				m_DefaultRefuseScanTime;	//ȱʡ���������󲻻�ɨ����˵�ʱ��
	int32_t				m_MaxPlatform;
	int32_t				m_MaxSpecial;				// ����OBJ������
	int32_t				m_MaxPlayerShopNum;			// ����̵������
	int32_t             m_MaxTimerCount;			//������ʱ����Ŀ
	int32_t				m_nMaxHumanUseTimer;		//Obj_Human��ʹ�ü�ʱ����Ŀ
	int32_t				m_nAvailableFollowDist;		// ��Ӹ������Ч����
	int32_t				m_nTimeForLoseFollow;		// ��Ӹ��泬����Ч����೤ʱ��������˳���Ӹ���(��)
	int32_t				m_nFoundDurationHour;		// ��������
	int32_t				m_nDefaultMaxMemberCount;	// Ĭ�ϰ�����ɰ�����
	int32_t				m_nResponseUserCount;		// ���������Ҫ����Ӧ���������������
	int32_t				m_nPasswdPoint;				// ���ٵ��Ѻö���Ҫ�����������
	int32_t				m_nPromptPoint;				// ���ٵ��Ѻö���Ҫ��ʾ������
	int32_t				m_nDeleteDelayTime;			// ����Сʱ�Ժ��ٴ�ǿ�ƽ����ɾ����������
	int32_t				m_nExpPoint;				// ��ö��پ�����������Ѻö�
	int32_t				m_nHashOnlineUserCount ;	//������ҵ�hash���С
	int32_t				m_nHashMailUserCount ;		//ӵ���ʼ������hash���С
	int32_t				m_nMaxOfflineUserCount ;	//��½����Ϸ������������ֵ
	float			m_fRespawnParam ;			//����ˢ������ϵ��
	int32_t				m_KickUserTime ;			//����������Ϸ��ҵ��߳�����ʱ��
	int32_t				m_nDefaultDamageFluctuation; // ȱʡ���˺�����

	int32_t				m_nMinGoodBadValue;			// ��С�ƶ�ֵ
	int32_t				m_nMaxGoodBadValue;			// ����ƶ�ֵ
	int32_t				m_nLevelNeeded;				// ����ƶ�ֵ����͵ȼ�
	int32_t				m_nMemberLevel;				// �ܸ��ӳ������ƶ�ֵ�Ķ�����ߵȼ�
	float			m_fGoodBadRadius;			// ����ƶ�ֵ�İ뾶
	int32_t				m_nBonusPerMember;			// ÿ����Ч���������ӵ��ƶ�ֵ����
	int32_t				m_nMaxBonus;				// ɱ��һֻ���ܸ��ӳ�����������ƶ�ֵ����
	int32_t				m_nPenaltyWhenMemberDie;	// ��Ч���ѵ��������ٶӳ����ƶ�ֵ����
	int32_t             m_nWorldChatItemIndex;

	int32_t				m_nOldRegionCount;			//�����ĸ���

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
