#include "remote_packet.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_remote_packet_view == 64, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_remote_packet_view; ++index)
        if (norrath_sound_remote_packet_view[index] != 0) return 1;
    std::puts("remote_packet passed");
    return 0;
}
