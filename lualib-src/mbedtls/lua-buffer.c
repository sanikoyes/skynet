#include "lua-mbedtls.h"

#define CLASS_NAME "mbedtls_buffer"

#define SIGNATURE 0x00C0FFEE
typedef struct {
	size_t sig;
	char *name;
	size_t sz;
	char ud[1];
} BUF;

void *luaL_checkbuffer(lua_State *L, int idx) {

	BUF *buf = (BUF *) luaL_checkudata(L, idx, CLASS_NAME);
	if(buf->sig != SIGNATURE)
		luaL_argerror(L, idx, "Invalid buffer");
	return buf->ud;
}

void *luaL_checklbuffer(lua_State *L, int idx, size_t *l) {

	BUF *buf = (BUF *) luaL_checkudata(L, idx, CLASS_NAME);
	if(buf->sig != SIGNATURE)
		luaL_argerror(L, idx, "Invalid buffer");
	if(l != NULL)
		*l = buf->sz;
	return buf->ud;
}

static int l_new(lua_State *L) {

	size_t len = luaL_checknumber(L, 1);
	const char *desc = luaL_optstring(L, 2, "buffer");
	size_t sdesc = strlen(desc);

	BUF *buf = (BUF *) lua_newuserdata(L, sizeof(BUF) + len);
	buf->sig = SIGNATURE;
	buf->sz = len;
	buf->name = malloc(sdesc + 1);
	memcpy(buf->name, desc, sdesc);
	buf->name[sdesc] = '\0';

	luaL_getmetatable(L, CLASS_NAME);
	lua_setmetatable(L, -2);

	return 1;
}

static const luaL_Reg funcs[] = {
	{ "buffer", l_new },
	{ NULL, NULL },
};

static int l_from_string(lua_State *L) {

	BUF *buf = (BUF *) luaL_checkudata(L, 1, CLASS_NAME);
	if(buf->sig != SIGNATURE)
		luaL_argerror(L, 1, "Invalid buffer");
	size_t ss;
	const char *s = luaL_checklstring(L, 2, &ss);
	if(ss > buf->sz) {
		char msg[1024];
		sprintf(msg, "str sz(%ld) must less than buf sz(%ld)", ss, buf->sz);
		luaL_argerror(L, 2, msg);
	}
	memcpy(buf->ud, s, ss);
	return 0;
}

static int l_to_string(lua_State *L) {

	BUF *buf = (BUF *) luaL_checkudata(L, 1, CLASS_NAME);
	if(buf->sig != SIGNATURE)
		luaL_argerror(L, 1, "Invalid buffer");
	lua_pushlstring(L, buf->ud, buf->sz);
	return 1;
}

static int l_tostring(lua_State *L) {

	BUF *buf = (BUF *) luaL_checkudata(L, 1, CLASS_NAME);
	if(buf->sig != SIGNATURE)
		luaL_argerror(L, 1, "Invalid buffer");
	lua_pushfstring(L, "%s '%s'(%ld bytes): %p", CLASS_NAME, buf->name, buf->sz, buf);
	return 1;
}

static int l_gc(lua_State *L) {

	BUF *buf = (BUF *) luaL_checkudata(L, 1, CLASS_NAME);
	if(buf->sig != SIGNATURE)
		luaL_argerror(L, 1, "Invalid buffer");
	free(buf->name);
	return 0;
}

static const luaL_Reg methods[] = {
	{ "from_string", l_from_string },
	{ "to_string", l_to_string },
	{ "__tostring", l_tostring },
	{ "__gc", l_gc },
	{ NULL, NULL },
};

void register_buffer(lua_State *L) {

	luaL_setfuncs(L, funcs, 0);
	luaL_newclass(L, CLASS_NAME, methods);
}
