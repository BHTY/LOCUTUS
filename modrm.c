#include "i386.h"
#include "utils.h"
#include "common.h"

uint32_t decode_sib_32(x86_context_t* pCPU, uint8_t modrm, int* segment) {
    uint8_t sib = fetch8(pCPU);
    uint32_t offset = 0;

    if (BASE(sib) == 5 && MOD(modrm) == 0) {
        offset = fetch32(pCPU);
    }
    else {
        offset = pCPU->regs[BASE(sib)];

        if (BASE(sib) == 5 || BASE(sib) == 4) {
            if (segment) *segment = SS;
        }
    }

    if (INDEX(sib) != 4) {
        offset += pCPU->regs[INDEX(sib)] * (1 << SCALE(sib));
    }

    return offset;
}

uint32_t decode_mod_rm_32(x86_context_t* pCPU, uint8_t modrm, int* segment) {
    uint8_t mod = MOD(modrm);
    uint8_t rm = RM(modrm);
    uint32_t offset = 0;

    switch (rm) {
    case 4:
        offset = decode_sib_32(pCPU, modrm, segment);
        break;
    case 5:
        if (!mod) {
            offset = fetch32(pCPU);
            break;
        }
        if (segment) *segment = SS;
    default:
        offset = pCPU->regs[rm];
        break;
    }

    if (mod == 1) {
        offset += (int32_t)(int8_t)fetch8(pCPU);
    }
    else if (mod == 2) {
        offset += fetch32(pCPU);
    }

    return offset;
}

uint32_t decode_mod_rm_16(x86_context_t* pCPU, uint8_t modrm, int* segment) {
    uint8_t mod = MOD(modrm);
    uint8_t rm = RM(modrm);
    uint32_t offset = 0;

    switch (rm) {
    case 0:
        offset = pCPU->bx + pCPU->si;
        break;
    case 1:
        offset = pCPU->bx + pCPU->di;
        break;
    case 2:
        offset = pCPU->bp + pCPU->si;
        if (segment) *segment = SS;
        break;
    case 3:
        offset = pCPU->bp + pCPU->di;
        if (segment) *segment = SS;
        break;
    case 4:
        offset = pCPU->si;
        break;
    case 5:
        offset = pCPU->di;
        break;
    case 6:
        offset = mod ? pCPU->bp : fetch16(pCPU);
        break;
    case 7:
        offset = pCPU->bx;
        break;
    }

    if (mod == 1) { // + disp8
        offset += (int16_t)(int8_t)fetch8(pCPU);
    }
    else if (mod == 2) { // + disp16
        offset += fetch16(pCPU);
    }

    return offset & 0xFFFF;
}