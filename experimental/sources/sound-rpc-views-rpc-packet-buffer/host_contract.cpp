#include "rpc_packet_buffer.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_rpc_packet_buffer_view == 2048, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_rpc_packet_buffer_view; ++index)
        if (norrath_sound_rpc_packet_buffer_view[index] != 0) return 1;
    std::puts("rpc_packet_buffer passed");
    return 0;
}
