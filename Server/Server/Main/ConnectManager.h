

#ifndef __CONNECTMANAGER_H__
#define __CONNECTMANAGER_H__

#include "BaseLib.h"
#include "LoginPlayerManager.h"


//����������ӽ���Ŀͻ���
class ConnectManager : public Thread
{
public :
	ConnectManager( ) ;
	~ConnectManager( ) ;

	//��ʼ��ģ��
	bool			Init( ) ;
	//ģ�鴦��ѭ��
	virtual void	run () ;
	//ģ���˳�
	virtual void	stop( ){ m_Active = false ; } ;
	//ģ���˳�����
	void			Quit( ) ;

	//�жϵ�ǰģ���Ƿ��ڻ״̬
	bool			IsActive( ){ return m_Active ; } ;
private :
	bool			m_Active ;//�Ƿ��ı�־


};

extern ConnectManager* g_pConnectManager ;

#endif
