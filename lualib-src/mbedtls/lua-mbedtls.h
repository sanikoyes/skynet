#ifndef __LUA_MBEDTLS_H__
#define __LUA_MBEDTLS_H__

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define LUA_LIB
#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>

#define BIND_CONSTANT(name)\
	lua_pushinteger(L, name);\
	lua_setfield(L, -2, #name);

void luaL_newclass(lua_State *L, const char *name, const luaL_Reg *methods);

void register_buffer(lua_State *L);

void *luaL_checkbuffer(lua_State *L, int idx);
void *luaL_checklbuffer(lua_State *L, int idx, size_t *l);

#endif // __LUA_MBEDTLS_H__
