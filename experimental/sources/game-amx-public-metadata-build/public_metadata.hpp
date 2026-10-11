#pragma once
#include "dependencies.hpp"
static_assert(sizeof(long long) == 8, "full native64 signed index and status lanes");
namespace norrath_amx_public_metadata_detail {
inline int signed_count(norrath_constructor_detail::Address location) {
    const unsigned word = norrath_constructor_dependency_detail::read_half(location);
    return word < 0x8000u ? static_cast<int>(word) : static_cast<int>(word) - 65536;
}
}
extern "C" {
unsigned long long norrath_amx_num_publics(void* amx, void* count_word);
unsigned long long norrath_amx_get_public(void* amx, long long index, char* name);
unsigned long long norrath_amx_num_public_variables(void* amx, void* count_word);
unsigned long long norrath_amx_get_public_variable(void* amx, long long index,
                                                char* name, void* address_word);
}
