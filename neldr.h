/*	Data structures and routines for loading New Executable format modules into memory.	*/

#ifndef __NELDR_H__
#define __NELDR_H__

#include <windows.h>

/*	NE flag words	*/

/*	The type is a 3-bit integer ORed with the other flags.	*/
#define SEGFLAGS_TYPE_CODE	0
#define SEGFLAGS_TYPE_DATA	1
#define SEGFLAGS_TYPE_MASK	0x0007

#define SEGFLAGS_MOVEABLE	0x0010
#define SEGFLAGS_PRELOAD    0x0040
#define SEGFLAGS_HAS_RELOCS	0x0100
#define SEGFLAGS_DISCARD	0xF000

typedef struct _NE_SEGMENT_ENTRY {	/*	Segment table entry	*/
	WORD	SectorBase;				/*  Offset in sectors from the start of the file	*/
	WORD	Size;					/*	Size in file, in bytes	*/
	WORD	Flags;					/*	See flags above	*/
	WORD	MinAlloc;				/*	Minimum size in memory.	*/
} NE_SEGMENT_ENTRY, *PNE_SEGMENT_ENTRY;

/*	Resources	*/

/*	Entry Table	*/

/*	Relocation Data	*/
#define ENTRY_FLAGS_EXPORTED	0x01
#define ENTRY_FLAGS_GLOBALDATA	0x02

#define RELOC_SOURCE_LOW_BYTE 0x00
#define RELOC_SOURCE_SEGMENT 0x02
#define RELOC_SOURCE_FAR_ADDR 0x03 /* (32-bit pointer) */
#define RELOC_SOURCE_OFFSET 0x05 /* (16-bit offset) */

#define RELOC_TARGET_INTERNAL_REF 0x00
#define RELOC_TARGET_IMPORT_ORDINAL 0x01
#define RELOC_TARGET_IMPORT_NAME 0x02
#define RELOC_TARGET_OS_FIXUP 0x03
#define RELOC_TARGET_MASK 0x03

#define RELOC_FLAGS_ADDITIVE 0x04
#define RELOC_FLAGS_MASK 0xFC

#define OS_FIXUP_TYPE_FIARQQ_FJARQQ 0x0001
#define OS_FIXUP_TYPE_FISRQQ_FJSRQQ 0x0002
#define OS_FIXUP_TYPE_FICRQQ_FJCRQQ 0x0003
#define OS_FIXUP_TYPE_FIERQQ        0x0004
#define OS_FIXUP_TYPE_FIDRQQ        0x0005
#define OS_FIXUP_TYPE_FIWRQQ        0x0006

typedef struct _NE_RELOC_ENTRY {	/*	Relocation table entry	*/
	BYTE	Source;					/*	See RELOC_SOURCE_*.	*/
	BYTE	FlagsAndTarget;			/*	One of RELOC_TARGET_* ORed with any number of RELOC_FLAGS_*.	*/
	WORD	SourceChainOffset;
	union {							/*	Variant depends on target type	*/
		struct {
			BYTE	SegmentNumber;	/*	0xFF = movable segment, other value = fixed segment	*/
			BYTE	Zero;
			WORD	SegmentIndex;	/*	If fixed seg, offset into segment. If movable, ordinal index into entry table	*/
		} InternalRef;
		struct {
			WORD	ModRefTblIndex;	/*	Index into module reference table	*/
			WORD	ProcNameOffset;	/*	Offset from start of imported-names table to procedure name string	*/
		} ImportByName;
		struct {
			WORD	ModRefTblIndex;	/*	Index into module reference table	*/
			WORD	Ordinal;		/*	Ordinal number of the imported procedure	*/
		} ImportByOrdinal;
		struct {
			WORD	OSFixupType;	/*	One of OS_FIXUP_TYPE_*	*/
			WORD Zero;
		} OSFixup;
	} Value;
} NE_RELOC_ENTRY, *PNE_RELOC_ENTRY;

typedef struct _NE_RELOC_TABLE {	/*	Segment relocation table	*/
	WORD			EntryCount;
	NE_RELOC_ENTRY	Entries[1];
} NE_RELOC_TABLE, *PNE_RELOC_TABLE;

#endif
