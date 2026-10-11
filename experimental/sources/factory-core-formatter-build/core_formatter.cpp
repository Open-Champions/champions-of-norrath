#include "core_formatter.hpp"

extern "C" int norrath_factory_vfprintf_reentrant(unsigned char* reent, unsigned char* stream, const char* format, const unsigned char* argument_cursor) {
    using namespace norrath_core_format_detail;
    enum { ALT = 1u, HEXPREFIX = 2u, LADJUST = 4u, LONGDBL = 8u,
           LONGINT = 16u, QUADINT = 32u, SHORTINT = 64u,
           ZEROPAD = 128u, FLOATING = 256u };
    const Word file = address(stream);
    Word next = address(format);
    Word arguments = address(argument_cursor);
    const Word locale = address(norrath_factory_localeconv());
    const Word decimal_point = read_word(locale);
    alignas(16) unsigned char state[4];
    Word file_flags = read_half(file + 12u);
    write_word(address(state), 0u);
    if ((file_flags & 8u) == 0u || read_word(file + 16u) == 0u) {
        if (norrath_factory_stream_write_setup(stream) != 0) return -1;
        file_flags = read_half(file + 12u);
    }
    if ((file_flags & 0x1au) == 0xau) {
        Word fd = read_half(file + 14u);
        if ((fd & 0x8000u) == 0u)
            return norrath_factory_buffered_printf(stream, format, argument_cursor);
    }

    OutputQueue output(stream);
    alignas(16) unsigned char work[348];
    alignas(16) unsigned char exponent_text[16];
    unsigned char prefix[2];
    unsigned char sign;
    unsigned char float_sign;
    alignas(4) unsigned char wide_character[4];
    alignas(4) unsigned char decimal_position[4];
    alignas(4) unsigned char digit_count[4];
    int result = 0;
    Word flags;
    int width, precision, numeric_precision, size, effective_size;
    int character, base, exponent_length;
    Word data, alphabet;
    Wide number, floating_bits;
    bool signed_number;

    for (;;) {
        // 306ba8..306c50: the live global context/maximum are freshly read
        // for every actual _mbtowc_r call, including the terminal scan.
        const Word literal_start = next;
        int consumed;
        for (;;) {
            Word live_reent = read_word(address(norrath_factory_impure_pointer_word));
            Word maximum_bytes = read_word(address(norrath_factory_mb_cur_max_word));
            consumed = norrath_factory_mbtowc_reentrant(bytes(live_reent), wide_character,
                reinterpret_cast<const char*>(bytes(next)), maximum_bytes, state);
            if (consumed <= 0) break;
            Word wc = read_word(address(wide_character));
            next += Word(consumed);
            if (wc == 37u) { next -= 1u; break; }
        }
        int literal_length = signed_word(next - literal_start);
        if (literal_length != 0) {
            if (!output.emit(literal_start, literal_length)) goto finish;
            result = add(result, literal_length);
        }
        if (consumed <= 0) goto complete;
        ++next;
        sign = 0;
        flags = 0u;
        width = 0;
        precision = -1;
        numeric_precision = 0;

    read_conversion:
        character = signed_byte(bytes(next)[0]);
        ++next;
    dispatch_conversion:
        switch (character) {
        case ' ':
            if (sign == 0) sign = ' ';
            goto read_conversion;
        case '#': flags |= ALT; goto read_conversion;
        case '*': {
            width = signed_word(read_word(arguments));
            arguments += 8u;
            if (width < 0) { width = negate(width); flags |= LADJUST; }
            goto read_conversion;
        }
        case '+': sign = '+'; goto read_conversion;
        case '-': flags |= LADJUST; goto read_conversion;
        case '.': {
            character = signed_byte(bytes(next)[0]);
            ++next;
            if (character == '*') {
                precision = signed_word(read_word(arguments));
                arguments += 8u;
                if (precision < -1) precision = -1;
                goto read_conversion;
            }
            Word digits = 0u;
            while (Word(character) - 48u < 10u) {
                digits = digits * 10u + Word(character) - 48u;
                character = signed_byte(bytes(next)[0]);
                ++next;
            }
            precision = signed_word(digits);
            if (precision < -1) precision = -1;
            goto dispatch_conversion;
        }
        case '0': flags |= ZEROPAD; goto read_conversion;
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': {
            Word digits = 0u;
            do {
                digits = digits * 10u + Word(character) - 48u;
                character = signed_byte(bytes(next)[0]);
                ++next;
            } while (Word(character) - 48u < 10u);
            width = signed_word(digits);
            goto dispatch_conversion;
        }
        case 'L': flags |= LONGDBL; goto read_conversion;
        case 'h': flags |= SHORTINT; goto read_conversion;
        case 'l':
            character = signed_byte(bytes(next)[0]);
            ++next;
            if (character == 'l') { flags |= QUADINT; goto read_conversion; }
            flags |= LONGINT;
            goto dispatch_conversion;
        case 'q': flags |= QUADINT; goto read_conversion;
        case 'c':
            work[0] = bytes(arguments)[0];
            arguments += 8u;
            data = address(work);
            size = 1;
            sign = 0;
            goto emit_conversion;
        case 'D': flags |= LONGINT; [[fallthrough]];
        case 'd': case 'i':
            number = read_integer(arguments, flags, true);
            arguments += 8u;
            base = 1;
            if (number & 0x8000000000000000ULL) {
                number = 0ULL - number;
                sign = '-';
            }
            signed_number = true;
            goto integer_digits;
        case 'E': case 'G': case 'e': case 'f': case 'g':
            if (precision == -1) precision = 6;
            else if ((character == 'g' || character == 'G') && precision == 0) precision = 1;
            floating_bits = read_wide(arguments);
            arguments += 8u;
            if (norrath_factory_double_isinf(floating_bits) != 0) {
                if (norrath_factory_double_compare(floating_bits, 0ULL) < 0) sign = '-';
                data = address(norrath_formatter_inf_text_view);
                size = 3;
                goto emit_conversion;
            }
            if (norrath_factory_double_isnan(floating_bits) != 0) {
                data = address(norrath_formatter_nan_text_view);
                size = 3;
                goto emit_conversion;
            }
            flags |= FLOATING;
            data = address(norrath_factory_float_digits(reent, floating_bits, precision, flags,
                &float_sign, decimal_position, character, digit_count));
            if (character == 'g' || character == 'G') {
                int position = signed_word(read_word(address(decimal_position)));
                if (position < -3 || precision < position)
                    character = character == 'g' ? 'e' : 'E';
                else character = 'g';
            }
            if (character < 'f') {
                int position = subtract(signed_word(read_word(address(decimal_position))), 1);
                write_word(address(decimal_position), Word(position));
                exponent_length = norrath_factory_format_exponent(exponent_text, position, character);
                int digits = signed_word(read_word(address(digit_count)));
                size = add(exponent_length, digits);
                if (digits >= 2 || (flags & ALT)) size = add(size, 1);
            } else if (character == 'f') {
                int position = signed_word(read_word(address(decimal_position)));
                if (position <= 0) size = add(precision, 2);
                else {
                    size = position;
                    if (precision != 0 || (flags & ALT)) size = add(add(position, precision), 1);
                }
            } else {
                int position = signed_word(read_word(address(decimal_position)));
                int digits = signed_word(read_word(address(digit_count)));
                if (position >= digits) size = add(position, (flags & ALT) != 0u ? 1 : 0);
                else if (position > 0) size = add(digits, 1);
                else size = add(subtract(digits, position), 2);
            }
            if (float_sign != 0) sign = '-';
            goto emit_conversion;
        case 'n': {
            Word destination = read_word(arguments);
            arguments += 8u;
            if (flags & LONGINT) write_wide(destination, extend_signed_word(Word(result)));
            else if (flags & SHORTINT) write_half(destination, Word(result));
            else write_word(destination, Word(result));
            continue;  // Actual no conversion flush/count update before next scan.
        }
        case 'O': flags |= LONGINT; [[fallthrough]];
        case 'o':
            number = read_integer(arguments, flags, false);
            arguments += 8u;
            base = 0;
            signed_number = false;
            goto integer_digits;
        case 'p':
            // Original LW sign-extends this pointer into the full64 magnitude.
            number = extend_signed_word(read_word(arguments));
            arguments += 8u;
            flags |= HEXPREFIX;
            alphabet = address(norrath_formatter_hex_lower_view);
            character = 'x';
            base = 2;
            signed_number = false;
            goto integer_digits;
        case 's': {
            data = read_word(arguments);
            arguments += 8u;
            if (data == 0u) data = address(norrath_formatter_null_text_view);
            if (precision >= 0) {
                Word found = address(norrath_factory_memchr(bytes(data), 0, Word(precision)));
                size = found == 0u ? precision : signed_word(found - data);
                if (precision < size) size = precision;
            } else size = signed_word(norrath_factory_strlen(reinterpret_cast<const char*>(bytes(data))));
            sign = 0;
            goto emit_conversion;
        }
        case 'U': flags |= LONGINT; [[fallthrough]];
        case 'u':
            number = read_integer(arguments, flags, false);
            arguments += 8u;
            base = 1;
            signed_number = false;
            goto integer_digits;
        case 'X': case 'x':
            alphabet = address(character == 'X' ? norrath_formatter_hex_upper_view : norrath_formatter_hex_lower_view);
            number = read_integer(arguments, flags, false);
            arguments += 8u;
            if ((flags & ALT) && number != 0ULL) flags |= HEXPREFIX;
            base = 2;
            signed_number = false;
            goto integer_digits;
        default:
            if (character == 0) goto complete;
            // F, high signed bytes and every unrecognized specifier are literal
            // one-byte conversions, with their original parsed field width.
            work[0] = static_cast<unsigned char>(character);
            data = address(work);
            size = 1;
            sign = 0;
            goto emit_conversion;
        }

    integer_digits:
        // QUADINT alone is intentionally not an integer-width selector.
        if (!signed_number) sign = 0;
        numeric_precision = precision;
        if (precision >= 0) flags &= ~Word(ZEROPAD);
        data = address(work) + 348u;
        if (number != 0ULL || numeric_precision != 0) {
            switch (base) {
            case 0: {
                unsigned char first;
                do {
                    first = static_cast<unsigned char>((number & 7ULL) + 48ULL);
                    number >>= 3;
                    --data;
                    bytes(data)[0] = first;
                } while (number != 0ULL);
                if ((flags & ALT) && first != '0') { --data; bytes(data)[0] = '0'; }
                break;
            }
            case 1:
                while (number >= 10ULL) {
                    Wide remainder = norrath_factory_umod64(number, 10ULL);
                    --data;
                    bytes(data)[0] = static_cast<unsigned char>(remainder + 48ULL);
                    number = norrath_factory_udiv64(number, 10ULL);
                }
                --data;
                bytes(data)[0] = static_cast<unsigned char>(number + 48ULL);
                break;
            case 2:
                do {
                    Word digit = Word(number & 15ULL);
                    --data;
                    number >>= 4;
                    bytes(data)[0] = bytes(alphabet + digit)[0];
                } while (number != 0ULL);
                break;
            default:
                data = address(norrath_formatter_bad_base_view);
                size = signed_word(norrath_factory_strlen(reinterpret_cast<const char*>(bytes(data))));
                goto emit_conversion;
            }
        }
        size = signed_word(address(work) + 348u - data);

    emit_conversion:
        // 307364..307727: every piece remains queued by address; output calls
        // may change stream/input memory before subsequent reads/conversions.
        effective_size = maximum(size, numeric_precision);
        if (sign != 0) effective_size = add(effective_size, 1);
        else effective_size = add(effective_size, int(flags & HEXPREFIX));
        if ((flags & (LADJUST | ZEROPAD)) == 0u)
            if (!output.pad(norrath_formatter_space16_view, subtract(width, effective_size))) goto finish;
        if (sign != 0) {
            if (!output.emit(address(&sign), 1)) goto finish;
        } else if (flags & HEXPREFIX) {
            prefix[0] = '0'; prefix[1] = static_cast<unsigned char>(character);
            if (!output.emit(address(prefix), 2)) goto finish;
        }
        if ((flags & (LADJUST | ZEROPAD)) == ZEROPAD)
            if (!output.pad(norrath_formatter_zero16_view, subtract(width, effective_size))) goto finish;
        if (!output.pad(norrath_formatter_zero16_view, subtract(numeric_precision, size))) goto finish;

        if ((flags & FLOATING) == 0u) {
            if (!output.emit(data, size)) goto finish;
        } else if (character >= 'f') {
            if (norrath_factory_double_compare(floating_bits, 0ULL) == 0) {
                if (!output.emit(address(norrath_formatter_zero_text_view), 1)) goto finish;
                int position = signed_word(read_word(address(decimal_position)));
                int digits = signed_word(read_word(address(digit_count)));
                if (position < digits || (flags & ALT)) {
                    if (!output.emit(decimal_point, 1)) goto finish;
                    if (!output.pad(norrath_formatter_zero16_view, subtract(signed_word(read_word(address(digit_count))), 1))) goto finish;
                }
            } else {
                int position = signed_word(read_word(address(decimal_position)));
                if (position <= 0) {
                    if (!output.emit(address(norrath_formatter_zero_text_view), 1)) goto finish;
                    position = signed_word(read_word(address(decimal_position)));
                    int digits = signed_word(read_word(address(digit_count)));
                    if (position != 0 || digits != 0) {
                        if (!output.emit(decimal_point, 1)) goto finish;
                        if (!output.pad(norrath_formatter_zero16_view, negate(signed_word(read_word(address(decimal_position)))))) goto finish;
                        if (!output.emit(data, signed_word(read_word(address(digit_count))))) goto finish;
                    }
                } else if (position >= signed_word(read_word(address(digit_count)))) {
                    if (!output.emit(data, signed_word(read_word(address(digit_count))))) goto finish;
                    int zeros = subtract(signed_word(read_word(address(decimal_position))), signed_word(read_word(address(digit_count))));
                    if (!output.pad(norrath_formatter_zero16_view, zeros)) goto finish;
                    if (flags & ALT)
                        if (!output.emit(address(norrath_formatter_dot_text_view), 1)) goto finish;
                } else {
                    if (!output.emit(data, position)) goto finish;
                    position = signed_word(read_word(address(decimal_position)));
                    data += Word(position);
                    if (!output.emit(address(norrath_formatter_dot_text_view), 1)) goto finish;
                    int tail = subtract(signed_word(read_word(address(digit_count))), signed_word(read_word(address(decimal_position))));
                    if (!output.emit(data, tail)) goto finish;
                }
            }
        } else {
            int digits = signed_word(read_word(address(digit_count)));
            if (digits >= 2 || (flags & ALT)) {
                prefix[0] = bytes(data)[0];
                ++data;
                prefix[1] = '.';
                if (!output.emit(address(prefix), 2)) goto finish;
                if (norrath_factory_double_compare(floating_bits, 0ULL) != 0) {
                    if (!output.emit(data, subtract(signed_word(read_word(address(digit_count))), 1))) goto finish;
                } else {
                    if (!output.pad(norrath_formatter_zero16_view, subtract(signed_word(read_word(address(digit_count))), 1))) goto finish;
                }
            } else {
                if (!output.emit(data, 1)) goto finish;
            }
            if (!output.emit(address(exponent_text), exponent_length)) goto finish;
        }
        if (flags & LADJUST)
            if (!output.pad(norrath_formatter_space16_view, subtract(width, effective_size))) goto finish;
        // Count updates before this conversion-ending flush; a threshold failure
        // earlier in this conversion retains only the preceding count.
        result = add(result, maximum(effective_size, width));
        if (output.pending() != 0u && !output.flush()) goto finish;
        output.reset_count();
        output.reset_cursor();
    }

complete:
    if (output.pending() != 0u && !output.flush()) goto finish;
    output.reset_count();
finish:
    // A nonzero __sprint return alone does not force -1 in this original root.
    return (read_half(file + 12u) & 0x40u) ? -1 : result;
}
