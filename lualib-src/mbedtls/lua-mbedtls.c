#include "lua-mbedtls.h"

static const luaL_Reg funcs[] = {
    { NULL, NULL },
};

static int l_tostring(lua_State *L) {

	lua_getmetatable(L, 1);
	if(!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return 0;
	}
	lua_pushstring(L, "__name");
	lua_rawget(L, -2);
	if(!lua_isstring(L, -1)) {
		lua_pop(L, 2);
		return 0;
	}
	const char *name = lua_tostring(L, -1);
	lua_pop(L, 2);
	lua_pushfstring(L, "%s: %p", name, lua_touserdata(L, 1));
	return 1;
}

static const luaL_Reg metatables[] = {
	{ "__tostring", l_tostring },
	{ NULL, NULL },
};

void luaL_newclass(lua_State *L, const char *name, const luaL_Reg *methods) {

	luaL_newmetatable(L, name);
	luaL_setfuncs(L, metatables, 0);
	{
		const luaL_Reg *m = NULL;

		lua_pushstring(L, name);
		lua_setfield(L, -2, "__name");
		luaL_newlib(L, methods);
		lua_setfield(L, -2, "__index");

		for(m = methods; m->name; m++) {

			if(strncmp(m->name, "__", 2) == 0) {
				lua_pushcclosure(L, m->func, 0);
				lua_setfield(L, -2, m->name);
			}
		}
	}
	lua_pop(L, 1);
}

LUA_API int luaopen_mbedtls_core(lua_State * const L) {

    luaL_newlib(L, funcs);
    register_buffer(L);
    return 1;
}
