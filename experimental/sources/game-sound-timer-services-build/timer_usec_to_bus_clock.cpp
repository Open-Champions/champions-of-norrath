#include "sound_time_conversion.hpp"
using namespace norrath_sound_time_conversion_detail;
extern "C" unsigned long long norrath_sound_timer_usec_to_bus_clock(
    unsigned long long seconds,unsigned long long microseconds) {
    // Both original MULTU units consume their operands' low32 words. Each
    // MFHI/MFLO pair is assembled as an unsigned64 product by DSLL32/DSRL32.
    // C++ unsigned arithmetic expresses that whole exact bit algorithm; no
    // foreign HI/LO value, guessed timing conversion or kernel reply is read.
    const unsigned seconds_word=static_cast<unsigned>(seconds);
    const unsigned microseconds_word=static_cast<unsigned>(microseconds);
    const Raw64 seconds_product=Raw64(seconds_word)*bus_clock_word_multiplier;
    const Raw64 microseconds_product=
        Raw64(microseconds_word)*bus_clock_word_multiplier;
    const Raw64 fractional_product=norrath_factory_udiv64(
        microseconds_product,microseconds_divisor);
    return seconds_product+fractional_product;
}
