

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "BaseLib.h"


//Config
struct LOG_CONFIG
{
	bool			m_LogAssert2Stderr;
	bstd::string	m_LogDir;
	bstd::string	m_FilePrefix;
	bstd::string	m_HostName;
	bstd::string	m_InvocationName;
	LOG_CONFIG()
	{
		m_LogDir = "RunTimeLog";
		m_FilePrefix = "";
		m_LogAssert2Stderr = true;
		m_HostName = "Unknown";
		m_InvocationName = "EXE";
	}
};



class Config
{
public :
	Config( ) ;
	~Config( ) ;

	bool					Init(const CHAR* argv0) ;
	void					ReLoad( ) ;


	void					LoadLogConfig(const CHAR* argv0) ;
	
public :
	LOG_CONFIG			m_LogConfig ;

};


extern Config g_Config ;

#endif
