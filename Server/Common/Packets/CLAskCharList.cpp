//#include "stdafx.h"

#include "CLAskCharList.h"


bool CLAskCharList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	return true ;

	__LEAVE_FUNCTION

	return false ;
}

bool CLAskCharList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	return true ;

	__LEAVE_FUNCTION

	return false ;
}

uint32_t CLAskCharList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CLAskCharListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return false ;
}


const	CHAR*	CLAskCharList::GetAccount()	const
{
	return szAccount;
}
void			CLAskCharList::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT);
	szAccount[MAX_ACCOUNT] = '\0' ;
}

