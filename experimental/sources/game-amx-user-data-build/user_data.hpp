#pragma once
#include "constructor.hpp"
static_assert(sizeof(long long) == 8, "native64 tag comparison and status lanes");
extern "C" {
unsigned long long norrath_amx_get_user_data(void* amx, long long tag,
                                           void* output_word);
unsigned long long norrath_amx_set_user_data(void* amx, long long tag,
                                           unsigned long long user_data);
}
