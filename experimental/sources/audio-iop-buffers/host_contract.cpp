#include "iop_buffers.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_batch_iop_buffer_word_view == 4, "batch word");
    static_assert(sizeof norrath_sound_iop_buffer_word_view == 4, "iop word");
    for (unsigned char byte : norrath_sound_batch_iop_buffer_word_view)
        if (byte != 0) return 1;
    for (unsigned char byte : norrath_sound_iop_buffer_word_view)
        if (byte != 0) return 2;
    std::puts("iop buffer words passed");
    return 0;
}
