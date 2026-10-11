#pragma once
#include "constructor.hpp"
static_assert(sizeof(long long) == 8, "original signed native64 address comparison");
extern "C" {
unsigned long long norrath_amx_get_address(void* amx, long long script_address,
                                          void** physical_address);
unsigned long long norrath_amx_get_string(char* destination, const void* cells);
}
