

#ifndef __THREADMANAGER_H__
#define __THREADMANAGER_H__

#include "BaseLib.h"
#include "ServerThread.h"



//�������߳����⣬�����̶߳��ɴ�ģ��������
//Login �˵� ThreadManager ȡ��ThreadPool
//��Ϊ����Ҫ�ܳ��� 
class ThreadManager
{
public :
	ThreadManager( ) ;
	~ThreadManager( ) ;

	//��ʼ��
	bool				Init( ) ;
	//���������߳�
	bool				Start( ) ;
	//ֹͣ�����߳�
	bool				Stop( ) ;
	
	//ȡ�õ�ǰ�ķ������߳�
	ServerThread*		GetServerThread(){ 
		return m_pServerThread ; 
	} ;
	//ȡ�õ�ǰ���е��߳�����
	uint32_t				GetTotalThreads(){ return m_nThreads ; } ;
protected :
	ServerThread*		m_pServerThread ;
	uint32_t				m_nThreads ;

};
extern ThreadManager*	g_pThreadManager ;




#endif
