#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "BaseLib.h"

class Login
{
public:
	Login();
	~Login();

public:
	//Login ��������ʼ��
	bool		Init();

	//Login ��������ѭ��
	bool		Loop();

	//Login �������˳�
	bool		Exit();


	//ֹͣģ��ִ�У������ô˽ӿں󣬻Ὣ����ӵ���̵߳�ģ������Ϊ���״̬
	//�������̵߳�ѭ��������һ��ѭ��������ͻ��˳�
	void		Stop( ) ;

protected:
	//����������ݿռ�
	bool		NewLogin();
	//��ʼ��������ݿռ�
	bool		InitLogin();
	bool		m_bExited;
};


extern Login	g_Login;



#endif