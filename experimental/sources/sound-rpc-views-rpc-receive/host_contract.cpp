#include "rpc_receive.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_rpc_receive_view == 64, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_rpc_receive_view; ++index)
        if (norrath_sound_rpc_receive_view[index] != 0) return 1;
    std::puts("rpc_receive passed");
    return 0;
}
