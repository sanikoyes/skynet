#include "lua-mbedtls.h"
#include <mbedtls/aes.h>

#define CLASS_NAME "mbedtls_aes_context"

static int l_context(lua_State *L) {

	mbedtls_aes_context *ctx = lua_newuserdata(L, sizeof(mbedtls_aes_context));
	mbedtls_aes_init(ctx);

	luaL_getmetatable(L, CLASS_NAME);
	lua_setmetatable(L, -2);

	return 1;
}

static const luaL_Reg funcs[] = {
	{ "context", l_context },
	{ NULL, NULL },
};

static int l_mbedtls_aes_setkey_enc(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	const unsigned char *key = (const unsigned char *) luaL_checkstring(L, 2);
	unsigned int keybits = luaL_checkinteger(L, 3);

	lua_pushinteger(L, mbedtls_aes_setkey_enc(ctx, key, keybits));
	return 1;
}

static int l_mbedtls_aes_setkey_dec(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	const unsigned char *key = (const unsigned char *) luaL_checkstring(L, 2);
	unsigned int keybits = luaL_checkinteger(L, 3);

	lua_pushinteger(L, mbedtls_aes_setkey_dec(ctx, key, keybits));
	return 1;
}

static int l_mbedtls_aes_crypt_ecb(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	int mode = luaL_checkinteger(L, 2);
	size_t sinput;
	const unsigned char *input = (const unsigned char *) luaL_checklstring(L, 3, &sinput);
	if(sinput != 16)
		luaL_argerror(L, 3, "input length must be 16 bytes");

	unsigned char output[16];
	lua_pushinteger(L, mbedtls_aes_crypt_ecb(ctx, mode, input, output));
	lua_pushlstring(L, (const char *) output, sizeof(output));
	return 2;
}

static int l_mbedtls_aes_crypt_cbc(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	int mode = luaL_checkinteger(L, 2);
	size_t length = (size_t) luaL_checknumber(L, 3);
	size_t siv;
	unsigned char *iv = luaL_checklbuffer(L, 4, &siv);
	if(siv != 16)
		luaL_argerror(L, 4, "iv length must be 16 bytes");
	const unsigned char *input = (const unsigned char *) luaL_checkstring(L, 5);
	unsigned char *output = (unsigned char *) malloc(length);
	if(output == NULL)
		return 0;
	lua_pushinteger(L, mbedtls_aes_crypt_cbc(ctx, mode, length, iv, input, output));
	lua_pushlstring(L, (const char *) output, length);
	free(output);
	return 2;
}

static int l_mbedtls_aes_crypt_cfb128(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	int mode = luaL_checkinteger(L, 2);
	size_t length = (size_t) luaL_checknumber(L, 3);
	size_t iv_off = (size_t) luaL_checknumber(L, 4);
	size_t siv;
	unsigned char *iv = luaL_checklbuffer(L, 5, &siv);
	if(siv != 16)
		luaL_argerror(L, 5, "iv length must be 16 bytes");
	const unsigned char *input = (const unsigned char *) luaL_checkstring(L, 6);
	unsigned char *output = (unsigned char *) malloc(length);
	if(output == NULL)
		return 0;
	lua_pushinteger(L, mbedtls_aes_crypt_cfb128(ctx, mode, length, &iv_off, iv, input, output));
	lua_pushnumber(L, iv_off);
	lua_pushlstring(L, (const char *) output, length);
	free(output);
	return 3;
}

static int l_mbedtls_aes_crypt_cfb8(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	int mode = luaL_checkinteger(L, 2);
	size_t length = (size_t) luaL_checknumber(L, 3);
	size_t siv;
	unsigned char *iv = luaL_checklbuffer(L, 4, &siv);
	if(siv != 16)
		luaL_argerror(L, 4, "iv length must be 16 bytes");
	const unsigned char *input = (const unsigned char *) luaL_checkstring(L, 5);
	unsigned char *output = (unsigned char *) malloc(length);
	if(output == NULL)
		return 0;
	lua_pushinteger(L, mbedtls_aes_crypt_cfb8(ctx, mode, length, iv, input, output));
	lua_pushlstring(L, (const char *) iv, sizeof(iv));
	lua_pushlstring(L, (const char *) output, length);
	free(output);
	return 3;
}

static int l_mbedtls_aes_crypt_ctr(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t length = (size_t) luaL_checknumber(L, 2);
	size_t nc_off = (size_t) luaL_checknumber(L, 3);

	size_t snonce_counter;
	unsigned char *nonce_counter = luaL_checklbuffer(L, 4, &snonce_counter);
	if(snonce_counter != 16)
		luaL_argerror(L, 4, "nonce_counter length must be 16 bytes");

	size_t sstream_block;
	unsigned char *stream_block = luaL_checklbuffer(L, 5, &sstream_block);
	if(sstream_block != 16)
 		luaL_argerror(L, 5, "stream_block length must be 16 bytes");

	const unsigned char *input = (const unsigned char *) luaL_checkstring(L, 6);

	unsigned char *output = (unsigned char *) malloc(length);
	lua_pushinteger(L, mbedtls_aes_crypt_ctr(ctx, length, &nc_off, nonce_counter, stream_block, input, output));
	lua_pushnumber(L, nc_off);
	lua_pushlstring(L, (const char *) output, length);
	return 3;
}

static int l_mbedtls_aes_encrypt(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t sinput;
	const unsigned char *input = (const unsigned char *) luaL_checklstring(L, 2, &sinput);
	if(sinput != 16)
		luaL_argerror(L, 2, "input length must be 16 bytes");
	unsigned char output[16];
	mbedtls_aes_encrypt(ctx, input, output);
	lua_pushlstring(L, (const char *) output, sizeof(output));
	return 1;
}

static int l_mbedtls_aes_decrypt(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t sinput;
	const unsigned char *input = (const unsigned char *) luaL_checklstring(L, 2, &sinput);
	if(sinput != 16)
		luaL_argerror(L, 2, "input length must be 16 bytes");
	unsigned char output[16];
	mbedtls_aes_decrypt(ctx, input, output);
	lua_pushlstring(L, (const char *) output, sizeof(output));
	return 1;
}

static int l_gc(lua_State *L) {

	mbedtls_aes_context *ctx = (mbedtls_aes_context *) luaL_checkudata(L, 1, CLASS_NAME);
	mbedtls_aes_free(ctx);
	return 0;
}

static const luaL_Reg methods[] = {
	{ "setkey_enc", l_mbedtls_aes_setkey_enc },
	{ "setkey_dec", l_mbedtls_aes_setkey_dec },
	{ "crypt_ecb", l_mbedtls_aes_crypt_ecb },
	{ "crypt_cbc", l_mbedtls_aes_crypt_cbc },
	{ "crypt_cfb128", l_mbedtls_aes_crypt_cfb128 },
	{ "crypt_cfb8", l_mbedtls_aes_crypt_cfb8 },
	{ "crypt_ctr", l_mbedtls_aes_crypt_ctr },
	{ "encrypt", l_mbedtls_aes_encrypt },
	{ "decrypt", l_mbedtls_aes_decrypt },
	{ "__gc", l_gc },
	{ NULL, NULL },
};

LUA_API int luaopen_mbedtls_aes_core(lua_State * const L) {

	luaL_newclass(L, CLASS_NAME, methods);

	luaL_newlib(L, funcs);
	BIND_CONSTANT(MBEDTLS_AES_ENCRYPT);
	BIND_CONSTANT(MBEDTLS_AES_DECRYPT);
	BIND_CONSTANT(MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
	BIND_CONSTANT(MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);
    return 1;
}
