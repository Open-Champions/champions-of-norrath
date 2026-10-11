#include "formatter_helpers.hpp"
extern "C" int norrath_factory_format_exponent(unsigned char* destination, int exponent, int format_character) {
    using namespace norrath_core_format_detail;
    const Word beginning = address(destination);
    bytes(beginning)[0] = static_cast<unsigned char>(format_character);
    Word output = beginning + 1u;
    int magnitude = exponent;
    if (magnitude < 0) {
        magnitude = negate(magnitude);  // Preserves original signed32 INT_MIN wrap.
        bytes(output)[0] = '-';
    } else bytes(output)[0] = '+';
    ++output;
    alignas(16) unsigned char reverse[308];
    const Word end = address(reverse) + 308u;
    Word digit = end;
    if (magnitude >= 10) {
        do {
            const int quotient = magnitude / 10;
            const int remainder = magnitude % 10;
            --digit;
            bytes(digit)[0] = static_cast<unsigned char>(add(remainder, 48));
            magnitude = quotient;
        } while (magnitude >= 10);
        --digit;
        bytes(digit)[0] = static_cast<unsigned char>(add(magnitude, 48));
        while (digit < end) {
            unsigned char byte = bytes(digit)[0];
            ++digit;
            bytes(output)[0] = byte;
            ++output;
        }
    } else {
        bytes(output)[0] = '0';
        ++output;
        bytes(output)[0] = static_cast<unsigned char>(add(magnitude, 48));
        ++output;
    }
    return signed_word(output - beginning);  // Exactly no terminal NUL store.
}
