#include "remote_interrupt_gps.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_remote_interrupt_gps_view == 12, "size");
    static const unsigned char expected[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    static_assert(sizeof expected == 12, "expected size");
    if (std::memcmp(norrath_sound_remote_interrupt_gps_view, expected, 12) != 0) return 1;
    std::puts("remote_interrupt_gps passed");
    return 0;
}
