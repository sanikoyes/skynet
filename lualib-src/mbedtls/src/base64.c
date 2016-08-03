#include "lua-mbedtls.h"
#include <mbedtls/base64.h>

static int l_encode(lua_State *L) {

	size_t slen;
	const unsigned char *src = (const unsigned char *) luaL_checklstring(L, 1, &slen);

	size_t olen;
	mbedtls_base64_encode(NULL, 0, &olen, src, slen);

	unsigned char *dst = malloc(olen);
	if(dst == NULL) {
		lua_pushnil(L);
		return 1;
	}
	mbedtls_base64_encode(dst, olen, &olen, src, slen);
	lua_pushlstring(L, (const char *) dst, olen);
	free(dst);
	return 1;
}

static int l_decode(lua_State *L) {

	size_t slen;
	const unsigned char *src = (const unsigned char *) luaL_checklstring(L, 1, &slen);

	size_t olen;
	mbedtls_base64_decode(NULL, 0, &olen, src, slen);

	unsigned char *dst = malloc(olen);
	if(dst == NULL) {
		lua_pushnil(L);
		return 1;
	}
	mbedtls_base64_decode(dst, olen, &olen, src, slen);
	lua_pushlstring(L, (const char *) dst, olen);
	free(dst);
	return 1;
}

static const luaL_Reg funcs[] = {
	{ "encode", l_encode },
	{ "decode", l_decode },
	{ NULL, NULL },
};

LUA_API int luaopen_mbedtls_base64_core(lua_State * const L) {

	luaL_newlib(L, funcs);
	return 1;
}
