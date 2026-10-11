#pragma once
#include "sound_timer_delay.hpp"
extern "C" {
// Byte-equivalent function type to existing genuine number_parser.hpp:
// NorrathUnsigned64 is unsigned long long. No new provider is declared.
unsigned long long norrath_factory_udiv64(
    unsigned long long numerator,unsigned long long denominator);
}
namespace norrath_sound_time_conversion_detail {
using namespace norrath_sound_timer_delay_detail;
static_assert(sizeof(unsigned)==4 && sizeof(Raw64)==8,"actual MULTU word operands and full64 product/result");
constexpr Raw64 bus_clock_word_multiplier=0x08ca0000ull;
constexpr Raw64 microseconds_divisor=0x000f4240ull;
}
