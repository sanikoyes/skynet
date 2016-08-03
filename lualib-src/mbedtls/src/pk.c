#include "lua-mbedtls.h"
#include <mbedtls/pk.h>
#include <mbedtls/ctr_drbg.h>

#define CLASS_NAME "mbedtls_pk_context"

static int l_context(lua_State *L) {

	mbedtls_pk_context *ctx = lua_newuserdata(L, sizeof(mbedtls_pk_context));
	mbedtls_pk_init(ctx);

	luaL_getmetatable(L, CLASS_NAME);
	lua_setmetatable(L, -2);

	return 1;
}

static const luaL_Reg funcs[] = {
	{ "context", l_context },
	{ NULL, NULL },
};

// static inline mbedtls_rsa_context *mbedtls_pk_rsa( const mbedtls_pk_context pk )
// static inline mbedtls_ecp_keypair *mbedtls_pk_ec( const mbedtls_pk_context pk )
// const mbedtls_pk_info_t *mbedtls_pk_info_from_type( mbedtls_pk_type_t pk_type );
// int mbedtls_pk_setup( mbedtls_pk_context *ctx, const mbedtls_pk_info_t *info );
// int mbedtls_pk_setup_rsa_alt( mbedtls_pk_context *ctx, void * key,
//                          mbedtls_pk_rsa_alt_decrypt_func decrypt_func,
//                          mbedtls_pk_rsa_alt_sign_func sign_func,
//                          mbedtls_pk_rsa_alt_key_len_func key_len_func );
// size_t mbedtls_pk_get_bitlen( const mbedtls_pk_context *ctx );
// static inline size_t mbedtls_pk_get_len( const mbedtls_pk_context *ctx )
// int mbedtls_pk_can_do( const mbedtls_pk_context *ctx, mbedtls_pk_type_t type );
// int mbedtls_pk_verify( mbedtls_pk_context *ctx, mbedtls_md_type_t md_alg,
//                const unsigned char *hash, size_t hash_len,
//                const unsigned char *sig, size_t sig_len );
// int mbedtls_pk_verify_ext( mbedtls_pk_type_t type, const void *options,
//                    mbedtls_pk_context *ctx, mbedtls_md_type_t md_alg,
//                    const unsigned char *hash, size_t hash_len,
//                    const unsigned char *sig, size_t sig_len );

// int mbedtls_pk_sign( mbedtls_pk_context *ctx, mbedtls_md_type_t md_alg,
//              const unsigned char *hash, size_t hash_len,
//              unsigned char *sig, size_t *sig_len,
//              int (*f_rng)(void *, unsigned char *, size_t), void *p_rng );
static int l_mbedtls_pk_sign(lua_State *L) {

	mbedtls_pk_context *ctx = (mbedtls_pk_context *) luaL_checkudata(L, 1, CLASS_NAME);
	int md_alg = luaL_checkinteger(L, 2);
	size_t hash_len;
	const unsigned char *hash = (const unsigned char *) luaL_checklstring(L, 3, &hash_len);
	mbedtls_ctr_drbg_context *ctr_drbg = (mbedtls_ctr_drbg_context *) luaL_checkudata(L, 4, "mbedtls_ctr_drbg_context");

	// TODO:
	int (*f_rng)(void *, unsigned char *, size_t) = mbedtls_ctr_drbg_random;

	unsigned char sig[1024];
	size_t sig_len;
	lua_pushinteger(L, mbedtls_pk_sign(ctx, md_alg, hash, hash_len, sig, &sig_len, f_rng, ctr_drbg));
	lua_pushlstring(L, (const char *) sig, sig_len);
	return 2;
}
// int mbedtls_pk_decrypt( mbedtls_pk_context *ctx,
//                 const unsigned char *input, size_t ilen,
//                 unsigned char *output, size_t *olen, size_t osize,
//                 int (*f_rng)(void *, unsigned char *, size_t), void *p_rng );
// int mbedtls_pk_encrypt( mbedtls_pk_context *ctx,
//                 const unsigned char *input, size_t ilen,
//                 unsigned char *output, size_t *olen, size_t osize,
//                 int (*f_rng)(void *, unsigned char *, size_t), void *p_rng );
// int mbedtls_pk_check_pair( const mbedtls_pk_context *pub, const mbedtls_pk_context *prv );
// int mbedtls_pk_debug( const mbedtls_pk_context *ctx, mbedtls_pk_debug_item *items );
// const char * mbedtls_pk_get_name( const mbedtls_pk_context *ctx );
// mbedtls_pk_type_t mbedtls_pk_get_type( const mbedtls_pk_context *ctx );

static int l_mbedtls_pk_parse_key(lua_State *L) {

	mbedtls_pk_context *ctx = (mbedtls_pk_context *) luaL_checkudata(L, 1, CLASS_NAME);
	size_t keylen;
	const unsigned char *key = (const unsigned char *) luaL_checklstring(L, 2, &keylen);
	size_t pwdlen;
	const unsigned char *pwd = (const unsigned char *) luaL_checklstring(L, 3, &pwdlen);
	lua_pushinteger(L, mbedtls_pk_parse_key(ctx, key, keylen, pwd, pwdlen));
	return 1;
}

// int mbedtls_pk_parse_public_key( mbedtls_pk_context *ctx,
//                          const unsigned char *key, size_t keylen );
// int mbedtls_pk_parse_keyfile( mbedtls_pk_context *ctx,
//                       const char *path, const char *password );
// int mbedtls_pk_parse_public_keyfile( mbedtls_pk_context *ctx, const char *path );
// int mbedtls_pk_write_key_der( mbedtls_pk_context *ctx, unsigned char *buf, size_t size );
// int mbedtls_pk_write_pubkey_der( mbedtls_pk_context *ctx, unsigned char *buf, size_t size );
// int mbedtls_pk_write_pubkey_pem( mbedtls_pk_context *ctx, unsigned char *buf, size_t size );
// int mbedtls_pk_write_key_pem( mbedtls_pk_context *ctx, unsigned char *buf, size_t size );
// int mbedtls_pk_parse_subpubkey( unsigned char **p, const unsigned char *end,
//                         mbedtls_pk_context *pk );
// int mbedtls_pk_write_pubkey( unsigned char **p, unsigned char *start,
//                      const mbedtls_pk_context *key );
// int mbedtls_pk_load_file( const char *path, unsigned char **buf, size_t *n );

static int l_gc(lua_State *L) {

	mbedtls_pk_context *ctx = (mbedtls_pk_context *) luaL_checkudata(L, 1, CLASS_NAME);
	mbedtls_pk_free(ctx);
	return 0;
}

static const luaL_Reg methods[] = {
	{ "parse_key", l_mbedtls_pk_parse_key },
	{ "sign", l_mbedtls_pk_sign },
	{ "__gc", l_gc },
	{ NULL, NULL },
};

LUA_API int luaopen_mbedtls_pk_core(lua_State * const L) {

	luaL_newclass(L, CLASS_NAME, methods);

	luaL_newlib(L, funcs);
	BIND_CONSTANT(MBEDTLS_ERR_PK_ALLOC_FAILED);
	BIND_CONSTANT(MBEDTLS_ERR_PK_TYPE_MISMATCH);
	BIND_CONSTANT(MBEDTLS_ERR_PK_BAD_INPUT_DATA);
	BIND_CONSTANT(MBEDTLS_ERR_PK_FILE_IO_ERROR);
	BIND_CONSTANT(MBEDTLS_ERR_PK_KEY_INVALID_VERSION);
	BIND_CONSTANT(MBEDTLS_ERR_PK_KEY_INVALID_FORMAT);
	BIND_CONSTANT(MBEDTLS_ERR_PK_UNKNOWN_PK_ALG);
	BIND_CONSTANT(MBEDTLS_ERR_PK_PASSWORD_REQUIRED);
	BIND_CONSTANT(MBEDTLS_ERR_PK_PASSWORD_MISMATCH);
	BIND_CONSTANT(MBEDTLS_ERR_PK_INVALID_PUBKEY);
	BIND_CONSTANT(MBEDTLS_ERR_PK_INVALID_ALG);
	BIND_CONSTANT(MBEDTLS_ERR_PK_UNKNOWN_NAMED_CURVE);
	BIND_CONSTANT(MBEDTLS_ERR_PK_FEATURE_UNAVAILABLE);
	BIND_CONSTANT(MBEDTLS_ERR_PK_SIG_LEN_MISMATCH);

	BIND_CONSTANT(MBEDTLS_PK_NONE);
	BIND_CONSTANT(MBEDTLS_PK_RSA);
	BIND_CONSTANT(MBEDTLS_PK_ECKEY);
	BIND_CONSTANT(MBEDTLS_PK_ECKEY_DH);
	BIND_CONSTANT(MBEDTLS_PK_ECDSA);
	BIND_CONSTANT(MBEDTLS_PK_RSA_ALT);
	BIND_CONSTANT(MBEDTLS_PK_RSASSA_PSS);

    return 1;
}
