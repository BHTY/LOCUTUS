#include "i386.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "names.h"

#define i386 x86_context_t

void dump_seg_386(i386* pCPU, int seg) {
	SEGREG sgmt = pCPU->seg_regs[seg];

	printf("%s (%04X): Base=%08X Limit=%08X DPL=%d ", seg_regs[seg], sgmt.selector, sgmt.base, sgmt.limit, DPL(sgmt));

	if (VALID(sgmt)) {
		if (STYPE(sgmt)) {
			if (EXEC(sgmt)) {
				printf("CODE ");
			}
			else {
				printf("DATA ");
			}

			if (MODE(sgmt)) {
				printf("USE32 ");
			}
			else {
				printf("USE16 ");
			}
		}
		else {
			printf("SYSTEM \n");
		}
	}

	else {
		printf("NOT VALID ");
	}

	printf("\n");
}

#define PRINT_FLAG(mask, set, clear)       if(pCPU->eflags & (1 << mask)){ \
												printf(set); \
											} else { \
												printf(clear); \
											}

void dump_eflags_i386(i386* pCPU) {
	PRINT_FLAG(V8086, "V", "");
	PRINT_FLAG(RESUME, "R", "");
	PRINT_FLAG(NESTED, "N", "");
	PRINT_FLAG(OVERFLOW, "O", "");
	PRINT_FLAG(DIRECTION, "D", "d");
	PRINT_FLAG(INTERRUPT, "I", "");
	PRINT_FLAG(TRAP, "T", "");
	PRINT_FLAG(SIGN, "S", "");
	PRINT_FLAG(ZERO, "Z", "");
	PRINT_FLAG(AC, "A", "");
	PRINT_FLAG(PARITY, "P", "");
	PRINT_FLAG(CARRY, "C", "");

	//printf(" IOPL=%d", IOPL(pCPU->eflags));
}

void dump_regs_386(i386* pCPU) {
	int i;

	printf("EIP= %08X CPL=%d EFLAGS= ", pCPU->eip, pCPU->cpl);
	dump_eflags_i386(pCPU);

	for (i = 0; i < 8; i++) {
		if ((i % 4) == 0) printf("\n");

		printf("%s: %08X  ", regs_32[i], pCPU->regs[i]);
	}

	printf("\n");
}

void dump_segs_386(i386* pCPU) {
	int i;

	for (i = 0; i < 6; i++) {
		dump_seg_386(pCPU, i);
	}
}

void dump_cr_386(i386* pCPU) {
	int i;

	for (i = 0; i < 8; i++) {
		if (i == 4) printf("\n");

		printf("CR%d: %p  ", i, pCPU->cr[i]);
	}
}

void dump_gdt_ldt_idt(i386* pCPU) {

}

void dump_386(i386* pCPU) {
	dump_regs_386(pCPU);
	dump_segs_386(pCPU);
	dump_cr_386(pCPU);
	dump_gdt_ldt_idt(pCPU);
	printf("\n");
}