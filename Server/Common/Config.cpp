//#include "stdafx.h"

#include "Config.h"
#include "Ini.h"
#include "Log.h"
#include "Assertx.h"
//#include "FileDef.h"

Config g_Config ;

Config::Config( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

Config::~Config( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

bool Config::Init( )
{
__ENTER_FUNCTION

	LoadConfigInfo( ) ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}

void Config::ReLoad( )
{

}

void Config::LoadConfigInfo( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}


