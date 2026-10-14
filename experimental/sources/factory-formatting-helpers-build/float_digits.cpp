#include "formatter_helpers.hpp"
extern "C" unsigned char* norrath_factory_float_digits(unsigned char* reent, NorrathFormatUnsigned64 bits, int precision, unsigned flags, unsigned char* sign_byte, unsigned char* decimal_position_word, int format_character, unsigned char* digit_count_word) {
    using namespace norrath_core_format_detail;
    int mode = 3;
    if (format_character != 'f') {
        mode = 2;
        if (format_character == 'e' || format_character == 'E') precision = add(precision, 1);
    }
    Wide value = bits;
    if (value & 0x8000000000000000ULL) {
        value = norrath_factory_double_subtract(0ULL, value);
        sign_byte[0] = '-';
    } else sign_byte[0] = 0;
    // Actual eight-byte local dtoa outputs, not initialized surrogate answers.
    alignas(4) unsigned char ignored_sign_word[4];
    alignas(4) unsigned char returned_end_word[4];
    Word digits = address(norrath_factory_dtoa_reentrant(reent, value, mode, precision,
        decimal_position_word, ignored_sign_word, returned_end_word));
    const bool trimmed_general = (format_character == 'g' || format_character == 'G') && (flags & 1u) == 0u;
    if (!trimmed_general) {
        Word target_end = digits + Word(precision);
        if (format_character == 'f') {
            if (bytes(digits)[0] == '0') {
                if (norrath_factory_double_compare(value, 0ULL) != 0)
                    write_word(address(decimal_position_word), 1u - Word(precision));
            }
            target_end += read_word(address(decimal_position_word));
        }
        const int compared = norrath_factory_double_compare(value, 0ULL);
        Word end = read_word(address(returned_end_word));
        if (compared == 0) end = target_end;
        write_word(address(returned_end_word), end);
        while (end < target_end) {
            const Word after = end + 1u;
            bytes(end)[0] = '0';
            write_word(address(returned_end_word), after);
            end = after;
        }
    }
    const Word end = read_word(address(returned_end_word));
    write_word(address(digit_count_word), end - digits);
    return bytes(digits);
}
