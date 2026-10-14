#include "rpc_client.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_rpc_client_view == 96, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_rpc_client_view; ++index)
        if (norrath_sound_rpc_client_view[index] != 0) return 1;
    std::puts("rpc_client passed");
    return 0;
}
