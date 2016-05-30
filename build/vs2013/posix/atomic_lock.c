#include "atomic_lock.h"

__declspec(naked) int __sync_fetch_and_sub(int *p, int n) {
	__asm {
		push	ebp
		mov		ebp,esp
		mov		edx,[n]
		mov		eax,[p]
		neg		edx
		lock	xadd [eax],edx
		mov		edx,eax
		pop		ebp
		ret
	}
}

__declspec(naked) int __sync_fetch_and_add(int *p, int n) {
	__asm {
		push	ebp
		mov		ebp,esp
		mov		edx,[n]
		mov		eax,[p]
		lock	xadd [eax],edx
		mov		eax,edx
		pop		ebp
		ret
	}
}

__declspec(naked) int __sync_add_and_fetch(int *p, int n) {
	__asm {
		push	ebp
		mov		ebp,esp
		mov     ecx,[n]
		mov     edx,[p]
		mov		eax,ecx
		lock	xadd [edx],eax
		add		eax,ecx
		pop		ebp
		ret
	}
}

__declspec(naked) int __sync_sub_and_fetch(int *p, int n) {
	__asm {
		push	ebp
		mov		ebp,esp
		mov		eax,[n]
		mov		edx,[p]
		neg		eax
		mov		ecx,eax
		mov		eax,ecx
		lock	xadd [edx],eax
		add		eax,ecx
		pop		ebp
		ret
	}
}

__declspec(naked) int __sync_lock_test_and_set(int *p, int n) {
	__asm {
		push	ebp
		mov		ebp,esp
		mov		edx,[n]
		mov		eax,[p]
		xchg	[eax],edx
		mov		eax,edx
		pop		ebp
		ret
	}
}

__declspec(naked) void __sync_lock_release(int *p) {
	__asm {
		push	ebp
		mov		ebp,esp
		mov		eax,[p]
		mov		edx,0
		mov		[eax],edx
		nop
		pop		ebp
		ret
	}
}

__declspec(naked) void __sync_synchronize() {
	__asm {
		push	ebp
		mov		ebp,esp
		lock	or [esp],0
		pop		ebp
		ret
	}
}

__declspec(naked) char __sync_bool_compare_and_swap(int *p, int value, int compare) {
	__asm {
		push	ebp
		mov		ebp,esp
		mov		ecx,[compare]
		mov		eax,[value]
		mov		edx,[p]
		lock	cmpxchg [edx],ecx
		sete	al
		movzx	eax,al
		pop		ebp
		ret
	}
}

__declspec(naked) int __sync_and_and_fetch(int *p, int n) {
	__asm {
		push	ebp
		mov		ebp,esp
		push	esi
		push	ebx
		mov		esi,[n]
		mov		edx,[p]
		mov		eax,[edx]
retry:
		mov		ecx,eax
		and		ecx,esi
		mov		ebx,ecx
		lock	cmpxchg [edx],ecx
		sete	cl
		test	cl,cl
		je		retry
		mov		eax,ebx
		pop		ebx
		pop		esi
		pop		ebp
		ret
	}
}
