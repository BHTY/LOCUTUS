#pragma once

#include <stdint.h>
#include "i386.h"

extern int i386_parity_table[];

#define add8(pCPU, dst, src)    _adc8(pCPU, dst, src, 0)
#define add16(pCPU, dst, src)   _adc16(pCPU, dst, src, 0)
#define add32(pCPU, dst, src)   _adc32(pCPU, dst, src, 0)
#define adc8(pCPU, dst, src)    _adc8(pCPU, dst, src, GET_FLAG(CARRY))
#define adc16(pCPU, dst, src)    _adc16(pCPU, dst, src, GET_FLAG(CARRY))
#define adc32(pCPU, dst, src)    _adc32(pCPU, dst, src, GET_FLAG(CARRY))
uint8_t _adc8(x86_context_t* pCPU, uint8_t dst, uint8_t src, uint8_t c);
uint16_t _adc16(x86_context_t* pCPU, uint16_t dst, uint16_t src, uint8_t c);
uint32_t _adc32(x86_context_t* pCPU, uint32_t dst, uint32_t src, uint8_t c);

uint8_t or8(x86_context_t* pCPU, uint8_t dst, uint8_t src);
uint16_t or16(x86_context_t* pCPU, uint16_t dst, uint16_t src);
uint32_t or32(x86_context_t* pCPU, uint32_t dst, uint32_t src);

#define sub8(pCPU, dst, src)    _sbb8(pCPU, dst, src, 0)
#define sub16(pCPU, dst, src)   _sbb16(pCPU, dst, src, 0)
#define sub32(pCPU, dst, src)   _sbb32(pCPU, dst, src, 0)
#define sbb8(pCPU, dst, src)    _sbb8(pCPU, dst, src, GET_FLAG(CARRY))
#define sbb16(pCPU, dst, src)   _sbb16(pCPU, dst, src, GET_FLAG(CARRY))
#define sbb32(pCPU, dst, src)   _sbb32(pCPU, dst, src, GET_FLAG(CARRY))
uint8_t _sbb8(x86_context_t* pCPU, uint8_t dst, uint8_t src, uint8_t b);
uint16_t _sbb16(x86_context_t* pCPU, uint16_t dst, uint16_t src, uint8_t b);
uint32_t _sbb32(x86_context_t* pCPU, uint32_t dst, uint32_t src, uint8_t b);

uint8_t cmp8(x86_context_t* pCPU, uint8_t dst, uint8_t src);
uint16_t cmp16(x86_context_t* pCPU, uint16_t dst, uint16_t src);
uint32_t cmp32(x86_context_t* pCPU, uint32_t dst, uint32_t src);

uint8_t and8(x86_context_t* pCPU, uint8_t dst, uint8_t src);
uint16_t and16(x86_context_t* pCPU, uint16_t dst, uint16_t src);
uint32_t and32(x86_context_t* pCPU, uint32_t dst, uint32_t src);

uint8_t xor8(x86_context_t* pCPU, uint8_t dst, uint8_t src);
uint16_t xor16(x86_context_t* pCPU, uint16_t dst, uint16_t src);
uint32_t xor32(x86_context_t* pCPU, uint32_t dst, uint32_t src);

uint8_t test8(x86_context_t* pCPU, uint8_t dst, uint8_t src);
uint16_t test16(x86_context_t* pCPU, uint16_t dst, uint16_t src);
uint32_t test32(x86_context_t* pCPU, uint32_t dst, uint32_t src);

uint8_t inc8(x86_context_t* pCPU, uint8_t dst);
uint16_t inc16(x86_context_t* pCPU, uint16_t dst);
uint32_t inc32(x86_context_t* pCPU, uint32_t dst);

uint8_t dec8(x86_context_t* pCPU, uint8_t dst);
uint16_t dec16(x86_context_t* pCPU, uint16_t dst);
uint32_t dec32(x86_context_t* pCPU, uint32_t dst);

uint16_t mul8(x86_context_t* pCPU, uint8_t dst, uint8_t src);
uint32_t mul16(x86_context_t* pCPU, uint16_t dst, uint16_t src);
uint64_t mul32(x86_context_t* pCPU, uint32_t dst, uint32_t src);

int16_t imul8(x86_context_t* pCPU, int8_t dst, int8_t src);
int32_t imul16(x86_context_t* pCPU, int16_t dst, int16_t src);
int64_t imul32(x86_context_t* pCPU, int32_t dst, int32_t src);

uint16_t div8(x86_context_t* pCPU, uint16_t dst, uint8_t src);
uint32_t div16(x86_context_t* pCPU, uint32_t dst, uint16_t src);
uint64_t div32(x86_context_t* pCPU, uint64_t dst, uint32_t src);

int16_t idiv8(x86_context_t* pCPU, int16_t dst, int8_t src);
int32_t idiv16(x86_context_t* pCPU, int32_t dst, int16_t src);
int64_t idiv32(x86_context_t* pCPU, int64_t dst, int32_t src);

uint8_t rol8(x86_context_t* pCPU, uint8_t value, uint8_t shamt);
uint8_t ror8(x86_context_t* pCPU, uint8_t value, uint8_t shamt);
uint8_t rcl8(x86_context_t* pCPU, uint8_t value, uint8_t shamt);
uint8_t rcr8(x86_context_t* pCPU, uint8_t value, uint8_t shamt);
uint8_t shl8(x86_context_t* pCPU, uint8_t value, uint8_t shamt);
uint8_t shr8(x86_context_t* pCPU, uint8_t value, uint8_t shamt);
uint8_t shl8(x86_context_t* pCPU, uint8_t value, uint8_t shamt);
uint8_t sar8(x86_context_t* pCPU, uint8_t value, uint8_t shamt);

uint16_t rol16(x86_context_t* pCPU, uint16_t value, uint8_t shamt);
uint16_t ror16(x86_context_t* pCPU, uint16_t value, uint8_t shamt);
uint16_t rcl16(x86_context_t* pCPU, uint16_t value, uint8_t shamt);
uint16_t rcr16(x86_context_t* pCPU, uint16_t value, uint8_t shamt);
uint16_t shl16(x86_context_t* pCPU, uint16_t value, uint8_t shamt);
uint16_t shr16(x86_context_t* pCPU, uint16_t value, uint8_t shamt);
uint16_t shl16(x86_context_t* pCPU, uint16_t value, uint8_t shamt);
uint16_t sar16(x86_context_t* pCPU, uint16_t value, uint8_t shamt);

uint32_t rol32(x86_context_t* pCPU, uint32_t value, uint8_t shamt);
uint32_t ror32(x86_context_t* pCPU, uint32_t value, uint8_t shamt);
uint32_t rcl32(x86_context_t* pCPU, uint32_t value, uint8_t shamt);
uint32_t rcr32(x86_context_t* pCPU, uint32_t value, uint8_t shamt);
uint32_t shl32(x86_context_t* pCPU, uint32_t value, uint8_t shamt);
uint32_t shr32(x86_context_t* pCPU, uint32_t value, uint8_t shamt);
uint32_t shl32(x86_context_t* pCPU, uint32_t value, uint8_t shamt);
uint32_t sar32(x86_context_t* pCPU, uint32_t value, uint8_t shamt);

#define S8(num)				((num)&0x80)
#define S16(num)			((num)&0x8000)
#define S32(num)			((num)&0x80000000)

#define SetCF8(x)       COND_FLAG(CARRY, (x) & 0x100)
#define SetCF16(x)      COND_FLAG(CARRY, (x) & 0x10000)
#define SetCF32(x)      COND_FLAG(CARRY, (x) & (((uint64_t)1) << 32))

#define SetZF8(x)        COND_FLAG(ZERO, ((x) & 0xFF) == 0)
#define SetZF16(x)        COND_FLAG(ZERO, ((x) & 0xFFFF) == 0)
#define SetZF32(x)        COND_FLAG(ZERO, ((x) & 0xFFFFFFFF) == 0)

#define SetAF(x,y,z)    COND_FLAG(AC, ((x) ^ (y) ^ (z)) & 0x10)
#define SetPF(x)        COND_FLAG(PARITY, !i386_parity_table[(x) & 0xFF])

#define SetSF8(x)       COND_FLAG(SIGN, (x) & 0x80)
#define SetSF16(x)      COND_FLAG(SIGN, (x) & 0x8000)
#define SetSF32(x)      COND_FLAG(SIGN, (x) & 0x80000000)

#define SetSZPF8(x)     {SetSF8(x); SetZF8(x); SetPF(x);}
#define SetSZPF16(x)    {SetSF16(x); SetZF16(x); SetPF(x);}
#define SetSZPF32(x)    {SetSF32(x); SetZF32(x); SetPF(x);}

#define SetOF_Add8(a,b,d)	COND_FLAG(OVERFLOW, (!(S8(a) ^ S8(b))) && (S8(a) ^ S8(d)))
#define SetOF_Add16(a,b,d)	COND_FLAG(OVERFLOW, (!(S16(a) ^ S16(b))) && (S16(a) ^ S16(d)))
#define SetOF_Add32(a,b,d)	COND_FLAG(OVERFLOW, (!(S32(a) ^ S32(b))) && (S32(a) ^ S32(d)))

#define SetOF_Sub8(a,b,d)	COND_FLAG(OVERFLOW, (!(S8(d) ^ S8(b))) && (S8(a) ^ S8(d)))
#define SetOF_Sub16(a,b,d)	COND_FLAG(OVERFLOW, (!(S16(d) ^ S16(b))) && (S16(a) ^ S16(d)))
#define SetOF_Sub32(a,b,d)	COND_FLAG(OVERFLOW, (!(S32(d) ^ S32(b))) && (S32(a) ^ S32(d)))
