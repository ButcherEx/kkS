/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __DB_HANDLER__
#define __DB_HANDLER__

/*
** ���ݿ⴦���ִ࣬�в�ѯ�����¡�ɾ�����滻(replace into)����
*** COMMON FUNCTION **
** 1.�н��������: CommonSelect (��ѯ)
** 2.�޽��������: CommonUpdate (���¡�ɾ�����滻)
** 3. �޽�����첽��־����: CommonUpdateLog(���¡�ɾ�����滻)
*** NOTICE ***
** 1.����Ҫ���ڴ�ȫ���ⲿ�����߷��䲢��������ֻʹ�ò�����
** 2.������ݿⷵ�صĽ�������ɹ��첻�������������ݰ�
** 3.�����ݿ�д����߸��¶�������ʱ��ÿ�����ݵĴ�С����һ��
** 4.����CommonUpdateֻ���ṩһ����ʽ����Ӧ�ֶ�����UpdateFormatFunc�ĺ��� 
** 5.����CommonSelect���˵��������躯������Ҫ�ṩһ��������������ProcResultFunc�ĺ���
** 6.��־ʵʱ�Բ���,�������첽��ʽ��Ҫ����updateFormatFunc2�ĺ���
*/

// ���ݿ⴦����
typedef fastdelegate::FastDelegate3<MySqlStream*, DWORD, DWORD, VOID> SelectFormatFunc;
typedef fastdelegate::FastDelegate3<MySqlStream*, DWORD, VOID*, VOID> UpdateFormatFunc;
typedef fastdelegate::FastDelegate3<MySqlStream*, CONST CHAR*, CONST VOID*, VOID> updateFormatFunc2;
typedef fastdelegate::FastDelegate3<VOID*&, INT*, QueryResult*, VOID> ProcResultFunc;
#define MakeDelegateOfDBHandler(func) fastdelegate::MakeDelegate(this, func)

class MyDBHandler : public MyNonCopyable
{
public:
	VOID attach(DataBase *db);
	DataBase *detach( );
public:
	// ��������,ͬ������
	BOOL commonSelect(
		IN OUT VOID* &pBuffer, 
		OUT INT *pNumber, 
		DWORD Param1, 
		DWORD Param2, 
		SelectFormatFunc formatFunc, 
		ProcResultFunc procsFunc
	);

	// ��������(ɾ��/����/����),������,ͬ������
	BOOL commonUpdate(
		DWORD Param1, 
		CONST VOID *pBuffer, 
		INT elmCnt, 
		INT sizeOne, 
		UpdateFormatFunc formatFunc, 
		OUT VOID **ppBuffer = NULL
	);

	// ��־����,��������,�첽����(ɾ��/����/����)
	VOID commonUpdateLog(
		CONST CHAR *tabePostFix, 
		CONST VOID *pBuffer, 
		updateFormatFunc2 formatFunc
	);
public:
	MyDBHandler( );
	virtual ~MyDBHandler( );
private:
	DataBase *m_Database;
};

#endif /* __DB_HANDLER__ */





