#include "command_storage.cpp"

#include <cstdio>

template <unsigned Expected>
static int zeros(const unsigned char* bytes) {
    for (unsigned i = 0; i < Expected; ++i)
        if (bytes[i] != 0) return 1;
    return 0;
}

int main() {
    static_assert(sizeof norrath_sound_command_packet_buffer_view == 128, "packet span");
    static_assert(sizeof norrath_sound_command_send_buffer_view == 64, "send span");
    static_assert(sizeof norrath_sound_command_initialization_packet_view == 20, "init packet");
    static_assert(sizeof norrath_sound_command_dma_handle_view == 4, "dma handle");
    static_assert(sizeof norrath_sound_command_state_view == 40, "state span");
    static_assert(sizeof norrath_sound_command_system_handlers_view == 384, "handlers");
    static_assert(sizeof norrath_sound_system_register_words_view == 128, "registers");
    static_assert(sizeof norrath_sound_command_initialized_word_view == 4, "init word");
    if (zeros<128>(norrath_sound_command_packet_buffer_view)) return 1;
    if (zeros<64>(norrath_sound_command_send_buffer_view)) return 2;
    if (zeros<20>(norrath_sound_command_initialization_packet_view)) return 3;
    if (zeros<4>(norrath_sound_command_dma_handle_view)) return 4;
    if (zeros<40>(norrath_sound_command_state_view)) return 5;
    if (zeros<384>(norrath_sound_command_system_handlers_view)) return 6;
    if (zeros<128>(norrath_sound_system_register_words_view)) return 7;
    if (zeros<4>(norrath_sound_command_initialized_word_view)) return 8;
    std::puts("command storage layout passed");
    return 0;
}
