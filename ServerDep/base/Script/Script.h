/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __SCRIPT__
#define __SCRIPT__

#include <PreCompier.h>
#include <lua.hpp>

BASE_NAME_SPACES

lua_State *luaNew( );
void luaFree(lua_State *L);

void luaPrintStack(lua_State *L);
void luaRegisterCFunction(lua_State *L, lua_CFunction func, const  char *funcName);
void luaRegisterCClosure(lua_State *L, lua_CFunction func, const  char *funcName, const  char *fmt, ...);
void luaRegisterLibs(lua_State *L, const luaL_Reg *l);
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
bool luaCallFunction(lua_State *L, const  char* funcName, const  char* fmt, ...);
bool luaLoadFile(lua_State *L, const  char *file);

/*
@@ @type : i : int32_t; u:uint32_t; d:double; b:boolean;
** if @LUA_STRING is defined. s:string
** All below functions(aeluaL_push*,  aeluaL_setarray*, aeluaL_settable*, -
** aeluaL_getarray*, aeluaL_gettable*) won't support 'L','S','I'.
*/
void luaPushTable(lua_State *L);
void luaSetArrayField(lua_State *L,  int32_t index, char type, const  void* val);
void luaSetTableField(lua_State *L, const  char* k, char type, const  void *val);
void luaSetArrayFieldEx(lua_State *L, int32_t tableindex, int32_t index, char type, const  void* val);
void luaSetTableFieldEx(lua_State *L, int32_t tableindex, const  char* k, char type, const  void *val);

void luaGetArrayField(lua_State *L, int32_t index, char type, void *val);
void luaGetTableField(lua_State *L, const  char* k, char type, void *val);
void luaGetArrayFieldEX(lua_State *L, int32_t tableindex, int32_t index, char type, void *val);
void luaGetTableFieldEx(lua_State *L, int32_t tableindex, const  char* k, char type, void *val);

#if defined(LUA_INT64)
/* push int64_t to lua*/
void luaPushInt64(lua_State *L, int64_t i64);
/*
** @idx the low 32 bits' stack index
** in the stack,low 32 bits must below high 32 bits.
*/
int64_t luaGetInt64(lua_State *L, int32_t idx);
#endif

BASE_NAME_SPACEE

#ifdef SCRIPT_UNIT_TEST
AUTOTEST_DEF(luaUnitTest)
#endif

#endif /*__SCRIPT__*/
