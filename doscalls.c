#include <Windows.h>
#include <stdio.h>

#define OS2_APIRET ULONG
#define OS2_HFILE ULONG

OS2_APIRET DosPutMessage(OS2_HFILE hFile, ULONG cbMsg, PCHAR pBuf) {
	return !(fwrite(pBuf, 1, cbMsg, __acrt_iob_func(hFile)) == cbMsg);
}



