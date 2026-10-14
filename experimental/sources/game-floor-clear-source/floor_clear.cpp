#include "floor_clear.hpp"

namespace {
inline unsigned address(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
inline void* pointer(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}
inline void store_word(unsigned location, unsigned value) {
    volatile unsigned char* const out =
        reinterpret_cast<volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(location));
    out[0] = static_cast<unsigned char>(value);
    out[1] = static_cast<unsigned char>(value >> 8);
    out[2] = static_cast<unsigned char>(value >> 16);
    out[3] = static_cast<unsigned char>(value >> 24);
}
inline unsigned arithmetic_shift(unsigned word, unsigned distance) {
    return (word >> distance) |
        ((word & 0x80000000u) ? (0xffffffffu << (32u - distance)) : 0u);
}
}

extern "C" void norrath_floor_clear() {
    const unsigned pool = address(norrath_floor_sprite_packet_view);
    // Publication occurs in the original call's delay slot, before the clear.
    store_word(address(norrath_floor_sprite_count_word_view), 0u);
    (void)norrath_block_memory_set(pointer(pool), 0, 128u * 768u);

    unsigned next = pool + 127u * 768u;
    unsigned link = pool + 126u * 768u + 752u;
    for (int record = 126; record >= 0; --record) {
        store_word(link, next);
        link -= 768u;
        next -= 768u;
    }
    store_word(pool + 127u * 768u + 752u, 0u);

    // The free-list head is observable before the second genuine helper call.
    store_word(address(norrath_floor_free_list_word_view), pool);
    (void)norrath_block_memory_set(norrath_floor_sprite_grid_view, 0, 3600u);

    // Preserve the word arithmetic on real program boundaries. No program
    // payload, fixed original address, or precomputed command is supplied.
    const unsigned program_bytes = address(norrath_floor_program_end_view) -
                                   address(norrath_floor_program_start_view);
    const unsigned rounded_bytes = arithmetic_shift(program_bytes + 15u, 4u) << 4;
    const unsigned command = (arithmetic_shift(16384u - rounded_bytes, 3u) + 2u) |
                             0x14000000u;
    unsigned packet = pool + 127u * 768u + 744u;
    for (int record = 127; record >= 0; --record) {
        store_word(packet, command);
        packet -= 768u;
    }
    store_word(address(norrath_floor_fade_packet_word_view), 0u);
}
