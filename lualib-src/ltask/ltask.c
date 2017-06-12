#include "simplelock.h"
#include "simplethread.h"
#include "schedule.h"
#include "serialize.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>

// Only support one schedule per process, never release it after create
static struct schedule *S = NULL;

static int
linit(lua_State *L) {
	if (S)
		return luaL_error(L, "already init");
	S= schedule_create(luaL_checkinteger(L, 1));
	return 0;
}

static inline void
check_schedule(lua_State *L) {
	if (S==NULL)
		luaL_error(L, "init first");
}

struct ltask {
	lua_State *L;
	taskid id;
};

static int
inittask(lua_State *L) {
	luaL_openlibs(L);
	const char *filename = (const char *)lua_touserdata(L,1);
	int err = luaL_loadfile(L, filename);
	if (err != LUA_OK)
		lua_error(L);
	return 1;
}

static int
ltask(lua_State *L) {
	lua_State *task = luaL_newstate();
	if (task == NULL)
		return luaL_error(L, "luaL_newstate failed");
	const char * filename = luaL_checkstring(L, 1);
	lua_pushcfunction(task, inittask);
	lua_pushlightuserdata(task, (void *)filename);
	int err = lua_pcall(task, 1, 1, 0);
	if (err != LUA_OK) {
		size_t sz;
		const char * msg = lua_tolstring(task, -1, &sz);
		if (msg) {
			lua_pushlstring(L, msg, sz);
			lua_close(task);
			lua_error(L);
		} else {
			lua_close(task);
			return luaL_error(L, "fail to new task %s", filename);
		}
	}
	lua_pushcfunction(L, seri_pack);
	lua_insert(L, 2);
	int top = lua_gettop(L);
	lua_call(L, top-2, 1);
	void * args = lua_touserdata(L, 2);
	lua_pushcfunction(task, seri_unpack);
	lua_pushlightuserdata(task, args);
	err = lua_pcall(task, 1, LUA_MULTRET, 0);
	if (err != LUA_OK) {
		lua_close(task);
		return luaL_error(L, "pass argments to new task %s", filename);
	}

	check_schedule(L);
	struct ltask *t = malloc(sizeof(*t));
	t->L = task;
	t->id = 0;
	taskid id = schedule_opentask(S, t);
	t->id = id;
	lua_pushinteger(task, id);
	lua_rawsetp(task, LUA_REGISTRYINDEX, S);
	lua_pushinteger(L, id);

	return 1;
}

struct workshop;

struct worker {
	struct workshop * workshop;
	struct thread_event event;
	int id;
	int suspend;
};

struct workshop {
	int threads;
	struct worker * w;
};

static int
run_slice(struct worker *w) {
	struct schedule *s = S;
	struct ltask *t = schedule_grabtask(s, w->id);
	if (t == NULL)
		return 0;
	lua_State *L = t->L;
	int args = lua_gettop(L) - 1;
	int ret = lua_resume(L, NULL, args);
	if (ret == LUA_YIELD) {
		lua_settop(L, 0);
		schedule_releasetask(s, t->id);
		return 1;
	}
	if (ret != LUA_OK) {
		// todo: call luaL_traceback
		printf("error: %s\n", lua_tostring(L, -1));
	}
	t->L = NULL;
	lua_close(L);
	schedule_closetask(s, t->id);
	schedule_releasetask(s, t->id);
	return 1;
}

static void
worker_func(void *p) {
	struct worker *w = p;
	for (;;) {
		w->suspend = 0;
		while (run_slice(w)) {
			struct workshop * ws = w->workshop;
			int i;
			for (i=0;i<ws->threads;i++) {
				struct worker *w = &ws->w[i];
				if (w->suspend) {
					thread_event_trigger(&w->event);
				}
			}
		}
		w->suspend = 1;
		thread_event_wait(&w->event);
	}
}

static int
lrun(lua_State *L) {
	check_schedule(L);
	int threads = schedule_threads(S);
#ifdef _MSC_VER
	struct worker *w = malloc(threads * sizeof(struct worker));
	struct thread *t = malloc(threads * sizeof(struct thread));
#else
	struct worker w[threads];
	struct thread t[threads];
#endif
	struct workshop ws;
	ws.threads = threads;
	ws.w = w;
	int i;
	for (i=0;i<threads;i++) {
		w[i].id = i;
		w[i].suspend = 0;
		w[i].workshop = &ws;
		thread_event_create(&w[i].event);
		t[i].func = worker_func;
		t[i].ud = &w[i];
	}
	thread_join(t,threads);
	for (i=0;i<threads;i++) {
		thread_event_release(&w[i].event);
	}
#ifdef _MSC_VER
	free(w);
	free(t);
#endif
	return 0;
}

static int
lchannel(lua_State *L) {
	check_schedule(L);
	channelid id = schedule_newchannel(S);
	lua_pushinteger(L, id);
	return 1;
}

static int
lsend(lua_State *L) {
	channelid id = luaL_checkinteger(L,1);
	lua_pushcfunction(L, seri_pack);
	lua_replace(L, 1);
	int top = lua_gettop(L);
	lua_call(L, top-1, 1);
	void * msg = lua_touserdata(L, 1);
	schedule_send(S, id, msg);
	return 0;
}

static int
lselect(lua_State *L) {
	lua_rawgetp(L, LUA_REGISTRYINDEX, S);
	if (lua_type(L, -1) != LUA_TNUMBER)
		return luaL_error(L, "select should call in a task");
	taskid id = lua_tointeger(L, -1);
	lua_pop(L, 1);
	int n = lua_gettop(L);
#ifdef _MSC_VER
	channelid *channels = malloc(n * sizeof(channelid));
#else
	channelid channels[n];
#endif
	int i;
	for (i=0;i<n;i++) {
		channels[i] = luaL_checkinteger(L, i+1);
	}
	channelid c = schedule_select(S, id, n, channels);
	if (c == 0) {
#ifdef _MSC_VER
		free(channels);
#endif
		return 0;
	}
	lua_pushinteger(L, c);
#ifdef _MSC_VER
	free(channels);
#endif
	return 1;
}

static int
lrecv(lua_State *L) {
	int c = luaL_checkinteger(L, 1);
	void * msg = schedule_recv(S, c);
	if (msg == NULL) {
		if (schedule_isclosed(S, c))
			return 0;
		lua_pushboolean(L, 0);
		return 1;
	}
	lua_settop(L, 0);
	lua_pushboolean(L, 1);
	lua_pushcfunction(L, seri_unpack);
	lua_pushlightuserdata(L, msg);
	lua_call(L, 1, LUA_MULTRET);
	return lua_gettop(L);
}

static int
ltaskid(lua_State *L) {
	lua_rawgetp(L, LUA_REGISTRYINDEX, S);
	return 1;
}

static int
lclosechannel(lua_State *L) {
	channelid c = luaL_checkinteger(L,1);
	schedule_closechannel(S,c);
	return 0;
}

int
luaopen_skynet_ltask(lua_State *L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "init", linit },
		{ "task", ltask },
		{ "channel", lchannel },
		{ "send", lsend },
		{ "recv", lrecv },
		{ "select", lselect },
		{ "taskid", ltaskid },
		{ "close", lclosechannel },
		{ "run", lrun },
		{ NULL, NULL },
	};
	luaL_newlib(L,l);
	return 1;
}
