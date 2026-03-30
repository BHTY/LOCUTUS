#pragma once

#include <stdint.h>

typedef struct x86_system {
	uint8_t(*bus_read_8)(uint32_t addr);
	uint16_t(*bus_read_16)(uint32_t addr);
	uint32_t(*bus_read_32)(uint32_t addr);

	void (*bus_write_8)(uint32_t addr, uint8_t value);
	void (*bus_write_16)(uint32_t addr, uint16_t value);
	void (*bus_write_32)(uint32_t addr, uint32_t value);

	uint8_t(*io_read_8)(uint16_t port);
	uint16_t(*io_read_16)(uint16_t port);
	uint32_t(*io_read_32)(uint16_t port);

	void (*io_write_8)(uint16_t port, uint8_t value);
	void (*io_write_16)(uint16_t port, uint16_t value);
	void (*io_write_32)(uint16_t port, uint32_t value);
} x86_system_t;

typedef struct _SEGREG {
	uint16_t selector;

	//descriptor
	uint32_t base;
	uint32_t limit;
	uint8_t access;
	uint8_t flags;
} SEGREG;

#define PL(desc)    ((desc.selector) & 0x3)

#define VALID(desc)	((desc.access & 0x80) >> 7)
#define DPL(desc)	((desc.access & 0x60) >> 5)
#define STYPE(desc)	((desc.access & 0x10) >> 4)
#define EXEC(desc)	((desc.access & 0x8) >> 3)
#define CONF(desc)	((desc.access & 0x4) >> 2)
#define RW(desc)	((desc.access & 0x2) >> 1)

#define MODE(desc)	((desc.flags & 0x4) >> 2)
#define GRAN(desc)	((desc.flags & 0x8) >> 3)

typedef struct _DESCREG {
	uint16_t limit;
	uint32_t base;
} DESCREG;

typedef struct x86_context {
	union {
		struct {
			struct {
				union {
					struct {
						uint8_t al;
						uint8_t ah;
					};
					uint16_t ax;
					uint32_t eax;
				};
			};

			struct {
				union {
					struct {
						uint8_t cl;
						uint8_t ch;
					};
					uint16_t cx;
					uint32_t ecx;
				};
			};

			struct {
				union {
					struct {
						uint8_t dl;
						uint8_t dh;
					};
					uint16_t dx;
					uint32_t edx;
				};
			};

			struct {
				union {
					struct {
						uint8_t bl;
						uint8_t bh;
					};
					uint16_t bx;
					uint32_t ebx;
				};
			};

			union {
				uint16_t sp;
				uint32_t esp;
			};

			union {
				uint16_t bp;
				uint32_t ebp;
			};

			union {
				uint16_t si;
				uint32_t esi;
			};

			union {
				uint16_t di;
				uint32_t edi;
			};
		};
		uint32_t regs[8];
	};

	union {
		struct {
			union {
				uint16_t msw;
				uint32_t cr0;
			};
			uint32_t cr1, cr2, cr3, cr4, cr5, cr6, cr7, cr8;
		};
		uint32_t cr[9];
	};

	union {
		struct {
			uint32_t dr0, dr1, dr2, dr3, dr4, dr5, dr6, dr7;
		};
		uint32_t dr[8];
	};

	union {
		struct {
			uint32_t tr0, tr1, tr2, tr3, tr4, tr5, tr6, tr7;
		};
		uint32_t tr[8];
	};

	union {
		uint16_t ip;
		uint32_t eip;
	};

	union {
		uint16_t flags;
		uint32_t eflags;
	};

	union {
		struct {
			SEGREG es, cs, ss, ds, fs, gs;
		};
		SEGREG seg_regs[6];
	};

	DESCREG gdtr, ldtr, idtr, taskr;
	int cpl;

	x86_system_t system;

	int segment_prefix;
	int segment_override;

	int addr_override;
	int op_override;

	int running;
} x86_context_t;

extern int i386_parity_table[256];

#define ES 0
#define CS 1
#define SS 2
#define DS 3
#define FS 4
#define GS 5

#define CARRY 0
#define PARITY 2
#define AC 4
#define ZERO 6
#define SIGN 7
#define TRAP 8
#define INTERRUPT 9
#define DIRECTION 10
#define OVERFLOW 11
#define NESTED 14
#define RESUME 16
#define V8086 17

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

#define AL 0
#define CL 1
#define DL 2
#define BL 3
#define AH 4
#define CH 5
#define DH 6
#define BH 7

#define CARRY 0
#define PARITY 2
#define AC 4
#define ZERO 6
#define SIGN 7
#define TRAP 8
#define INTERRUPT 9
#define DIRECTION 10
#define OVERFLOW 11
#define NESTED 14
#define RESUME 16
#define V8086 17

//Jump Types
#define JO 0
#define JNO 1
#define JB 2
#define JAE 3
#define JE 4
#define JNE 5
#define JBE 6
#define JA 7
#define JS 8
#define JNS 9
#define JP 10
#define JNP 11
#define JL 12
#define JGE 13
#define JLE 14
#define JG 15
#define ALWAYS 16
#define JCXZ 17

#define PE (pCPU->cr0 & 0x1)

#define IOPL(flags) (((flags) & 0x3000) >> 12)

#define GET_FLAG(flag)     ((pCPU->eflags & (1 << (flag))) >> (flag))
#define SET_FLAG(flag)		(pCPU->eflags |= (1 << (flag)))
#define CLEAR_FLAG(flag)	(pCPU->eflags &= ~(1 << (flag)))
#define COND_FLAG(flag, val)	if(val){SET_FLAG(flag);}else{CLEAR_FLAG(flag);}

void i386_decode_opcode(x86_context_t* pCPU);
void i386_decode_two_byte(x86_context_t* pCPU);

void i386_load_sreg(x86_context_t* pCPU, int segment, uint16_t selector);

void i386_far_call(x86_context_t* pCPU, uint16_t sgmt, uint32_t target);
void i386_far_jump(x86_context_t* pCPU, uint16_t sgmt, uint32_t target);

#define NULL ((void*)0)
