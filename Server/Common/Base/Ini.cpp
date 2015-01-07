//********************************************
//	Ini ��غ���
//********************************************


#include "Ini.h"
#include "Assertx.h"
////////////////////////////////////////////////
// ͨ�ýӿ�
////////////////////////////////////////////////

//��ʼ��
Ini::Ini()
{
	__ENTER_FUNCTION

	m_lDataLen = 0;
	m_strData = NULL;
	IndexNum = 0;
	IndexList = NULL;

	__LEAVE_FUNCTION
}

//��ʼ��
Ini::Ini( const CHAR *filename )
{
	__ENTER_FUNCTION

	m_lDataLen=0;
	m_strData=NULL;
	IndexNum=0;
	IndexList=NULL;
	memset( m_strFileName, 0, _MAX_PATH ) ;
	memset( m_szValue, 0, MAX_INI_VALUE ) ;
	memset( m_szRet, 0, MAX_INI_VALUE) ;

	Open(filename);

	__LEAVE_FUNCTION
}

//�����ͷ�
Ini::~Ini()
{
	__ENTER_FUNCTION

	if( m_lDataLen != 0 )
	{
		SAFE_FREE( m_strData );
		m_lDataLen = 0;
	}

	if( IndexNum != 0 )
	{
		SAFE_DELETE( IndexList );
		IndexNum = 0;
	}

	__LEAVE_FUNCTION
}

//�����ļ�
bool Ini::Open( const CHAR *filename )
{
	__ENTER_FUNCTION

	strncpy(m_strFileName, filename, _MAX_PATH-1);

	SAFE_FREE( m_strData );

	//��ȡ�ļ�����
	FILE* fp;
	fp = fopen(filename,"rb");
	if(fp == 0)
	{
		m_lDataLen = -1;
	}
	else
	{
		fseek( fp, 0L, SEEK_END );
		m_lDataLen	= ftell( fp );
		fclose(fp);
	}
	
	
	//�ļ�����
	if( m_lDataLen > 0 )
	{
		m_strData = (CHAR*)malloc( (size_t)m_lDataLen ) ;
		//m_strData = new CHAR[m_lDataLen];
		memset( m_strData, 0, m_lDataLen ) ;

		FILE *fp;
		fp=fopen(filename, "rb");
		AssertEx( fp!=NULL, filename );
		fread(m_strData, m_lDataLen, 1, fp);		//������
		fclose(fp);

		//��ʼ������
		InitIndex();
		return true;
	}
	else	// �ļ�������
	{
		// �Ҳ����ļ�
		m_lDataLen=1;
		m_strData = (CHAR*)malloc( (size_t)m_lDataLen ) ;
		//m_strData = new CHAR[m_lDataLen];
		memset(m_strData, 0, 1);
		InitIndex();
	}

	return false;

	__LEAVE_FUNCTION

	return 0 ;
}

//�ر��ļ�
void Ini::Close()
{
	__ENTER_FUNCTION

	if( m_lDataLen != 0 )
	{
		SAFE_DELETE( m_strData );
		m_lDataLen = 0;
	}

	if( IndexNum != 0 )
	{
		SAFE_DELETE( IndexList );
		IndexNum = 0;
	}

	__LEAVE_FUNCTION
}

//д���ļ�
bool Ini::Save(CHAR *filename)
{
	__ENTER_FUNCTION

	if( filename==NULL )
	{
		filename=m_strFileName;
	}

	FILE *fp;
	fp=fopen(filename, "wb");
	AssertEx( fp!=NULL, filename );

	fwrite(m_strData, m_lDataLen, 1, fp);
	fclose(fp);

	return true;

	__LEAVE_FUNCTION

	return false ;
}

//�����ļ�����
CHAR *Ini::GetData()
{
	return m_strData;
}

//����ļ�������
int32_t Ini::GetLines(int32_t cur)
{
	__ENTER_FUNCTION

	int32_t n=1;
	for(int32_t i=0; i<cur; i++)
	{
		if( m_strData[i]=='\n' )
			n++;
	}
	return n;

	__LEAVE_FUNCTION
		return 0;
}

//����ļ�������
int32_t Ini::GetLines()
{
	__ENTER_FUNCTION

	int32_t		n = 0;
	int32_t		i;
	for(i=0; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='\n' )
			n++;
	}
	if(i>=m_lDataLen)
		return n+1;
	return n;

	__LEAVE_FUNCTION

		return 0;
}

////////////////////////////////////////////////
// �ڲ�����
////////////////////////////////////////////////

//��������е�����λ��
void Ini::InitIndex()
{
	__ENTER_FUNCTION

	IndexNum=0;

	for(int32_t i=0; i<m_lDataLen; i++)
	{
		//�ҵ�
		if( m_strData[i]=='[' && ( m_strData[i-1]=='\n' || i==0 ) )
		{
			IndexNum++;
		}
	}

	//�����ڴ�
	SAFE_DELETE( IndexList );
	if( IndexNum>0 )
		IndexList=new int32_t[IndexNum];

	int32_t n=0;

	for(int32_t i=0; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='[' && ( m_strData[i-1]=='\n' || i==0 ) )
		{
			IndexList[n]=i+1;
			n++;
		}
	}

	__LEAVE_FUNCTION
}

//����ָ������λ��
int32_t Ini::FindIndex(CHAR *string)
{
	__ENTER_FUNCTION

	for(int32_t i=0; i<IndexNum; i++)
	{
		CHAR *str=ReadText( IndexList[i] );
		if( strcmp(string, str) == 0 )
		{
//			SAFE_FREE( str );
			return IndexList[i];
		}
//		SAFE_FREE( str );
	}
	return -1;

	__LEAVE_FUNCTION

	return 0 ;
}

//����ָ�����ݵ�λ��
int32_t Ini::FindData(int32_t index, CHAR *string)
{
	__ENTER_FUNCTION

	int32_t p=index;	//ָ��

	while(1)
	{
		p=GotoNextLine(p);
		CHAR *name=ReadDataName(p);
		if( strcmp(string, name)==0 )
		{
			SAFE_DELETE( name );
			return p;
		}

		if ( name[0] == '[' )
		{
			SAFE_DELETE( name );
			return -1;
		}

		SAFE_DELETE( name );
		if( p>=m_lDataLen ) return -1;
	}
	return -1;

	__LEAVE_FUNCTION

	return 0 ;
}

//����
int32_t Ini::GotoNextLine(int32_t p)
{
	__ENTER_FUNCTION

	int32_t i = 0;
	for(i=p; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='\n' )
			return i+1;
	}
	return i;

	__LEAVE_FUNCTION

		return p;
}

//��ָ��λ�ö�һ��������
CHAR *Ini::ReadDataName(int32_t &p)
{
	__ENTER_FUNCTION

	CHAR chr;
	CHAR *Ret;
	int32_t m=0;

	Ret=new CHAR[64];
	memset(Ret, 0, 64);

	for(int32_t i=p; i<m_lDataLen; i++)
	{
		chr = m_strData[i];

		//����
		if( chr == '\r' )
		{
			p=i+1;
			return Ret;
		}
		
		//����
		if( chr == '=' || chr == ';' )
		{
			p=i+1;
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
	}
	return Ret;

	__LEAVE_FUNCTION

	return 0 ;
}

//��ָ��λ�ö�һ�ַ���
CHAR *Ini::ReadText(int32_t p)
{
	__ENTER_FUNCTION

	CHAR chr;
	CHAR *Ret;
	int32_t n=p, m=0;

	int32_t LineNum = GotoNextLine(p) - p + 1;
	Ret=(CHAR*)m_szValue;//new CHAR[LineNum];
	memset(Ret, 0, LineNum);

	for(int32_t i=0; i<m_lDataLen-p; i++)
	{
		chr = m_strData[n];

		//����
		if( chr == ';' || chr == '\r' || chr == '\t' || chr == ']' )
		{
			//ShowMessage(Ret);
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
		n++;
	}

	return Ret;

	__LEAVE_FUNCTION

	return 0 ;
}

//����һ������
bool Ini::AddIndex(CHAR *index)
{
	__ENTER_FUNCTION

	CHAR str[256];
	memset(str, 0, 256);
	int32_t n=FindIndex(index);

	if( n == -1 )	//�½�����
	{
		sprintf(str,"\r\n[%s]",index);
		m_strData = (CHAR *)realloc(m_strData, m_lDataLen+strlen(str));	//���·����ڴ�
		sprintf(&m_strData[m_lDataLen], "%s", str);
		m_lDataLen+=(long)(strlen(str));

		InitIndex();
		return true;
	}
	
	return false;	//�Ѿ�����

	__LEAVE_FUNCTION

	return 0 ;
}

//�ڵ�ǰλ�ü���һ������
bool Ini::AddData(int32_t p, CHAR *name, CHAR *string)
{
	__ENTER_FUNCTION

	CHAR *str;
	int32_t len=(int32_t)(strlen(string));
	str=new CHAR[len+256];
	memset(str, 0, len+256);
	sprintf(str,"%s=%s",name,string);
	len=(int32_t)(strlen(str));

	p=GotoNextLine(p);	//����
	m_strData = (CHAR *)realloc(m_strData, m_lDataLen+len);	//���·����ڴ�

	CHAR *temp=new CHAR[m_lDataLen-p];
	memcpy(temp, &m_strData[p], m_lDataLen-p);
	memcpy(&m_strData[p+len], temp, m_lDataLen-p);	//�Ѻ���İᵽĩβ
	memcpy(&m_strData[p], str, len);
	m_lDataLen+=len;

	SAFE_DELETE( temp );
	SAFE_DELETE( str );
	return true;

	__LEAVE_FUNCTION

	return 0 ;
}

//�ڵ�ǰλ���޸�һ�����ݵ�ֵ
bool Ini::ModityData(int32_t p, CHAR *name, CHAR *string)
{
	__ENTER_FUNCTION

	int32_t n=FindData(p, name);

	CHAR *t=ReadText(n);
	p=n+(int32_t)(strlen(t));
//	if( strlen(t)>0 ) free(t);

	int32_t newlen=(int32_t)(strlen(string));
	int32_t oldlen=p-n;

	m_strData = (CHAR *)realloc(m_strData, m_lDataLen+newlen-oldlen);	//���·����ڴ�

	CHAR *temp=new CHAR[m_lDataLen-p];
	memcpy(temp, &m_strData[p], m_lDataLen-p);
	memcpy(&m_strData[n+newlen], temp, m_lDataLen-p);			//�Ѻ���İᵽĩβ
	memcpy(&m_strData[n], string, newlen);
	m_lDataLen+=newlen-oldlen;

	SAFE_DELETE( temp );
	return true;

	__LEAVE_FUNCTION

	return 0 ;
}

//��ָ���ƶ�����INDEX�����һ��
int32_t Ini::GotoLastLine(CHAR *index)
{
	__ENTER_FUNCTION

	int32_t n=FindIndex(index);
	n=GotoNextLine(n);
	while(1)
	{
		if( m_strData[n] == '\r' || m_strData[n] == EOF || m_strData[n] == -3 || m_strData[n] == ' ' || m_strData[n] == '/' || m_strData[n] == '\t' || m_strData[n] == '\n' )
		{
			return n;
		}
		else
		{
			n=GotoNextLine(n);
			if( n >= m_lDataLen ) return n;
		}
	}

	__LEAVE_FUNCTION

	return 0 ;
}

/////////////////////////////////////////////////////////////////////
// ����ӿ�
/////////////////////////////////////////////////////////////////////

//����ͨ��ʽ��һ�ַ�������
CHAR *Ini::ReadText(CHAR *index, CHAR *name, CHAR* str, int32_t size)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%s]", m_strFileName, index, name ) ;

	int32_t n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	if ( n == -1 )
		return NULL;

	int32_t m=FindData(n, name);
	AssertEx( m != -1, szTmp );
	if ( m == -1 )
		return NULL;

	CHAR* ret = ReadText(m);
	strncpy( str, ret, size ) ;
	return ret ;

	__LEAVE_FUNCTION

	return 0 ;
}

//����������ȡ
bool Ini::ReadTextIfExist(CHAR *index, CHAR *name, CHAR* str, int32_t size)
{
__ENTER_FUNCTION

	int32_t n = FindIndex(index);
	
	if( n == -1 )
		return false;

	int32_t m = FindData(n, name);
	
	if( m == -1 )
		return false;

	CHAR* ret = ReadText(m);
	strncpy( str, ret, size );
	return true;

__LEAVE_FUNCTION

	return false ;
}
	
//��ָ�����ж�һ�ַ���
CHAR *Ini::ReadText(CHAR *index, int32_t lines, CHAR* str, int32_t size)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%d]", m_strFileName, index, lines ) ;


	int32_t n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	//����ָ������
	n=GotoNextLine(n);
	for(int32_t i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	//������
	while( n<=m_lDataLen )
	{
		if( m_strData[n] == '=' )
		{
			n++;
			CHAR* ret = ReadText(n);
			strncpy( str, ret, size ) ;
			return ret ;
		}
		if( m_strData[n] == '\r' )
		{
			return NULL;
		}
		n++;
	}

	return NULL;

	__LEAVE_FUNCTION

	return 0 ;
}

//����ͨ��ʽ��һ����
int32_t Ini::ReadInt(CHAR *index, CHAR *name)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%s]", m_strFileName, index, name ) ;

	int32_t n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	int32_t m=FindData(n, name);
	AssertEx( m != -1, szTmp );

	CHAR *str=ReadText(m);
	int32_t ret=atoi(str);
//	free(str);
	return ret;

	__LEAVE_FUNCTION

	return 0 ;
}

bool Ini::ReadIntIfExist(CHAR *section, CHAR *key, int32_t& nResult)
{
__ENTER_FUNCTION

	int32_t n=FindIndex(section);

	if( n == -1 )
		return false;

	int32_t m=FindData(n, key);

	if( m == -1 )
		return false;

	CHAR *str=ReadText(m);
	nResult=atoi(str);
	return true;

__LEAVE_FUNCTION

	return false;
}

//��ָ�����ж�һ����
int32_t Ini::ReadInt(CHAR *index, int32_t lines)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%d]", m_strFileName, index, lines ) ;

	int32_t n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	//����ָ������
	n=GotoNextLine(n);
	for(int32_t i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	//������
	while( n<m_lDataLen )
	{
		if( m_strData[n] == '=' )
		{
			n++;
			CHAR *str=ReadText(n);
			int32_t ret=atoi(str);
//			free(str);
			return ret;
		}
		if( m_strData[n] == '\r' )
		{
			return ERROR_DATA;
		}
		n++;
	}

	return ERROR_DATA;

	__LEAVE_FUNCTION

	return 0 ;
}

//��ָ�����ж�һ��������
CHAR *Ini::ReadCaption(CHAR *index, int32_t lines, CHAR* str, int32_t size)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%d]", m_strFileName, index, lines ) ;

	int32_t n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	//����ָ������
	n=GotoNextLine(n);
	for(int32_t i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	CHAR* ret = ReadDataName(n);
	strncpy( str, ret, size ) ;
	return ret ;

	__LEAVE_FUNCTION

	return 0 ;
}

//����ͨ��ʽдһ�ַ�������
bool Ini::Write(CHAR *index, CHAR *name, CHAR *string)
{
	__ENTER_FUNCTION

	int32_t n=FindIndex(index);
	if( n == -1 )	//�½�����
	{
		AddIndex(index);
		n=FindIndex(index);
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int32_t m=FindData(n, name);
	if( m==-1 )		//�½�����
	{
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;

	__LEAVE_FUNCTION

	return 0 ;
}

//����ͨ��ʽдһ����
bool Ini::Write(CHAR *index, CHAR *name, int32_t num)
{
	__ENTER_FUNCTION

	CHAR string[32];
	sprintf(string, "%d", num);

	int32_t n=FindIndex(index);
	if( n == -1 )	//�½�����
	{
		AddIndex(index);
		n=FindIndex(index);
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int32_t m=FindData(n, name);
	if( m==-1 )		//�½�����
	{
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;

	__LEAVE_FUNCTION

	return 0 ;
}

//��������������
int32_t Ini::GetContinueDataNum(CHAR *index)
{
	__ENTER_FUNCTION

	int32_t num=0;
	int32_t n=FindIndex(index);
	n=GotoNextLine(n);
	while(1)
	{
		if( m_strData[n] == '\r' || m_strData[n] == EOF || m_strData[n] == -3 || m_strData[n] == ' ' || m_strData[n] == '/' || m_strData[n] == '\t' || m_strData[n] == '\n' )
		{
			return num;
		}
		else
		{
			num++;
			n=GotoNextLine(n);
			if( n >= m_lDataLen ) return num;
		}
	}

	__LEAVE_FUNCTION

	return 0 ;
}
//��ָ���ж�һ�ַ���
CHAR* Ini::ReadOneLine(int32_t p)
{
__ENTER_FUNCTION

	int32_t start = FindOneLine(p);
	memset(m_szRet,0,sizeof(m_szRet));
	

	for(int32_t i=start; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='\n' || m_strData[i]=='\0')
		{
			memset(m_szRet,0,sizeof(m_szRet));
			strncpy(m_szRet,&m_strData[start],i-start);
			m_szRet[i-start] = '\0';
			return m_szRet;
		}

	}

__LEAVE_FUNCTION

	return m_szRet;
}
int32_t Ini::FindOneLine(int32_t p)
{
	CHAR*	Ret = NULL;
	int32_t		n = 0;
	int32_t     m = 0;
	if(p==0)	return -1;
	if(p==1)	return 0;
	for(int32_t i=0; i<m_lDataLen; i++)
	{
		if ( m_strData[i]=='\n' )
			n++;
		if ( n==p-1 )				//�ҵ�Ҫ��Ҫ�ҵĵ���
			return i+1;
	}

	return -1; //û���ҵ�
}
int32_t Ini::ReturnLineNum(CHAR* string)
{
	int32_t p = FindIndex(string);
	CHAR*	Ret = NULL;
	int32_t		n = 0;
	int32_t     m = 0;
	if(p==0)	return -1;
	if(p==1)	return 0;
	for(int32_t i=0; i<p; i++)
	{
		if ( m_strData[i]=='\n' )
			n++;
	}
	return n;

	return -1; //û���ҵ�
}

