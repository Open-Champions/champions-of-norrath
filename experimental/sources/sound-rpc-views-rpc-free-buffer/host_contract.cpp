#include "rpc_free_buffer.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_rpc_free_buffer_view == 2048, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_rpc_free_buffer_view; ++index)
        if (norrath_sound_rpc_free_buffer_view[index] != 0) return 1;
    std::puts("rpc_free_buffer passed");
    return 0;
}
