// Observed: VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v at 0x011f59d0, 2024 bytes, pinned SLUS_205.65: the instantiation of the VI
// formatter for 16-bit characters (the template argument is unsigned short), the sibling of the char instantiation at
// 0x011f52e0. Arguments: 16-bit output buffer (a0), size (a1), 16-bit format (a2), argument block (a3). Returns nothing. The
// argument block is read as 8-byte slots as in the narrow instantiation. The size is halved with an unsigned shift to give
// DstCount, the number of 16-bit units (so a negative size gives a huge capacity and passes the check).
//  1. DstCount must be positive; otherwise _VIAssert("DstCount > 0", "../core/vistring.cpp", 662) is called.
//  2. While at least two units of room remain (written + 1 < DstCount) and the format is not at its end:
//     a. A format unit other than '%' is copied unchanged.
//     b. After '%' the format units (the '%' included) are converted one at a time to a byte and stored in a 64-byte
//        specification buffer: a unit below 256 gives its low byte, a larger unit gives the byte for its first match in
//        _8VIString$ConversionTable1252 (ConversionTableSize.180 entries; index - 128), no match gives '?'. The scan ends at the
//        first converted byte, from the second position on, that is one of "cCdiIouxXeEfgGnpsSh%"; a unit that converts to
//        zero, or 63 stored bytes, ends the whole conversion and the output is terminated where it stands.
//     c. That last byte selects the conversion:
//        c, C    the low 16 bits of the next slot are stored as one unit when room remains.
//        %       one '%' unit.
//        d i o u x X  the next slot's low 32 bits are passed with the whole specification to sprintf into a 64-byte buffer; each
//                byte of the result is widened (bytes 0x80..0x9f through the table, others unchanged) and appended while room
//                remains.
//        I       the next slot's 64 bits are formatted with the text "%lli" and appended the same way.
//        e E f g G  the next slot's 64 bits (a double passed by bits) are formatted with the specification and appended.
//        p       the next slot's low 32 bits are formatted with the specification and appended.
//        n       the next slot's low 32 bits are an address that receives the count of units written so far, then it continues
//                as p with the following slot (no break in the original).
//        s       the next slot is the address of a narrow string (the text "(null)" when zero), widened as above and appended.
//        S       the next slot is the address of a 16-bit string (the 16-bit text "(null)" when zero). Each unit is stored; the
//                source unit is then read again and the stored unit is replaced by '?' when they differ (it can differ only when
//                the output overlaps the source). The scan continues after the output is full.
//        h       the next slot is the address of a byte block and the one after it a count: each byte as two upper-case hex digits,
//                with one space after every fourth byte that is followed by another byte.
//        any other byte (unreachable) calls _VIAssertf after storing the file name and line 791.
//  3. The output is zero terminated at the position reached.
// Inferred: the wide twin of the narrow formatter; the narrow-to-wide widening is the Windows-1252-style table also used by
// str8to16. Unknown: the owner of the argument-block layout and the meaning of the assertion text beyond what it prints.
#ifdef NORRATH_HOST_CONTRACT
extern "C" void* norrath_host_buffer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_buffer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
extern const unsigned short _8VIString$ConversionTable1252[32];
extern int norrath_conversion_table_size __asm__("ConversionTableSize.180");
extern const char* __VIAssertf_file;
extern int __VIAssertf_line;
void _VIAssert(const char* expression, const char* file, int line);
void _VIAssertf(int condition, const char* format, ...);
int sprintf(char* destination, const char* format, ...);

static inline int slot_int(const void* slot) { return *static_cast<const int*>(slot); }
static inline long long slot_long(const void* slot) { return *static_cast<const long long*>(slot); }
static inline unsigned slot_word(const void* slot) { return *static_cast<const unsigned*>(slot); }

static inline unsigned short widen_byte(unsigned char value) {
    if (static_cast<unsigned char>(value + 128) < 32) return _8VIString$ConversionTable1252[value - 128];
    return value;
}

static void append_narrow(unsigned short* destination, int capacity, int& count, const char* text) {
    for (; *text != 0; ++text) {
        if (count + 1 < capacity) destination[count++] = widen_byte(static_cast<unsigned char>(*text));
    }
}

void VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v(unsigned short* destination, int size, const unsigned short* format, void* args) {
    static const char kSpecialTerminators[] = "cCdiIouxXeEfgGnpsSh%";
    static const char kHexDigits[] = "0123456789ABCDEF";
    const unsigned char* next_slot = static_cast<const unsigned char*>(args);
    const int capacity = static_cast<int>(static_cast<unsigned>(size) >> 1);
    int count = 0;
    int position = 0;
    if (capacity <= 0) _VIAssert("DstCount > 0", "../core/vistring.cpp", 662);
    while (count + 1 < capacity && format[position] != 0) {
        const unsigned short current = format[position];
        if (current != '%') {
            destination[count++] = current;
            ++position;
            continue;
        }
        char specification[64];
        char terminator;
        int length = 0;
        for (;;) {
            const unsigned short unit = format[position];
            char converted;
            if (unit < 256) {
                converted = static_cast<char>(unit);
            } else {
                converted = '?';
                for (int index = 0; index < norrath_conversion_table_size; ++index) {
                    if (unit == _8VIString$ConversionTable1252[index]) {
                        converted = static_cast<char>(index - 128);
                        break;
                    }
                }
            }
            specification[length++] = converted;
            ++position;
            bool found = false;
            if (length >= 2) {
                for (const char* member = kSpecialTerminators; *member != 0; ++member) {
                    if (*member == converted) {
                        found = true;
                        break;
                    }
                }
            }
            if (found) {
                terminator = converted;
                break;
            }
            if (length == 63 || converted == 0) goto finish;
        }
        specification[length] = 0;
        {
            char text[64];
            switch (terminator) {
                case 'c':
                case 'C':
                    if (count + 1 >= capacity) goto finish;
                    destination[count++] = *reinterpret_cast<const unsigned short*>(next_slot);
                    next_slot += 8;
                    break;
                case '%':
                    if (count + 1 >= capacity) goto finish;
                    destination[count++] = '%';
                    break;
                case 'd':
                case 'i':
                case 'o':
                case 'u':
                case 'x':
                case 'X': {
                    const int value = slot_int(next_slot);
                    next_slot += 8;
                    sprintf(text, specification, value);
                    append_narrow(destination, capacity, count, text);
                    break;
                }
                case 'I': {
                    const long long value = slot_long(next_slot);
                    next_slot += 8;
                    sprintf(text, "%lli", value);
                    append_narrow(destination, capacity, count, text);
                    break;
                }
                case 'e':
                case 'E':
                case 'f':
                case 'g':
                case 'G': {
                    const long long bits = slot_long(next_slot);
                    next_slot += 8;
                    sprintf(text, specification, bits);
                    append_narrow(destination, capacity, count, text);
                    break;
                }
                case 'n':
                    *static_cast<int*>(NORRATH_WORD_TO_POINTER(slot_word(next_slot))) = count;
                    next_slot += 8;
                    // fall through: the original continues into the 'p' conversion with the next slot
                case 'p': {
                    const int value = slot_int(next_slot);
                    next_slot += 8;
                    sprintf(text, specification, value);
                    append_narrow(destination, capacity, count, text);
                    break;
                }
                case 's': {
                    const char* string = static_cast<const char*>(NORRATH_WORD_TO_POINTER(slot_word(next_slot)));
                    next_slot += 8;
                    if (string == nullptr) string = "(null)";
                    append_narrow(destination, capacity, count, string);
                    break;
                }
                case 'S': {
                    static const unsigned short kNull16[] = {'(', 'n', 'u', 'l', 'l', ')', 0};
                    const unsigned short* wide = static_cast<const unsigned short*>(NORRATH_WORD_TO_POINTER(slot_word(next_slot)));
                    next_slot += 8;
                    if (wide == nullptr) wide = kNull16;
                    for (; *wide != 0; ++wide) {
                        if (count + 1 < capacity) {
                            const unsigned short unit = *wide;
                            destination[count] = unit;
                            if (unit != *wide) destination[count] = '?';
                            ++count;
                        }
                    }
                    break;
                }
                case 'h': {
                    const unsigned char* block = static_cast<const unsigned char*>(NORRATH_WORD_TO_POINTER(slot_word(next_slot)));
                    next_slot += 8;
                    const int bytes = slot_int(next_slot);
                    next_slot += 8;
                    for (int index = 0; index < bytes; ++index) {
                        destination[count++] = static_cast<unsigned short>(static_cast<signed char>(kHexDigits[block[index] >> 4]));
                        if (count + 1 == capacity) break;
                        destination[count++] = static_cast<unsigned short>(static_cast<signed char>(kHexDigits[block[index] & 15]));
                        if (count + 1 == capacity) break;
                        if (index + 1 < bytes && ((index + 1) & 3) == 0) {
                            destination[count++] = ' ';
                            if (count + 1 == capacity) break;
                        }
                    }
                    break;
                }
                default:
                    __VIAssertf_file = "../core/vistring.cpp";
                    __VIAssertf_line = 791;
                    _VIAssertf(0, "VIString::vsnprintf16 unimplemented format '%s'", specification);
                    break;
            }
        }
    }
finish:
    destination[count] = 0;
}
}
