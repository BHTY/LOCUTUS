#include "i386.h"
#include "utils.h"
#include "i386ops.h"
#include <assert.h>
#include <stdio.h>

/* All of the 386 emulation code needs to properly handle GP-faults and other exceptions... */

void i386_far_call(x86_context_t* pCPU, uint16_t sgmt, uint32_t target) {
    if (OP_SZ) {
        PUSH32(pCPU->seg_regs[CS].selector);
        PUSH32(pCPU->eip);
    }
    else {
        PUSH16(pCPU->seg_regs[CS].selector);
        PUSH16(pCPU->ip);
    }

    i386_far_jump(pCPU, sgmt, target);
}

void i386_far_jump(x86_context_t* pCPU, uint16_t sgmt, uint32_t target) {
    if (PE) {
        i386_load_sreg(pCPU, CS, sgmt);
        pCPU->eip = target;
    }
    else {
        pCPU->eip = target;
        i386_load_sreg(pCPU, CS, sgmt);
    }
}

void i386_load_sreg(x86_context_t* pCPU, int segment, uint16_t selector) {
    if (PE) {
        //printf("FIXME: Protected mode segment loads!\n");
        //assert(0);
        pCPU->seg_regs[segment].base = (selector >> 3) << 16;
        //printf("new base addr = %08X\n", pCPU->seg_regs[segment].base);
    }
    else {
        pCPU->seg_regs[segment].base = selector << 4;
    }
    pCPU->seg_regs[segment].selector = selector;
}

void i386_decode_opcode(x86_context_t* pCPU) {
    uint8_t opcode = fetch8(pCPU);
    i386_op_func handler;

    if (OP_SZ)
        handler = x86_opcode_table_1[opcode].handler32;
    else
        handler = x86_opcode_table_1[opcode].handler16;

    if (handler) {
        handler(pCPU);
    }
    else {
        printf("Unknown opcode 0x%02X\n", opcode);
        assert(0);
    }
}

void i386_decode_two_byte(x86_context_t* pCPU) {
    uint8_t opcode = fetch8(pCPU);
    i386_op_func handler;

    if (OP_SZ)
        handler = x86_opcode_table_2[opcode].handler32;
    else
        handler = x86_opcode_table_2[opcode].handler16;

    if (handler) {
        handler(pCPU);
    }
    else {
        printf("Unknown opcode 0x0F 0x%02X\n", opcode);
        assert(0);
    }
}
