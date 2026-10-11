#include "register.hpp"
extern "C" unsigned long long norrath_amx_register_game_natives(void* amx) {
    return norrath_amx_register(amx, norrath_amx_game_native_registry_view, -1ll);
}
