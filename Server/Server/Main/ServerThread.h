

#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include "BaseLib.h"

//�������������֮������ͨѶ���߳�

class ServerThread : public Thread
{
public :
	ServerThread( ) ;
	~ServerThread( ) ;

	//��ѭ��
	virtual void	run( ) ;

	//ֹͣ�߳�ִ��
	virtual void	stop( ) { m_Active = false ; } ;

	//�жϵ�ǰ�߳��Ƿ񼤻�
	bool			IsActive( ){ return m_Active ; } ;



private :
	bool		m_Active ;


};





#endif
