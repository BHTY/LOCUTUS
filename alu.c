#include "alu.h"

uint8_t _adc8(x86_context_t* pCPU, uint8_t dst, uint8_t src, uint8_t c) {
    uint16_t res = (uint16_t)dst + (uint16_t)src + (uint16_t)c;
    SetCF8(res);
    SetOF_Add8(res, src, dst);
    SetAF(res, src, dst);
    SetSZPF8(res);
    return res;
}
uint16_t _adc16(x86_context_t* pCPU, uint16_t dst, uint16_t src, uint8_t c) {
    uint32_t res = (uint32_t)dst + (uint32_t)src + (uint32_t)c;
    SetCF16(res);
    SetOF_Add16(res, src, dst);
    SetAF(res, src, dst);
    SetSZPF16(res);
    return res;
}
uint32_t _adc32(x86_context_t* pCPU, uint32_t dst, uint32_t src, uint8_t c) {
    uint64_t res = (uint64_t)dst + (uint64_t)src + (uint64_t)c;
    SetCF32(res);
    SetOF_Add32(res, src, dst);
    SetAF(res, src, dst);
    SetSZPF32(res);
    return res;
}

uint8_t or8(x86_context_t* pCPU, uint8_t dst, uint8_t src) {
    uint8_t res = dst | src;
    CLEAR_FLAG(CARRY);
    CLEAR_FLAG(OVERFLOW);
    SetSZPF8(res);
    return res;
}
uint16_t or16(x86_context_t* pCPU, uint16_t dst, uint16_t src) {
    uint16_t res = dst | src;
    CLEAR_FLAG(CARRY);
    CLEAR_FLAG(OVERFLOW);
    SetSZPF16(res);
    return res;
}
uint32_t or32(x86_context_t* pCPU, uint32_t dst, uint32_t src) {
    uint32_t res = dst | src;
    CLEAR_FLAG(CARRY);
    CLEAR_FLAG(OVERFLOW);
    SetSZPF32(res);
    return res;
}

uint8_t _sbb8(x86_context_t* pCPU, uint8_t dst, uint8_t src, uint8_t b) {
    uint16_t res = (uint16_t)dst - (uint16_t)src - (uint16_t)b;
    SetCF8(res);
    SetOF_Sub8(res, src, dst);
    SetAF(res, src, dst);
    SetSZPF8(res);
    return res;
}
uint16_t _sbb16(x86_context_t* pCPU, uint16_t dst, uint16_t src, uint8_t b) {
    uint32_t res = (uint32_t)dst - (uint32_t)src - (uint16_t)b;
    SetCF16(res);
    SetOF_Sub16(res, src, dst);
    SetAF(res, src, dst);
    SetSZPF16(res);
    return res;
}
uint32_t _sbb32(x86_context_t* pCPU, uint32_t dst, uint32_t src, uint8_t b) {
    uint64_t res = (uint64_t)dst - (uint64_t)src - (uint64_t)b;
    SetCF32(res);
    SetOF_Sub32(res, src, dst);
    SetAF(res, src, dst);
    SetSZPF32(res);
    return res;
}

uint8_t cmp8(x86_context_t* pCPU, uint8_t dst, uint8_t src) {
    sub8(pCPU, dst, src);
    return dst;
}
uint16_t cmp16(x86_context_t* pCPU, uint16_t dst, uint16_t src) {
    sub16(pCPU, dst, src);
    return dst;
}
uint32_t cmp32(x86_context_t* pCPU, uint32_t dst, uint32_t src) {
    sub32(pCPU, dst, src);
    return dst;
}

uint8_t test8(x86_context_t* pCPU, uint8_t dst, uint8_t src) {
    and8(pCPU, dst, src);
    return dst;
}
uint16_t test16(x86_context_t* pCPU, uint16_t dst, uint16_t src) {
    and16(pCPU, dst, src);
    return dst;
}
uint32_t test32(x86_context_t* pCPU, uint32_t dst, uint32_t src) {
    and32(pCPU, dst, src);
    return dst;
}

uint8_t and8(x86_context_t* pCPU, uint8_t dst, uint8_t src) {
    uint8_t res = dst & src;
    CLEAR_FLAG(CARRY);
    CLEAR_FLAG(OVERFLOW);
    SetSZPF8(res);
    return res;
}
uint16_t and16(x86_context_t* pCPU, uint16_t dst, uint16_t src) {
    uint16_t res = dst & src;
    CLEAR_FLAG(CARRY);
    CLEAR_FLAG(OVERFLOW);
    SetSZPF16(res);
    return res;
}
uint32_t and32(x86_context_t* pCPU, uint32_t dst, uint32_t src) {
    uint32_t res = dst & src;
    CLEAR_FLAG(CARRY);
    CLEAR_FLAG(OVERFLOW);
    SetSZPF32(res);
    return res;
}

uint8_t xor8(x86_context_t* pCPU, uint8_t dst, uint8_t src) {
    uint8_t res = dst ^ src;
    CLEAR_FLAG(CARRY);
    CLEAR_FLAG(OVERFLOW);
    SetSZPF8(res);
    return res;
}
uint16_t xor16(x86_context_t* pCPU, uint16_t dst, uint16_t src) {
    uint16_t res = dst ^ src;
    CLEAR_FLAG(CARRY);
    CLEAR_FLAG(OVERFLOW);
    SetSZPF16(res);
    return res;
}
uint32_t xor32(x86_context_t* pCPU, uint32_t dst, uint32_t src) {
    uint32_t res = dst ^ src;
    CLEAR_FLAG(CARRY);
    CLEAR_FLAG(OVERFLOW);
    SetSZPF32(res);
    return res;
}

uint8_t inc8(x86_context_t* pCPU, uint8_t dst) {
    uint16_t res = (uint16_t)dst + 1;
    SetOF_Add8(res, 1, dst);
    SetAF(res, 1, dst);
    SetSZPF8(res);
    return res;
}
uint16_t inc16(x86_context_t* pCPU, uint16_t dst) {
    uint32_t res = (uint32_t)dst + 1;
    SetOF_Add16(res, 1, dst);
    SetAF(res, 1, dst);
    SetSZPF16(res);
    return res;
}
uint32_t inc32(x86_context_t* pCPU, uint32_t dst) {
    uint64_t res = (uint64_t)dst + 1;
    SetOF_Add32(res, 1, dst);
    SetAF(res, 1, dst);
    SetSZPF32(res);
    return res;
}

uint8_t dec8(x86_context_t* pCPU, uint8_t dst) {
    uint16_t res = (uint16_t)dst - 1;
    SetOF_Sub8(res, 1, dst);
    SetAF(res, 1, dst);
    SetSZPF8(res);
    return res;
}
uint16_t dec16(x86_context_t* pCPU, uint16_t dst) {
    uint32_t res = (uint32_t)dst - 1;
    SetOF_Sub16(res, 1, dst);
    SetAF(res, 1, dst);
    SetSZPF16(res);
    return res;
}
uint32_t dec32(x86_context_t* pCPU, uint32_t dst) {
    uint64_t res = (uint64_t)dst - 1;
    SetOF_Sub32(res, 1, dst);
    SetAF(res, 1, dst);
    SetSZPF32(res);
    return res;
}


uint16_t mul8(x86_context_t* pCPU, uint8_t dst, uint8_t src) {
    uint16_t res = (uint16_t)dst * (uint16_t)src;
    COND_FLAG(CARRY, res > 0xFF);
    COND_FLAG(OVERFLOW, res > 0xFF);
    return res;
}
uint32_t mul16(x86_context_t* pCPU, uint16_t dst, uint16_t src) {
    uint32_t res = (uint32_t)dst * (uint32_t)src;
    COND_FLAG(CARRY, res > 0xFFFF);
    COND_FLAG(OVERFLOW, res > 0xFFFF);
    return res;
}
uint64_t mul32(x86_context_t* pCPU, uint32_t dst, uint32_t src) {
    uint64_t res = (uint64_t)dst * (uint64_t)src;
    COND_FLAG(CARRY, res > 0xFFFFFFFF);
    COND_FLAG(OVERFLOW, res > 0xFFFFFFFF);
    return res;
}

int16_t imul8(x86_context_t* pCPU, int8_t dst, int8_t src) {
    int16_t res = (int16_t)dst * (int16_t)src;
    COND_FLAG(CARRY, !(res == (int16_t)(int8_t)res));
    COND_FLAG(OVERFLOW, !(res == (int16_t)(int8_t)res));
    return res;
}
int32_t imul16(x86_context_t* pCPU, int16_t dst, int16_t src) {
    int32_t res = (int32_t)dst * (int32_t)src;
    COND_FLAG(CARRY, !(res == (int32_t)(int16_t)res));
    COND_FLAG(OVERFLOW, !(res == (int32_t)(int16_t)res));
    return res;
}
int64_t imul32(x86_context_t* pCPU, int32_t dst, int32_t src) {
    int64_t res = (int64_t)dst * (int64_t)src;
    COND_FLAG(CARRY, !(res == (int64_t)(int32_t)res));
    COND_FLAG(OVERFLOW, !(res == (int64_t)(int32_t)res));
    return res;
}

uint16_t div8(x86_context_t* pCPU, uint16_t dst, uint8_t src) {
    uint8_t quotient = dst / src;
    uint8_t remainder = dst % src;
    return ((uint16_t)remainder << 8) | quotient;
}
uint32_t div16(x86_context_t* pCPU, uint32_t dst, uint16_t src) {
    uint16_t quotient = dst / src;
    uint16_t remainder = dst % src;
    return ((uint32_t)remainder << 16) | quotient;
}
uint64_t div32(x86_context_t* pCPU, uint64_t dst, uint32_t src) {
    uint32_t quotient = dst / src;
    uint32_t remainder = dst % src;
    return ((uint64_t)remainder << 32) | quotient;
}

int16_t idiv8(x86_context_t* pCPU, int16_t dst, int8_t src) {
    int8_t quotient = dst / src;
    int8_t remainder = dst % src;
    return ((int16_t)remainder << 8) | quotient;
}
int32_t idiv16(x86_context_t* pCPU, int32_t dst, int16_t src) {
    int16_t quotient = dst / src;
    int16_t remainder = dst % src;
    return ((int32_t)remainder << 16) | quotient;
}
int64_t idiv32(x86_context_t* pCPU, int64_t dst, int32_t src) {
    int32_t quotient = dst / src;
    int32_t remainder = dst % src;
    return ((int64_t)remainder << 32) | quotient;
}

uint8_t shr8(x86_context_t* pCPU, uint8_t dst, uint8_t count) {
    uint8_t res = dst >> count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & ((1 << count - 1)));
        SetSZPF8(res);
    }
    return res;
}
uint16_t shr16(x86_context_t* pCPU, uint16_t dst, uint8_t count) {
    uint16_t res = dst >> count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & ((1 << count - 1)));
        SetSZPF16(res);
    }
    return res;
}
uint32_t shr32(x86_context_t* pCPU, uint32_t dst, uint8_t count) {
    uint32_t res = dst >> count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (count - 1)));
        SetSZPF32(res);
    }
    return res;
}

int8_t sar8(x86_context_t* pCPU, int8_t dst, uint8_t count) {
    int8_t res = dst >> count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (count - 1)));
        SetSZPF8(res);
    }
    return res;
}
int16_t sar16(x86_context_t* pCPU, int16_t dst, uint8_t count) {
    int16_t res = dst >> count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (count - 1)));
        SetSZPF16(res);
    }
    return res;
}
int32_t sar32(x86_context_t* pCPU, int32_t dst, uint8_t count) {
    int32_t res = dst >> count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (count - 1)));
        SetSZPF32(res);
    }
    return res;
}

uint8_t shl8(x86_context_t* pCPU, uint8_t dst, uint8_t count) {
    uint8_t res = dst << count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (8 - count)));
        SetSZPF8(res);
    }
    return res;
}
uint16_t shl16(x86_context_t* pCPU, uint16_t dst, uint8_t count) {
    uint16_t res = dst << count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (16 - count)));
        SetSZPF16(res);
    }
    return res;
}
uint32_t shl32(x86_context_t* pCPU, uint32_t dst, uint8_t count) {
    uint32_t res = dst >> count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (32 - count)));
        SetSZPF32(res);
    }
    return res;
}

int8_t sal8(x86_context_t* pCPU, int8_t dst, uint8_t count) {
    int8_t res = dst << count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (8 - count)));
        SetSZPF8(res);
    }
    return res;
}
int16_t sal16(x86_context_t* pCPU, int16_t dst, uint8_t count) {
    int16_t res = dst << count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (16 - count)));
        SetSZPF16(res);
    }
    return res;
}
int32_t sal32(x86_context_t* pCPU, int32_t dst, uint8_t count) {
    int32_t res = dst >> count;
    if (count != 0) {
        COND_FLAG(CARRY, dst & (1 << (32 - count)));
        SetSZPF32(res);
    }
    return res;
}

uint8_t rol8(x86_context_t* pCPU, uint8_t value, uint8_t shamt) {

}
uint8_t ror8(x86_context_t* pCPU, uint8_t value, uint8_t shamt) {

}
uint8_t rcl8(x86_context_t* pCPU, uint8_t value, uint8_t shamt) {

}
uint8_t rcr8(x86_context_t* pCPU, uint8_t value, uint8_t shamt) {

}
uint16_t rol16(x86_context_t* pCPU, uint16_t value, uint8_t shamt) {

}
uint16_t ror16(x86_context_t* pCPU, uint16_t value, uint8_t shamt) {

}
uint16_t rcl16(x86_context_t* pCPU, uint16_t value, uint8_t shamt) {

}
uint16_t rcr16(x86_context_t* pCPU, uint16_t value, uint8_t shamt) {

}
uint32_t rol32(x86_context_t* pCPU, uint32_t value, uint8_t shamt) {

}
uint32_t ror32(x86_context_t* pCPU, uint32_t value, uint8_t shamt) {

}
uint32_t rcl32(x86_context_t* pCPU, uint32_t value, uint8_t shamt) {

}
uint32_t rcr32(x86_context_t* pCPU, uint32_t value, uint8_t shamt) {

}



