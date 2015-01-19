
#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__

#include "Base.h"


class TimeManager
{
public :
	TimeManager( ) ;
	~TimeManager( ) ;

	bool			Init( ) ;

	//��ǰʱ�����ֵ����ʼֵ����ϵͳ��ͬ������
	//���ص�ֵΪ��΢�λ��ʱ��ֵ
	uint32_t			CurrentTime( ) ;
	//ֱ�ӷ��أ�������ϵͳ�ӿ�
	uint32_t			CurrentSavedTime( ){ return m_CurrentTime ; } ;
	//ȡ�÷������˳�������ʱ��ʱ�����ֵ
	uint32_t			StartTime( ){ return m_StartTime ; } ;

	//����ǰ��ϵͳʱ���ʽ����ʱ���������
	void			SetTime( ) ;

	// �õ���׼ʱ��
	time_t			GetANSITime( );

public :
	//***ע�⣺
	//���½ӿڵ���û��ϵͳ���ã�ֻ���ʱ��������ڵ�����
	//

	//ȡ������ʱ��ʱ��ġ��ꡢ�¡��ա�Сʱ���֡��롢���ڵ�ֵ��
	int32_t				GetYear( ){ return m_TM.tm_year+1900 ; } ;	//[1900,????]
	int32_t				GetMonth( ){ return m_TM.tm_mon ; } ;		//[0,11]
	int32_t				GetDay( ){ return m_TM.tm_mday ; } ;		//[1,31]
	int32_t				GetHour( ){ return m_TM.tm_hour ; } ;		//[0,23]
	int32_t				GetMinute( ){ return m_TM.tm_min ; } ;		//[0,59]
	int32_t				GetSecond( ){ return m_TM.tm_sec ; } ;		//[0,59]
	//ȡ�õ�ǰ�����ڼ���0��ʾ�������죬1��6��ʾ������һ��������
	uint32_t			GetWeek( ){ return m_TM.tm_wday ; } ;
	//����ǰ��ʱ�䣨�ꡢ�¡��ա�Сʱ���֣�ת����һ��uint32_t����ʾ
	//���磺0,507,211,233 ��ʾ "2005.07.21 12:33"
	uint32_t			Time2DWORD( ) ;
	//ȡ�õ�ǰ������[4bit 0-9][4bit 0-11][5bit 0-30][5bit 0-23][6bit 0-59][6bit 0-59]
	uint32_t			CurrentDate( ) ;
	//ȡ�÷����������������ʱ�䣨���룩
	uint32_t			RunTime( ){ 
		CurrentTime( ) ;
		return (m_CurrentTime-m_StartTime);  
	} ;
	uint16_t			RunTick( )
	{
		CurrentTime();
		return uint16_t(m_CurrentTime-m_StartTime);  
	};
	//ȡ����������ʱ���ʱ����λ�����룩, Ret = Date2-Data1
	uint32_t			DiffTime( uint32_t Date1, uint32_t Date2 ) ;
	//��һ��uint32_t������ת����һ��tm�ṹ
	void			ConvertUT( uint32_t Date, tm* TM ) ;
	//��һ��tm�ṹת����һ��uint32_t������
	void			ConvertTU( tm* TM, uint32_t& Date ) ;
	//ȡ������Ϊ��λ��ʱ��ֵ, ǧλ��������ݣ�������λ����ʱ�䣨������
	uint32_t			GetDayTime( ) ;
	//�õ���ǰ�ǽ����ʲôʱ��2310��ʾ23��10��
	uint16_t			GetTodayTime();
	bool			FormatTodayTime(uint16_t& nTime);


public :
	uint32_t			m_StartTime ;
	uint32_t			m_CurrentTime ;
	time_t			m_SetTime ;
	tm				m_TM ;
#ifdef __LINUX__
	struct timeval _tstart, _tend;
	struct timezone tz;
#endif



};

extern TimeManager g_TimeManager;




#endif
