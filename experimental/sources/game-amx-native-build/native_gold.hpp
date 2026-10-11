#pragma once
#include "ability_recompute.hpp"
static_assert(sizeof(long long)==8, "native lower64 return transport");
extern "C" {
// Entire genuine script natives. Both input lanes are pointers; AMX is unused.
// Check returns the sign extension of the wrapped word sum. No registry owner
// or general callback-signature conversion is supplied by these declarations.
long long norrath_amx_native_give_player_gold(void* amx,const void* parameters);
long long norrath_amx_native_take_player_gold(void* amx,const void* parameters);
long long norrath_amx_native_check_player_gold(void* amx,const void* parameters);
}
