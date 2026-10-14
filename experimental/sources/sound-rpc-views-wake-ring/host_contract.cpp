#include "wake_ring.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_wake_ring_view == 1080, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_wake_ring_view; ++index)
        if (norrath_sound_wake_ring_view[index] != 0) return 1;
    std::puts("wake_ring passed");
    return 0;
}
