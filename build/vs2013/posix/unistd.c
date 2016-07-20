#include "unistd.h"
#include <assert.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <conio.h>
#include <Windows.h>
#include <WinSock2.h>

static LONGLONG get_cpu_freq() {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
	return freq.QuadPart;
}

pid_t getpid() {
	return GetCurrentProcess();
}

int kill(pid_t pid, int exit_code) {
	return TerminateProcess(pid, exit_code);
}


#define NANOSEC 1000000000
#define MICROSEC 1000000

void usleep(size_t us) {
	if(us > 1000) {
		Sleep(us / 1000);
		return;
	}
	LONGLONG delta = get_cpu_freq() / MICROSEC * us;
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	LONGLONG start = counter.QuadPart;
	for(;;) {
		QueryPerformanceCounter(&counter);
		if(counter.QuadPart - start >= delta)
			return;
	}
}

void sleep(size_t ms) {
	Sleep(ms);
}


int clock_gettime(int what, struct timespec *ti) {

	switch(what) {
	case CLOCK_MONOTONIC:
	case CLOCK_REALTIME:
	case CLOCK_THREAD_CPUTIME_ID: {
		LONGLONG freq = get_cpu_freq();
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		ti->tv_sec = counter.QuadPart / freq;
		ti->tv_nsec = (LONGLONG)(counter.QuadPart / ((double)freq / NANOSEC)) % NANOSEC;
		//ti->tv_nsec *= 1000;
		return 0;
	default:
		__asm int 3;
	}
	break;
	}
	return -1;
}

int flock(int fd, int flag) {
	// Not implemented
	__asm int 3;
}

void sigfillset(int *flag) {
	// Not implemented
}

void sigaction(int flag, struct sigaction *action, int param) {
	// Not implemented
	//__asm int 3;
}

static void
socket_keepalive(int fd) {
	int keepalive = 1;
	int ret = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive));  

	assert(ret != SOCKET_ERROR);
}

int pipe(int fd[2]) {

	int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	srand(time(NULL));
	// use random port(range from 60000 to 60999) to simulate pipe()
	for(;;) {
		int port = 60000 + rand() % 1000;
		sin.sin_port = htons(port);
		if(!bind(listen_fd, (struct sockaddr*)&sin, sizeof(sin)))
			break;
	}

	listen(listen_fd, 5);
	printf("Windows sim pipe() listen at %s:%d\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));

	socket_keepalive(listen_fd);

	int client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(connect(client_fd, (struct sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		closesocket(listen_fd);
		return -1;
	}

    struct sockaddr_in client_addr;
	size_t name_len = sizeof(client_addr);
	int client_sock = accept(listen_fd, (struct sockaddr*)&client_addr, &name_len);
	//FD_SET( clientSock, &g_fdClientSock);

	// TODO: close listen_fd

	fd[0] = client_sock;
	fd[1] = client_fd;

	socket_keepalive(client_sock);
	socket_keepalive(client_fd);

	return 0;

	////HANDLE hReadPipe, hWritePipe;
	//SECURITY_ATTRIBUTES sa;
	//sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	//sa.lpSecurityDescriptor = NULL;
	//sa.bInheritHandle = TRUE;
	//if(CreatePipe(&fd[0],&fd[1],&sa,0))
	//	return 0;
	//return -1;
}

int write(int fd, const void *ptr, size_t sz) {

	WSABUF vecs[1];
	vecs[0].buf = ptr;
	vecs[0].len = sz;

    DWORD bytesSent;
    if(WSASend(fd, vecs, 1, &bytesSent, 0, NULL, NULL))
        return -1;
    else
        return bytesSent;
	//DWORD writed = 0;
	//if(WriteFile(fd, ptr, sz, &writed, NULL) == TRUE)
	//	return writed;
	//return -1;
}

int read(int fd, void *buffer, size_t sz) {

	// read console input
	if(fd == 0) {
		char *buf = (char *) buffer;
		while(buf - (char *) buffer < sz) {

			if(!_kbhit())
				break;
			char ch = _getch();
			*buf++ = ch;
			_putch(ch);
			if(ch == '\r') {
				if(buf - (char *) buffer >= sz)
					break;
				*buf++ = '\n';
				_putch('\n');
			}
		}
		return buf - (char *) buffer;
	}

	WSABUF vecs[1];
	vecs[0].buf = buffer;
	vecs[0].len = sz;

    DWORD bytesRecv = 0;
    DWORD flags = 0;
    if(WSARecv(fd, vecs, 1, &bytesRecv, &flags, NULL, NULL)) {
		if(WSAGetLastError() == WSAECONNRESET)
			return 0;
        return -1;
	} else
        return bytesRecv;
	//DWORD read = 0;
	//if(ReadFile(fd, buffer, sz, &read, NULL) == TRUE)
	//	return read;
	//return -1;
}

int close(int fd) {
	shutdown(fd, SD_BOTH);
	return closesocket(fd);
}

int daemon(int a, int b) {
	// Not implemented
	__asm int 3;
	return 0;
}

char *strsep(char **stringp, const char *delim)
{
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;
    if ((s = *stringp)== NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc =*spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}