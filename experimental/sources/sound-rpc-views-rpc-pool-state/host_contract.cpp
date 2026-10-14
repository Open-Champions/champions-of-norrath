#include "rpc_pool_state.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_rpc_packet_pool_state_view == 64, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_rpc_packet_pool_state_view; ++index)
        if (norrath_sound_rpc_packet_pool_state_view[index] != 0) return 1;
    std::puts("rpc_pool_state passed");
    return 0;
}
