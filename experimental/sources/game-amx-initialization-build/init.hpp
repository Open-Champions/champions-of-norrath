#pragma once
#include "constructor.hpp"
extern "C" {
unsigned long long norrath_amx_initialize(void* amx, void* program);
unsigned long long norrath_amx_browse_relocate(void* amx);
// Initialization takes these genuine code addresses; it does not invoke them.
// Callback's observed A0/A1/A2/A3 are AMX/index/result/parameter addresses.
unsigned long long norrath_amx_callback(void* amx, int index, int* result, void* parameters);
unsigned long long norrath_amx_debug(void* amx);
extern unsigned char norrath_amx_little_endian_word_view[];
}
