/*	Module Table List manager for image loader	*/
#include "mte.h"

MTE* g_MTEListHead = NULL;

/*	Returns the MTE corresponding to the given module name, or NULL if not found.	*/
MTE* GetMTEByName(char* ModuleName) {
	MTE* current = g_MTEListHead;

	while (current) {
		if (strcmp(current->ModuleName, ModuleName) == 0) {
			return current;
		}
		current = current->Next;
	}

	return NULL;
}

/*	Returns the entry point corresponding to the given name, or NULL if not found.	*/
MTE_ENTRY* GetEntryByName(MTE* pMTE, char* EntryName) {
	int i;

	for (i = 0; i < pMTE->EntryPointCount; i++) {
		if (strcmp(pMTE->EntryPoints[i].Name, EntryName) == 0) {
			return &pMTE->EntryPoints[i];
		}
	}

	return NULL;
}

/*	Returns the entry point corresponding to the given ordinal, or NULL if not found.	*/
MTE_ENTRY* GetEntryByOrdinal(MTE* pMTE, WORD Ordinal) {
	int i;

	for (i = 0; i < pMTE->EntryPointCount; i++) {
		if (pMTE->EntryPoints[i].Ordinal == Ordinal) {
			return &pMTE->EntryPoints[i];
		}
	}

	return NULL;
}
