

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Base.h"


//Config
struct LOG_CONFIG
{
	bool			m_LogAssert2Stderr;
	bstd::string	m_LogDir;
	bstd::string	m_FilePrefix;
	LOG_CONFIG()
	{
		m_LogDir = "RunTimeLog";
		m_FilePrefix = "";
		m_LogAssert2Stderr = false;
	}
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
	LOG_CONFIG			m_LogConfig ;

};


extern Config g_Config ;

#endif
