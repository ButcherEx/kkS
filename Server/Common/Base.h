/////////////////////////////////////////////////////////////////////////////////
//�ļ����ƣ�BaseType.h
//�汾˵����Windows����ϵͳ��Ҫ����꣺__WINDOWS__
//			Linux����ϵͳ��Ҫ����꣺__LINUX__
//�޸������
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __BASE_H__
#define __BASE_H__

/////////////////////////////////////////////////////////////////////////////////
//��ǰ������ϵͳͷ�ļ�����
/////////////////////////////////////////////////////////////////////////////////

//#pragma warning(disable:4800)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>

#if defined(__WINDOWS__)
	#include <windows.h>
 	#include <direct.h>
	#include <fileapi.h>
	#include <io.h>
#elif defined(__LINUX__)
	#include <sys/stat.h>
	#include <sys/utsname.h>
	#include <sys/time.h>
#endif


//////////////////////////////////////////////////////////////////////////
//������ͷ�ļ�
//////////////////////////////////////////////////////////////////////////
#define BOOST_ALL_NO_LIB
#define USE_BOOST_LIGHTWEIGHT_MUTEX
#define BOOST_SPIRIT_THREADSAFE 

#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/list.hpp>
#include <boost/container/map.hpp>
#include <boost/container/string.hpp>

#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/filesystem.hpp>
#pragma warning(disable:4250)
#include <boost/filesystem/fstream.hpp>
#pragma warning(default:4250)

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <boost/thread.hpp>
#include <boost/threadpool.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "MacroDefine.h"
#include "BaseType.h"

namespace bstd  = boost::container;
namespace bsys  = boost::system;
namespace bfs	= boost::filesystem;
namespace bsys	= boost::system;

#include "GameUtil.h"
#include "Log.h"
#include "FLString.h"

#endif
