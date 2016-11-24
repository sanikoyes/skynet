#include "lua-mbedtls.h"
#include <mbedtls/entropy.h>

#define CLASS_NAME "mbedtls_entropy_context"

static int l_context(lua_State *L) {

	mbedtls_entropy_context *ctx = lua_newuserdata(L, sizeof(mbedtls_entropy_context));
	mbedtls_entropy_init(ctx);

	luaL_getmetatable(L, CLASS_NAME);
	lua_setmetatable(L, -2);

	return 1;
}

static const luaL_Reg funcs[] = {
    { "context", l_context },
    { NULL, NULL },
};

// int mbedtls_entropy_add_source( mbedtls_entropy_context *ctx,
//                         mbedtls_entropy_f_source_ptr f_source, void *p_source,
//                         size_t threshold, int strong );

static int l_mbedtls_entropy_gather(lua_State *L) {

	mbedtls_entropy_context *ctx = (mbedtls_entropy_context *) luaL_checkudata(L, 1, CLASS_NAME);
	lua_pushinteger(L, mbedtls_entropy_gather(ctx));
	return 1;
}

// int mbedtls_entropy_func( void *data, unsigned char *output, size_t len );

static int l_mbedtls_entropy_update_manual(lua_State *L) {

	mbedtls_entropy_context *ctx = (mbedtls_entropy_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t len;
	const unsigned char *data = (const unsigned char *) luaL_checklstring(L, 2, &len);
	lua_pushinteger(L, mbedtls_entropy_update_manual(ctx, data, len));
	return 1;
}

#if 0
static int l_mbedtls_entropy_update_nv_seed(lua_State *L) {

	mbedtls_entropy_context *ctx = (mbedtls_entropy_context *) luaL_checkudata(L, 1, CLASS_NAME);
	lua_pushinteger(L, mbedtls_entropy_update_nv_seed(ctx));
	return 1;
}
#endif

static int l_mbedtls_entropy_write_seed_file(lua_State *L) {

	mbedtls_entropy_context *ctx = (mbedtls_entropy_context *) luaL_checkudata(L, 1, CLASS_NAME);
	const char *path = luaL_checkstring(L, 2);
	lua_pushinteger(L, mbedtls_entropy_write_seed_file(ctx, path));
	return 1;
}
#if 0
static int l_mbedtls_entropy_update_seed_file(lua_State *L) {

	mbedtls_entropy_context *ctx = (mbedtls_entropy_context *) luaL_checkudata(L, 1, CLASS_NAME);
	const char *path = luaL_checkstring(L, 2);
	lua_pushinteger(L, mbedtls_entropy_update_seed_file(ctx, path));
	return 1;
}
#endif
static int l_gc(lua_State *L) {

	mbedtls_entropy_context *ctx = (mbedtls_entropy_context *) luaL_checkudata(L, 1, CLASS_NAME);
	mbedtls_entropy_free(ctx);
	return 0;
}

static const luaL_Reg methods[] = {
	{ "gather", l_mbedtls_entropy_gather },
	{ "update_manual", l_mbedtls_entropy_update_manual },
#if 0
	{ "update_nv_seed", l_mbedtls_entropy_update_nv_seed },
#endif
	{ "write_seed_file", l_mbedtls_entropy_write_seed_file },
#if 0
	{ "update_seed_file", l_mbedtls_entropy_update_seed_file },
#endif
	{ "__gc", l_gc },
	{ NULL, NULL },
};

LUA_API int luaopen_mbedtls_entropy_core(lua_State * const L) {

	luaL_newclass(L, CLASS_NAME, methods);

	luaL_newlib(L, funcs);
	BIND_CONSTANT(MBEDTLS_ERR_ENTROPY_SOURCE_FAILED);
	BIND_CONSTANT(MBEDTLS_ERR_ENTROPY_MAX_SOURCES);
	BIND_CONSTANT(MBEDTLS_ERR_ENTROPY_NO_SOURCES_DEFINED);
	BIND_CONSTANT(MBEDTLS_ERR_ENTROPY_NO_STRONG_SOURCE);
	BIND_CONSTANT(MBEDTLS_ERR_ENTROPY_FILE_IO_ERROR);
	BIND_CONSTANT(MBEDTLS_ENTROPY_MAX_SOURCES);
	BIND_CONSTANT(MBEDTLS_ENTROPY_BLOCK_SIZE);
	BIND_CONSTANT(MBEDTLS_ENTROPY_MAX_SEED_SIZE);
	BIND_CONSTANT(MBEDTLS_ENTROPY_SOURCE_MANUAL);
	BIND_CONSTANT(MBEDTLS_ENTROPY_MAX_SOURCES);
	BIND_CONSTANT(MBEDTLS_ENTROPY_SOURCE_STRONG);
	BIND_CONSTANT(MBEDTLS_ENTROPY_SOURCE_WEAK);
	return 1;
}
