#pragma once
#include "dependencies.hpp"
static_assert(sizeof(long long)==8,"native lower64 return lanes");
extern "C" {
extern unsigned char norrath_game_bail_from_loop_view[];
extern unsigned char norrath_script_game_state_data_view[];
extern unsigned char norrath_script_post_action_view[];
long long norrath_amx_native_leave_game(void* amx,const void* parameters);
long long norrath_script_get_game_state(char* name);
unsigned long long norrath_script_set_post_action(char* action);
}
