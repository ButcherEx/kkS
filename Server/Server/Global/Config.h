

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Base.h"


//Config
struct LOG_CONFIG
{
	int32_t			m_ThreadNum;
	bstd::string	m_HostName;
	bstd::string	m_InvocationName;
	bool			m_LogAssert2Stderr;
	LOG_CONFIG()
	{
		m_ThreadNum = 8;
		m_LogAssert2Stderr = true;
		m_HostName = "Unknown";
		m_InvocationName = ".exe";
	}
};



class Config
{
public :
	Config( ) ;
	~Config( ) ;
public:
	bool Init(const CHAR* argv0) ;
	void ReLoad( ) ;
	void LoadLogConfig(const CHAR* argv0) ;
	
public :
	LOG_CONFIG			m_LogConfig ;

};


extern Config g_Config ;

#endif
