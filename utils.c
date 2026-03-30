#include "utils.h"
#include "i386.h"
#include "modrm.h"
#include "common.h"
#include <assert.h>
#include <stdio.h>

char getch(void);

typedef void (*i386_op_func)(x86_context_t* pCPU);

#include <windows.h>
#include "os2arena.h"

DWORD i386_get_stack_dword(x86_context_t* pCPU, DWORD index);
WORD i386_get_stack_word(x86_context_t* pCPU, DWORD index);

void Dos16PutMessage(x86_context_t* pCPU) {
    WORD arg1 = i386_get_stack_word(pCPU, 0xA);
    WORD arg2 = i386_get_stack_word(pCPU, 0x8);
    DWORD arg3 = i386_get_stack_dword(pCPU, 0x4);
    DWORD_PTR farg3 = OS2Map(DosSelToFlat(arg3));

    printf("DOS16PUTMESSAGE(%04X, %04X, %04X:%04X)\n", arg1, arg2, arg3 >> 16, arg3 & 0xFFFF);

    pCPU->ax = DosPutMessage(arg1, arg2, farg3);

    //while (1);
}

void DosExit(UINT ulAction, UINT ulResult) {
    while (1);
}

void Dos16Exit(x86_context_t* pCPU) {
	WORD arg1 = i386_get_stack_word(pCPU, 0x6);
	WORD arg2 = i386_get_stack_word(pCPU, 0x4);

	printf("DOS16EXIT(%04X, %04X)\n", arg1, arg2);

	DosExit(arg1, arg2);
}

USHORT DosGetHugeShift(PUSHORT ShiftCount)
{
    *ShiftCount = 8;
    return 0; // NO_ERROR
}

void Dos16GetHugeShift(x86_context_t* pCPU) {
    DWORD arg1 = i386_get_stack_dword(pCPU, 0x4);
	DWORD_PTR farg1 = OS2Map(DosSelToFlat(arg1));

	printf("DOS16GETHUGESHIFT(%04X:%04X)\n", arg1 >> 16, arg1 & 0xFFFF);

    pCPU->ax = DosGetHugeShift(farg1);
}

USHORT DosGetMachineMode(PBYTE MachineMode)
{
    *MachineMode = 1;
    return 0; // NO_ERROR
}

void Dos16GetMachineMode(x86_context_t* pCPU) {
    DWORD arg1 = i386_get_stack_dword(pCPU, 0x4);
    DWORD_PTR farg1 = OS2Map(DosSelToFlat(arg1));

    printf("DOS16GETMACHINEMODE(%04X:%04X)\n", arg1 >> 16, arg1 & 0xFFFF);

    pCPU->ax = DosGetMachineMode(farg1);
}

USHORT DosGetVersion(PUSHORT VersionWord)
{
	*VersionWord = 0x201; // OS/2 2.0
	return 0; // NO_ERROR
}

void Dos16GetVersion(x86_context_t* pCPU) {
    DWORD arg1 = i386_get_stack_dword(pCPU, 0x4);
    DWORD_PTR farg1 = OS2Map(DosSelToFlat(arg1));

    printf("DOS16GETVERSION(%04X:%04X)\n", arg1 >> 16, arg1 & 0xFFFF);

    pCPU->ax = DosGetVersion(farg1);
}

void Dos16GetPID(x86_context_t* pCPU) { //PPIDINFO structure
    printf("DOS16GETPID\n");
    pCPU->ax = 0;
}

USHORT VioWrtTTY(PCHAR CharStr, ULONG Length, USHORT VioHandle) {
    return 0; // NO_ERROR
}

void Vio16WrtTTY(x86_context_t* pCPU) {
    DWORD arg1 = i386_get_stack_dword(pCPU, 0x8);
	WORD arg2 = i386_get_stack_word(pCPU, 0x6);
    WORD arg3 = i386_get_stack_word(pCPU, 0x4);
    DWORD_PTR farg1 = OS2Map(DosSelToFlat(arg1));

	printf("VIOWRTTTY(%04X:%04X, %04X, %04X)\n", arg1 >> 16, arg1 & 0xFFFF, arg2, arg3);

    pCPU->ax = VioWrtTTY(farg1, arg2, arg3);
}

void Dos16ReallocSeg(x86_context_t *pCPU) {
	WORD arg1 = i386_get_stack_word(pCPU, 0x6);
    WORD arg2 = i386_get_stack_word(pCPU, 0x4);

    printf("DOS16REALLOCSEG(%04X, %04X)\n", arg1, arg2);

    pCPU->ax = 0;
}

void Win16Initialize(x86_context_t* pCPU) {
	WORD arg1 = i386_get_stack_word(pCPU, 0x4);

	printf("WIN16INITIALIZE(%04X)\n", arg1);

    pCPU->ax = 0;
}

void Win16CreateMsgQueue(x86_context_t* pCPU) {
    WORD arg1 = i386_get_stack_word(pCPU, 0x6);
    WORD arg2 = i386_get_stack_word(pCPU, 0x4);

    printf("WIN16CREATEMSGQUEUE(%04X, %04X)\n", arg1, arg2);

    pCPU->ax = 0;
}


i386_op_func syscall_table[] = {Dos16PutMessage, // 0
                                Dos16Exit, // 1
        Dos16GetHugeShift,  // 2
    Dos16GetMachineMode, // 3
    Dos16GetVersion, // 4
    Dos16GetPID,  // 5
    Vio16WrtTTY, // 6
    NULL, // 7
    NULL, // 8
    NULL, // 9
    NULL, // 10
    NULL, // 11
    Win16CreateMsgQueue, // 12
    NULL, // 13
    NULL, // 14
    NULL, // 15
    NULL, // 16
    NULL, // 17
    NULL, // 18
    Win16Initialize, // 19
    NULL, // 20
    Dos16ReallocSeg, // 21
};


/* Simulate i386 trap */
void do_int(x86_context_t* pCPU, uint8_t vector) {
    switch (vector) {
    case 0x10:
        if (pCPU->ah == 0x0E) printf("%c", pCPU->al);
        return;
    case 0x16: {
        char c = getch();
        pCPU->al = c;
        pCPU->ah = c;
        return;
    }
    case 0x30: {
        if (pCPU->ax >= 22) assert(0);
        syscall_table[pCPU->ax](pCPU);
        return;
    }
    }

    printf("Interrupt vector 0x%02X\n", vector);
    assert(0);
}

int jump_condition(x86_context_t* pCPU, int condition) {
    switch (condition) {
    case JO:
        return GET_FLAG(OVERFLOW);
    case JNO:
        return !GET_FLAG(OVERFLOW);
    case JB:
        return GET_FLAG(CARRY);
    case JAE:
        return !GET_FLAG(CARRY);
    case JE:
        return GET_FLAG(ZERO);
        break;
    case JNE:
        return !GET_FLAG(ZERO);
    case JBE:
        return GET_FLAG(CARRY) || GET_FLAG(ZERO);
    case JA:
        return !GET_FLAG(CARRY) && !GET_FLAG(ZERO);
    case JS:
        return GET_FLAG(SIGN) == 1;
    case JNS:
        return GET_FLAG(SIGN) == 0;
    case JP:
        return GET_FLAG(PARITY) == 1;
    case JNP:
        return GET_FLAG(PARITY) == 0;
    case JL:
        return GET_FLAG(SIGN) != GET_FLAG(OVERFLOW);
    case JGE:
        return GET_FLAG(SIGN) == GET_FLAG(OVERFLOW);
    case JLE:
        return GET_FLAG(ZERO) || (GET_FLAG(SIGN) != GET_FLAG(OVERFLOW));
    case JG:
        return !GET_FLAG(ZERO) && (GET_FLAG(SIGN) == GET_FLAG(OVERFLOW));
    case ALWAYS:
        return 1;
    case JCXZ:
        if (OP_SZ) { //32-bit
            return pCPU->ecx == 0;
        }
        else {
            return pCPU->cx == 0;
        }
        return 0;
    }
}

uint32_t string_src_8(x86_context_t* pCPU) {
    int segment = pCPU->segment_prefix ? pCPU->segment_override : DS;
    uint32_t offset = ADDR_SZ ? pCPU->esi : pCPU->si;

    if (ADDR_SZ) {
        pCPU->esi += GET_FLAG(DIRECTION) ? -1 : 1;
    }
    else {
        pCPU->si += GET_FLAG(DIRECTION) ? -1 : 1;
    }

    return linear(pCPU, segment, offset);
}

uint32_t string_src_16(x86_context_t* pCPU) {
    int segment = pCPU->segment_prefix ? pCPU->segment_override : DS;
    uint32_t offset = ADDR_SZ ? pCPU->esi : pCPU->si;

    if (ADDR_SZ) {
        pCPU->esi += GET_FLAG(DIRECTION) ? -2 : 2;
    }
    else {
        pCPU->si += GET_FLAG(DIRECTION) ? -2 : 2;
    }

    return linear(pCPU, segment, offset);
}

uint32_t string_src_32(x86_context_t* pCPU) {
    int segment = pCPU->segment_prefix ? pCPU->segment_override : DS;
    uint32_t offset = ADDR_SZ ? pCPU->esi : pCPU->si;

    if (ADDR_SZ) {
        pCPU->esi += GET_FLAG(DIRECTION) ? -4 : 4;
    }
    else {
        pCPU->si += GET_FLAG(DIRECTION) ? -4 : 4;
    }

    return linear(pCPU, segment, offset);
}

uint32_t string_dst_8(x86_context_t* pCPU) {
    uint32_t offset = ADDR_SZ ? pCPU->edi : pCPU->di;

    if (ADDR_SZ) {
        pCPU->edi += GET_FLAG(DIRECTION) ? -1 : 1;
    }
    else {
        pCPU->di += GET_FLAG(DIRECTION) ? -1 : 1;
    }

    return linear(pCPU, ES, offset);
}

uint32_t string_dst_16(x86_context_t* pCPU) {
    uint32_t offset = ADDR_SZ ? pCPU->edi : pCPU->di;

    if (ADDR_SZ) {
        pCPU->edi += GET_FLAG(DIRECTION) ? -2 : 2;
    }
    else {
        pCPU->di += GET_FLAG(DIRECTION) ? -2 : 2;
    }

    return linear(pCPU, ES, offset);
}

uint32_t string_dst_32(x86_context_t* pCPU) {
    uint32_t offset = ADDR_SZ ? pCPU->edi : pCPU->di;

    if (ADDR_SZ) {
        pCPU->edi += GET_FLAG(DIRECTION) ? -4 : 4;
    }
    else {
        pCPU->di += GET_FLAG(DIRECTION) ? -4 : 4;
    }

    return linear(pCPU, ES, offset);
}

uint32_t get_offset(x86_context_t* pCPU) {
    return linear(pCPU, pCPU->segment_prefix ? pCPU->segment_override : DS, ADDR_SZ ? fetch32(pCPU) : fetch16(pCPU));
}

uint32_t get_ea(x86_context_t* pCPU, uint8_t modrm) {
    int segment = DS;
    uint32_t offset = ADDR_SZ ? decode_mod_rm_32(pCPU, modrm, &segment) : decode_mod_rm_16(pCPU, modrm, &segment);
    return linear(pCPU, pCPU->segment_prefix ? pCPU->segment_override : segment, offset);
}

uint8_t fetch8(x86_context_t* pCPU) {
    uint8_t value = READ8(linear(pCPU, CS, pCPU->eip));
    pCPU->eip++;
    return value;
}

uint16_t fetch16(x86_context_t* pCPU) {
    uint16_t value = READ16(linear(pCPU, CS, pCPU->eip));
    pCPU->eip += 2;
    return value;
}

uint32_t fetch32(x86_context_t* pCPU) {
    uint32_t value = READ32(linear(pCPU, CS, pCPU->eip));
    pCPU->eip += 4;
    return value;
}

void push16(x86_context_t* pCPU, uint16_t value) {
    pCPU->esp -= 2;
    if (!MODE(pCPU->ss)) pCPU->esp &= 0xFFFF;
    WRITE16(linear(pCPU, SS, pCPU->esp), value);
}

void push32(x86_context_t* pCPU, uint32_t value) {
    pCPU->esp -= 4;
    if (!MODE(pCPU->ss)) pCPU->esp &= 0xFFFF;
    WRITE32(linear(pCPU, SS, pCPU->esp), value);
}

uint16_t pop16(x86_context_t* pCPU) {
    uint16_t value = READ16(linear(pCPU, SS, pCPU->esp));
    pCPU->esp += 2;
    if (!MODE(pCPU->ss)) pCPU->esp &= 0xFFFF;
    return value;
}

uint32_t pop32(x86_context_t* pCPU) {
    uint32_t value = READ32(linear(pCPU, SS, pCPU->esp));
    pCPU->esp += 4;
    if (!MODE(pCPU->ss)) pCPU->esp &= 0xFFFF;
    return value;
}

uint32_t linear(x86_context_t* pCPU, int segment, uint32_t offset) {
    return pCPU->seg_regs[segment].base + offset;
}
