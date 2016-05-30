#include "skynet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

struct logger {
	FILE * handle;
	int close;
};

struct logger *
logger_create(void) {
	struct logger * inst = skynet_malloc(sizeof(*inst));
	inst->handle = NULL;
	inst->close = 0;
	return inst;
}

void
logger_release(struct logger * inst) {
	if (inst->close) {
		fclose(inst->handle);
	}
	skynet_free(inst);
}

static int
_logger(struct skynet_context * context, void *ud, int type, int session, uint32_t source, const void * msg, size_t sz) {
	struct logger * inst = ud;
#ifdef _MSC_VER
	fwprintf(inst->handle, L"[:%08x] ",source);
	int wlen = MultiByteToWideChar(CP_UTF8,0,msg,sz,NULL,0);
	wchar_t *wbuf = (wchar_t*)malloc((sz+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8,0,msg,sz,wbuf,wlen);
	wbuf[wlen]=0;
	fwprintf(inst->handle,L"%s\n",wbuf);

	if(inst->handle != stdout)
		fwprintf(stdout, L"%s\n", wbuf);

	free(wbuf);
#else
	fprintf(inst->handle, "[:%08x] ",source);
	fwrite(msg, sz , 1, inst->handle);
	fprintf(inst->handle, "\n");
#endif
	fflush(inst->handle);

	return 0;
}

int
logger_init(struct logger * inst, struct skynet_context *ctx, const char * parm) {
	if (parm) {
		inst->handle = fopen(parm,"w");
		if (inst->handle == NULL) {
			return 1;
		}
		inst->close = 1;
	} else {
		inst->handle = stdout;
	}
#ifdef _MSC_VER
	_wsetlocale(0, L"chs");
#endif
	if (inst->handle) {
		skynet_callback(ctx, inst, _logger);
		skynet_command(ctx, "REG", ".logger");
		return 0;
	}
	return 1;
}
