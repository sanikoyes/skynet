#include "lua-mbedtls.h"
#include <mbedtls/md.h>

#define CLASS_NAME "mbedtls_md_context_t"

static const mbedtls_md_info_t *check_md_info_t(lua_State *L, int idx) {

	const mbedtls_md_info_t *info = NULL;
	int type = lua_type(L, idx);
	if(type == LUA_TSTRING)
		info = mbedtls_md_info_from_string(lua_tostring(L, idx));
	else if(type == LUA_TNUMBER)
		info = mbedtls_md_info_from_type(lua_tonumber(L, idx));
	if(info == NULL)
		luaL_argerror(L, idx, "must be integer[0-9] or string[MD2,MD4,MD5,RIPEMD160,SHA1,SHA,SHA224,SHA256,SHA384,SHA512]");
	return info;
}

// int mbedtls_md_hmac( const mbedtls_md_info_t *md_info, const unsigned char *key, size_t keylen,
//                 const unsigned char *input, size_t ilen,
//                 unsigned char *output );
// static int l_mbedtls_md_hmac(lua_State *L) {

// unsigned char mbedtls_md_get_size( const mbedtls_md_info_t *md_info );
// mbedtls_md_type_t mbedtls_md_get_type( const mbedtls_md_info_t *md_info );
// const char *mbedtls_md_get_name( const mbedtls_md_info_t *md_info );
// int mbedtls_md( const mbedtls_md_info_t *md_info, const unsigned char *input, size_t ilen,
//         unsigned char *output );
// int mbedtls_md_file( const mbedtls_md_info_t *md_info, const char *path,
//                      unsigned char *output );

// 	const mbedtls_md_info_t *md_info = check_md_info_t(L, 1);
// 	size_t keylen;
// 	const unsigned char *key = luaL_checklstring(L, 2, &keylen);
// 	size_t ilen;
// 	const unsigned char *input = luaL_checklstring(L, 3, &ilen);
// }

static int l_context(lua_State *L) {

	mbedtls_md_context_t *ctx = lua_newuserdata(L, sizeof(mbedtls_md_context_t));
	mbedtls_md_init(ctx);

	luaL_getmetatable(L, CLASS_NAME);
	lua_setmetatable(L, -2);

	return 1;
}

static const luaL_Reg funcs[] = {
	{ "context", l_context },
	{ NULL, NULL },
};

// const int *mbedtls_md_list( void );
// const mbedtls_md_info_t *mbedtls_md_info_from_string( const char *md_name );
// const mbedtls_md_info_t *mbedtls_md_info_from_type( mbedtls_md_type_t md_type );

static int l_mbedtls_md_init_ctx(lua_State *L) {

	mbedtls_md_context_t *ctx = (mbedtls_md_context_t *) luaL_checkudata(L, 1, CLASS_NAME);
	const mbedtls_md_info_t *md_info = check_md_info_t(L, 2);
	lua_pushinteger(L, mbedtls_md_init_ctx(ctx, md_info));
	return 1;	
}

static int l_mbedtls_md_setup(lua_State *L) {

	mbedtls_md_context_t *ctx = (mbedtls_md_context_t *) luaL_checkudata(L, 1, CLASS_NAME);
	const mbedtls_md_info_t *md_info = check_md_info_t(L, 2);
	int hmac = luaL_checkinteger(L, 3);
	lua_pushinteger(L, mbedtls_md_setup(ctx, md_info, hmac));
	return 1;
}
// int mbedtls_md_clone( mbedtls_md_context_t *dst,
//                       const mbedtls_md_context_t *src );

static int l_mbedtls_md_starts(lua_State *L) {

	mbedtls_md_context_t *ctx = (mbedtls_md_context_t *) luaL_checkudata(L, 1, CLASS_NAME);
	lua_pushinteger(L, mbedtls_md_starts(ctx));
	return 1;
}

static int l_mbedtls_md_update(lua_State *L) {

	mbedtls_md_context_t *ctx = (mbedtls_md_context_t *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t ilen;
	const unsigned char *input = (const unsigned char *) luaL_checklstring(L, 2, &ilen);
	lua_pushinteger(L, mbedtls_md_update(ctx, input, ilen));
	return 1;
}

static int l_mbedtls_md_finish(lua_State *L) {

	mbedtls_md_context_t *ctx = (mbedtls_md_context_t *) luaL_checkudata(L, 1, CLASS_NAME);

	unsigned char output[64];
	lua_pushinteger(L, mbedtls_md_finish(ctx, output));
	lua_pushlstring(L, (const char *) output, sizeof(output));
	return 2;
}

// int mbedtls_md_hmac_starts( mbedtls_md_context_t *ctx, const unsigned char *key,
//                     size_t keylen );
// int mbedtls_md_hmac_update( mbedtls_md_context_t *ctx, const unsigned char *input,
//                     size_t ilen );
// int mbedtls_md_hmac_finish( mbedtls_md_context_t *ctx, unsigned char *output);
// int mbedtls_md_hmac_reset( mbedtls_md_context_t *ctx );

static int l_gc(lua_State *L) {

	mbedtls_md_context_t *ctx = (mbedtls_md_context_t *) luaL_checkudata(L, 1, CLASS_NAME);
	mbedtls_md_free(ctx);
	return 0;
}

static const luaL_Reg methods[] = {
	{ "init_ctx", l_mbedtls_md_init_ctx },
	{ "setup", l_mbedtls_md_setup },
	{ "starts", l_mbedtls_md_starts },
	{ "update", l_mbedtls_md_update },
	{ "finish", l_mbedtls_md_finish },
	{ "__gc", l_gc },
	{ NULL, NULL },
};

LUA_API int luaopen_mbedtls_md_core(lua_State * const L) {

	luaL_newclass(L, CLASS_NAME, methods);

	luaL_newlib(L, funcs);
	BIND_CONSTANT(MBEDTLS_ERR_MD_FEATURE_UNAVAILABLE);
	BIND_CONSTANT(MBEDTLS_ERR_MD_BAD_INPUT_DATA);
	BIND_CONSTANT(MBEDTLS_ERR_MD_ALLOC_FAILED);
	BIND_CONSTANT(MBEDTLS_ERR_MD_FILE_IO_ERROR);

	BIND_CONSTANT(MBEDTLS_MD_NONE);
	BIND_CONSTANT(MBEDTLS_MD_MD2);
	BIND_CONSTANT(MBEDTLS_MD_MD4);
	BIND_CONSTANT(MBEDTLS_MD_MD5);
	BIND_CONSTANT(MBEDTLS_MD_SHA1);
	BIND_CONSTANT(MBEDTLS_MD_SHA224);
	BIND_CONSTANT(MBEDTLS_MD_SHA256);
	BIND_CONSTANT(MBEDTLS_MD_SHA384);
	BIND_CONSTANT(MBEDTLS_MD_SHA512);
	BIND_CONSTANT(MBEDTLS_MD_RIPEMD160);
	BIND_CONSTANT(MBEDTLS_MD_MAX_SIZE);
    return 1;
}
