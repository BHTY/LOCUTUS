/*	Basic APIs for controlling the image loader	*/

/* TODO: Sniff out DLL exports, call DLL entry points */

#include <windows.h>

typedef ULONG OS2_APIRET;
typedef USHORT OS2_HMODULE;


OS2_APIRET DosLoadModule(PCHAR pszName, UINT cbName, LPSTR pszModname, OS2_HMODULE* phmod) {
	/**
	 *	DosLoadModule performs the following steps when bringing a new image into memory:
	 *	1.) Figure out the file to load
	 *			a.	If the module name is a full path, use that. 
	 *			b.	If it's a path with .DLL/.EXE at the end, search the appropriate paths 
	 *				for a matching file. 
	 *			c.	If the module name is just the title ("DOSCALLS"), then call 
	 *				GetMTEByName to find the appropriate MTE for the module. If one exists, 
	 *				increment the reference count, call the DLL entry point, get the 
	 *				corresponding handle, and return that along with an appropriate success 
	 *				code. If the module isn't already loaded into memory, follow the same
	 *				steps as if the module name had a .DLL extension appended onto the end
	 *				in 1b
	 *	2.) Attempt to load the file contents into memory, reporting an appropriate error on
	 *		failure
	 *	3.) Reject the EXE header if it's invalid. If it's LX or NE, pass along to the next
	 *		stage loader.
	 *  --- these following steps are part of the NE/LX loader, but fundamentally the same 
	 *		and included for completion	---
	 *  4.) Get the module name and follow the same steps as in 1c to check if the module is
	 *      already loaded, and following the same process if it is. 
	 *	5.) Call DosLoadModule to load any dependencies of the module. If there is any kind
	 *		of failure within the recursive call, then we pass that error up the chain,
	 *		propagating up the error code and failing module.
	 *  6.) Try to locate all imported procedures from imported modules, and if any are missing,
	 *		fail with an appropriate error code.
	 *	7.) Create an MTE for the module, adding it to the module list.
	 *	8.) Call the entry point of the module. If it fails, free the MTE and return an
	 *		appropriate error code.
	 *	9.) Obtain an OS/2 HMODULE that maps to the MTE, and return that along with an
	 *		appropriate success code.
	 *	--- we're now back in the main loader ---
	 *	10.)Free the file contents from memory if they were loaded
	 */


}


