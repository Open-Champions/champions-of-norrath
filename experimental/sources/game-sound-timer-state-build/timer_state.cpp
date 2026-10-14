#include "sound_timer_initialization.hpp"

static_assert(sizeof(unsigned) == 4, "Timer fields use 32-bit words");
static_assert(sizeof(unsigned char) == 1, "Timer views use bytes");

namespace norrath_sound_timer_state_detail {
// These are the original logical starting fields. The gate in InitTimer
// observes a negative handler slot; counter operations observe an invalid current
// handle. Word 12 starts at one and InitTimer deliberately leaves it untouched.
enum : unsigned {
    no_registered_interrupt_handler = ~0u,
    initial_word12 = 1u,
    no_current_counter = ~0u
};
}

// Keep the existing byte-view declaration canonical across all source users.
// Express each logical word as little-endian bytes rather than introducing a
// struct alias, a native address binding, or a copied binary data block.
#define NORRATH_TIMER_WORD(value) \
    static_cast<unsigned char>(static_cast<unsigned>(value)), \
    static_cast<unsigned char>(static_cast<unsigned>(value) >> 8), \
    static_cast<unsigned char>(static_cast<unsigned>(value) >> 16), \
    static_cast<unsigned char>(static_cast<unsigned>(value) >> 24)

extern "C" {
alignas(8) unsigned char norrath_sound_timer_control_view[32] = {
    NORRATH_TIMER_WORD(0u), NORRATH_TIMER_WORD(0u),
    NORRATH_TIMER_WORD(norrath_sound_timer_state_detail::no_registered_interrupt_handler),
    NORRATH_TIMER_WORD(norrath_sound_timer_state_detail::initial_word12),
    NORRATH_TIMER_WORD(0u), NORRATH_TIMER_WORD(0u),
    NORRATH_TIMER_WORD(0u),
    NORRATH_TIMER_WORD(norrath_sound_timer_state_detail::no_current_counter)
};

// These separate zero-initialized spans correspond to distinct original BSS
// regions. InitTimer clears the full counter span and links 128 records at a
// 64-byte stride. _InitAlarm links 64 records at a 16-byte stride, touching only
// their next words and the head. Its other fields retain loader-initial zeros.
// The handle encoding discards six counter-address bits and four alarm-address
// bits. Their base alignment must preserve the same pointer/handle round trip.
// Sizes describe these owned regions, not arbitrary-handle validity or bounds.
alignas(64) unsigned char norrath_sound_timer_counter_buffer_view[8192] = {};
alignas(16) unsigned char norrath_sound_timer_alarm_buffer_view[1024] = {};
alignas(4) unsigned char norrath_sound_timer_free_alarm_head_view[4] = {};
}

#undef NORRATH_TIMER_WORD
