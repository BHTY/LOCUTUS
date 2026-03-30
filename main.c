#include <stdio.h>
#include <windows.h>
#include "neldr.h"
#include "os2arena.h"
#include "dis386.h"
#include "i386.h"
#include "mte.h"

x86_context_t thecpu;

DWORD i386_get_stack_dword(x86_context_t* pCPU, DWORD index) {
	FLATPTR ea = linear(pCPU, SS, pCPU->esp + index);
	return *(DWORD*)OS2Map(ea);
}

WORD i386_get_stack_word(x86_context_t* pCPU, DWORD index) {
	FLATPTR ea = linear(pCPU, SS, pCPU->esp + index);
	return *(WORD*)OS2Map(ea);
}


char* load_file(char* filename) {
	FILE* fp = fopen(filename, "rb");
	char* buffer;
	unsigned size;

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buffer = malloc(size);

	fread(buffer, 1, size, fp);
	fclose(fp);
	return buffer;
}

void DumpNEHeader(PIMAGE_OS2_HEADER pOs2Hdr) {
	printf("Linker Version:       %d.%d\n", pOs2Hdr->ne_ver, pOs2Hdr->ne_rev);
	printf("Entry Table Offset:   %04X\n", pOs2Hdr->ne_enttab);
	printf("Entry Table Size:     %04X\n", pOs2Hdr->ne_cbenttab);
	printf("Flags:                %04X\n", pOs2Hdr->ne_flags);
	printf("Auto Data Segment:    %04X\n", pOs2Hdr->ne_autodata);
	printf("Initial Heap Size:    %04X\n", pOs2Hdr->ne_heap);
	printf("Initial Stack Size:   %04X\n", pOs2Hdr->ne_stack);
	printf("Initial CS:IP:        %04X:%04X\n", pOs2Hdr->ne_csip >> 16, pOs2Hdr->ne_csip & 0xFFFF);
	printf("Initial SS:SP:        %04X:%04X\n", pOs2Hdr->ne_sssp >> 16, pOs2Hdr->ne_sssp & 0xFFFF);
	printf("Segments:             %d\n", pOs2Hdr->ne_cseg);
	printf("Module References:    %d\n", pOs2Hdr->ne_cmod);
	printf("Size of NRNT:         %d\n", pOs2Hdr->ne_cbnrestab);
	printf("Segment Table Offset: %04X\n", pOs2Hdr->ne_segtab);  //
	printf("Rsrc    Table Offset: %04X\n", pOs2Hdr->ne_rsrctab);
	printf("ResName Table Offset: %04X\n", pOs2Hdr->ne_restab);  //
	printf("ModRef  Table Offset: %04X\n", pOs2Hdr->ne_modtab);
	printf("Import  Table Offset: %04X\n", pOs2Hdr->ne_imptab);
	printf("NRNT    Table Offset: %04X\n", pOs2Hdr->ne_nrestab); //
	printf("Target OS:            %02X\n", pOs2Hdr->ne_exetyp);
}

void DumpResNameTable(PIMAGE_OS2_HEADER pOs2Hdr) {
	char szName[256];
	char* NameTable = PTR_ADD(pOs2Hdr, pOs2Hdr->ne_restab);
	if (pOs2Hdr->ne_restab == 0) return;

	printf("Resident Name Table:\n");

	while (1) {
		WORD Ordinal;
		BYTE NameLength = *(NameTable++);

		if (NameLength == 0) break;

		memcpy(szName, NameTable, NameLength);
		szName[NameLength] = 0;
		NameTable += NameLength;
		Ordinal = *(WORD*)NameTable;
		NameTable += 2;
		printf("  %04X: %s\n", Ordinal, szName);
	}
}

void DumpNonResNameTable(PIMAGE_OS2_HEADER pOs2Hdr) {
	char szName[256];
	char* NameTable = PTR_ADD(pOs2Hdr, pOs2Hdr->ne_nrestab);
	if (pOs2Hdr->ne_nrestab == 0) return;

	printf("Non-Resident Name Table:\n");

	while (1) {
		WORD Ordinal;
		BYTE NameLength = *(NameTable++);

		if (NameLength == 0) break;

		memcpy(szName, NameTable, NameLength);
		szName[NameLength] = 0;
		NameTable += NameLength;
		Ordinal = *(WORD*)NameTable;
		NameTable += 2;
		printf("  %04X: %s\n", Ordinal, szName);
	}
}

void DumpModRefTable(PIMAGE_OS2_HEADER pOs2Hdr) {
	PWORD ModRefTable = PTR_ADD(pOs2Hdr, pOs2Hdr->ne_modtab);
	PBYTE ImportTable = PTR_ADD(pOs2Hdr, pOs2Hdr->ne_imptab);
	int i,p;

	printf("Module Reference Table:\n");

	for (i = 0; i < pOs2Hdr->ne_cmod; i++) {
		//printf("Module Reference %d: %04X\n", i + 1, ModRefTable[i]);
		PBYTE Name = PTR_ADD(ImportTable, ModRefTable[i]);
		BYTE NameLength = *(Name++);

		printf("  %u: ", i + 1);

		for (p = 0; p < NameLength; p++) {
			printf("%c", *(Name++));
		}

		printf("\n");
	}
}

void GetImportName(PIMAGE_OS2_HEADER pOs2Hdr, WORD ProcNameOffset, char* str) {
	PBYTE ImportTable = PTR_ADD(pOs2Hdr, pOs2Hdr->ne_imptab);
	PBYTE ImportName = PTR_ADD(ImportTable, ProcNameOffset);
	BYTE ImportNameLength = *(ImportName++);
	memcpy(str, ImportName, ImportNameLength);
	str[ImportNameLength] = 0;
}

void GetModuleRefName(PIMAGE_OS2_HEADER pOs2Hdr, WORD ModRefTblIndex, char* str) {
	PWORD ModRefTable = PTR_ADD(pOs2Hdr, pOs2Hdr->ne_modtab);
	WORD NameOffset = ModRefTable[ModRefTblIndex - 1];
	GetImportName(pOs2Hdr, NameOffset, str);
}

extern PVOID OS2ArenaBase;

void run(x86_context_t* pCPU) {
	char buf[80];

	while (1) {
		uint8_t* data = OS2Map(linear(pCPU, CS, pCPU->ip));

		//dump_386(pCPU);

		disasm_opcode(buf, data, pCPU->ip, 0, 0, 0, 0, 0);
		printf("%04X:%04X: %s\n", pCPU->cs.selector, pCPU->ip, buf);

		//disassemble(data, 1, pCPU->ip, 0);

		//getchar();

		i386_decode_opcode(pCPU);
	}
}

WORD SegmentTable[1024];	/*	Table mapping logical segments in the file to segments in memory	*/

void DoFixup(BYTE Source, PBYTE DestAddr, WORD Segment, DWORD Offset) {
	switch (Source) {
		case RELOC_SOURCE_LOW_BYTE:
			*DestAddr = Offset & 0xFF;
			break;
		case RELOC_SOURCE_SEGMENT:
			*(PWORD)DestAddr = Segment;
			break;
		case RELOC_SOURCE_FAR_ADDR:
			*(PWORD)DestAddr = Offset & 0xFFFF;
			*(PWORD)(DestAddr+2) = Segment;
			break;
		case RELOC_SOURCE_OFFSET:
			*(PWORD)DestAddr = Offset & 0xFFFF;
			break;
	}
}

void LoadSegments(PIMAGE_OS2_HEADER pOs2Hdr, char* buf) {
	NE_SEGMENT_ENTRY* pSegEntry = PTR_ADD(pOs2Hdr, pOs2Hdr->ne_segtab);

	// load segments
	for (int i = 0; i < pOs2Hdr->ne_cseg; i++) {
		FLATPTR linaddr;
		DWORD faraddr;
		DWORD BaseOffset = pSegEntry[i].SectorBase * (1 << pOs2Hdr->ne_align);
		printf("Segment %d: Base=%08X Size=%04X Flags=%04X MinAlloc=%04X\n", i + 1, BaseOffset, pSegEntry[i].Size, pSegEntry[i].Flags, pSegEntry[i].MinAlloc);

		{
			linaddr = OS2Alloc(0, BYTES_TO_HUNKS(pSegEntry[i].MinAlloc));
			faraddr = DosFlatToSel(linaddr);

			memcpy(OS2Map(linaddr), PTR_ADD(buf, BaseOffset), pSegEntry[i].Size);
			memset(OS2Map(linaddr + pSegEntry[i].Size), 0, pSegEntry[i].MinAlloc - pSegEntry[i].Size);

			SegmentTable[i + 1] = faraddr >> 16;

			printf("  Allocated to linear address %08X (%04X:%04X)\n", linaddr, faraddr >> 16, faraddr & 0xffff);

			if (!(pSegEntry[i].Flags & SEGFLAGS_TYPE_DATA)) {
				char* code = PTR_ADD(buf, BaseOffset);
				DWORD ip = 0;

				while (ip < pSegEntry[i].Size) {
					char str[512];

					printf("    %04X: ", ip);

					ip += disasm_opcode(str, code + ip, ip, 0, 0, 0, 0, 0);

					printf("%s\n", str);
				}
			}
		}
	}

	// now do relocations
	for (int i = 0; i < pOs2Hdr->ne_cseg; i++) {
		if (pSegEntry[i].Flags & SEGFLAGS_HAS_RELOCS) {
			int p;
			DWORD TableOffset = pSegEntry[i].SectorBase * (1 << pOs2Hdr->ne_align) + pSegEntry[i].Size;
			PNE_RELOC_TABLE RelocTable = PTR_ADD(buf, TableOffset);
			DWORD_PTR linaddr = DosSelToFlat(SegmentTable[i + 1] << 16);

			for (p = 0; p < RelocTable->EntryCount; p++) {
				PNE_RELOC_ENTRY Entry = &RelocTable->Entries[p];
				PBYTE DestAddr;
				WORD Segment = 0;
				DWORD Offset = 0;

				printf("  %u: ", p);
				printf("(src=%02X flags=%02X offset=%04X) ", Entry->Source, Entry->FlagsAndTarget & RELOC_FLAGS_MASK, Entry->SourceChainOffset);

				DestAddr = OS2Map(linaddr + Entry->SourceChainOffset);


				switch (Entry->FlagsAndTarget & RELOC_TARGET_MASK) {
					case RELOC_TARGET_INTERNAL_REF: {
						printf("internal (segnum=%02X segidx=%02X)", Entry->Value.InternalRef.SegmentNumber, Entry->Value.InternalRef.SegmentIndex);
						Segment = SegmentTable[Entry->Value.InternalRef.SegmentNumber];
						break;
					}
					case RELOC_TARGET_IMPORT_ORDINAL: {
						char szModuleName[256];
						WORD ModRefTblIndex = Entry->Value.ImportByOrdinal.ModRefTblIndex;
						WORD Ordinal = Entry->Value.ImportByOrdinal.Ordinal;
						GetModuleRefName(pOs2Hdr, ModRefTblIndex, szModuleName);

						printf("importbyordinal(%s.%u)", szModuleName, Ordinal);

						MTE* pMTE = GetMTEByName(szModuleName);
						MTE_ENTRY* pEntry = GetEntryByOrdinal(pMTE, Ordinal);

						Segment = pEntry->Segment;
						Offset = pEntry->Offset;
						
						break;
					}
					case RELOC_TARGET_IMPORT_NAME: {
						char szModuleName[256];
						char szProcName[256];
						WORD ModRefTblIndex = Entry->Value.ImportByName.ModRefTblIndex;
						WORD ProcNameOffset = Entry->Value.ImportByName.ProcNameOffset;
						GetModuleRefName(pOs2Hdr, ModRefTblIndex, szModuleName);
						GetImportName(pOs2Hdr, ProcNameOffset, szProcName);
						printf("importbyname(%s.%s)", szModuleName, szProcName);

						MTE* pMTE = GetMTEByName(szModuleName);
						MTE_ENTRY* pEntry = GetEntryByName(pMTE, szProcName);

						Segment = pEntry->Segment;
						Offset = pEntry->Offset;

						break;
					}
					case RELOC_TARGET_OS_FIXUP: {
						printf("osfixup");
						break;
					}
				}

				DoFixup(Entry->Source, DestAddr, Segment, Offset);

				printf("\n");
			}
		}
	}

	i386_load_sreg(&thecpu, CS, SegmentTable[pOs2Hdr->ne_csip >> 16]);
	thecpu.ip = pOs2Hdr->ne_csip & 0xFFFF;
	i386_load_sreg(&thecpu, SS, SegmentTable[pOs2Hdr->ne_sssp >> 16]);
	thecpu.sp = pOs2Hdr->ne_sssp & 0xFFFF;
	i386_load_sreg(&thecpu, DS, SegmentTable[pOs2Hdr->ne_autodata]);

	thecpu.ax = 0x2f;

	run(&thecpu);
}

int LoadNewEXE(char* name) {
	char* buf = load_file(name);
	PIMAGE_DOS_HEADER pDosHdr = buf;
	PIMAGE_OS2_HEADER pOs2Hdr;

	if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE) {
		printf("Not a valid MZ file.\n");
		return 1;
	}

	if (pDosHdr->e_lfarlc != 0x40) {
		printf("Not a valid NE file.\n");
		return 1;
	}

	pOs2Hdr = PTR_ADD(buf, pDosHdr->e_lfanew);

	if (pOs2Hdr->ne_magic != IMAGE_OS2_SIGNATURE) {
		printf("Not a valid NE file.\n");
		return 1;
	}

	DumpNEHeader(pOs2Hdr);
	DumpResNameTable(pOs2Hdr);
	DumpNonResNameTable(pOs2Hdr);
	DumpModRefTable(pOs2Hdr);
	//entry

	LoadSegments(pOs2Hdr, buf);

}


uint8_t* TranslateEmulatedToVirtualAddress(uint32_t addr) {
	return OS2Map(addr);
}

uint32_t io_read_32(uint16_t port) {
	return 0xfc;
}

uint16_t io_read_16(uint16_t port) {
	return io_read_32(port) & 0xFFFF;
}

uint8_t io_read_8(uint16_t port) {
	return io_read_32(port) & 0xFF;
}

uint8_t bus_read_8(uint32_t addr) {
	return (uint32_t) * (TranslateEmulatedToVirtualAddress(addr));
}

uint16_t bus_read_16(uint32_t addr) {
	return (uint32_t) * (uint16_t*)(TranslateEmulatedToVirtualAddress(addr));
}

uint32_t bus_read_32(uint32_t addr) {
	return *(uint32_t*)(TranslateEmulatedToVirtualAddress(addr));
}

void bus_write_8(uint32_t addr, uint8_t value) {
	*(TranslateEmulatedToVirtualAddress(addr)) = value;
}

void bus_write_16(uint32_t addr, uint16_t value) {
	*(uint16_t*)(TranslateEmulatedToVirtualAddress(addr)) = value;
}

void bus_write_32(uint32_t addr, uint32_t value) {
	*(uint32_t*)(TranslateEmulatedToVirtualAddress(addr)) = value;
}


void init_386(x86_context_t* pCPU) {
	memset(pCPU, 0, sizeof(*pCPU));

	//mark everything as writable valid data USE16 nonsystem segments with limit 4GB
	pCPU->es.access = 0x80 | 0x10 | 0x2;
	pCPU->fs.access = 0x80 | 0x10 | 0x2;
	pCPU->gs.access = 0x80 | 0x10 | 0x2;
	pCPU->ds.access = 0x80 | 0x10 | 0x2;
	pCPU->ss.access = 0x80 | 0x10 | 0x2;
	pCPU->cs.access = 0x80 | 0x10 | 0x8 | 0x2;

	pCPU->es.limit = 0xFFFF;
	pCPU->fs.limit = 0xFFFF;
	pCPU->gs.limit = 0xFFFF;
	pCPU->ds.limit = 0xFFFF;
	pCPU->ss.limit = 0xFFFF;
	pCPU->cs.limit = 0xFFFF;

	pCPU->system.bus_read_8 = bus_read_8;
	pCPU->system.bus_read_16 = bus_read_16;
	pCPU->system.bus_read_32 = bus_read_32;

	pCPU->system.bus_write_8 = bus_write_8;
	pCPU->system.bus_write_16 = bus_write_16;
	pCPU->system.bus_write_32 = bus_write_32;

	pCPU->msw = 1;

	// AX = environment segment
	// BX = command-line offset
	// CX = size of automatic data segment
}


void WriteThunk(PBYTE pBuf, WORD ThunkIndex, WORD argbytes) {
	pBuf += ThunkIndex * 8;

	pBuf[0] = 0xb8;
	pBuf[1] = ThunkIndex & 0xFF;
	pBuf[2] = ThunkIndex >> 8;
	pBuf[3] = 0xcd;
	pBuf[4] = 0x30;
	pBuf[5] = 0xca;
	pBuf[6] = argbytes;
	pBuf[7] = 0x00;
}


void init_mte() {
	MTE* MsgMTE = malloc(sizeof(MTE));
	MTE* DoscallsMTE = malloc(sizeof(MTE)+sizeof(MTE_ENTRY)*4);
	MTE* ViocallsMTE = malloc(sizeof(MTE));
	MTE* PmwinMTE = malloc(sizeof(MTE) + sizeof(MTE_ENTRY) * 13);

	MsgMTE->Next = NULL;
	strcpy(MsgMTE->ModuleName, "MSG");
	MsgMTE->ReferenceCount = 1;
	MsgMTE->EntryPointCount = 1;
	MsgMTE->EntryPoints[0].Segment = 0x4f;
	MsgMTE->EntryPoints[0].Offset = 0x0;
	MsgMTE->EntryPoints[0].Ordinal = 1;
	strcpy(MsgMTE->EntryPoints[0].Name, "DOSPUTMESSAGE");

	DoscallsMTE->Next = MsgMTE;
	strcpy(DoscallsMTE->ModuleName, "DOSCALLS");
	DoscallsMTE->ReferenceCount = 1;
	DoscallsMTE->EntryPointCount = 6;

	DoscallsMTE->EntryPoints[0].Segment = 0x4f;
	DoscallsMTE->EntryPoints[0].Offset = 0x8;
	DoscallsMTE->EntryPoints[0].Ordinal = 5;
	strcpy(DoscallsMTE->EntryPoints[0].Name, "DOSEXIT");

	DoscallsMTE->EntryPoints[1].Segment = 0x4f;
	DoscallsMTE->EntryPoints[1].Offset = 0x8*2;
	DoscallsMTE->EntryPoints[1].Ordinal = 41;
	strcpy(DoscallsMTE->EntryPoints[1].Name, "DOSGETHUGESHIFT");

	DoscallsMTE->EntryPoints[2].Segment = 0x4f;
	DoscallsMTE->EntryPoints[2].Offset = 0x8*3;
	DoscallsMTE->EntryPoints[2].Ordinal = 49;
	strcpy(DoscallsMTE->EntryPoints[2].Name, "DOSGETMACHINEMODE");

	DoscallsMTE->EntryPoints[3].Segment = 0x4f;
	DoscallsMTE->EntryPoints[3].Offset = 0x8*4;
	DoscallsMTE->EntryPoints[3].Ordinal = 92;
	strcpy(DoscallsMTE->EntryPoints[3].Name, "DOSGETVERSION");

	DoscallsMTE->EntryPoints[4].Segment = 0x4f;
	DoscallsMTE->EntryPoints[4].Offset = 0x8*5;
	DoscallsMTE->EntryPoints[4].Ordinal = 94;
	strcpy(DoscallsMTE->EntryPoints[4].Name, "DOSGETPID");

	DoscallsMTE->EntryPoints[5].Segment = 0x4f;
	DoscallsMTE->EntryPoints[5].Offset = 0x8 * 21;
	DoscallsMTE->EntryPoints[5].Ordinal = 38;
	strcpy(DoscallsMTE->EntryPoints[5].Name, "DOSREALLOCSEG");

	ViocallsMTE->Next = DoscallsMTE;
	strcpy(ViocallsMTE->ModuleName, "VIOCALLS");
	ViocallsMTE->ReferenceCount = 1;
	ViocallsMTE->EntryPointCount = 1;

	ViocallsMTE->EntryPoints[0].Segment = 0x4f;
	ViocallsMTE->EntryPoints[0].Offset = 0x8 * 6;
	ViocallsMTE->EntryPoints[0].Ordinal = 19;
	strcpy(ViocallsMTE->EntryPoints[0].Name, "VIOWRTTTY");

	PmwinMTE->Next = ViocallsMTE;
	strcpy(PmwinMTE->ModuleName, "PMWIN");
	PmwinMTE->ReferenceCount = 1;
	PmwinMTE->EntryPointCount = 14;

	PmwinMTE->EntryPoints[0].Segment = 0x4f;
	PmwinMTE->EntryPoints[0].Offset = 0x8 * 7;
	PmwinMTE->EntryPoints[0].Ordinal = 3;
	strcpy(PmwinMTE->EntryPoints[0].Name, "WINREGISTERCLASS");

	PmwinMTE->EntryPoints[1].Segment = 0x4f;
	PmwinMTE->EntryPoints[1].Offset = 0x8 * 8;
	PmwinMTE->EntryPoints[1].Ordinal = 6;
	strcpy(PmwinMTE->EntryPoints[1].Name, "WINCREATEWINDOW");

	PmwinMTE->EntryPoints[2].Segment = 0x4f;
	PmwinMTE->EntryPoints[2].Offset = 0x8 * 9;
	PmwinMTE->EntryPoints[2].Ordinal = 7;
	strcpy(PmwinMTE->EntryPoints[2].Name, "WINDESTROYWINDOW");

	PmwinMTE->EntryPoints[3].Segment = 0x4f;
	PmwinMTE->EntryPoints[3].Offset = 0x8 * 10;
	PmwinMTE->EntryPoints[3].Ordinal = 45;
	strcpy(PmwinMTE->EntryPoints[0].Name, "WINBEGINPAINT");

	PmwinMTE->EntryPoints[4].Segment = 0x4f;
	PmwinMTE->EntryPoints[4].Offset = 0x8 * 11;
	PmwinMTE->EntryPoints[4].Ordinal = 46;
	strcpy(PmwinMTE->EntryPoints[4].Name, "WINENDPAINT");

	PmwinMTE->EntryPoints[5].Segment = 0x4f;
	PmwinMTE->EntryPoints[5].Offset = 0x8 * 12;
	PmwinMTE->EntryPoints[5].Ordinal = 58;
	strcpy(PmwinMTE->EntryPoints[5].Name, "WINCREATEMSGQUEUE");

	PmwinMTE->EntryPoints[6].Segment = 0x4f;
	PmwinMTE->EntryPoints[6].Offset = 0x8 * 13;
	PmwinMTE->EntryPoints[6].Ordinal = 59;
	strcpy(PmwinMTE->EntryPoints[6].Name, "WINDESTROYMSGQUEUE");

	PmwinMTE->EntryPoints[7].Segment = 0x4f;
	PmwinMTE->EntryPoints[7].Offset = 0x8 * 14;
	PmwinMTE->EntryPoints[7].Ordinal = 65;
	strcpy(PmwinMTE->EntryPoints[7].Name, "WINGETMSG");

	PmwinMTE->EntryPoints[8].Segment = 0x4f;
	PmwinMTE->EntryPoints[8].Offset = 0x8 * 15;
	PmwinMTE->EntryPoints[8].Ordinal = 68;
	strcpy(PmwinMTE->EntryPoints[8].Name, "WINDISPATCHMSG");

	PmwinMTE->EntryPoints[9].Segment = 0x4f;
	PmwinMTE->EntryPoints[9].Offset = 0x8 * 16;
	PmwinMTE->EntryPoints[9].Ordinal = 69;
	strcpy(PmwinMTE->EntryPoints[0].Name, "WINPOSTMSG");

	PmwinMTE->EntryPoints[10].Segment = 0x4f;
	PmwinMTE->EntryPoints[10].Offset = 0x8 * 17;
	PmwinMTE->EntryPoints[10].Ordinal = 173;
	strcpy(PmwinMTE->EntryPoints[10].Name, "WINFILLRECT");

	PmwinMTE->EntryPoints[11].Segment = 0x4f;
	PmwinMTE->EntryPoints[11].Offset = 0x8 * 18;
	PmwinMTE->EntryPoints[11].Ordinal = 178;
	strcpy(PmwinMTE->EntryPoints[11].Name, "WINDEFWINDOWPROC");

	PmwinMTE->EntryPoints[12].Segment = 0x4f;
	PmwinMTE->EntryPoints[12].Offset = 0x8 * 19;
	PmwinMTE->EntryPoints[12].Ordinal = 246;
	strcpy(PmwinMTE->EntryPoints[12].Name, "WININITIALIZE");

	PmwinMTE->EntryPoints[13].Segment = 0x4f;
	PmwinMTE->EntryPoints[13].Offset = 0x8 * 20;
	PmwinMTE->EntryPoints[13].Ordinal = 247;
	strcpy(PmwinMTE->EntryPoints[13].Name, "WINTERMINATE");

	g_MTEListHead = PmwinMTE;

	PBYTE pBuf = OS2Map(0x90000);
	WriteThunk(pBuf, 0, 8);
	WriteThunk(pBuf , 1, 4);
	WriteThunk(pBuf , 2, 4);
	WriteThunk(pBuf , 3, 4);
	WriteThunk(pBuf , 4, 4);
	WriteThunk(pBuf , 5, 4);
	WriteThunk(pBuf , 6, 8);
	WriteThunk(pBuf , 7, 14);
	WriteThunk(pBuf , 8, 0);
	WriteThunk(pBuf , 9, 0);
	WriteThunk(pBuf , 10, 0);
	WriteThunk(pBuf , 11, 0);
	WriteThunk(pBuf , 12, 4);
	WriteThunk(pBuf , 13, 0);
	WriteThunk(pBuf , 14, 0);
	WriteThunk(pBuf , 15, 0);
	WriteThunk(pBuf , 16, 0);
	WriteThunk(pBuf , 17, 0);
	WriteThunk(pBuf , 18 ,0);
	WriteThunk(pBuf , 19, 2);
	WriteThunk(pBuf , 20, 2);
	WriteThunk(pBuf , 21, 4);
}


int main() {
	printf("%d\n", InitOS2Arena());
	init_mte();
	init_386(&thecpu);
	//LoadNewEXE("c:\\users\\will\\nedump\\t.exe");
	//LoadNewEXE("c:\\users\\will\\downloads\\hello16.exe");
	//LoadNewEXE("c:\\os2test\\testpm.exe");
	LoadNewEXE("c:\\os2test\\testargv.exe");

}