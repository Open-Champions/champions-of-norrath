#pragma once
#include "constructor.hpp"
static_assert(sizeof(long long) == 8 && sizeof(unsigned short) == 2,
              "observed native count and header halfword widths");
extern "C" {
// S3 captures the original entire signed native64 A2 count.
unsigned long long norrath_amx_register(void* amx, const void* registry,
                                       long long native_count);
unsigned long long norrath_amx_register_game_natives(void* amx);
// Genuine LOCAL OBJECT182 nativeList; no owner or copied table emitted.
extern const unsigned char norrath_amx_game_native_registry_view[];
}
