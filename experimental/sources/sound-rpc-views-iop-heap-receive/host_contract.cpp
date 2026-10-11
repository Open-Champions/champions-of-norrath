#include "iop_heap_receive.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_iop_heap_receive_view == 64, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_iop_heap_receive_view; ++index)
        if (norrath_sound_iop_heap_receive_view[index] != 0) return 1;
    std::puts("iop_heap_receive passed");
    return 0;
}
