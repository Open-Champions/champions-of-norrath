#pragma once
#include "sound_timer_infrastructure.hpp"
extern "C" {
// Whole real16-byte wrappers stay unfinished. Both move raw64A0 to raw64A1
// before entering real SetT2; the eventual hardwareSW is32 bits. No guessed
// kernel/device status or original-address provider is supplied.
void norrath_sound_timer_set_compare_register(unsigned long long raw_word);
void norrath_sound_timer_set_mode_register(unsigned long long raw_word);
}
namespace norrath_sound_timer_helpers_detail {
using namespace norrath_sound_timer_infrastructure_detail;
// These are genuine native device addresses from original LUI/ORI accesses,
// not ELF views, address aliases, source storage owners or mapped responses.
constexpr Address native_count_register=0x10001000u;
constexpr Address native_mode_register=0x10001010u;
__attribute__((always_inline)) inline Raw64 load_unsigned_word(Address at) {
    Raw64 value;
    asm volatile("lwu %0,0(%1)":"=r"(value):"r"(at):"memory");
    return value;
}
inline bool signed_delta_before_29491(Raw64 delta) {
    // Original DSUBU then signed64 SLTI29491. Unsigned C++ arithmetic keeps
    // wrapping; explicit sign bit avoids host signed-overflow assumptions.
    return signed_negative(delta)||delta<29491ull;
}
}
