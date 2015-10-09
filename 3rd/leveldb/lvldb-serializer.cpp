#include "lvldb-serializer.hpp"

int LvLDBSerializer::set_lua_value(int idx) {
	int lua_value_type = lua_type(L, idx);

	write((unsigned char*)&lua_value_type, sizeof(int));

	switch (lua_value_type) {
	case LUA_TBOOLEAN: {
		int int_val = lua_toboolean(L, -1);
		write((unsigned char*)&int_val, MAR_CHR);
		break;
	}
	case LUA_TSTRING: {
		size_t l;
		const char *str_val = lua_tolstring(L, idx, &l);
		write((unsigned char*)str_val, l);
		break;
	}
	case LUA_TNUMBER: {
		lua_Number num_val = lua_tonumber(L, idx);
		write((unsigned char*)&num_val, MAR_I64);
		break;
	}
	default: {
		luaL_argerror(L, idx, "Expecting number, string or boolean");
		return 1; // not executed
	}
	}

	return 0;
}

size_t LvLDBSerializer::buffer_len() {
	return data_len;
}

unsigned char *LvLDBSerializer::buffer_data() {
	return data;
}

extern "C" {

	int LvLDBSerializer::write(unsigned char *str, size_t len) {
		if (len > UINT32_MAX)
			luaL_error(L, "buffer too long");

		if (size - head < len) {
			size_t new_size = size << 1;
			size_t cur_head = head;
			while (new_size - cur_head <= len) {
				new_size = new_size << 1;
			}
			if (!(data = (unsigned char*)realloc(data, new_size)))
				luaL_error(L, "Out of memory!");

			size = new_size;
		}

		memcpy(&data[head], str, len);
		head += len;
		data_len += len;

		return 0;
	}
}
