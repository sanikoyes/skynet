#include "lua-mbedtls.h"
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>

#define CLASS_NAME "mbedtls_ctr_drbg_context"

static int l_context(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = lua_newuserdata(L, sizeof(mbedtls_ctr_drbg_context));
	mbedtls_ctr_drbg_init(ctx);

	luaL_getmetatable(L, CLASS_NAME);
	lua_setmetatable(L, -2);

	return 1;
}

static const luaL_Reg funcs[] = {
	{ "context", l_context },
	{ NULL, NULL },
};

static int l_mbedtls_ctr_drbg_seed(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	mbedtls_entropy_context *entropy = (mbedtls_entropy_context *) luaL_checkudata(L, 2, "mbedtls_entropy_context");
	size_t len;
	const unsigned char *custom = (const unsigned char *) luaL_checklstring(L, 3, &len);

	lua_pushinteger(L, mbedtls_ctr_drbg_seed(ctx, mbedtls_entropy_func, entropy, custom, len));
	return 1;
}

static int l_mbedtls_ctr_drbg_set_prediction_resistance(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	int resistance = luaL_checkinteger(L, 2);
	mbedtls_ctr_drbg_set_prediction_resistance(ctx, resistance);
	return 0;
}

static int l_mbedtls_ctr_drbg_set_entropy_len(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t len = luaL_checknumber(L, 2);
	mbedtls_ctr_drbg_set_entropy_len(ctx, len);
	return 0;
}

static int l_mbedtls_ctr_drbg_set_reseed_interval(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t interval = luaL_checknumber(L, 2);
	mbedtls_ctr_drbg_set_reseed_interval(ctx, interval);
	return 0;
}

static int l_mbedtls_ctr_drbg_reseed(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t len;
	const unsigned char *additional = (const unsigned char *) luaL_checklstring(L, 2, &len);
	lua_pushinteger(L, mbedtls_ctr_drbg_reseed(ctx, additional, len));
	return 1;
}

static int l_mbedtls_ctr_drbg_update(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t add_len;
	const unsigned char *additional = (const unsigned char *) luaL_checklstring(L, 2, &add_len);
	mbedtls_ctr_drbg_update(ctx, additional, add_len);
	return 0;
}

static int l_mbedtls_ctr_drbg_random_with_add(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t output_len;
	unsigned char *output = luaL_checklbuffer(L, 2, &output_len);
	size_t add_len;
	const unsigned char *additional = (const unsigned char *) luaL_checklstring(L, 3, &add_len);
	lua_pushinteger(L, mbedtls_ctr_drbg_random_with_add(ctx, output, output_len, additional, add_len));
	return 1;
}

static int l_mbedtls_ctr_drbg_random(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t output_len;
	unsigned char *output = luaL_checklbuffer(L, 2, &output_len);
	lua_pushinteger(L, mbedtls_ctr_drbg_random(ctx, output, output_len));
	return 1;
}

static int l_mbedtls_ctr_drbg_write_seed_file(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	const char *path = luaL_checkstring(L, 2);
	lua_pushinteger(L, mbedtls_ctr_drbg_write_seed_file(ctx, path));
	return 1;
}

static int l_mbedtls_ctr_drbg_update_seed_file(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	const char *path = luaL_checkstring(L, 2);
	lua_pushinteger(L, mbedtls_ctr_drbg_update_seed_file(ctx, path));
	return 1;
}

static int l_gc(lua_State *L) {

	mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 1, CLASS_NAME);
	mbedtls_ctr_drbg_free(ctx);
	return 0;
}

static const luaL_Reg methods[] = {
	{ "seed", l_mbedtls_ctr_drbg_seed },
	{ "set_prediction_resistance", l_mbedtls_ctr_drbg_set_prediction_resistance },
	{ "set_entropy_len", l_mbedtls_ctr_drbg_set_entropy_len },
	{ "set_reseed_interval", l_mbedtls_ctr_drbg_set_reseed_interval },
	{ "reseed", l_mbedtls_ctr_drbg_reseed },
	{ "update", l_mbedtls_ctr_drbg_update },
	{ "random_with_add", l_mbedtls_ctr_drbg_random_with_add },
	{ "random", l_mbedtls_ctr_drbg_random },
	{ "write_seed_file", l_mbedtls_ctr_drbg_write_seed_file },
	{ "update_seed_file", l_mbedtls_ctr_drbg_update_seed_file },
	{ "__gc", l_gc },
	{ NULL, NULL },
};

LUA_API int luaopen_mbedtls_ctr_drbg_core(lua_State * const L) {

	luaL_newclass(L, CLASS_NAME, methods);

	luaL_newlib(L, funcs);
	BIND_CONSTANT(MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED);
	BIND_CONSTANT(MBEDTLS_ERR_CTR_DRBG_REQUEST_TOO_BIG);
	BIND_CONSTANT(MBEDTLS_ERR_CTR_DRBG_INPUT_TOO_BIG);
	BIND_CONSTANT(MBEDTLS_ERR_CTR_DRBG_FILE_IO_ERROR);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_BLOCKSIZE);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_KEYSIZE);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_KEYBITS);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_SEEDLEN);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_ENTROPY_LEN);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_RESEED_INTERVAL);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_MAX_INPUT);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_MAX_REQUEST);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_MAX_SEED_INPUT);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_PR_OFF);
	BIND_CONSTANT(MBEDTLS_CTR_DRBG_PR_ON);
    return 1;
}
