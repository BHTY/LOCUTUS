#pragma once

#include <stdint.h>

uint32_t decode_mod_rm_16(x86_context_t* pCPU, uint8_t modrm, int* segment);
uint32_t decode_mod_rm_32(x86_context_t* pCPU, uint8_t modrm, int* segment);
