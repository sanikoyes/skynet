#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include <cstring>
#include <stdlib.h>
#include <stdint.h>
#include "lua.hpp"

#define MAR_CHR 1
#define MAR_I32 4
#define MAR_I64 8

class LvLDBSerializer {
public:
	LvLDBSerializer(lua_State *L) : size(128), seek(0), head(0), data_len(0) {
		this->L = L;

		if (!(data = (unsigned char*)malloc(size)))
			luaL_error(L, "Out of memory!");
	}

	~LvLDBSerializer() {
		free(data);
	}

	int set_lua_value(int idx);

	size_t buffer_len();
	unsigned char * buffer_data();

private:
	lua_State *L;
	size_t size;
	size_t seek;
	size_t head;
	size_t data_len;
	unsigned char *data;

	int write(unsigned char *str, size_t len);
};

#endif /* BUFFER_HPP_ */
