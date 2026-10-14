#pragma once
#include "skill_progression.hpp"
extern "C" {
int norrath_sound_play_sequence(void* animation_header, void* object, float gain);
// Same actual complete132-byte animAdd original already recovered in services.
void norrath_actor_animation_add(void* animation_state, void* animation_header, int flags);
void norrath_list_destroy(void* head);
int norrath_game_is_paused();
// Observed addresses only; no sound table, state allocation or initializer.
extern unsigned char norrath_sound_sequence_records_view[];
extern unsigned char norrath_sound_group_limits_view[];
extern unsigned char norrath_sound_sequence_id_spinner_view[];
}
namespace norrath_sound_sequence_detail {
using namespace norrath_skill_progression_detail;
using Wide = unsigned long long;
inline unsigned read_byte_fresh(Address a) {
    return *static_cast<const volatile unsigned char*>(bytes(a));
}
inline void write_byte_ordered(Address a, unsigned value) {
    *static_cast<volatile unsigned char*>(bytes(a)) = static_cast<unsigned char>(value);
}
inline Wide read_wide_fresh(Address a) {
    const unsigned low = read_word_fresh(a), high = read_word_fresh(a + 4u);
    return Wide(low) | (Wide(high) << 32);
}
inline void write_wide_ordered(Address a, Wide value) {
    write_word_ordered(a, unsigned(value));
    write_word_ordered(a + 4u, unsigned(value >> 32));
}
inline void write_float_ordered(Address a, float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    const unsigned bits = unsigned(p[0]) | (unsigned(p[1]) << 8) |
                          (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
    write_word_ordered(a, bits);
}
}
