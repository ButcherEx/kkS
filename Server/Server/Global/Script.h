/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __SCRIPT__
#define __SCRIPT__

#include "Base.h"
#include "lua.hpp"

class Lua
{
public:
	static lua_State *New( );
	static void Free(lua_State *L);

	static void PrintStack(lua_State *L);
	static void RegisterCFunction(lua_State *L, lua_CFunction func, const  CHAR *funcName);
	static void RegisterCClosure(lua_State *L, lua_CFunction func, const  CHAR *funcName, const  CHAR *fmt, ...);
	static void RegisterLibs(lua_State *L, const luaL_Reg *l);
	/*
	@@ fmt input params and output params
	** i : int32_t; u:uint32_t; d:double; s:string; b:boolean;
	** if @LUA_STRING is defined. I: out string buffer len; S(out only): string buffer
	** if @LUA_INT64 is defined. L: (out/in) int64_t
	** b:boolean (0 or 1);'>' output values
	** @LUAT_STRING_NUM : S max number
	** eg "iiudIL>iiuSL"
	*/
#if defined (LUA_STRING)
#define LUAT_STRING_NUM 16
#endif
	static bool Call(lua_State *L, const  CHAR* funcName, const  CHAR* fmt, ...);
	static bool LoadFile(lua_State *L, const  CHAR *file);

	/*
	@@ @type : i : int32_t; u:uint32_t; d:double; b:boolean;
	** if @LUA_STRING is defined. s:string
	** All below functions(aeluaL_push*,  aeluaL_setarray*, aeluaL_settable*, -
	** aeluaL_getarray*, aeluaL_gettable*) won't support 'L','S','I'.
	*/
	static void CreateTable(lua_State *L);
	static void SetArrayField(lua_State *L,  int32_t index, CHAR type, const  void* val);
	static void SetTableField(lua_State *L, const  CHAR* k, CHAR type, const  void *val);
	static void SetArrayFieldEx(lua_State *L, int32_t tableindex, int32_t index, CHAR type, const  void* val);
	static void SetTableFieldEx(lua_State *L, int32_t tableindex, const  CHAR* k, CHAR type, const  void *val);

	static void GetArrayField(lua_State *L, int32_t index, CHAR type, void *val);
	static void GetTableField(lua_State *L, const  CHAR* k, CHAR type, void *val);
	static void GetArrayFieldEX(lua_State *L, int32_t tableindex, int32_t index, CHAR type, void *val);
	static void GetTableFieldEx(lua_State *L, int32_t tableindex, const  CHAR* k, CHAR type, void *val);


#if defined(LUA_INT64)
	/* push int64_t to lua*/
	static void PushInt64(lua_State *L, int64_t i64);
	/*
	** @idx the low 32 bits' stack index
	** in the stack,low 32 bits must below high 32 bits.
	*/
	static int64_t GetInt64(lua_State *L, int32_t idx);
#endif
};


//////////////////////////////////////////////////////////////////////////
// test function
void doScriptTest(void* file);
//////////////////////////////////////////////////////////////////////////




#endif /*__SCRIPT__*/
