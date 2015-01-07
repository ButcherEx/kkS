//#include "stdafx.h"
//////////////////////////////////////////////////////////////////////
//
// FileAPI.cpp
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////

#include "FileAPI.h"


#if __WINDOWS__
#include <io.h>			// for _open()
#include <fcntl.h>		// for _open()/_close()/_read()/_write()...
#include <string.h>		// for memcpy()
#elif __LINUX__
#include <sys/types.h>	// for open()
#include <sys/stat.h>	// for open()
#include <unistd.h>		// for fcntl()
#include <fcntl.h>		// for fcntl()
#include <sys/ioctl.h>	// for ioctl()
#include <errno.h>		// for errno
#endif


//////////////////////////////////////////////////
// external variables
//////////////////////////////////////////////////
#if __LINUX__
extern int32_t errno;
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int32_t FileAPI::open_ex ( const CHAR * filename , int32_t flags ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	int32_t fd = open(filename,flags);
#elif __WINDOWS__
	int32_t fd = _open(filename,flags);
#endif
	if ( fd < 0 ) {

#if __LINUX__
		switch ( errno ) {
		case EEXIST : 
		case ENOENT  : 
		case EISDIR : 
		case EACCES : 
		case ENAMETOOLONG : 
		case ENOTDIR : 
		case ENXIO   : 
		case ENODEV  : 
		case EROFS   : 
		case ETXTBSY : 
		case EFAULT  : 
		case ELOOP   : 
		case ENOSPC  : 
		case ENOMEM  : 
		case EMFILE  : 
		case ENFILE  : 
		default :
			{
				break;
			}
		}//end of switch
#elif __WINDOWS__
	// ...
#endif
	}

	return fd;

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int32_t FileAPI::open_ex ( const CHAR * filename , int32_t flags , int32_t mode ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	int32_t fd = open(filename,flags,mode);
#elif __WINDOWS__
	int32_t fd = _open(filename,flags,mode);
#endif

	if ( fd < 0 ) {
#if __LINUX__
		switch ( errno ) {
		case EEXIST : 
		case EISDIR : 
		case EACCES : 
		case ENAMETOOLONG : 
		case ENOENT  : 
		case ENOTDIR : 
		case ENXIO   : 
		case ENODEV  : 
		case EROFS   : 
		case ETXTBSY : 
		case EFAULT  : 
		case ELOOP   : 
		case ENOSPC  : 
		case ENOMEM  : 
		case EMFILE  : 
		case ENFILE  : 
		default :
			{
				break;
			}
		}//end of switch
#elif __WINDOWS__
	// ...
#endif
	}

	return fd;

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// uint32_t FileAPI::read_ex ( INT fd , void * buf , uint32_t len ) 
//
//
// exception version of read()
//
// Parameters 
//     fd  - file descriptor
//     buf - reading buffer
//     len - reading length
//
// Return
//     length of reading bytes
//
//
//////////////////////////////////////////////////////////////////////
uint32_t FileAPI::read_ex ( int32_t fd , void * buf , uint32_t len ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	int32_t result = read ( fd , buf , len );
#elif __WINDOWS__
	int32_t result = _read ( fd , buf , len );
#endif

	if ( result < 0 ) {

#if __LINUX__
		switch ( errno ) {
			case EINTR : 
			case EAGAIN : 
			case EBADF : 
			case EIO : 
			case EISDIR : 
			case EINVAL : 
			case EFAULT : 
			case ECONNRESET :
			default : 
				{
					break;
				}
		}
#elif __WINDOWS__
	// ...
#endif
	} else if ( result == 0 ) {
	}

	return result;

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// uint32_t FileAPI::write_ex ( INT fd , void * buf , uint32_t len ) 
//
//
// exception version of write()
//
// Parameters 
//     fd  - file descriptor
//     buf - writing buffer
//     len - writing length
//
// Return
//     length of reading bytes
//
//
//////////////////////////////////////////////////////////////////////
uint32_t FileAPI::write_ex ( int32_t fd , const void * buf , uint32_t len ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	int32_t result = write ( fd , buf , len );
#elif __WINDOWS__
	int32_t result = _write ( fd , buf , len );
#endif

	if ( result < 0 ) {
		
#if __LINUX__
		switch ( errno ) {
			case EAGAIN : 
			case EINTR : 
			case EBADF : 
			case EPIPE : 
			case EINVAL: 
			case EFAULT: 
			case ENOSPC : 
			case EIO : 
			case ECONNRESET :
			default : 
				{
					break;
				}
		}
#elif __WINDOWS__
	//...
#endif
	}

	return result;

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// void FileAPI::close_ex ( INT fd ) 
//
//
// exception version of close()
//
// Parameters
//     fd - file descriptor
//
// Return
//     none
//
//
//////////////////////////////////////////////////////////////////////
void FileAPI::close_ex ( int32_t fd ) 
{
	__ENTER_FUNCTION_FOXNET

	if ( close(fd) < 0 ) {
#if __LINUX__
		switch ( errno ) {
			case EBADF : 
			default :
				{
					break;
				}
		}
#elif __WINDOWS__
#endif
	}

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// INT FileAPI::fcntl_ex ( INT fd , INT cmd ) 
//
//
// Parameters
//     fd  - file descriptor
//     cmd - file control command
//
// Return
//     various according to cmd
//
//
//////////////////////////////////////////////////////////////////////
int32_t FileAPI::fcntl_ex ( int32_t fd , int32_t cmd ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	int32_t result = fcntl ( fd , cmd );
	if ( result < 0 ) {
		switch ( errno ) {
			case EINTR : 
			case EBADF : 
			case EACCES : 
			case EAGAIN : 
			case EDEADLK : 
			case EMFILE  : 
			case ENOLCK : 
			default : 
				{
					break;
				}
		}
	}
	return result;
#elif __WINDOWS__
	return 0 ;
#endif

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// INT FileAPI::fcntl_ex ( INT fd , INT cmd , LONG arg ) 
//
//
// Parameters
//     fd  - file descriptor
//     cmd - file control command
//     arg - command argument
//
// Return
//     various according to cmd
//
//
//////////////////////////////////////////////////////////////////////
int32_t FileAPI::fcntl_ex ( int32_t fd , int32_t cmd , LONG arg ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	int32_t result = fcntl ( fd , cmd , arg );
	if ( result < 0 ) {
		switch ( errno ) {
			case EINTR : 
			case EINVAL : 
			case EBADF : 
			case EACCES : 
			case EAGAIN : 
			case EDEADLK : 
			case EMFILE  : 
			case ENOLCK : 
			default : 
				{
					break;
				}
		}
	}
	return result;
#elif __WINDOWS__
	return 0 ;
#endif

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// BOOL getfilenonblocking_ex ( INT fd ) 
//
//
// check if this file is nonblocking mode
//
// Parameters
//     fd - file descriptor
//
// Return
//     TRUE if nonblocking, FALSE if blocking
//
//
//////////////////////////////////////////////////////////////////////
bool FileAPI::getfilenonblocking_ex ( int32_t fd ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	int32_t flags = fcntl_ex( fd , F_GETFL , 0 );
	return flags | O_NONBLOCK;
#elif __WINDOWS__
	return FALSE;
#endif

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// void setfilenonblocking_ex ( INT fd , BOOL on ) 
//
//
// make this file blocking/nonblocking
//
// Parameters
//     fd - file descriptor
//     on - TRUE if nonblocking, FALSE if blocking
//
// Return
//     none
//
//
//////////////////////////////////////////////////////////////////////
void FileAPI::setfilenonblocking_ex ( int32_t fd , bool on ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	int32_t flags = fcntl_ex( fd , F_GETFL , 0 );

	if ( on )
		// make nonblocking fd
		flags |= O_NONBLOCK;
	else
		// make blocking fd
		flags &= ~O_NONBLOCK;

	fcntl_ex( fd , F_SETFL , flags );
#elif __WINDOWS__
#endif

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// void FileAPI::ioctl_ex ( INT fd , INT request , void * argp )
//
//
// exception version of ioctl()
//
// Parameters
//     fd      - file descriptor
//     request - i/o control request
//     argp    - argument
//
// Return
//     none
//
//
//////////////////////////////////////////////////////////////////////
void FileAPI::ioctl_ex ( int32_t fd , int32_t request , void * argp )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	if ( ioctl(fd,request,argp) < 0 ) {
		switch ( errno ) {
		case EBADF : 
		case ENOTTY : 
		case EINVAL : 
		default :
			{
				break;
			}
		}
	}
#elif __WINDOWS__
#endif

	__LEAVE_FUNCTION_FOXNET
}



//////////////////////////////////////////////////////////////////////
//
// void FileAPI::setfilenonblocking_ex2 ( INT fd , BOOL on )
//
//
// make this stream blocking/nonblocking using ioctl_ex()
//
// Parameters
//     fd - file descriptor
//     on - TRUE if nonblocking, FALSE else
//
// Return
//     none
//
//
//////////////////////////////////////////////////////////////////////
void FileAPI::setfilenonblocking_ex2 ( int32_t fd , bool on )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	ULONG arg = ( on == TRUE ? 1 : 0 );
	ioctl_ex(fd,FIONBIO,&arg);
#elif __WINDOWS__
#endif

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// uint32_t FileAPI::available_ex ( INT fd )
//
//
// how much bytes available in this stream? using ioctl_ex()
//
// Parameters
//     fd - file descriptor
//
// Return
//     #bytes available
//
//
//////////////////////////////////////////////////////////////////////
uint32_t FileAPI::availablefile_ex ( int32_t fd )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	uint32_t arg = 0;
	ioctl_ex(fd,FIONREAD,&arg);
	return arg;
#elif __WINDOWS__
	return 0;
#endif

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// INT FileAPI::dup_ex ( INT fd )
//
//
//////////////////////////////////////////////////////////////////////
int32_t FileAPI::dup_ex ( int32_t fd )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	int32_t newfd = dup(fd);
#elif __WINDOWS__
	int32_t newfd = _dup(fd);
#endif

	if ( newfd < 0 ) {
#if __LINUX__
		switch ( errno ) {
		case EBADF : 
		case EMFILE : 
		default :
			{
				break;
			}
		}//end of switch
#elif __WINDOWS__
#endif
	}

	return newfd;

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// LONG FileAPI::lseek_ex ( INT fd , LONG offset , INT whence )
//
//
//////////////////////////////////////////////////////////////////////
LONG FileAPI::lseek_ex ( int32_t fd , LONG offset , int32_t whence )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	LONG result = lseek(fd,offset,whence);
	if ( result < 0 ) {
		switch ( errno ) {
		case EBADF : 
		case ESPIPE : 
		case EINVAL : 
		default :
			{
				break;
			}
		}
	}
#elif __WINDOWS__
	LONG result = _lseek(fd,offset,whence);
	if ( result < 0 ) {
	}
#endif

	return result;

	__LEAVE_FUNCTION_FOXNET

}

LONG FileAPI::tell_ex( int32_t fd )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	LONG result;
#elif __WINDOWS__
	LONG result = _tell(fd);
	if ( result < 0 ) {
	}
#endif

	return result ;

	__LEAVE_FUNCTION_FOXNET
}

