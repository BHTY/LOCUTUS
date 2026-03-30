#include "i386ops.h"
#include "modrm.h"
#include "utils.h"
#include "alu.h"
#include <stdint.h>
#include <assert.h>

#define imul_r16_rm16_i16 NULL
#define imul_r32_rm32_i32 NULL
#define imul_r16_rm16_i8 NULL
#define imul_r32_rm32_i8 NULL
#define groupC0_8 NULL
#define groupC1_32 NULL
#define enter32 NULL
#define leave32 NULL
#define groupD0_8 NULL
#define groupD1_16 NULL
#define groupD1_32 NULL
#define groupD2_8 NULL
#define groupD3_16 NULL
#define groupD3_32 NULL

op_rm8_r8(add_rm8_r8, add8);        // Opcode 0x00
op_rm16_r16(add_rm16_r16, add16);   // Opcode 0x01
op_rm32_r32(add_rm32_r32, add32);
op_r8_rm8(add_r8_rm8, add8);        // Opcode 0x02
op_r16_rm16(add_r16_rm16, add16);   // Opcode 0x03
op_r32_rm32(add_r32_rm32, add32);
op_al_i8(add_al_i8, add8);          // Opcode 0x04
op_ax_i16(add_ax_i16, add16);       // Opcode 0x05
op_eax_i32(add_eax_i32, add32);
push_seg16(push_es16, ES);          // Opcode 0x06
push_seg32(push_es32, ES);
pop_seg16(pop_es16, ES);            // Opcode 0x07
pop_seg32(pop_es32, ES);

op_rm8_r8(or_rm8_r8, or8);          // Opcode 0x08
op_rm16_r16(or_rm16_r16, or16);     // Opcode 0x09
op_rm32_r32(or_rm32_r32, or32);
op_r8_rm8(or_r8_rm8, or8);          // Opcode 0x0A
op_r16_rm16(or_r16_rm16, or16);     // Opcode 0x0B
op_r32_rm32(or_r32_rm32, or32);
op_al_i8(or_al_i8, or8);            // Opcode 0x0C
op_ax_i16(or_ax_i16, or16);         // Opcode 0x0D
op_eax_i32(or_eax_i32, or32);
push_seg16(push_cs16, CS);          // Opcode 0x0E
push_seg32(push_cs32, CS);

op_rm8_r8(adc_rm8_r8, adc8);        // Opcode 0x10
op_rm16_r16(adc_rm16_r16, adc16);   // Opcode 0x11
op_rm32_r32(adc_rm32_r32, adc32);
op_r8_rm8(adc_r8_rm8, adc8);        // Opcode 0x12
op_r16_rm16(adc_r16_rm16, adc16);   // Opcode 0x13
op_r32_rm32(adc_r32_rm32, adc32);
op_al_i8(adc_al_i8, adc8);          // Opcode 0x14
op_ax_i16(adc_ax_i16, adc16);       // Opcode 0x15
op_eax_i32(adc_eax_i32, adc32);
push_seg16(push_ss16, SS);          // Opcode 0x16
push_seg32(push_ss32, SS);
pop_seg16(pop_ss16, SS);            // Opcode 0x17
pop_seg32(pop_ss32, SS);

op_rm8_r8(sbb_rm8_r8, sbb8);        // Opcode 0x18
op_rm16_r16(sbb_rm16_r16, sbb16);   // Opcode 0x19
op_rm32_r32(sbb_rm32_r32, sbb32);
op_r8_rm8(sbb_r8_rm8, sbb8);        // Opcode 0x1A
op_r16_rm16(sbb_r16_rm16, sbb16);   // Opcode 0x1B
op_r32_rm32(sbb_r32_rm32, sbb32);
op_al_i8(sbb_al_i8, sbb8);          // Opcode 0x1C
op_ax_i16(sbb_ax_i16, sbb16);       // Opcode 0x1D
op_eax_i32(sbb_eax_i32, sbb32);
push_seg16(push_ds16, DS);          // Opcode 0x1E
push_seg32(push_ds32, DS);
pop_seg16(pop_ds16, DS);            // Opcode 0x1F
pop_seg32(pop_ds32, DS);

op_rm8_r8(and_rm8_r8, and8);        // Opcode 0x20
op_rm16_r16(and_rm16_r16, and16);   // Opcode 0x21
op_rm32_r32(and_rm32_r32, and32);
op_r8_rm8(and_r8_rm8, and8);        // Opcode 0x22
op_r16_rm16(and_r16_rm16, and16);   // Opcode 0x23
op_r32_rm32(and_r32_rm32, and32);
op_al_i8(and_al_i8, and8);          // Opcode 0x24
op_ax_i16(and_ax_i16, and16);       // Opcode 0x25
op_eax_i32(and_eax_i32, and32);
sgmt_override(seg_ES, ES);          // Opcode 0x26
// DAA (Opcode 0x27)

op_rm8_r8(sub_rm8_r8, sub8);        // Opcode 0x28
op_rm16_r16(sub_rm16_r16, sub16);   // Opcode 0x29
op_rm32_r32(sub_rm32_r32, sub32);
op_r8_rm8(sub_r8_rm8, sub8);        // Opcode 0x2A
op_r16_rm16(sub_r16_rm16, sub16);   // Opcode 0x2B
op_r32_rm32(sub_r32_rm32, sub32);
op_al_i8(sub_al_i8, sub8);          // Opcode 0x2C
op_ax_i16(sub_ax_i16, sub16);       // Opcode 0x2D
op_eax_i32(sub_eax_i32, sub32);
sgmt_override(seg_CS, CS);          // Opcode 0x2E
// DAS (Opcode 0x2F)

op_rm8_r8(xor_rm8_r8, xor8);        // Opcode 0x30
op_rm16_r16(xor_rm16_r16, xor16);   // Opcode 0x31
op_rm32_r32(xor_rm32_r32, xor32);
op_r8_rm8(xor_r8_rm8, xor8);        // Opcode 0x32
op_r16_rm16(xor_r16_rm16, xor16);   // Opcode 0x33
op_r32_rm32(xor_r32_rm32, xor32);
op_al_i8(xor_al_i8, xor8);          // Opcode 0x34
op_ax_i16(xor_ax_i16, xor16);       // Opcode 0x35
op_eax_i32(xor_eax_i32, xor32);
sgmt_override(seg_SS, SS);          // Opcode 0x36
// DAA (Opcode 0x37)

op_rm8_r8(cmp_rm8_r8, cmp8);        // Opcode 0x38
op_rm16_r16(cmp_rm16_r16, cmp16);   // Opcode 0x39
op_rm32_r32(cmp_rm32_r32, cmp32);
op_r8_rm8(cmp_r8_rm8, cmp8);        // Opcode 0x3A
op_r16_rm16(cmp_r16_rm16, cmp16);   // Opcode 0x3B
op_r32_rm32(cmp_r32_rm32, cmp32);
op_al_i8(cmp_al_i8, cmp8);          // Opcode 0x3C
op_ax_i16(cmp_ax_i16, cmp16);       // Opcode 0x3D
op_eax_i32(cmp_eax_i32, cmp32);
sgmt_override(seg_DS, DS);          // Opcode 0x3E
// AAS (Opcode 0x3F)

inc_r16(inc_ax, EAX);               // Opcode 0x40
inc_r32(inc_eax, EAX);
inc_r16(inc_cx, ECX);               // Opcode 0x41
inc_r32(inc_ecx, ECX);
inc_r16(inc_dx, EDX);               // Opcode 0x42
inc_r32(inc_edx, EDX);
inc_r16(inc_bx, EBX);               // Opcode 0x43
inc_r32(inc_ebx, EBX);
inc_r16(inc_sp, ESP);               // Opcode 0x44
inc_r32(inc_esp, ESP);
inc_r16(inc_bp, EBP);               // Opcode 0x45
inc_r32(inc_ebp, EBP);
inc_r16(inc_si, ESI);               // Opcode 0x46
inc_r32(inc_esi, ESI);
inc_r16(inc_di, EDI);               // Opcode 0x47
inc_r32(inc_edi, EDI);

dec_r16(dec_ax, EAX);               // Opcode 0x48
dec_r32(dec_eax, EAX);
dec_r16(dec_cx, ECX);               // Opcode 0x49
dec_r32(dec_ecx, ECX);
dec_r16(dec_dx, EDX);               // Opcode 0x4A
dec_r32(dec_edx, EDX);
dec_r16(dec_bx, EBX);               // Opcode 0x4B
dec_r32(dec_ebx, EBX);
dec_r16(dec_sp, ESP);               // Opcode 0x4C
dec_r32(dec_esp, ESP);
dec_r16(dec_bp, EBP);               // Opcode 0x4D
dec_r32(dec_ebp, EBP);
dec_r16(dec_si, ESI);               // Opcode 0x4E
dec_r32(dec_esi, ESI);
dec_r16(dec_di, EDI);               // Opcode 0x4F
dec_r32(dec_edi, EDI);

push_r16(push_ax, EAX);             // Opcode 0x50
push_r32(push_eax, EAX);
push_r16(push_cx, ECX);             // Opcode 0x51
push_r32(push_ecx, ECX);
push_r16(push_dx, EDX);             // Opcode 0x52
push_r32(push_edx, EDX);
push_r16(push_bx, EBX);             // Opcode 0x53
push_r32(push_ebx, EBX);
push_r16(push_sp, ESP);             // Opcode 0x54
push_r32(push_esp, ESP);
push_r16(push_bp, EBP);             // Opcode 0x55
push_r32(push_ebp, EBP);
push_r16(push_si, ESI);             // Opcode 0x56
push_r32(push_esi, ESI);
push_r16(push_di, EDI);             // Opcode 0x57
push_r32(push_edi, EDI);

pop_r16(pop_ax, EAX);             // Opcode 0x58
pop_r32(pop_eax, EAX);
pop_r16(pop_cx, ECX);             // Opcode 0x59
pop_r32(pop_ecx, ECX);
pop_r16(pop_dx, EDX);             // Opcode 0x5A
pop_r32(pop_edx, EDX);
pop_r16(pop_bx, EBX);             // Opcode 0x5B
pop_r32(pop_ebx, EBX);
pop_r16(pop_sp, ESP);             // Opcode 0x5C
pop_r32(pop_esp, ESP);
pop_r16(pop_bp, EBP);             // Opcode 0x5D
pop_r32(pop_ebp, EBP);
pop_r16(pop_si, ESI);             // Opcode 0x5E
pop_r32(pop_esi, ESI);
pop_r16(pop_di, EDI);             // Opcode 0x5F
pop_r32(pop_edi, EDI);

// Opcode 0x60
void pusha(x86_context_t* pCPU) {
    uint16_t temp = REG16(ESP);

    PUSH16(REG16(EAX));
    PUSH16(REG16(ECX));
    PUSH16(REG16(EDX));
    PUSH16(REG16(EBX));
    PUSH16(temp);
    PUSH16(REG16(EBP));
    PUSH16(REG16(ESI));
    PUSH16(REG16(EDI));
}
void pushad(x86_context_t* pCPU) {
    uint32_t temp = REG32(ESP);

    PUSH32(REG32(EAX));
    PUSH32(REG32(ECX));
    PUSH32(REG32(EDX));
    PUSH32(REG32(EBX));
    PUSH32(temp);
    PUSH32(REG32(EBP));
    PUSH32(REG32(ESI));
    PUSH32(REG32(EDI));
}

// Opcode 0x61
void popa(x86_context_t* pCPU) {
    REG16(EDI) = POP16();
    REG16(ESI) = POP16();
    REG16(EBP) = POP16();
    POP16();
    REG16(EBX) = POP16();
    REG16(EDX) = POP16();
    REG16(ECX) = POP16();
    REG16(EAX) = POP16();
}
void popad(x86_context_t* pCPU) {
    REG32(EDI) = POP32();
    REG32(ESI) = POP32();
    REG32(EBP) = POP32();
    POP32();
    REG32(EBX) = POP32();
    REG32(EDX) = POP32();
    REG32(ECX) = POP32();
    REG32(EAX) = POP32();
}

sgmt_override(seg_FS, FS);          // Opcode 0x64
sgmt_override(seg_GS, GS);          // Opcode 0x65

// Opcode 0x66
void operand_size(x86_context_t* pCPU) {
    pCPU->op_override = 1;
    i386_decode_opcode(pCPU);
    pCPU->op_override = 0;
}

// Opcode 0x67
void address_size(x86_context_t* pCPU) {
    pCPU->addr_override = 1;
    i386_decode_opcode(pCPU);
    pCPU->addr_override = 0;
}

// Opcode 0x68
void push_i16(x86_context_t* pCPU) {
    push16(pCPU, fetch16(pCPU));
}
void push_i32(x86_context_t* pCPU) {
    push32(pCPU, fetch32(pCPU));
}

// Opcode 0x69

// Opcode 0x6A
void push_i8_16(x86_context_t* pCPU) {
    uint16_t imm = (int16_t)(int8_t)fetch8(pCPU);
    push16(pCPU, imm);
}
void push_i8_32(x86_context_t* pCPU) {
    uint32_t imm = (int32_t)(int8_t)fetch8(pCPU);
    push32(pCPU, imm);
}

// Opcode 0x6B

// Opcode 0x6C
void insb(x86_context_t* pCPU) {
    uint8_t byte = pCPU->system.io_read_8(pCPU->dx);
    WRITE8(string_dst_8(pCPU), byte);
}

// Opcode 0x6D
void insw(x86_context_t* pCPU) {
    uint16_t word = pCPU->system.io_read_16(pCPU->dx);
    WRITE16(string_dst_16(pCPU), word);
}
void insd(x86_context_t* pCPU) {
    uint32_t dword = pCPU->system.io_read_32(pCPU->dx);
    WRITE32(string_dst_32(pCPU), dword);
}

// Opcode 0x6E
void outsb(x86_context_t* pCPU) {
    uint8_t byte = READ8(string_src_8(pCPU));
    pCPU->system.io_write_8(pCPU->dx, byte);
}

// Opcode 0x6F
void outsw(x86_context_t* pCPU) {
    uint16_t word = READ16(string_src_16(pCPU));
    pCPU->system.io_write_16(pCPU->dx, word);
}
void outsd(x86_context_t* pCPU) {
    uint32_t dword = READ32(string_src_32(pCPU));
    pCPU->system.io_write_32(pCPU->dx, dword);
}

jcc_rel8(jo_rel8, JO);              // Opcode 0x70
jcc_rel8(jno_rel8, JNO);              // Opcode 0x71
jcc_rel8(jc_rel8, JB);              // Opcode 0x72
jcc_rel8(jnc_rel8, JAE);              // Opcode 0x73
jcc_rel8(jz_rel8, JE);              // Opcode 0x74
jcc_rel8(jnz_rel8, JNE);              // Opcode 0x75
jcc_rel8(jbe_rel8, JBE);              // Opcode 0x76
jcc_rel8(ja_rel8, JA);              // Opcode 0x77
jcc_rel8(js_rel8, JS);              // Opcode 0x78
jcc_rel8(jns_rel8, JNS);              // Opcode 0x79
jcc_rel8(jp_rel8, JP);              // Opcode 0x7A
jcc_rel8(jnp_rel8, JNP);              // Opcode 0x7B
jcc_rel8(jl_rel8, JL);              // Opcode 0x7C
jcc_rel8(jge_rel8, JGE);              // Opcode 0x7D
jcc_rel8(jle_rel8, JLE);              // Opcode 0x7E
jcc_rel8(jg_rel8, JG);              // Opcode 0x7F

// Opcode 0x80
void group80_8(x86_context_t* pCPU) { // OP r/m8, i8
    uint32_t ea;
    uint8_t src, dst;
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        dst = LOAD_RM8(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        dst = READ8(ea);
    }

    src = fetch8(pCPU);

    switch (REG(modrm)) {
    case 0:
        dst = add8(pCPU, dst, src);
        break;
    case 1:
        dst = or8(pCPU, dst, src);
        break;
    case 2:
        dst = adc8(pCPU, dst, src);
        break;
    case 3:
        dst = sbb8(pCPU, dst, src);
        break;
    case 4:
        dst = and8(pCPU, dst, src);
        break;
    case 5:
        dst = sub8(pCPU, dst, src);
        break;
    case 6:
        dst = xor8(pCPU, dst, src);
        break;
    case 7:
        dst = cmp8(pCPU, dst, src);
        break;
    }

    if (modrm >= 0xc0) {
        STORE_RM8(modrm, dst);
    }
    else {
        WRITE8(ea, dst);
    }
}

// Opcode 0x81
void group81_16(x86_context_t* pCPU) { // OP r/m16, i16
    uint32_t ea;
    uint16_t src, dst;
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        dst = LOAD_RM16(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        dst = READ16(ea);
    }

    src = fetch16(pCPU);

    switch (REG(modrm)) {
    case 0:
        dst = add16(pCPU, dst, src);
        break;
    case 1:
        dst = or16(pCPU, dst, src);
        break;
    case 2:
        dst = adc16(pCPU, dst, src);
        break;
    case 3:
        dst = sbb16(pCPU, dst, src);
        break;
    case 4:
        dst = and16(pCPU, dst, src);
        break;
    case 5:
        dst = sub16(pCPU, dst, src);
        break;
    case 6:
        dst = xor16(pCPU, dst, src);
        break;
    case 7:
        dst = cmp16(pCPU, dst, src);
        break;
    }

    if (modrm >= 0xc0) {
        STORE_RM16(modrm, dst);
    }
    else {
        WRITE16(ea, dst);
    }
}
void group81_32(x86_context_t* pCPU) { // OP r/m32, i32
    uint32_t ea;
    uint32_t src, dst;
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        dst = LOAD_RM32(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        dst = READ32(ea);
    }

    src = fetch32(pCPU);

    switch (REG(modrm)) {
    case 0:
        dst = add32(pCPU, dst, src);
        break;
    case 1:
        dst = or32(pCPU, dst, src);
        break;
    case 2:
        dst = adc32(pCPU, dst, src);
        break;
    case 3:
        dst = sbb32(pCPU, dst, src);
        break;
    case 4:
        dst = and32(pCPU, dst, src);
        break;
    case 5:
        dst = sub32(pCPU, dst, src);
        break;
    case 6:
        dst = xor32(pCPU, dst, src);
        break;
    case 7:
        dst = cmp32(pCPU, dst, src);
        break;
    }

    if (modrm >= 0xc0) {
        STORE_RM32(modrm, dst);
    }
    else {
        WRITE32(ea, dst);
    }
}

// Opcode 0x83
void group83_16(x86_context_t* pCPU) { // OP r/m16, si8
    uint32_t ea;
    uint16_t src, dst;
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        dst = LOAD_RM16(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        dst = READ16(ea);
    }

    src = (int16_t)(int8_t)fetch8(pCPU);

    switch (REG(modrm)) {
    case 0:
        dst = add16(pCPU, dst, src);
        break;
    case 1:
        dst = or16(pCPU, dst, src);
        break;
    case 2:
        dst = adc16(pCPU, dst, src);
        break;
    case 3:
        dst = sbb16(pCPU, dst, src);
        break;
    case 4:
        dst = and16(pCPU, dst, src);
        break;
    case 5:
        dst = sub16(pCPU, dst, src);
        break;
    case 6:
        dst = xor16(pCPU, dst, src);
        break;
    case 7:
        dst = cmp16(pCPU, dst, src);
        break;
    }

    if (modrm >= 0xc0) {
        STORE_RM16(modrm, dst);
    }
    else {
        WRITE16(ea, dst);
    }
}
void group83_32(x86_context_t* pCPU) { // OP r/m32, si8
    uint32_t ea;
    uint32_t src, dst;
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        dst = LOAD_RM32(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        dst = READ32(ea);
    }

    src = (int32_t)(int16_t)fetch32(pCPU);

    switch (REG(modrm)) {
    case 0:
        dst = add32(pCPU, dst, src);
        break;
    case 1:
        dst = or32(pCPU, dst, src);
        break;
    case 2:
        dst = adc32(pCPU, dst, src);
        break;
    case 3:
        dst = sbb32(pCPU, dst, src);
        break;
    case 4:
        dst = and32(pCPU, dst, src);
        break;
    case 5:
        dst = sub32(pCPU, dst, src);
        break;
    case 6:
        dst = xor32(pCPU, dst, src);
        break;
    case 7:
        dst = cmp32(pCPU, dst, src);
        break;
    }

    if (modrm >= 0xc0) {
        STORE_RM32(modrm, dst);
    }
    else {
        WRITE32(ea, dst);
    }
}

op_rm8_r8(test_rm8_r8, test8);              // Opcode 0x84
op_rm16_r16(test_rm16_r16, test16);         // Opcode 0x85
op_rm32_r32(test_rm32_r32, test32);

// Opcode 0x86
void xchg_r8_rm8(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        uint8_t src = LOAD_RM8(modrm);
        uint8_t dst = LOAD_REG8(modrm);
        STORE_REG8(modrm, src);
        STORE_RM8(modrm, dst);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        uint8_t src = READ8(ea);
        uint8_t dst = LOAD_REG8(modrm);
        WRITE8(ea, dst);
        STORE_REG8(modrm, src);
    }
}

// Opcode 0x87
void xchg_r16_rm16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        uint16_t src = LOAD_RM16(modrm);
        uint16_t dst = LOAD_REG16(modrm);
        STORE_REG16(modrm, src);
        STORE_RM16(modrm, dst);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        uint16_t src = READ16(ea);
        uint16_t dst = LOAD_REG16(modrm);
        WRITE16(ea, dst);
        STORE_REG16(modrm, src);
    }
}
void xchg_r32_rm32(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        uint32_t src = LOAD_RM32(modrm);
        uint32_t dst = LOAD_REG32(modrm);
        STORE_REG32(modrm, src);
        STORE_RM32(modrm, dst);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        uint32_t src = READ32(ea);
        uint32_t dst = LOAD_REG32(modrm);
        WRITE32(ea, dst);
        STORE_REG32(modrm, src);
    }
}

// Opcode 0x88
void mov_rm8_r8(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint8_t src = LOAD_REG8(modrm);

    if (modrm >= 0xc0) {
        STORE_RM8(modrm, src);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        WRITE8(ea, src);
    }
}

// Opcode 0x89
void mov_rm16_r16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint16_t src = LOAD_REG16(modrm);

    if (modrm >= 0xc0) {
        STORE_RM16(modrm, src);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        WRITE16(ea, src);
    }
}
void mov_rm32_r32(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint32_t src = LOAD_REG32(modrm);

    if (modrm >= 0xc0) {
        STORE_RM32(modrm, src);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        WRITE32(ea, src);
    }
}

// Opcode 0x8A
void mov_r8_rm8(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint8_t src;

    if (modrm >= 0xc0) {
        src = LOAD_RM8(modrm);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        src = READ8(ea);
    }

    STORE_REG8(modrm, src);
}

// Opcode 0x8B
void mov_r16_rm16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint16_t src;

    if (modrm >= 0xc0) {
        src = LOAD_RM16(modrm);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        src = READ16(ea);
    }

    STORE_REG16(modrm, src);
}
void mov_r32_rm32(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint32_t src;

    if (modrm >= 0xc0) {
        src = LOAD_RM32(modrm);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        src = READ32(ea);
    }

    STORE_REG32(modrm, src);
}

// Opcode 0x8C
void mov_rm16_sreg(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint16_t src = pCPU->seg_regs[REG(modrm)].selector;

    if (modrm >= 0xc0) {
        STORE_RM16(modrm, src);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        WRITE16(ea, src);
    }
}

// Opcode 0x8D
void lea16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint32_t ea = ADDR_SZ ? decode_mod_rm_32(pCPU, modrm, NULL) : decode_mod_rm_16(pCPU, modrm, NULL);
    STORE_REG16(modrm, ea);
}
void lea32(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint32_t ea = ADDR_SZ ? decode_mod_rm_32(pCPU, modrm, NULL) : decode_mod_rm_16(pCPU, modrm, NULL);
    STORE_REG32(modrm, ea);
}

// Opcode 0x8E
void mov_sreg_rm16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint16_t selector;

    if (modrm >= 0xc0) {
        selector = LOAD_RM16(modrm);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        selector = READ16(ea);
    }

    i386_load_sreg(pCPU, REG(modrm), selector);
}

// Opcode 0x8F
void pop_rm16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint16_t value = pop16(pCPU);

    if (modrm >= 0xc0) {
        STORE_RM16(modrm, value);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        WRITE16(ea, value);
    }
}
void pop_rm32(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint32_t value = pop32(pCPU);

    if (modrm >= 0xc0) {
        STORE_RM32(modrm, value);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        WRITE32(ea, value);
    }
}

// Opcode 0x90
void nop(x86_context_t* pCPU) {
}

xchg_ax_r16(xchg_ax_cx, ECX);       // Opcode 0x91
xchg_eax_r32(xchg_eax_ecx, ECX);
xchg_ax_r16(xchg_ax_dx, EDX);       // Opcode 0x92
xchg_eax_r32(xchg_eax_edx, EDX);
xchg_ax_r16(xchg_ax_bx, EBX);       // Opcode 0x93
xchg_eax_r32(xchg_eax_ebx, EBX);
xchg_ax_r16(xchg_ax_sp, ESP);       // Opcode 0x94
xchg_eax_r32(xchg_eax_esp, ESP);
xchg_ax_r16(xchg_ax_bp, EBP);       // Opcode 0x95
xchg_eax_r32(xchg_eax_ebp, EBP);
xchg_ax_r16(xchg_ax_si, ESI);       // Opcode 0x96
xchg_eax_r32(xchg_eax_esi, ESI);
xchg_ax_r16(xchg_ax_di, EDI);       // Opcode 0x97
xchg_eax_r32(xchg_eax_edi, EDI);

// Opcode 0x98
void cbw(x86_context_t* pCPU) {
    pCPU->ax = (int16_t)(int8_t)(pCPU->al);
}
void cwde(x86_context_t* pCPU) {
    pCPU->eax = (int32_t)(int16_t)(pCPU->ax);
}

// Opcode 0x99
void cwd(x86_context_t* pCPU) {
    if (pCPU->ax & 0x8000) {
        pCPU->dx = 0xFFFF;
    }
    else {
        pCPU->dx = 0x0000;
    }
}
void cdq(x86_context_t* pCPU) {
    if (pCPU->eax & 0x80000000) {
        pCPU->edx = 0xFFFFFFFF;
    }
    else {
        pCPU->edx = 0x00000000;
    }
}

// Opcode 0x9A
void call_abs16(x86_context_t* pCPU) {
    uint16_t offset = fetch16(pCPU);
    uint16_t segment = fetch16(pCPU);

    i386_far_call(pCPU, segment, offset);
}
void call_abs32(x86_context_t* pCPU) {
    uint32_t offset = fetch32(pCPU);
    uint16_t segment = fetch16(pCPU);

    i386_far_call(pCPU, segment, offset);
}

// Opcode 0x9C
void pushf(x86_context_t* pCPU) {
    push16(pCPU, pCPU->flags);
}
void pushfd(x86_context_t* pCPU) {
    push32(pCPU, pCPU->eflags);
}

// Opcode 0x9D
void popf(x86_context_t* pCPU) {
    pCPU->flags = pop16(pCPU);
}
void popfd(x86_context_t* pCPU) {
    pCPU->eflags = pop32(pCPU);
}

// Opcode 0x9E
void sahf(x86_context_t* pCPU) {
    COND_FLAG(CARRY, pCPU->ah & (1 << CARRY));
    COND_FLAG(PARITY, pCPU->ah & (1 << PARITY));
    COND_FLAG(AC, pCPU->ah & (1 << AC));
    COND_FLAG(ZERO, pCPU->ah & (1 << ZERO));
    COND_FLAG(SIGN, pCPU->ah & (1 << SIGN));
}

// Opcode 0x9F
void lahf(x86_context_t* pCPU) {
    pCPU->ah = pCPU->flags;
}

// Opcode 0xA0
void mov_al_m8(x86_context_t* pCPU) {
    uint32_t ea = get_offset(pCPU);
    pCPU->al = READ8(ea);
}

// Opcode 0xA1
void mov_ax_m16(x86_context_t* pCPU) {
    uint32_t ea = get_offset(pCPU);
    pCPU->ax = READ16(ea);
}
void mov_eax_m32(x86_context_t* pCPU) {
    uint32_t ea = get_offset(pCPU);
    pCPU->eax = READ32(ea);
}

// Opcode 0xA2
void mov_m8_al(x86_context_t* pCPU) {
    uint32_t ea = get_offset(pCPU);
    WRITE8(ea, pCPU->al);
}

// Opcode 0xA3
void mov_m16_ax(x86_context_t* pCPU) {
    uint32_t ea = get_offset(pCPU);
    WRITE16(ea, pCPU->ax);
}
void mov_m32_eax(x86_context_t* pCPU) {
    uint32_t ea = get_offset(pCPU);
    WRITE32(ea, pCPU->eax);
}

// Opcode 0xA4
void movsb(x86_context_t* pCPU) {
    uint8_t src = READ8(string_src_8(pCPU));
    WRITE8(string_dst_8(pCPU), src);
}

// Opcode 0xA5
void movsw(x86_context_t* pCPU) {
    uint16_t src = READ16(string_src_16(pCPU));
    WRITE16(string_dst_16(pCPU), src);
}
void movsd(x86_context_t* pCPU) {
    uint32_t src = READ32(string_src_32(pCPU));
    WRITE32(string_dst_32(pCPU), src);
}

// Opcode 0xA6
void cmpsb(x86_context_t* pCPU) {
    uint8_t src = READ8(string_src_8(pCPU));
    uint8_t dst = READ8(string_dst_8(pCPU));
    sub8(pCPU, src, dst);
}

// Opcode 0xA7
void cmpsw(x86_context_t* pCPU) {
    uint16_t src = READ16(string_src_16(pCPU));
    uint16_t dst = READ16(string_dst_16(pCPU));
    sub16(pCPU, src, dst);
}
void cmpsd(x86_context_t* pCPU) {
    uint32_t src = READ32(string_src_32(pCPU));
    uint32_t dst = READ32(string_dst_32(pCPU));
    sub32(pCPU, src, dst);
}

op_al_i8(test_al_i8, test8);            // Opcode 0xA8
op_ax_i16(test_ax_i16, test16);         // Opcode 0xA9
op_eax_i32(test_eax_i32, test32);

// Opcode 0xAA
void stosb(x86_context_t* pCPU) {
    WRITE8(string_dst_8(pCPU), pCPU->al);
}

// Opcode 0xAB
void stosw(x86_context_t* pCPU) {
    WRITE16(string_dst_16(pCPU), pCPU->ax);
}
void stosd(x86_context_t* pCPU) {
    WRITE32(string_dst_32(pCPU), pCPU->eax);
}

// Opcode 0xAC
void lodsb(x86_context_t* pCPU) {
    pCPU->al = READ8(string_src_8(pCPU));
}

// Opcode 0xAD
void lodsw(x86_context_t* pCPU) {
    pCPU->ax = READ16(string_src_16(pCPU));
}
void lodsd(x86_context_t* pCPU) {
    pCPU->eax = READ32(string_src_32(pCPU));
}

// Opcode 0xAE
void scasb(x86_context_t* pCPU) {
    uint32_t ea = string_dst_8(pCPU);
    sub8(pCPU, pCPU->al, READ8(ea));
}

// Opcode 0xAF
void scasw(x86_context_t* pCPU) {
    uint32_t ea = string_dst_16(pCPU);
    sub16(pCPU, pCPU->ax, READ16(ea));
}
void scasd(x86_context_t* pCPU) {
    uint32_t ea = string_dst_32(pCPU);
    sub32(pCPU, pCPU->eax, READ32(ea));
}

mov_r8_i8(mov_al_i8, AL);           // Opcode 0xB0
mov_r8_i8(mov_cl_i8, CL);           // Opcode 0xB1
mov_r8_i8(mov_dl_i8, DL);           // Opcode 0xB2
mov_r8_i8(mov_bl_i8, BL);           // Opcode 0xB3
mov_r8_i8(mov_ah_i8, AH);           // Opcode 0xB4
mov_r8_i8(mov_ch_i8, CH);           // Opcode 0xB5
mov_r8_i8(mov_dh_i8, DH);           // Opcode 0xB6
mov_r8_i8(mov_bh_i8, BH);           // Opcode 0xB7

mov_r16_i16(mov_ax_i16, EAX);       // Opcode 0xB8
mov_r32_i32(mov_eax_i32, EAX);
mov_r16_i16(mov_cx_i16, ECX);       // Opcode 0xB9
mov_r32_i32(mov_ecx_i32, ECX);
mov_r16_i16(mov_dx_i16, EDX);       // Opcode 0xBA
mov_r32_i32(mov_edx_i32, EDX);
mov_r16_i16(mov_bx_i16, EBX);       // Opcode 0xBB
mov_r32_i32(mov_ebx_i32, EBX);
mov_r16_i16(mov_sp_i16, ESP);       // Opcode 0xBC
mov_r32_i32(mov_esp_i32, ESP);
mov_r16_i16(mov_bp_i16, EBP);       // Opcode 0xBD
mov_r32_i32(mov_ebp_i32, EBP);
mov_r16_i16(mov_si_i16, ESI);       // Opcode 0xBE
mov_r32_i32(mov_esi_i32, ESI);
mov_r16_i16(mov_di_i16, EDI);       // Opcode 0xBF
mov_r32_i32(mov_edi_i32, EDI);

// Opcode 0xC1
void groupC1_16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint8_t imm8 = fetch8(pCPU);
    uint16_t rm;
    uint32_t ea;

    if (modrm >= 0xc0) {
        rm = LOAD_RM16(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        rm = READ16(ea);
    }

    switch (REG(modrm)) {
        case 0:
            rm = rol16(pCPU, rm, imm8);
            break;
        case 1:
            rm = ror16(pCPU, rm, imm8);
            break;
        case 2:
            rm = rcl16(pCPU, rm, imm8);
            break;
        case 3:
            rm = rcr16(pCPU, rm, imm8);
            break;
        case 4:
            rm = shl16(pCPU, rm, imm8);
            break;
        case 5:
            rm = shr16(pCPU, rm, imm8);
            break;
        case 6:
            rm = sal16(pCPU, rm, imm8);
            break;
        case 7:
            rm = sar16(pCPU, rm, imm8);
            break;
    }

    if (modrm >= 0xc0) {
        STORE_RM16(modrm, rm);
    }
    else {
        WRITE16(ea, rm);
    }
}

// Opcode 0xC2
void retn16_i16(x86_context_t* pCPU) {
    int16_t disp = fetch16(pCPU);
    pCPU->ip = pop16(pCPU);
    pCPU->sp += (int32_t)disp;
}
void retn32_i16(x86_context_t* pCPU) {
    int16_t disp = fetch16(pCPU);
    pCPU->eip = pop32(pCPU);
    pCPU->esp += (int32_t)disp;
}

// Opcode 0xC3
void retn16(x86_context_t* pCPU) {
    pCPU->ip = pop16(pCPU);
}
void retn32(x86_context_t* pCPU) {
    pCPU->eip = pop32(pCPU);
}

// Opcode 0xC6
void mov_rm8_i8(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        uint8_t value = fetch8(pCPU);
        STORE_RM8(modrm, value);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        uint8_t value = fetch8(pCPU);
        WRITE8(ea, value);
    }
}

// Opcode 0xC7
void mov_rm16_i16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        uint16_t value = fetch16(pCPU);
        STORE_RM16(modrm, value);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        uint16_t value = fetch16(pCPU);
        WRITE16(ea, value);
    }
}
void mov_rm32_i32(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);

    if (modrm >= 0xc0) {
        uint32_t value = fetch32(pCPU);
        STORE_RM32(modrm, value);
    }
    else {
        uint32_t ea = get_ea(pCPU, modrm);
        uint16_t value = fetch32(pCPU);
        WRITE32(ea, value);
    }
}

// Opcode 0xC8
void enter16(x86_context_t* pCPU) {
    uint16_t imm16 = fetch16(pCPU);
	uint8_t imm8 = fetch8(pCPU);
    push16(pCPU, pCPU->bp);
    pCPU->bp = pCPU->sp;
    pCPU->sp -= imm16;
}

// Opcode 0xC9
void leave16(x86_context_t* pCPU) {
    pCPU->sp = pCPU->bp;
    pCPU->bp = pop16(pCPU);
}

// Opcode 0xCA
void retf16_i16(x86_context_t* pCPU) {
    int16_t count = fetch16(pCPU);
    uint16_t offset = pop16(pCPU);
    uint16_t segment = pop16(pCPU);
    i386_far_jump(pCPU, segment, offset);
    pCPU->sp += (int32_t)count;
}
void retf32_i16(x86_context_t* pCPU) {
    int16_t count = fetch16(pCPU);
    uint32_t offset = fetch32(pCPU);
    uint16_t segment = fetch16(pCPU);
    i386_far_jump(pCPU, segment, offset);
    pCPU->esp = (int32_t)count;
}

// Opcode 0xCB
void retf16(x86_context_t* pCPU) {
    uint16_t offset = pop16(pCPU);
    uint16_t segment = pop16(pCPU);
    i386_far_jump(pCPU, segment, offset);
}
void retf32(x86_context_t* pCPU) {
    uint32_t offset = fetch32(pCPU);
    uint16_t segment = fetch16(pCPU);
    i386_far_jump(pCPU, segment, offset);
}

// Opcode 0xCC
void int3(x86_context_t* pCPU) {
    do_int(pCPU, 3);
}

// Opcode 0xCD
void int_imm(x86_context_t* pCPU) {
    do_int(pCPU, fetch8(pCPU));
}

// Opcode 0xCE
void into(x86_context_t* pCPU) {
    if (GET_FLAG(OVERFLOW)) {
        do_int(pCPU, 4);
    }
}

// Opcode 0xCF
void iret(x86_context_t* pCPU) {
    uint16_t ip = fetch16(pCPU);
    uint16_t sel = fetch16(pCPU);
    i386_far_jump(pCPU, sel, ip);
    pCPU->flags = pop16(pCPU);
}
void iretd(x86_context_t* pCPU) {
    uint32_t ip = fetch32(pCPU);
    uint32_t sel = fetch32(pCPU);
    i386_far_jump(pCPU, sel, ip);
    pCPU->eflags = pop32(pCPU);
}

// Opcode 0xD8
void xlat(x86_context_t* pCPU) {
    uint32_t base_addr = ADDR_SZ ? pCPU->ebx : pCPU->bx;
    int segment = pCPU->segment_prefix ? pCPU->segment_override : DS;
    pCPU->al = READ8(linear(pCPU, segment, base_addr + pCPU->al));
}

// Opcode 0xE0
void loopne16(x86_context_t* pCPU) {
    int8_t disp = fetch8(pCPU);
    pCPU->cx--;
    if (!GET_FLAG(ZERO) && pCPU->cx) pCPU->ip += (int16_t)disp;
}
void loopne32(x86_context_t* pCPU) {
    int8_t disp = fetch8(pCPU);
    pCPU->ecx--;
    if (!GET_FLAG(ZERO) && pCPU->ecx) pCPU->eip += (int32_t)disp;
}

// Opcode 0xE1
void loope16(x86_context_t* pCPU) {
    int8_t disp = fetch8(pCPU);
    pCPU->cx--;
    if (GET_FLAG(ZERO) && pCPU->cx) pCPU->ip += (int16_t)disp;
}
void loope32(x86_context_t* pCPU) {
    int8_t disp = fetch8(pCPU);
    pCPU->ecx--;
    if (GET_FLAG(ZERO) && pCPU->ecx) pCPU->eip += (int32_t)disp;
}

// Opcode 0xE2
void loop16(x86_context_t* pCPU) {
    int8_t disp = fetch8(pCPU);
    pCPU->cx--;
    if (pCPU->cx) pCPU->ip += (int16_t)disp;
}
void loop32(x86_context_t* pCPU) {
    int8_t disp = fetch8(pCPU);
    pCPU->ecx--;
    if (pCPU->ecx) pCPU->eip += (int32_t)disp;
}

// Opcode 0xE3
void jcxz16(x86_context_t* pCPU) {
    int8_t disp = fetch8(pCPU);
    if (pCPU->cx == 0) pCPU->ip += (int16_t)disp;
}
void jcxz32(x86_context_t* pCPU) {
    int8_t disp = fetch8(pCPU);
    if (pCPU->ecx == 0) pCPU->eip += (int32_t)disp;
}

// Opcode 0xE4
void in_al_i8(x86_context_t* pCPU) {
    uint8_t imm = fetch8(pCPU);
    pCPU->al = pCPU->system.io_read_8(imm);
}

// Opcode 0xE5
void in_ax_i8(x86_context_t* pCPU) {
    uint8_t imm = fetch8(pCPU);
    pCPU->ax = pCPU->system.io_read_16(imm);
}
void in_eax_i8(x86_context_t* pCPU) {
    uint8_t imm = fetch8(pCPU);
    pCPU->eax = pCPU->system.io_read_32(imm);
}

// Opcode 0xE6
void out_al_i8(x86_context_t* pCPU) {
    uint8_t imm = fetch8(pCPU);
    pCPU->system.io_write_8(imm, pCPU->al);
}

// Opcode 0xE7
void out_ax_i8(x86_context_t* pCPU) {
    uint8_t imm = fetch8(pCPU);
    pCPU->system.io_write_16(imm, pCPU->ax);
}
void out_eax_i8(x86_context_t* pCPU) {
    uint8_t imm = fetch8(pCPU);
    pCPU->system.io_write_32(imm, pCPU->eax);
}

// Opcode 0xE8
void call_rel16(x86_context_t* pCPU) {
    int16_t disp = fetch16(pCPU);
    push16(pCPU, pCPU->eip);
    pCPU->eip += (int32_t)disp;

    if (!MODE(pCPU->cs)) pCPU->eip &= 0xFFFF;
}
void call_rel32(x86_context_t* pCPU) {
    int32_t disp = fetch32(pCPU);
    push32(pCPU, pCPU->eip);
    pCPU->eip += (int32_t)disp;

    if (!MODE(pCPU->cs)) pCPU->eip &= 0xFFFF;
}

// Opcode 0xE9
void jmp_rel16(x86_context_t* pCPU) {
    int16_t disp = fetch16(pCPU);
    pCPU->eip += (int32_t)disp;

    if (!MODE(pCPU->cs)) pCPU->eip &= 0xFFFF;
}
void jmp_rel32(x86_context_t* pCPU) {
    int32_t disp = fetch32(pCPU);
    pCPU->eip += (int32_t)disp;

    if (!MODE(pCPU->cs)) pCPU->eip &= 0xFFFF;
}

// Opcode 0xEA
void jmp_abs16(x86_context_t* pCPU) {
    uint16_t offset = fetch16(pCPU);
    uint16_t segment = fetch16(pCPU);

    i386_far_jump(pCPU, segment, offset);
}
void jmp_abs32(x86_context_t* pCPU) {
    uint32_t offset = fetch32(pCPU);
    uint16_t segment = fetch16(pCPU);

    i386_far_jump(pCPU, segment, offset);
}

jcc_rel8(jmp_rel8, ALWAYS);         // Opcode 0xEB

// Opcode 0xEC
void in_al_dx(x86_context_t* pCPU) {
    pCPU->al = pCPU->system.io_read_8(pCPU->dx);
}

// Opcode 0xED
void in_ax_dx(x86_context_t* pCPU) {
    pCPU->ax = pCPU->system.io_read_16(pCPU->dx);
}
void in_eax_dx(x86_context_t* pCPU) {
    pCPU->eax = pCPU->system.io_read_32(pCPU->dx);
}

// Opcode 0xEE
void out_al_dx(x86_context_t* pCPU) {
    pCPU->system.io_write_8(pCPU->dx, pCPU->al);
}

// Opcode 0xEF
void out_ax_dx(x86_context_t* pCPU) {
    pCPU->system.io_write_16(pCPU->dx, pCPU->ax);
}
void out_eax_dx(x86_context_t* pCPU) {
    pCPU->system.io_write_32(pCPU->dx, pCPU->eax);
}

// Opcode 0xF0
void lock(x86_context_t* pCPU) {
}

// Opcode 0xF2
void repne16(x86_context_t* pCPU) {
    while (pCPU->cx--) {
        i386_decode_opcode(pCPU);
        if (GET_FLAG(ZERO)) break;
    }
}
void repne32(x86_context_t* pCPU) {
    while (pCPU->ecx--) {
        i386_decode_opcode(pCPU);
        if (GET_FLAG(ZERO)) break;
    }
}

// Opcode 0xF3
void repe16(x86_context_t* pCPU) {
    uint8_t opcode = READ8(linear(pCPU, CS, pCPU->eip));

    while (pCPU->cx--) {
        i386_decode_opcode(pCPU);

        if (opcode == 0xA6 || opcode == 0xA7 || opcode == 0xAE || opcode == 0xAF) {
            if (!GET_FLAG(ZERO)) break;
        }
    }
}
void repe32(x86_context_t* pCPU) {
    uint8_t opcode = READ8(linear(pCPU, CS, pCPU->eip));

    while (pCPU->ecx--) {
        i386_decode_opcode(pCPU);

        if (opcode == 0xA6 || opcode == 0xA7 || opcode == 0xAE || opcode == 0xAF) {
            if (!GET_FLAG(ZERO)) break;
        }
    }
}

// Opcode 0xF4
void hlt(x86_context_t* pCPU) {
    pCPU->running = 0;
}

// Opcode 0xF5
void cmc(x86_context_t* pCPU) {
    COND_FLAG(CARRY, !GET_FLAG(CARRY));
}

// Opcode 0xF6
void groupF6_8(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint8_t rm;
    uint32_t ea;

    if (modrm >= 0xc0) {
        rm = LOAD_RM8(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        rm = READ8(ea);
    }

    switch (REG(modrm)) {
    case 0: // TEST r/m8, i8
        and8(pCPU, rm, fetch8(pCPU));
        break;
    case 2: // NOT r/m8
        rm = ~rm;

        if (modrm >= 0xc0) {
            STORE_RM8(modrm, rm);
        }
        else {
            WRITE8(ea, rm);
        }

        break;
    case 3: // NEG r/m8
        rm = sub8(pCPU, 0, rm);

        if (modrm >= 0xc0) {
            STORE_RM8(modrm, rm);
        }
        else {
            WRITE8(ea, rm);
        }

        break;
    case 4: // MUL AL, r/m8
        pCPU->ax = mul8(pCPU, pCPU->al, rm);
        break;
    case 5: // IMUL AL, r/m8
        pCPU->ax = imul8(pCPU, pCPU->al, rm);
        break;
    case 6: // DIV AX, r/m8
        pCPU->ax = div8(pCPU, pCPU->ax, rm);
        break;
    case 7: // IDIV AX, r/m8
        pCPU->ax = idiv8(pCPU, pCPU->ax, rm);
        break;
    }
}

// Opcode 0xF7
void groupF7_16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint16_t rm;
    uint32_t ea;

    if (modrm >= 0xc0) {
        rm = LOAD_RM16(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        rm = READ16(ea);
    }

    switch (REG(modrm)) {
    case 0: // TEST r/m16, i16
        and16(pCPU, rm, fetch16(pCPU));
        break;
    case 2: // NOT r/m16
        rm = ~rm;

        if (modrm >= 0xc0) {
            STORE_RM16(modrm, rm);
        }
        else {
            WRITE16(ea, rm);
        }

        break;
    case 3: // NEG r/m16
        rm = sub16(pCPU, 0, rm);

        if (modrm >= 0xc0) {
            STORE_RM16(modrm, rm);
        }
        else {
            WRITE16(ea, rm);
        }

        break;
    case 4: // MUL AX, r/m16
    {
        uint32_t result = mul16(pCPU, pCPU->ax, rm);
        pCPU->ax = result;
        pCPU->dx = result >> 16;
        break;
    }
    case 5: // IMUL AX, r/m16
    {
        uint32_t result = imul16(pCPU, pCPU->ax, rm);
        pCPU->ax = result;
        pCPU->dx = result >> 16;
        break;
    }
    case 6: // DIV DX:AX, r/m16
    {
        uint32_t dividend = pCPU->ax | (pCPU->dx << 16);
        uint32_t result = div16(pCPU, dividend, rm);
        pCPU->ax = result; // quotient
        pCPU->dx = result >> 16; // remainder
        break;
    }
    case 7: // IDIV DX:AX, r/m16
    {
        uint32_t dividend = pCPU->ax | (pCPU->dx << 16);
        uint32_t result = idiv16(pCPU, dividend, rm);
        pCPU->ax = result; // quotient
        pCPU->dx = result >> 16; // remainder
        break;
    }
    }
}
void groupF7_32(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint32_t rm;
    uint32_t ea;

    if (modrm >= 0xc0) {
        rm = LOAD_RM32(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        rm = READ32(ea);
    }

    switch (REG(modrm)) {
    case 0: // TEST r/m32, i32
        and32(pCPU, rm, fetch32(pCPU));
        break;
    case 2: // NOT r/m32
        rm = ~rm;

        if (modrm >= 0xc0) {
            STORE_RM32(modrm, rm);
        }
        else {
            WRITE32(ea, rm);
        }

        break;
    case 3: // NEG r/m32
        rm = sub32(pCPU, 0, rm);

        if (modrm >= 0xc0) {
            STORE_RM32(modrm, rm);
        }
        else {
            WRITE32(ea, rm);
        }

        break;
    case 4: // MUL EAX, r/m32
    {
        uint64_t result = mul32(pCPU, pCPU->eax, rm);
        pCPU->eax = result;
        pCPU->edx = result >> 32;
        break;
    }
    case 5: // IMUL EAX, r/m32
    {
        uint64_t result = imul32(pCPU, pCPU->eax, rm);
        pCPU->eax = result;
        pCPU->edx = result >> 32;
        break;
    }
    case 6: // DIV EDX:EAX, r/m32
    {
        uint64_t dividend = pCPU->eax | ((uint64_t)pCPU->edx << 32);
        uint64_t result = div32(pCPU, dividend, rm);
        pCPU->eax = result; // quotient
        pCPU->edx = result >> 32; // remainder
        break;
    }
    case 7: // IDIV DX:AX, r/m16
    {
        uint64_t dividend = pCPU->eax | ((uint64_t)pCPU->edx << 32);
        uint64_t result = idiv32(pCPU, dividend, rm);
        pCPU->eax = result; // quotient
        pCPU->edx = result >> 32; // remainder
        break;
    }
    }
}

clr_flag(clc, CARRY);               // Opcode 0xF8
set_flag(stc, CARRY);               // Opcode 0xF9
clr_flag(cli, INTERRUPT);               // Opcode 0xFA
set_flag(sti, INTERRUPT);               // Opcode 0xFB
clr_flag(cld, DIRECTION);               // Opcode 0xFC
set_flag(std, DIRECTION);               // Opcode 0xFD

// Opcode 0xFE
void groupFE_8(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint8_t dst;
    uint32_t ea;

    if (modrm >= 0xc0) {
        dst = LOAD_RM8(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        dst = READ8(ea);
    }

    switch (REG(modrm)) {
    case 0: // INC r/m8
        dst = inc8(pCPU, dst);
        break;
    case 1: // DEC r/m8
        dst = dec8(pCPU, dst);
        break;
    default:
        assert(0);
        break;
    }

    if (modrm >= 0xc0) {
        STORE_RM8(modrm, dst);
    }
    else {
        WRITE8(ea, dst);
    }
}

// Opcode 0xFF
void groupFF_16(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint32_t ea;
    uint16_t rm;

    if (modrm >= 0xc0) {
        rm = LOAD_RM16(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        rm = READ16(ea);
    }

    switch (REG(modrm)) {
    case 0: // INC r/m16
        rm = inc16(pCPU, rm);

        if (modrm >= 0xc0) {
            STORE_RM16(modrm, rm);
        }
        else {
            WRITE16(ea, rm);
        }

        break;
    case 1: // DEC r/m16
        rm = dec16(pCPU, rm);

        if (modrm >= 0xc0) {
            STORE_RM16(modrm, rm);
        }
        else {
            WRITE16(ea, rm);
        }

        break;
    case 2: // CALL r/m16
        push16(pCPU, pCPU->ip);
        pCPU->ip = rm;
        break;
    case 3: // CALL FAR r/m16
    {
        uint16_t selector = READ16(ea + 2);
        i386_far_call(pCPU, selector, rm);
        break;
    }
    case 4: // JMP r/m16
        pCPU->eip = rm;
        break;
    case 5: // JMP FAR r/m16
    {
        uint16_t selector = READ16(ea + 2);
        i386_far_jump(pCPU, selector, rm);
        break;
    }
    case 6: // PUSH r/m16
        push16(pCPU, rm);
        break;
    }
}
void groupFF_32(x86_context_t* pCPU) {
    uint8_t modrm = fetch8(pCPU);
    uint32_t ea;
    uint32_t rm;

    if (modrm >= 0xc0) {
        rm = LOAD_RM32(modrm);
    }
    else {
        ea = get_ea(pCPU, modrm);
        rm = READ32(ea);
    }

    switch (REG(modrm)) {
    case 0: // INC r/m32
        rm = inc32(pCPU, rm);

        if (modrm >= 0xc0) {
            STORE_RM32(modrm, rm);
        }
        else {
            WRITE32(ea, rm);
        }

        break;
    case 1: // DEC r/m32
        rm = dec32(pCPU, rm);

        if (modrm >= 0xc0) {
            STORE_RM32(modrm, rm);
        }
        else {
            WRITE32(ea, rm);
        }

        break;
    case 2: // CALL r/m32
        push32(pCPU, pCPU->eip);
        pCPU->eip = rm;
        break;
    case 3: // CALL FAR r/m32
    {
        uint16_t selector = READ16(ea + 4);
        i386_far_call(pCPU, selector, rm);
        break;
    }
    case 4: // JMP r/m32
        pCPU->eip = rm;
        break;
    case 5: // JMP FAR r/m32
    {
        uint16_t selector = READ16(ea + 4);
        i386_far_jump(pCPU, selector, rm);
        break;
    }
    case 6: // PUSH r/m32
        push32(pCPU, rm);
        break;
    }
}

x86_opcode_t x86_opcode_table_1[256] = {
    // 0x00
    {add_rm8_r8, add_rm8_r8},
    {add_rm16_r16, add_rm32_r32},
    {add_r8_rm8, add_r8_rm8},
    {add_r16_rm16, add_r32_rm32},
    {add_al_i8, add_al_i8},
    {add_ax_i16, add_eax_i32},
    {push_es16, push_es32},
    {pop_es16, pop_es32},

    {or_rm8_r8, or_rm8_r8},
    {or_rm16_r16, or_rm32_r32},
    {or_r8_rm8, or_r8_rm8},
    {or_r16_rm16, or_r32_rm32},
    {or_al_i8, or_al_i8},
    {or_ax_i16, or_eax_i32},
    {push_cs16, push_cs32},
    {i386_decode_two_byte, i386_decode_two_byte},

    // 0x10
    {adc_rm8_r8, adc_rm8_r8},
    {adc_rm16_r16, adc_rm32_r32},
    {adc_r8_rm8, adc_r8_rm8},
    {adc_r16_rm16, adc_r32_rm32},
    {adc_al_i8, adc_al_i8},
    {adc_ax_i16, adc_eax_i32},
    {push_ss16, push_ss32},
    {pop_ss16, pop_ss32},

    {sbb_rm8_r8, sbb_rm8_r8},
    {sbb_rm16_r16, sbb_rm32_r32},
    {sbb_r8_rm8, sbb_r8_rm8},
    {sbb_r16_rm16, sbb_r32_rm32},
    {sbb_al_i8, sbb_al_i8},
    {sbb_ax_i16, sbb_eax_i32},
    {push_ds16, push_ds32},
    {pop_ds16, pop_ds32},

    // 0x20
    {and_rm8_r8, and_rm8_r8},
    {and_rm16_r16, and_rm32_r32},
    {and_r8_rm8, and_r8_rm8},
    {and_r16_rm16, and_r32_rm32},
    {and_al_i8, and_al_i8},
    {and_ax_i16, and_eax_i32},
    {seg_ES, seg_ES},
    {0, 0},

    {sub_rm8_r8, sub_rm8_r8},
    {sub_rm16_r16, sub_rm32_r32},
    {sub_r8_rm8, sub_r8_rm8},
    {sub_r16_rm16, sub_r32_rm32},
    {sub_al_i8, sub_al_i8},
    {sub_ax_i16, sub_eax_i32},
    {seg_CS, seg_CS},
    {0, 0},

    // 0x30
    {xor_rm8_r8, xor_rm8_r8},
    {xor_rm16_r16, xor_rm32_r32},
    {xor_r8_rm8, xor_r8_rm8},
    {xor_r16_rm16, xor_r32_rm32},
    {xor_al_i8, xor_al_i8},
    {xor_ax_i16, xor_eax_i32},
    {seg_SS, seg_SS},
    {0, 0},

    {cmp_rm8_r8, cmp_rm8_r8},
    {cmp_rm16_r16, cmp_rm32_r32},
    {cmp_r8_rm8, cmp_r8_rm8},
    {cmp_r16_rm16, cmp_r32_rm32},
    {cmp_al_i8, cmp_al_i8},
    {cmp_ax_i16, cmp_eax_i32},
    {seg_DS, seg_DS},
    {0, 0},

    // 0x40
    {inc_ax, inc_eax},
    {inc_cx, inc_ecx},
    {inc_dx, inc_edx},
    {inc_bx, inc_ebx},
    {inc_sp, inc_esp},
    {inc_bp, inc_ebp},
    {inc_si, inc_esi},
    {inc_di, inc_edi},

    {dec_ax, dec_eax},
    {dec_cx, dec_ecx},
    {dec_dx, dec_edx},
    {dec_bx, dec_ebx},
    {dec_sp, dec_esp},
    {dec_bp, dec_ebp},
    {dec_si, dec_esi},
    {dec_di, dec_edi},

    // 0x50
    {push_ax, push_eax},
    {push_cx, push_ecx},
    {push_dx, push_edx},
    {push_bx, push_ebx},
    {push_sp, push_esp},
    {push_bp, push_ebp},
    {push_si, push_esi},
    {push_di, push_edi},

    {pop_ax, pop_eax},
    {pop_cx, pop_ecx},
    {pop_dx, pop_edx},
    {pop_bx, pop_ebx},
    {pop_sp, pop_esp},
    {pop_bp, pop_ebp},
    {pop_si, pop_esi},
    {pop_di, pop_edi},

    // 0x60
    {pusha, pushad},
    {popa, popad},
    {0, 0},
    {0, 0},
    {seg_FS, seg_FS},
    {seg_GS, seg_GS},
    {operand_size, operand_size},
    {address_size, address_size},

    {push_i16, push_i32},
    {imul_r16_rm16_i16, imul_r32_rm32_i32},
    {push_i8_16, push_i8_32},
    {imul_r16_rm16_i8, imul_r32_rm32_i8},
    {insb, insb},
    {insw, insd},
    {outsb, outsb},
    {outsw, outsd},

    // 0x70
    {jo_rel8, jo_rel8},
    {jno_rel8, jno_rel8},
    {jc_rel8, jc_rel8},
    {jnc_rel8, jnc_rel8},
    {jz_rel8, jz_rel8},
    {jnz_rel8, jnz_rel8},
    {jbe_rel8, jbe_rel8},
    {ja_rel8, ja_rel8},

    {js_rel8, js_rel8},
    {jns_rel8, jns_rel8},
    {jp_rel8, jp_rel8},
    {jnp_rel8, jnp_rel8},
    {jl_rel8, jl_rel8},
    {jge_rel8, jge_rel8},
    {jle_rel8, jle_rel8},
    {jg_rel8, jg_rel8},

    // 0x80
    {group80_8, group80_8},
    {group81_16, group81_32},
    {group80_8, group80_8},
    {group83_16, group83_32},
    {test_rm8_r8, test_rm8_r8},
    {test_rm16_r16, test_rm32_r32},
    {xchg_r8_rm8, xchg_r8_rm8},
    {xchg_r16_rm16, xchg_r32_rm32},

    {mov_rm8_r8, mov_rm8_r8},
    {mov_rm16_r16, mov_rm32_r32},
    {mov_r8_rm8, mov_r8_rm8},
    {mov_r16_rm16, mov_r32_rm32},
    {mov_rm16_sreg, mov_rm16_sreg},
    {lea16, lea32},
    {mov_sreg_rm16, mov_sreg_rm16},
    {pop_rm16, pop_rm32},

    // 0x90
    {nop, nop},
    {xchg_ax_cx, xchg_eax_ecx},
    {xchg_ax_dx, xchg_eax_edx},
    {xchg_ax_bx, xchg_eax_ebx},
    {xchg_ax_sp, xchg_eax_esp},
    {xchg_ax_bp, xchg_eax_ebp},
    {xchg_ax_si, xchg_eax_esi},
    {xchg_ax_di, xchg_eax_edi},

    {cbw, cwde},
    {cwd, cdq},
    {call_abs16, call_abs32},
    {0, 0},
    {pushf, pushfd},
    {popf, popfd},
    {sahf, sahf},
    {lahf, lahf},

    // 0xA0
    {mov_al_m8, mov_al_m8},
    {mov_ax_m16, mov_eax_m32},
    {mov_m8_al, mov_m8_al},
    {mov_m16_ax, mov_m32_eax},
    {movsb, movsb},
    {movsw, movsd},
    {cmpsb, cmpsb},
    {cmpsw, cmpsd},

    {test_al_i8, test_al_i8},
    {test_ax_i16, test_eax_i32},
    {stosb, stosb},
    {stosw, stosd},
    {lodsb, lodsb},
    {lodsw, lodsd},
    {scasb, scasb},
    {scasw, scasd},

    // 0xB0
    {mov_al_i8, mov_al_i8},
    {mov_cl_i8, mov_cl_i8},
    {mov_dl_i8, mov_dl_i8},
    {mov_bl_i8, mov_bl_i8},
    {mov_ah_i8, mov_ah_i8},
    {mov_ch_i8, mov_ch_i8},
    {mov_dh_i8, mov_dh_i8},
    {mov_bh_i8, mov_dh_i8},

    {mov_ax_i16, mov_eax_i32},
    {mov_cx_i16, mov_ecx_i32},
    {mov_dx_i16, mov_edx_i32},
    {mov_bx_i16, mov_ebx_i32},
    {mov_sp_i16, mov_esp_i32},
    {mov_bp_i16, mov_ebp_i32},
    {mov_si_i16, mov_esi_i32},
    {mov_di_i16, mov_edi_i32},

    // 0xC0
    {groupC0_8, groupC0_8},
    {groupC1_16, groupC1_32},
    {retn16_i16, retn32_i16},
    {retn16, retn32},
    {0, 0},
    {0, 0},
    {mov_rm8_i8, mov_rm8_i8},
    {mov_rm16_i16, mov_rm32_i32},

    {enter16, enter32},
    {leave16, leave32},
    {retf16_i16, retf32_i16},
    {retf16, retf32},
    {int3, int3},
    {int_imm, int_imm},
    {into, into},
    {iret, iretd},

    // 0xD0
    {groupD0_8, groupD0_8},
    {groupD1_16, groupD1_32},
    {groupD2_8, groupD2_8},
    {groupD3_16, groupD3_32},
    {0, 0},
    {0, 0},
    {0, 0},
    {xlat, xlat},

    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},

    // 0xE0
    {loopne16, loopne32},
    {loope16, loope32},
    {loop16, loop32},
    {jcxz16, jcxz32},
    {in_al_i8, in_al_i8},
    {in_ax_i8, in_eax_i8},
    {out_al_i8, out_al_i8},
    {out_ax_i8, out_eax_i8},

    {call_rel16, call_rel32},
    {jmp_rel16, jmp_rel32},
    {jmp_abs16, jmp_abs32},
    {jmp_rel8, jmp_rel8},
    {in_al_dx, in_al_dx},
    {in_ax_dx, in_eax_dx},
    {out_al_dx, out_al_dx},
    {out_ax_dx, out_eax_dx},

    // 0xF0
    {lock, lock},
    {0, 0},
    {repne16, repne32},
    {repe16, repe32},
    {hlt, hlt},
    {cmc, cmc},
    {groupF6_8, groupF6_8},
    {groupF7_16, groupF7_32},

    {clc, clc},
    {stc, stc},
    {cli, cli},
    {sti, sti},
    {cld, cld},
    {std, std},
    {groupFE_8, groupFE_8},
    {groupFF_16, groupFF_32},
};

x86_opcode_t x86_opcode_table_2[256] = { 0 };
// MOVSX
// MOVZX
// BSWAP
// BSR
// BSF
// BTC
// BTR
// IMUL
// SHRD
// BTS
// POP GS
// POP FS
// PUSH FS
// PUSH GS
// BT
// SETcc
// Jcc
