// Observed _printf at 0x002d6aa0, 1524 bytes: the formatter scePrintf calls
// (see libc-format-sce-printf). Arguments: the format text and a cursor over
// 8-byte argument slots; each conversion reads the slot at the cursor's low
// end and advances by 8. Characters go out one at a time through the
// function pointer `_putchar` (0x00444960), reloaded for every call.
//
// Observed behavior:
//  * Interrupts are disabled with DIntr at entry; if it returned nonzero,
//    EIntr is called at exit and its result is returned. Otherwise 0.
//  * A character other than '%' is sent as a signed char.
//  * After '%', each character selects by (character - '0') in 0..72:
//    '0'  : width. If one digit follows, that is the width; if two follow, the
//           two-digit value, limited to 31. A width above 0 pre-fills that
//           many '0' bytes ending just before the buffer's last byte, and
//           the output starts at that pad start. A '0' not followed by a digit
//           is skipped. Any width character other than '0' (for instance the
//           '5' in "%5d") is not handled as width; see default.
//    'l','h': set the modifier (108 or 104) and continue with the next char.
//    'd'  : signed. 'l' reads 64 bits, 'h' a sign-extended halfword, otherwise a
//           sign-extended word. Zero prints "0". A negative sends '-' first
//           (before any zero pad) and prints the magnitude in decimal.
//    'u'  : unsigned decimal; 'l' 64 bits, 'h' zero-extended halfword, else word.
//    'o','x': octal and lowercase hexadecimal, same widths as 'u'.
//    'e','f': the 64-bit slot is converted by dptofp to a float. Zero prints
//           "0"; any other value is converted back with fptodp and handed to
//           printfloat (0x002d6938, a local routine of the original).
//    's'  : a 32-bit pointer. If the FIRST byte is zero it prints "(null)";
//           otherwise it prints bytes up to the NUL. A null pointer faults.
//    'c'  : the low signed byte of the slot.
//    anything else, including NUL and '1'..'9': skip that one character and
//           resume plain scanning after it (so "%5d" prints "d").
//  * Number text is built backwards in a 32-byte buffer ending in a NUL. If a
//    pad start is set and lies below the digits, output starts at the pad.
// Inferred: the pad start state is reset for every plain character and every
// '%' sequence; a modifier also resets for each. Unknown: what lies beyond
// the ones listed, and the unused return contract of _printf.
extern "C" {
extern int (*_putchar)(int character);
int DIntr();
int EIntr();
float dptofp(unsigned long long bits);
unsigned long long norrath_entity_fptodp(float value);
void printfloat(unsigned long long bits);
#if !defined(__mips__)
// Host builds cannot hold 32-bit addresses; the host check maps words to text.
const char* norrath_printf_host_text(unsigned word);
#endif
}

namespace {
inline int put(int character) { return _putchar(character); }

inline void put_text(const char* text) {
    while (*text != '\0') put(*text++);
}

template <typename T>
inline T slot(const unsigned char* cursor) {
    return *reinterpret_cast<const T*>(cursor - 8);
}

// Octal and hexadecimal digits of `value`, written backwards from `out`.
inline char* radix_digits(char* out, unsigned long long value, unsigned shift) {
    do {
        const unsigned digit = static_cast<unsigned>(value & ((1u << shift) - 1u));
        *--out = static_cast<char>(digit < 10u ? 48u + digit : 87u + digit);
        value >>= shift;
    } while (value != 0u);
    return out;
}

inline char* unsigned_decimal(char* out, unsigned long long value) {
    do {
        *--out = static_cast<char>(48u + static_cast<unsigned>(value % 10u));
        value = value / 10u;
    } while (value != 0u);
    return out;
}

// The signed path divides with the signed helpers after negating, as the
// original does, so the magnitude of the most negative 64-bit value keeps
// producing signed remainders.
inline char* signed_decimal(char* out, long long value) {
    do {
        *--out = static_cast<char>(48 + static_cast<int>(value % 10));
        value = value / 10;
    } while (value != 0);
    return out;
}

inline void emit(char* out, const char* pad) {
    if (pad != nullptr && pad < out) out = const_cast<char*>(pad);
    while (*out != '\0') put(*out++);
}

inline unsigned long long unsigned_slot(const unsigned char* cursor, int modifier) {
    if (modifier == 108) return slot<unsigned long long>(cursor);
    if (modifier == 104) return slot<unsigned short>(cursor);
    return slot<unsigned>(cursor);
}
}  // namespace

extern "C" int _printf(const char* format, const void* arguments) {
    const unsigned char* cursor = static_cast<const unsigned char*>(arguments);
    const int interrupts_were_on = DIntr();
    char buffer[32];
    while (*format != '\0') {
        const char* pad = nullptr;
        if (*format != '%') {
            put(*format);
            ++format;
            continue;
        }
        int modifier = 0;
        const char* p = format + 1;
        for (;;) {
            const signed char index = static_cast<signed char>(*p - '0');
            if (index < 0 || index > 72) { format = p + 1; break; }
            const char selector = *p;
            if (selector == '0') {
                const int first = p[1] - 48;
                if (static_cast<unsigned char>(first) < 10u) {
                    int width = first;
                    const char* last = p + 1;
                    if (static_cast<unsigned>(p[2] - 48) < 10u) {
                        width = first * 10 + (p[2] - 48);
                        last = p + 2;
                        if (width >= 32) width = 31;
                    }
                    if (width > 0) {
                        pad = buffer + 31 - width;
                        for (int count = width; count > 0; --count) buffer[31 - count] = '0';
                    }
                    p = last + 1;
                } else {
                    ++p;
                }
            } else if (selector == 'l') {
                modifier = 108;
                ++p;
            } else if (selector == 'h') {
                modifier = 104;
                ++p;
            } else if (selector == 'd') {
                cursor += 8;
                long long value = modifier == 108 ? slot<long long>(cursor)
                                : modifier == 104 ? slot<short>(cursor)
                                                  : slot<int>(cursor);
                buffer[31] = '\0';
                char* out = buffer + 31;
                if (value == 0) {
                    *--out = '0';
                } else {
                    if (value < 0) {
                        value = -value;
                        put('-');
                    }
                    out = signed_decimal(out, value);
                }
                emit(out, pad);
                format = p + 1;
                break;
            } else if (selector == 'u') {
                cursor += 8;
                const unsigned long long value = unsigned_slot(cursor, modifier);
                buffer[31] = '\0';
                char* out = buffer + 31;
                if (value == 0u) *--out = '0';
                else out = unsigned_decimal(out, value);
                emit(out, pad);
                format = p + 1;
                break;
            } else if (selector == 'o') {
                cursor += 8;
                const unsigned long long value = unsigned_slot(cursor, modifier);
                buffer[31] = '\0';
                char* out = buffer + 31;
                if (value == 0u) *--out = '0';
                else out = radix_digits(out, value, 3u);
                emit(out, pad);
                format = p + 1;
                break;
            } else if (selector == 'x') {
                cursor += 8;
                const unsigned long long value = unsigned_slot(cursor, modifier);
                buffer[31] = '\0';
                char* out = buffer + 31;
                if (value == 0u) *--out = '0';
                else out = radix_digits(out, value, 4u);
                emit(out, pad);
                format = p + 1;
                break;
            } else if (selector == 'e' || selector == 'f') {
                cursor += 8;
                const float value = dptofp(slot<unsigned long long>(cursor));
                if (value == 0.0f) put('0');
                else printfloat(norrath_entity_fptodp(value));
                format = p + 1;
                break;
            } else if (selector == 's') {
                cursor += 8;
#if defined(__mips__)
                const char* text = reinterpret_cast<const char*>(slot<unsigned>(cursor));
#else
                const char* text = norrath_printf_host_text(slot<unsigned>(cursor));
#endif
                if (*text == '\0') put_text("(null)");
                else while (*text != '\0') put(*text++);
                format = p + 1;
                break;
            } else if (selector == 'c') {
                cursor += 8;
                put(static_cast<int>(slot<signed char>(cursor)));
                format = p + 1;
                break;
            } else {
                format = p + 1;
                break;
            }
        }
    }
    if (interrupts_were_on != 0) return EIntr();
    return 0;
}
