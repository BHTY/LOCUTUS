#pragma once

#include "i386.h"
#include "utils.h"

typedef void (*i386_op_func)(x86_context_t*);

typedef struct x86_opcode {
    i386_op_func handler16;
    i386_op_func handler32;
} x86_opcode_t;

extern x86_opcode_t x86_opcode_table_1[256];
extern x86_opcode_t x86_opcode_table_2[256];
extern x86_opcode_t x86_opcode_table_group80[8];
extern x86_opcode_t x86_opcode_table_group81[8];
extern x86_opcode_t x86_opcode_table_group83[8];
extern x86_opcode_t x86_opcode_table_groupC0[8];
extern x86_opcode_t x86_opcode_table_groupC1[8];
extern x86_opcode_t x86_opcode_table_groupD0[8];
extern x86_opcode_t x86_opcode_table_groupD1[8];
extern x86_opcode_t x86_opcode_table_groupD2[8];
extern x86_opcode_t x86_opcode_table_groupD3[8];
extern x86_opcode_t x86_opcode_table_groupF6[8];
extern x86_opcode_t x86_opcode_table_groupF7[8];
extern x86_opcode_t x86_opcode_table_groupFE[8];
extern x86_opcode_t x86_opcode_table_groupFF[8];

#define sgmt_override(name, segment)    void name(x86_context_t* pCPU){\
                                            pCPU->segment_prefix = 1; \
                                            pCPU->segment_override = segment; \
                                            i386_decode_opcode(pCPU); \
                                            pCPU->segment_prefix = 0; \
                                        }

#define op_rm8_r8(name, op)     void name(x86_context_t* pCPU){ \
                                    uint8_t src, dst; \
                                    uint8_t modrm = fetch8(pCPU); \
                                    src = LOAD_REG8(modrm); \
                                    if (modrm >= 0xc0){ \
                                        dst = LOAD_RM8(modrm); \
                                        dst = op(pCPU, dst, src); \
                                        STORE_RM8(modrm, dst); \
                                    } else { \
                                        uint32_t ea = get_ea(pCPU, modrm); \
                                        dst = READ8(ea); \
                                        dst = op(pCPU, dst, src); \
                                        WRITE8(ea, dst); \
                                    } \
                                }

#define op_r8_rm8(name, op)     void name(x86_context_t* pCPU){ \
                                    uint8_t src, dst; \
                                    uint8_t modrm = fetch8(pCPU); \
                                    dst = LOAD_REG8(modrm); \
                                    if (modrm >= 0xc0){ \
                                        src = LOAD_RM8(modrm); \
                                        dst = op(pCPU, dst, src); \
                                    } else { \
                                        uint32_t ea = get_ea(pCPU, modrm); \
                                        src = READ8(ea); \
                                        dst = op(pCPU, dst, src); \
                                    } \
                                    STORE_REG8(modrm, dst); \
                                }

#define op_rm16_r16(name, op)     void name(x86_context_t* pCPU){ \
                                    uint16_t src, dst; \
                                    uint8_t modrm = fetch8(pCPU); \
                                    src = LOAD_REG16(modrm); \
                                    if (modrm >= 0xc0){ \
                                        dst = LOAD_RM16(modrm); \
                                        dst = op(pCPU, dst, src); \
                                        STORE_RM16(modrm, dst); \
                                    } else { \
                                        uint32_t ea = get_ea(pCPU, modrm); \
                                        dst = READ16(ea); \
                                        dst = op(pCPU, dst, src); \
                                        WRITE16(ea, dst); \
                                    } \
                                }

#define op_r16_rm16(name, op)     void name(x86_context_t* pCPU){ \
                                    uint16_t src, dst; \
                                    uint8_t modrm = fetch8(pCPU); \
                                    dst = LOAD_REG16(modrm); \
                                    if (modrm >= 0xc0){ \
                                        src = LOAD_RM16(modrm); \
                                        dst = op(pCPU, dst, src); \
                                    } else { \
                                        uint32_t ea = get_ea(pCPU, modrm); \
                                        src = READ16(ea); \
                                        dst = op(pCPU, dst, src); \
                                    } \
                                    STORE_REG16(modrm, dst); \
                                }

#define op_rm32_r32(name, op)     void name(x86_context_t* pCPU){ \
                                    uint32_t src, dst; \
                                    uint8_t modrm = fetch8(pCPU); \
                                    src = LOAD_REG32(modrm); \
                                    if (modrm >= 0xc0){ \
                                        dst = LOAD_RM32(modrm); \
                                        dst = op(pCPU, dst, src); \
                                        STORE_RM32(modrm, dst); \
                                    } else { \
                                        uint32_t ea = get_ea(pCPU, modrm); \
                                        dst = READ32(ea); \
                                        dst = op(pCPU, dst, src); \
                                        WRITE32(ea, dst); \
                                    } \
                                }

#define op_r32_rm32(name, op)     void name(x86_context_t* pCPU){ \
                                    uint32_t src, dst; \
                                    uint8_t modrm = fetch8(pCPU); \
                                    dst = LOAD_REG32(modrm); \
                                    if (modrm >= 0xc0){ \
                                        src = LOAD_RM32(modrm); \
                                        dst = op(pCPU, dst, src); \
                                    } else { \
                                        uint32_t ea = get_ea(pCPU, modrm); \
                                        src = READ32(ea); \
                                        dst = op(pCPU, dst, src); \
                                    } \
                                    STORE_REG32(modrm, dst); \
                                }

#define op_al_i8(name, op)      void name(x86_context_t* pCPU){ \
                                    uint8_t src; \
                                    src = fetch8(pCPU); \
                                    pCPU->al = op(pCPU, pCPU->al, src); \
                                }

#define op_ax_i16(name, op)      void name(x86_context_t* pCPU){ \
                                    uint16_t src; \
                                    src = fetch16(pCPU); \
                                    pCPU->ax = op(pCPU, pCPU->ax, src); \
                                }

#define op_eax_i32(name, op)      void name(x86_context_t* pCPU){ \
                                    uint32_t src; \
                                    src = fetch32(pCPU); \
                                    pCPU->eax = op(pCPU, pCPU->eax, src); \
                                }

#define push_seg16(name, sreg)      void name(x86_context_t* pCPU){ \
                                        push16(pCPU, pCPU->seg_regs[sreg].selector); \
                                    }

#define push_seg32(name, sreg)      void name(x86_context_t* pCPU){ \
                                        push32(pCPU, pCPU->seg_regs[sreg].selector); \
                                    }

#define pop_seg16(name, sreg)       void name(x86_context_t* pCPU){ \
                                        uint16_t selector = pop16(pCPU); \
                                        i386_load_sreg(pCPU, sreg, selector); \
                                    }

#define pop_seg32(name, sreg)       void name(x86_context_t* pCPU){ \
                                        uint16_t selector = pop32(pCPU); \
                                        i386_load_sreg(pCPU, sreg, selector); \
                                    }

#define inc_r16(name, reg)          void name(x86_context_t* pCPU){ \
                                        REG16(reg) = inc16(pCPU, REG16(reg)); \
                                    }

#define inc_r32(name, reg)          void name(x86_context_t* pCPU){ \
                                        REG32(reg) = inc32(pCPU, REG32(reg)); \
                                    }

#define dec_r16(name, reg)          void name(x86_context_t* pCPU){ \
                                        REG16(reg) = dec16(pCPU, REG16(reg)); \
                                    }

#define dec_r32(name, reg)          void name(x86_context_t* pCPU){ \
                                        REG32(reg) = dec32(pCPU, REG32(reg)); \
                                    }

#define push_r16(name, reg)         void name(x86_context_t* pCPU){ \
                                        push16(pCPU, REG16(reg)); \
                                    }

#define push_r32(name, reg)         void name(x86_context_t* pCPU){ \
                                        push32(pCPU, REG32(reg)); \
                                    }

#define pop_r16(name, reg)          void name(x86_context_t* pCPU){ \
                                        REG16(reg) = pop16(pCPU); \
                                    }

#define pop_r32(name, reg)          void name(x86_context_t* pCPU){ \
                                        REG32(reg) = pop32(pCPU); \
                                    }

#define jcc_rel8(name, cond)        void name(x86_context_t* pCPU){ \
                                        int8_t disp = fetch8(pCPU); \
                                        if(jump_condition(pCPU, cond)) pCPU->eip += (int32_t)disp; \
                                    }

#define xchg_ax_r16(name, reg)      void name(x86_context_t* pCPU){ \
                                        uint16_t temp = pCPU->ax; \
                                        pCPU->ax = REG16(reg); \
                                        REG16(reg) = temp; \
                                    }

#define xchg_eax_r32(name, reg)      void name(x86_context_t* pCPU){ \
                                        uint32_t temp = pCPU->eax; \
                                        pCPU->eax = REG32(reg); \
                                        REG32(reg) = temp; \
                                    }

#define mov_r8_i8(name, reg)        void name(x86_context_t* pCPU){ \
                                        REG8(reg) = fetch8(pCPU); \
                                    }

#define mov_r16_i16(name, reg)        void name(x86_context_t* pCPU){ \
                                        REG16(reg) = fetch16(pCPU); \
                                    }

#define mov_r32_i32(name, reg)        void name(x86_context_t* pCPU){ \
                                        REG32(reg) = fetch32(pCPU); \
                                    }

#define set_flag(name, flag)        void name(x86_context_t* pCPU){ \
                                        SET_FLAG(flag); \
                                    }

#define clr_flag(name, flag)        void name(x86_context_t* pCPU){ \
                                        CLEAR_FLAG(flag); \
                                    }
