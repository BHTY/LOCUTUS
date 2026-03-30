/*	Routines for managing the OS/2 arena, which is the virtual memory space where OS/2 data lives.	*/

#ifndef __OS2ARENA_H__
#define __OS2ARENA_H__

#include <Windows.h>

typedef DWORD_PTR FLATPTR;
typedef DWORD BLOCK_INDEX;
#define PTR_ADD(p1, p2)	((DWORD_PTR)((DWORD_PTR)(p1) + (DWORD_PTR)(p2)))
#define BYTES_TO_HUNKS(bytes)	(((bytes) + HUNK_SIZE - 1) / HUNK_SIZE)
#define BI_TO_FLATPTR(blkidx)	((FLATPTR)(blkidx) * HUNK_SIZE)
#define FLATPTR_TO_BI(flatptr)	((BLOCK_INDEX)((flatptr) / HUNK_SIZE))

#define ARENA_SIZE	(512 * 1024 * 1024)
#define HUNK_SIZE	(64 * 1024)

int InitOS2Arena();
PVOID OS2Map(FLATPTR Os2Ptr);
FLATPTR OS2Unmap(PVOID HostPtr);
void OS2Release(FLATPTR Os2Ptr, DWORD Blocks);
void OS2Reserve(FLATPTR Os2Ptr, DWORD Blocks);
FLATPTR OS2Alloc(FLATPTR Base, DWORD Blocks);
DWORD DosFlatToSel(DWORD FlatPtr);
DWORD DosSelToFlat(DWORD SegPtr);

#endif

