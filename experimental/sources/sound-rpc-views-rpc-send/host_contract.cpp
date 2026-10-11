#include "rpc_send.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_rpc_send_view == 64, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_rpc_send_view; ++index)
        if (norrath_sound_rpc_send_view[index] != 0) return 1;
    std::puts("rpc_send passed");
    return 0;
}
