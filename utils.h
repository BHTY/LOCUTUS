#pragma once
#include "common.h"
#include "i386.h"

#define ADDR_SZ     (pCPU->addr_override ^ MODE(pCPU->cs))
#define OP_SZ       (pCPU->op_override ^ MODE(pCPU->cs))

#define READ8(addr) pCPU->system.bus_read_8(addr)
#define READ16(addr) pCPU->system.bus_read_16(addr)
#define READ32(addr) pCPU->system.bus_read_32(addr)

#define WRITE8(addr, val)   pCPU->system.bus_write_8(addr, val)
#define WRITE16(addr, val)   pCPU->system.bus_write_16(addr, val)
#define WRITE32(addr, val)   pCPU->system.bus_write_32(addr, val)

#define REG8(index)             (*(((uint8_t*)((&(pCPU->regs[(index) & 3])))) + ((index) >> 2)))
#define REG16(index)            (*(uint16_t*)(&(pCPU->regs[index])))
#define REG32(index)            (pCPU->regs[index])

#define R8(index)                (*(((uint8_t*)((&(pCPU->regs[(index) & 3])))) + ((index) >> 2)))

#define LOAD_REG8(modrm)         (R8(REG(modrm)))
#define LOAD_REG16(modrm)        (pCPU->regs[REG(modrm)] & 0xFFFF)
#define LOAD_REG32(modrm)        (pCPU->regs[REG(modrm)])

#define LOAD_RM8(modrm)          (R8(RM(modrm)))
#define LOAD_RM16(modrm)         (pCPU->regs[RM(modrm)] & 0xFFFF)
#define LOAD_RM32(modrm)         (pCPU->regs[RM(modrm)])

#define STORE_REG8(modrm, val)   R8(REG(modrm)) = val;
#define STORE_REG16(modrm, val)  *(uint16_t*)(&(pCPU->regs[REG(modrm)])) = val;
#define STORE_REG32(modrm, val)  pCPU->regs[REG(modrm)] = val;

#define STORE_RM8(modrm, val)    R8(RM(modrm)) = val;
#define STORE_RM16(modrm, val)   *(uint16_t*)(&(pCPU->regs[RM(modrm)])) = val;
#define STORE_RM32(modrm, val)   pCPU->regs[RM(modrm)] = val;

#define PUSH16(value)            push16(pCPU, value)
#define PUSH32(value)            push32(pCPU, value)

#define POP16()                  pop16(pCPU)
#define POP32()                  pop32(pCPU)

int jump_condition(x86_context_t* pCPU, int condition);

uint32_t string_src_8(x86_context_t* pCPU);
uint32_t string_src_16(x86_context_t* pCPU);
uint32_t string_src_32(x86_context_t* pCPU);

uint32_t string_dst_8(x86_context_t* pCPU);
uint32_t string_dst_16(x86_context_t* pCPU);
uint32_t string_dst_32(x86_context_t* pCPU);

uint32_t get_ea(x86_context_t* pCPU, uint8_t modrm);

uint8_t fetch8(x86_context_t* pCPU);
uint16_t fetch16(x86_context_t* pCPU);
uint32_t fetch32(x86_context_t* pCPU);

void push16(x86_context_t* pCPU, uint16_t value);
void push32(x86_context_t* pCPU, uint32_t value);

uint16_t pop16(x86_context_t* pCPU);
uint32_t pop32(x86_context_t* pCPU);

uint32_t get_offset(x86_context_t* pCPU);

uint32_t linear(x86_context_t* pCPU, int segment, uint32_t offset);

void do_int(x86_context_t* pCPU, uint8_t vector);
