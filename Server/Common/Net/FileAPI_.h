//////////////////////////////////////////////////////////////////////
//
// FileAPI.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __FILE_API_H__
#define __FILE_API_H__

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "Base.h"

//////////////////////////////////////////////////////////////////////
//
// Exception based system-call(API) Collection
//
//////////////////////////////////////////////////////////////////////

namespace FileAPI 
{

	//
	// exception version of open ()
	//
	int32_t open_ex (const CHAR* filename, int32_t flags) ;

	int32_t open_ex (const CHAR* filename, int32_t flags, int32_t mode) ;

	//
	// exception version of close ()
	//
	void close_ex (int32_t fd) ;

	//
	// exception version of read ()
	//
	uint32_t read_ex (int32_t fd, void* buf, uint32_t len) ;

	//
	// exception version of write ()
	//
	uint32_t write_ex (int32_t fd, const void* buf, uint32_t len) ;

	//
	// exception version of fcntl ()
	//
	int32_t fcntl_ex (int32_t fd, int32_t cmd) ;

	//
	// exception version of fcntl ()
	//
	int32_t fcntl_ex (int32_t fd, int32_t cmd, LONG arg) ;

	//
	// is this stream is nonblocking?
	//
	// using fcntl_ex()
	//
	bool getfilenonblocking_ex (int32_t fd) ;

	//
	// make this strema blocking/nonblocking
	//
	// using fcntl_ex()
	//
	void setfilenonblocking_ex (int32_t fd, bool on) ;

	//
	// exception version of ioctl ()
	//
	void ioctl_ex (int32_t fd, int32_t request, void* argp);
		
	//
	// make this stream blocking/nonblocking
	//
	// using ioctl_ex()
	//
	void setfilenonblocking_ex2 (int32_t fd, bool on);

	//
	// how much bytes available in this stream?
	//
	// using ioctl_ex()
	//
	uint32_t availablefile_ex (int32_t fd);

	//
	// exception version of dup()
	//
	int32_t dup_ex (int32_t fd);

	//
	// exception version of lseek()
	//
	LONG lseek_ex(int32_t fd, LONG offset, int32_t whence);


	LONG tell_ex( int32_t fd ) ;


};//end of namespace FileAPI

#endif


