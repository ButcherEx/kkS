
//********************************************
//	Ini 相关函数
//********************************************

#ifndef __INI_H__
#define __INI_H__

#include "Base.h"



#define ERROR_DATA -99999999
#define MAX_INI_VALUE 1024

//配置文件类
class Ini
{
////////////////////////////////////////////////
// 内部数据
////////////////////////////////////////////////
private:
	CHAR			m_strFileName[_MAX_PATH];	//文件名
	long			m_lDataLen;					//文件长度
	CHAR*			m_strData;					//文件内容

	int32_t				IndexNum;					//索引数目（[]的数目）
	int32_t*			IndexList;					//索引点位置列表
	int32_t				Point;						//当前指针
	int32_t				Line, Word;					//当前行列

	CHAR			m_szValue[MAX_INI_VALUE] ;
	CHAR			m_szRet[MAX_INI_VALUE];

////////////////////////////////////////////////
// 通用接口
////////////////////////////////////////////////
public:
	Ini();
	Ini(const CHAR* filename);								//初始化打开配置文件
	virtual ~Ini();									//释放内存
	CHAR			*GetData();							//返回文件内容
	int32_t				GetLines(int32_t);						//返回文件的行数
	int32_t				GetLines();						//返回文件的行数

	bool			Open(const CHAR* filename);				//打开配置文件
	void			Close();							//关闭配置文件
	bool			Save(CHAR *filename=NULL);			//保存配置文件
	int32_t				FindIndex(CHAR *);					//返回标题位置

////////////////////////////////////////////////
// 内部函数
////////////////////////////////////////////////
private:
	void		InitIndex();						//初始化索引
	int32_t			FindData(int32_t, CHAR *);				//返回数据位置
	int32_t			GotoNextLine(int32_t); 					//提行
	CHAR*		ReadDataName(int32_t &);				//在指定位置读一数据名称
	CHAR*		ReadText(int32_t);						//在指定位置读字符串

	bool		AddIndex(CHAR *);					//加入一个索引
	bool		AddData(int32_t, CHAR *, CHAR *);		//在当前位置加入一个数据
	bool		ModityData(int32_t, CHAR *, CHAR *);	//在当前位置修改一个数据的值
	int32_t			GotoLastLine(CHAR *section);			//把指针移动到本INDEX的最后一行

////////////////////////////////////////////////
// 公用接口
////////////////////////////////////////////////
public:
	//读一个整数
	int32_t				ReadInt(CHAR *section, CHAR *key);
	//如果存在，则读一个整数
	bool			ReadIntIfExist(CHAR *section, CHAR *key, int32_t& nResult);
	//在指定的行读一整数
	int32_t				ReadInt(CHAR *section, int32_t lines);	
	//读一个字符串
	CHAR*			ReadText(CHAR *section, CHAR *key, CHAR* str, int32_t size);
	//如果存在则读取
	bool			ReadTextIfExist(CHAR *section, CHAR *key, CHAR* str, int32_t size);
	//在指定的行读一字符串
	CHAR*			ReadText(CHAR *section, int32_t lines, CHAR* str, int32_t size);	
	//在指定行读一字符名称
	CHAR*			ReadCaption(CHAR *section, int32_t lines, CHAR* str, int32_t size);
	//写一个整数
	bool			Write(CHAR *section, CHAR *key, int32_t num);			
	//写一个字符串
	bool			Write(CHAR *section, CHAR *key, CHAR *string);		
	//返回连续的行数（从INDEX到第一个空行）
	int32_t				GetContinueDataNum(CHAR *section);	
	//在指定位置读字符串
	CHAR*			ReadOneLine(int32_t);
	int32_t				FindOneLine(int32_t);
	//返回指定字符所在的行数
	int32_t				ReturnLineNum(CHAR*);
};



#endif



