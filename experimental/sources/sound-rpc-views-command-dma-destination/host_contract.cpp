#include "command_dma_destination.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_command_dma_destination_word_view == 4, "size");
    for (unsigned index = 0; index < sizeof norrath_sound_command_dma_destination_word_view; ++index)
        if (norrath_sound_command_dma_destination_word_view[index] != 0) return 1;
    std::puts("command_dma_destination passed");
    return 0;
}
