/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#include "Script.h"
#include "Config.h"
#include "Timer.h"
#include "Log.h"

LOG_DECL(LuaSystem);
LOG_IMPL(LuaSystem);

lua_State *Lua::New( )
{
    lua_State *L = luaL_newstate( );
    luaL_openlibs(L);

    return L;
}

void Lua::Free(lua_State *L)
{
    lua_close(L);
}

void Lua::PrintStack(lua_State *L)
{
	_MY_TRY
	{
		int32_t i, n;

		n = lua_gettop(L);
		LOGD(LuaSystem,"%s", "************" );
		LOGD(LuaSystem,"Lua stack's size=%d", n);
		for( i = 0; i < n; i++)
		{
			int t = lua_type(L, i);
			switch( t )
			{
			case LUA_TSTRING:
				LOGD(LuaSystem,"%s", lua_tostring(L, i));
				break;
			case LUA_TBOOLEAN:
				LOGD(LuaSystem,"%s", lua_toboolean(L, i) ? "TRUE" : "FALSE");
				break;
			case LUA_TNUMBER:
				LOGD(LuaSystem,"%d", (int32_t)lua_tonumber(L,i));
				break;
			case LUA_TFUNCTION:
				LOGD(LuaSystem,"%s", "LUA_TFUNCTION");
				break;
			case LUA_TUSERDATA:
				LOGD(LuaSystem,"%s", "LUA_TUSERDATA");
				break;
			case LUA_TTHREAD:
				LOGD(LuaSystem,"%s", "LUA_TTHREAD");
				break;
			case LUA_TTABLE:
				LOGD(LuaSystem,"%s", "LUA_TTABLE");
				break;
			default:
				LOGD(LuaSystem,"%s", lua_typename(L, i));
				break;
			};
		}
		LOGD(LuaSystem,"%s", "************");
	}
	_MY_CATCH
	{

	}
	
}

void Lua::RegisterCFunction(lua_State *L,
						  lua_CFunction func,
						  const  CHAR *funcName)
{
	lua_register(L,funcName, func);
}

void Lua::RegisterCClosure(lua_State *L,
						 lua_CFunction func,
						 const  CHAR *funcName,
						 const  CHAR *fmt, ...)
{
	int32_t args;
	va_list vl;
	va_start(vl, fmt);
	for( args = 0; *fmt; args++ ) {
		switch( *fmt++ ) {
		case 'd':
			lua_pushnumber(L, va_arg(vl, double));
			break;
		case 'i':
			lua_pushinteger(L, va_arg(vl, int32_t));
			break;
		case 'u':
			lua_pushnumber(L, va_arg(vl, uint32_t));
			break;
		case 'b':
			lua_pushboolean(L, va_arg(vl, int32_t));
			break;
		}
	}
	lua_pushcclosure(L, func, args);
	lua_setglobal(L, funcName);
}

void Lua::RegisterLibs(lua_State *L,
					 const luaL_Reg *l)
{
#pragma warning(disable:4309)
	luaL_newlib(L, l);
#pragma warning(default:4309)
}

bool Lua::Call(lua_State *L,
                                const  CHAR* funcName,
                                const  CHAR* fmt, ...)
{
	const CHAR *fmtBak = fmt;

	__ENTER_FUNCTION

	bool ret = true;
    va_list vl;
    int32_t args, res, idx;
#if defined (LUA_STRING)
    int32_t safestrnum = 0;
    int32_t safestrfill = 0;
    int32_t safestr_n[LUAT_STRING_NUM];
#endif

	const CHAR *fmtCheck = fmt;
	for(;*fmtCheck;fmtCheck++)
	{
		switch(*fmtCheck++)
		{
		case 'd':
		case 'i':
		case 'u':
		case 'b':
			break;
		case 's':
		case 'I':
		case 'S':
			
#if !defined (LUA_STRING)
			{
				LOGE(LuaSystem, "Error parameters format=%s %s %c.", funcName,fmtBak,*fmtCheck);
				return false;
			}
#endif
			break;
		case 'L':
#if !defined(LUA_INT64)
			{
				LOGE(LuaSystem, "Error parameters format=%s %s %c.", funcName,fmtBak,*fmtCheck);
				return false;
			}
#endif
			break;
		}
	}

    lua_getglobal(L, funcName);
    if( lua_isnil(L, -1) )
    {
        LOGE(LuaSystem, "Unknown function=%s", funcName );
        lua_pop(L, 1);
        return false;
    }

    va_start(vl, fmt);

    for(args = 0; *fmt; args++)
    {
        switch(*fmt++)
        {
        case 'd':
            lua_pushnumber(L, va_arg(vl, double));
            break;
        case 'i':
            lua_pushinteger(L, va_arg(vl, int32_t));
            break;
        case 'u':
            lua_pushnumber(L, va_arg(vl, uint32_t));
            break;
        case 'b':
            lua_pushboolean(L, va_arg(vl, int32_t));
            break;
#if defined (LUA_STRING)
        case 's':
            lua_pushstring(L, (const  CHAR *) va_arg(vl, ptrdiff_t));
            break;
        case 'I':
            safestr_n[safestrnum] = va_arg(vl, int32_t);
            ++safestrnum;
            break;
#endif // defined (LUA_STRING)

#if defined(LUA_INT64)
            /* args++ : int64_t will push two int32_t variables into stack */
        case 'L':
            PushInt64(L, va_arg(vl, int64_t));
            args++;
            break;
#endif // defined(LUA_INT64)

        case '>':
            goto end_arg;
            break;
        default:
			LOGE(LuaSystem, "Error parameters format=%s(%s) %c.", funcName, fmtBak, *fmt);
            break;
        }
    }

end_arg:
    res = (int32_t)strlen(fmt);


#if defined(LUA_INT64)
    {
        const  CHAR *cL = fmt;
        while(*cL)
        {
            switch( *cL++ )
            {
            case 'L':
                ++res;
                break;
            }
        }
    }
#endif

#if defined (LUA_STRING)
    args -= safestrnum;
#endif
	if( lua_pcall(L, args, res, 0) != 0 )
	{
		LOGE(LuaSystem, "lua_pcall is failed! Reason=%s.", lua_tostring(L, -1));
		lua_pop(L, 1);
		ret = false;
		goto end_call;
	}

    idx = -res;
    while(*fmt && idx)
    {
        switch( *fmt++ )
        {
        case 'd':
            *va_arg(vl, double*) = lua_tonumber(L, idx);
            break;
        case 'i':
            *va_arg(vl, int32_t*) = (int32_t)lua_tonumber(L, idx);
            break;
        case 'u':
            *va_arg(vl, uint32_t*) = (uint32_t)lua_tonumber(L, idx);
            break;
#if defined (LUA_STRING)
            /* we forbid lua return string to c. because may be overflow */
            /* the function caller must guarantee out buffer is enough */
        case 's':
            strcpy(va_arg(vl, CHAR*) , lua_tostring(L, idx));
            break;
        case 'S':
        {
            safestrfill < safestrnum ?
            strncpy( va_arg(vl, CHAR*),
                     lua_tostring(L, idx), safestr_n[safestrfill] )
            : 0;
            safestrfill++;
        }
        break;
#endif // defined (LUA_STRING)

#if defined(LUA_INT64)
        /* idx++ : int64_t will read two int32_t variables from stack */
        case 'L':
            *va_arg(vl, int64_t*) = GetInt64(L, idx);
            idx++;
            break;
#endif // defined(LUA_INT64)
        case 'b':
            *va_arg(vl, int32_t*) = (int32_t)lua_tonumber(L, idx);
            break;
        }
        idx++ ;
    }

    lua_pop(L, res);
end_call:
    va_end(vl);
	return ret;

	__LEAVE_FUNCTION_EX

		LOGE(LuaSystem, "Exception catched, Lua::Call(L,%s(%s))", funcName, fmtBak);
		return false;
}

bool Lua::LoadFile(lua_State *L, const  CHAR *file)
{
	if( luaL_loadfile(L, file) != 0 )
	{
		LOGE(LuaSystem, "luaL_loadfile is failed! Reason=%s.",lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}

    if( lua_pcall(L, 0, LUA_MULTRET, 0) != 0 )
    {
		LOGE(LuaSystem, "luaL_loadfile is failed! Reason=%s.", file, lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }

    lua_settop(L, 0);
	return true;
}


void Lua::CreateTable(lua_State *L)
{
    lua_newtable(L);
}

void Lua::SetArrayField(lua_State *L,
                                 int32_t index,
                                 CHAR type,
                                 const  void* val)
{
    lua_pushinteger(L, index);
    switch(type)
    {
    case 'd':
        lua_pushnumber(L, *(double*)val);
        break;
    case 'i':
        lua_pushinteger(L, *(int32_t*)val);
        break;
    case 'u':
        lua_pushnumber(L, *(uint32_t*)val);
        break;
#if defined(LUA_STRING)
    case 's':
        lua_pushstring(L, (const  CHAR *) val);
        break;
#endif //defined(LUA_STRING)
    case 'b':
        lua_pushboolean(L, *(int32_t*)val);
        break;
    default:
        return;
        break;
    }
    lua_settable(L, -3);
}

void Lua::SetTableField(lua_State *L,
                                 const  CHAR* k,
                                 CHAR type,
                                 const  void *val)
{
    switch(type)
    {
    case 'd':
        lua_pushnumber(L, *(double*)val);
        break;
    case 'i':
        lua_pushinteger(L, *(int32_t*)val);
        break;
    case 'u':
        lua_pushnumber(L, *(uint32_t*)val);
        break;
#if defined(LUA_STRING)
    case 's':
        lua_pushstring(L, (const  CHAR *) val);
        break;
#endif //defined(LUA_STRING)
    case 'b':
        lua_pushboolean(L, *(int32_t*)val);
        break;
    default:
        return;
        break;
    }
    lua_setfield(L, -2, k);
}

void Lua::SetArrayFieldEx(lua_State *L,
                                  int32_t tableindex,
                                  int32_t index,
                                  CHAR type,
                                  const  void* val)
{
    /* table  not on stack top */
    lua_pushinteger(L, index);
    switch(type)
    {
    case 'd':
        lua_pushnumber(L, *(double*)val);
        break;
    case 'i':
        lua_pushinteger(L, *(int*)val);
        break;
    case 'u':
        lua_pushnumber(L, *(uint32_t*)val);
        break;
#if defined(LUA_STRING)
    case 's':
        lua_pushstring(L, (const  CHAR *) val);
        break;
#endif //defined(LUA_STRING)
    case 'b':
        lua_pushboolean(L, *(int*)val);
        break;
    default:
        return;
        break;
    }
    lua_settable(L, tableindex);
}

void Lua::SetTableFieldEx(lua_State *L,
                                  int32_t tableindex,
                                  const  CHAR* k,
                                  CHAR type,
                                  const  void *val)
{
    switch(type)
    {
    case 'd':
        lua_pushnumber(L, *(double*)val);
        break;
    case 'i':
        lua_pushinteger(L, *(int32_t*)val);
        break;
    case 'u':
        lua_pushnumber(L, *(uint32_t*)val);
        break;
#if defined(LUA_STRING)
    case 's':
        lua_pushstring(L, (const  CHAR *) val);
        break;
#endif //defined(LUA_STRING)
    case 'b':
        lua_pushboolean(L, *(int32_t*)val);
        break;
    default:
        return;
        break;
    }
    lua_setfield(L, tableindex, k);
}

void Lua::GetArrayField(lua_State *L,
                                 int32_t index,
                                 CHAR type,
                                 void *val)
{
    lua_pushinteger(L, index);
    lua_gettable(L, -2);
    switch(type)
    {
    case 'd':
        *(( double*)val) = lua_tonumber(L, -1);
        break;
    case 'i':
        *(( int32_t*)val) = (int32_t)lua_tonumber(L, -1);
        break;
    case 'u':
        *(( uint32_t*)val) = (uint32_t)lua_tonumber(L, -1);
        break;
#if defined (LUA_STRING)
    case 's':
        strcpy(static_cast<CHAR*>(val), lua_tostring(L, -1));
        break;
#endif
    case 'b':
        *(( int32_t*)val) = (int32_t)lua_tonumber(L, -1);
        break;
    }
}

void Lua::GetTableField(lua_State *L,
                                 const  CHAR* k,
                                 CHAR type,
                                 void *val)
{
    switch(type)
    {
    case 'd':
        *(( double*)val) = lua_tonumber(L, -1);
        break;
    case 'i':
        *(( int*)val) = (int)lua_tonumber(L, -1);
        break;
    case 'u':
        *(( uint32_t*)val) = (uint32_t)lua_tonumber(L, -1);
        break;
#if defined (LUA_STRING)
    case 's':
        strcpy(static_cast<CHAR*>(val) , lua_tostring(L, -1));
        break;
#endif
    case 'b':
        *(( int*)val) = (int)lua_tonumber(L, -1);
        break;
    }
    lua_getfield(L, -1, k);
}

void Lua::GetArrayFieldEX(lua_State *L,
                                  int32_t tableindex,
                                  int32_t index,
                                  CHAR type,
                                  void *val)
{
    lua_pushinteger(L, index);
    lua_gettable(L, tableindex);
    switch(type)
    {
    case 'd':
        *(( double*)val) = lua_tonumber(L, -1);
        break;
    case 'i':
        *(( int32_t*)val) = (int32_t)lua_tonumber(L, -1);
        break;
    case 'u':
        *(( uint32_t*)val) = (uint32_t)lua_tonumber(L, -1);
        break;
#if defined (LUA_STRING)
    case 's':
        strcpy(static_cast<CHAR*>(val) , lua_tostring(L, -1));
        break;
#endif
    case 'b':
        *(( int32_t*)val) = (int32_t)lua_tonumber(L, -1);
        break;
    }
}

void Lua::GetTableFieldEx(lua_State *L,
                                  int32_t tableindex,
                                  const  CHAR* k,
                                  CHAR type,
                                  void *val)
{
    switch(type)
    {
    case 'd':
        *(( double*)val) = lua_tonumber(L, -1);
        break;
    case 'i':
        *(( int*)val) = (int)lua_tonumber(L, -1);
        break;
    case 'u':
        *(( uint32_t*)val) = (uint32_t)lua_tonumber(L, -1);
        break;
#if defined (LUA_STRING)
    case 's':
        strcpy(static_cast<CHAR*>(val) , lua_tostring(L, -1));
        break;
#endif
    case 'b':
        *(( int*)val) = (int)lua_tonumber(L, -1);
        break;
    }
    lua_getfield(L, tableindex, k);
}

#if defined(LUA_INT64)
/* push int64_t to lua */
void Lua::PushInt64(lua_State *L, int64_t i64)
{
    int32_t l = (int32_t) i64;
    int32_t h = (int32_t)(i64>>32);

    lua_pushinteger(L, l);
    lua_pushinteger(L, h);
}

int64_t Lua::GetInt64(lua_State *L, int32_t idx)
{
    int64_t i64;
    i64  = (uint32_t)lua_tonumber(L, idx);
    i64 |= (((int64_t)lua_tointeger(L, idx+1)) << 32);
    return i64;
}

#endif


static int luaff(lua_State *L)
{
	int32_t i = (int32_t)lua_tointeger(L, 1);
	int32_t i2 = (int32_t)lua_tointeger(L, 2);
	LOGD(LuaSystem, "lua call c funciton! %d,%d\n", i, i2);
	return 0;
}

static int aeluaL_regGlobal(lua_State *L)
{
	const CHAR *s = lua_tostring(L, 1);
	int32_t i = (int32_t)lua_tointeger(L, 2);
	return 0;
}

static int counter (lua_State *L) {
	double val = lua_tonumber(L, lua_upvalueindex(1));
	lua_pushnumber(L, ++val);   /* new val */
	lua_pushvalue(L, -1);       /* duplicate it */
	lua_replace(L, lua_upvalueindex(1));  /* update upvalue */
	return 1;  /* return new val */
}

void doScriptTest(void* file)
{
	lua_State *L = NULL;

	_MY_TRY
	{
		int32_t res;
		int32_t i1 = 1;
		int32_t i2 = 2;
		int32_t I0 = 64;
		CHAR outstr[64] = {0};
		int64_t i64 = (988998 | (((int64_t)9877778) << 32)), iout64 = 0;

		L = Lua::New();
		Lua::RegisterCFunction(L, &luaff, "luaff");
		Lua::RegisterCFunction(L, &aeluaL_regGlobal, "aeluaL_regGlobal");
		Lua::RegisterCFunction(L, &counter, "counter");

		if(!Lua::LoadFile(L, static_cast<const CHAR*>(file)))
			return ;

		Lua::Call(L, "myadd", "iisLI>iSL", i1, i2,
			__FUNCTION__, i64, 64, &res, outstr, &iout64);
		LOGD(LuaSystem,"lua function add(%d,%d)=%d,%s, in(int64_t):%I64d ?= %I64d", 
			i1, i2, res, outstr, i64, iout64);

		int32_t out1=0, out2=0;
		Lua::Call(L, "myadd1", "iiii>iii", i1, i2, 64, 64, &res, &out1, &out2);
		LOGD(LuaSystem,"lua function add(%d,%d)=%d", i1, i2, res);

		Lua::RegisterCClosure(L, &counter, "col1", "d", 0.0f);
		Lua::RegisterCClosure(L, &counter, "col2", "d", 0.0f);
		Lua::Call(L, "col1", "");
		Lua::Call(L, "col1", "");
		Lua::Call(L, "col2", "");
		Lua::Call(L, "col2", "");

		
	}
	_MY_CATCH
	{

	}

	if( L ) Lua::Free(L);
	
}


