#pragma once
#include "address_string.hpp"
#include "state.hpp"
extern "C" {
long long norrath_amx_native_get_game_state(void* amx, const void* parameters);
long long norrath_amx_native_set_post_action(void* amx, const void* parameters);
}
