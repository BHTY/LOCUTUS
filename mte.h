/*	Module Table List manager for image loader	*/

#ifndef __MTE_H__
#define __MTE_H__

#include <Windows.h>

// https://www.os2museum.com/wp/digging-into-os-2-2-0/
// the image loader needs a mapping of logical segments to real segments

typedef struct _MTE_ENTRY {			/*	Entry point within an MTE	*/
	char Name[128];					/*	Name of the entry point	*/
	WORD Ordinal;					/*	Ordinal of the entry point	*/
	WORD Flags;						/*  Flags for the entry point (e.g. 16-bit vs 32-bit)	*/
	WORD Segment;					/*	Segment number of the entry point	*/
	DWORD Offset;					/*	Offset of the entry point within the segment	*/
} MTE_ENTRY;

typedef struct _MTE {				/*	Module Table Entry	*/
	struct _MTE* Next;				/*	Next MTE in list	*/
	char ModuleName[128];			/*	The module's name	*/
	DWORD ReferenceCount;			/*	Number of references to this module	*/
	DWORD EntryPointCount;			/*	Number of entry points in the module	*/
	MTE_ENTRY EntryPoints[1];		/*	Entry point list	*/
} MTE;

typedef WORD OS2_HMODULE;

MTE* GetMTEByName(char* ModuleName);	/*	Returns the MTE corresponding to the given module name, or NULL if not found.	*/
MTE* GetMTEByHandle(OS2_HMODULE hMod);	/*	Returns the MTE corresponding to the given module handle, or NULL if not found.	*/
void AddMTE(MTE* mte);					/*	Adds a new MTE to the global list of MTEs, called by the image loader when a new module is loaded.	*/
void FreeMTE(MTE* pMTE);				/*	Decrements module reference count and frees if necessary, called by DosFreeModule	*/
MTE_ENTRY* GetEntryByName(MTE* pMTE, char* EntryName);	/*	Returns the entry point corresponding to the given name, or NULL if not found.	*/
MTE_ENTRY* GetEntryByOrdinal(MTE* pMTE, WORD Ordinal);	/*	Returns the entry point corresponding to the given ordinal, or NULL if not found.	*/

extern MTE* g_MTEListHead;					/*	Head of the global MTE list, which is a linked list of MTEs.	*/

// HMODULE (16-bit) <-> MTE* mapping
// Allocate and retrieve and free a mapping

// DosQueryModuleHandle transforms name of loaded module => module handle: calls GetMTEByName and then gets the appropriate handle
// DosQueryModuleName transforms module handle => name of loaded module: calls GetMTEByHandle and then gets the appropriate name
// DosLoadModule calls GetMTEByName to check if the module is already loaded. If so, it increments by the refcount and returns the handle. If not, it loads the module, creates a new MTE, and returns the handle. The handle can be the index of the MTE in a global list, or a pointer to the MTE itself (cast to OS2_HMODULE).
// DosGetProcAddr/DosQueryProcAddr gets address of a function in a loaded module
// DosQueryProcType can determine 16-bit vs 32-bit entry point using flags

#endif

