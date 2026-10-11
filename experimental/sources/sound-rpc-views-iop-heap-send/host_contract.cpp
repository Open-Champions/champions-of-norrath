#include "iop_heap_send.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_iop_heap_send_view == 64, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_iop_heap_send_view; ++index)
        if (norrath_sound_iop_heap_send_view[index] != 0) return 1;
    std::puts("iop_heap_send passed");
    return 0;
}
