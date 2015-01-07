
//********************************************
//	Ini ��غ���
//********************************************

#ifndef __INI_H__
#define __INI_H__

#include "Base.h"



#define ERROR_DATA -99999999
#define MAX_INI_VALUE 1024

//�����ļ���
class Ini
{
////////////////////////////////////////////////
// �ڲ�����
////////////////////////////////////////////////
private:
	CHAR			m_strFileName[_MAX_PATH];	//�ļ���
	long			m_lDataLen;					//�ļ�����
	CHAR*			m_strData;					//�ļ�����

	int32_t				IndexNum;					//������Ŀ��[]����Ŀ��
	int32_t*			IndexList;					//������λ���б�
	int32_t				Point;						//��ǰָ��
	int32_t				Line, Word;					//��ǰ����

	CHAR			m_szValue[MAX_INI_VALUE] ;
	CHAR			m_szRet[MAX_INI_VALUE];

////////////////////////////////////////////////
// ͨ�ýӿ�
////////////////////////////////////////////////
public:
	Ini();
	Ini(const CHAR* filename);								//��ʼ���������ļ�
	virtual ~Ini();									//�ͷ��ڴ�
	CHAR			*GetData();							//�����ļ�����
	int32_t				GetLines(int32_t);						//�����ļ�������
	int32_t				GetLines();						//�����ļ�������

	bool			Open(const CHAR* filename);				//�������ļ�
	void			Close();							//�ر������ļ�
	bool			Save(CHAR *filename=NULL);			//���������ļ�
	int32_t				FindIndex(CHAR *);					//���ر���λ��

////////////////////////////////////////////////
// �ڲ�����
////////////////////////////////////////////////
private:
	void		InitIndex();						//��ʼ������
	int32_t			FindData(int32_t, CHAR *);				//��������λ��
	int32_t			GotoNextLine(int32_t); 					//����
	CHAR*		ReadDataName(int32_t &);				//��ָ��λ�ö�һ��������
	CHAR*		ReadText(int32_t);						//��ָ��λ�ö��ַ���

	bool		AddIndex(CHAR *);					//����һ������
	bool		AddData(int32_t, CHAR *, CHAR *);		//�ڵ�ǰλ�ü���һ������
	bool		ModityData(int32_t, CHAR *, CHAR *);	//�ڵ�ǰλ���޸�һ�����ݵ�ֵ
	int32_t			GotoLastLine(CHAR *section);			//��ָ���ƶ�����INDEX�����һ��

////////////////////////////////////////////////
// ���ýӿ�
////////////////////////////////////////////////
public:
	//��һ������
	int32_t				ReadInt(CHAR *section, CHAR *key);
	//������ڣ����һ������
	bool			ReadIntIfExist(CHAR *section, CHAR *key, int32_t& nResult);
	//��ָ�����ж�һ����
	int32_t				ReadInt(CHAR *section, int32_t lines);	
	//��һ���ַ���
	CHAR*			ReadText(CHAR *section, CHAR *key, CHAR* str, int32_t size);
	//����������ȡ
	bool			ReadTextIfExist(CHAR *section, CHAR *key, CHAR* str, int32_t size);
	//��ָ�����ж�һ�ַ���
	CHAR*			ReadText(CHAR *section, int32_t lines, CHAR* str, int32_t size);	
	//��ָ���ж�һ�ַ�����
	CHAR*			ReadCaption(CHAR *section, int32_t lines, CHAR* str, int32_t size);
	//дһ������
	bool			Write(CHAR *section, CHAR *key, int32_t num);			
	//дһ���ַ���
	bool			Write(CHAR *section, CHAR *key, CHAR *string);		
	//������������������INDEX����һ�����У�
	int32_t				GetContinueDataNum(CHAR *section);	
	//��ָ��λ�ö��ַ���
	CHAR*			ReadOneLine(int32_t);
	int32_t				FindOneLine(int32_t);
	//����ָ���ַ����ڵ�����
	int32_t				ReturnLineNum(CHAR*);
};



#endif



