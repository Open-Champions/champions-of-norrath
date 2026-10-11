#include "number_parser.hpp"
extern "C" NorrathSigned64 norrath_factory_strtol_reentrant(unsigned char* reent, const char* input, char** end_pointer, int base) {
    using namespace norrath_parser_detail;
    const Word begin = address(input);
    Word cursor = begin;
    int character;
    do {
        character = signed_byte(*bytes(cursor));
        cursor += 1u;
    } while ((classify(character) & 8u) != 0u);

    bool negative = false;
    if (character == 45) {
        negative = true;
        character = signed_byte(*bytes(cursor));
        cursor += 1u;
    } else if (character == 43) {
        character = signed_byte(*bytes(cursor));
        cursor += 1u;
    }
    if ((base == 0 || base == 16) && character == 48) {
        const int next = signed_byte(*bytes(cursor));
        if (next == 120 || next == 88) {
            character = signed_byte(*bytes(cursor + 1u));
            cursor += 2u;
            base = 16;
        }
    }
    if (base == 0) base = character == 48 ? 8 : 10;

    const NorrathUnsigned64 limit = negative ? 0x8000000000000000ULL : 0x7fffffffffffffffULL;
    const NorrathUnsigned64 radix = static_cast<NorrathUnsigned64>(NorrathSigned64(base));
    const int cutlim = signed_word(static_cast<Word>(norrath_factory_umod64(limit, radix)));
    const NorrathUnsigned64 cutoff = norrath_factory_udiv64(limit, radix);
    int any = 0;
    NorrathUnsigned64 accumulated = 0;
    for (;;) {
        const unsigned flags = classify(character);
        int digit;
        if ((flags & 4u) != 0u) {
            digit = character - 48;
        } else if ((flags & 3u) != 0u) {
            digit = character - ((flags & 1u) != 0u ? 55 : 87);
        } else {
            break;
        }
        // The actual body does not validate base range or reject digit<0.
        // Signed-byte original table classification decides the digit value.
        if (digit >= base) break;
        if (any < 0 || accumulated > cutoff || (accumulated == cutoff && digit > cutlim)) {
            any = -1;
        } else {
            accumulated = norrath_factory_mul64(accumulated, radix)
                + static_cast<NorrathUnsigned64>(NorrathSigned64(digit));
            any = 1;
        }
        character = signed_byte(*bytes(cursor));
        cursor += 1u;
    }

    if (any < 0) {
        accumulated = limit;
        write_word(address(reent), 34u);
    } else if (negative) {
        accumulated = 0ULL - accumulated;
    }
    if (end_pointer != nullptr) {
        write_word(address(end_pointer), any != 0 ? cursor - 1u : begin);
    }
    return signed64_bits(accumulated);
}
