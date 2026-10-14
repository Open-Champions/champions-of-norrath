// Observed: VI_vsnprintf__H1Zc_PX01iPCX01Pv_v at 0x011f52e0, 1772 bytes, pinned SLUS_205.65: the instantiation of the
// VI formatter for narrow characters (the template argument is char). Arguments: output buffer (a0), capacity DstCount
// (a1), format (a2), argument block (a3). Returns nothing. The argument block is read as a run of 8-byte slots, one per
// argument, taking the low bytes of a slot for narrower values.
//  1. DstCount must be positive; otherwise _VIAssert("DstCount > 0", "../core/vistring.cpp", 662) is called.
//  2. While at least two bytes of room remain (written + 1 < DstCount) and the format is not at its end:
//     a. A character other than '%' is copied to the output.
//     b. After '%' the format characters (the '%' included) are copied into a 64-byte specification buffer, ending with a
//        NUL, up to and including the first character, from the second one on, that is one of "cCdiIouxXeEfgGnpsSh%".
//        A format that ends inside a specification, or a specification of 63 characters, ends the whole conversion and
//        the output is terminated where it stands.
//     c. That last character selects the conversion:
//        c, C    one byte, the low byte of the next slot, copied when room remains.
//        %       one '%' when room remains.
//        d i o u x X  the next slot's low 32 bits are passed with the whole specification to sprintf into a 64-byte buffer;
//                the buffer is appended (truncated to the room left).
//        I       the next slot's 64 bits are formatted with the text "%lli" the same way.
//        e E f g G  the next slot's 64 bits (a double passed by bits) are formatted with the specification the same way.
//        p       the next slot's low 32 bits are formatted with the specification the same way.
//        n       the next slot's low 32 bits are an address that receives the count written so far; then it continues
//                as p with the following slot (there is no break between the two in the original).
//        s       the next slot is the address of a narrow string (the text "(null)" when zero), appended without
//                reading any width or precision.
//        S       the next slot is the address of a 16-bit string (the 16-bit text "(null)" when zero). Each unit below 256
//                is stored as its low byte, a larger unit is looked up in _8VIString$ConversionTable1252 (the first match,
//                ConversionTableSize.180 entries, gives the byte index + 128), and a unit with no match becomes '?'. After
//                storing, the stored byte is compared as a signed char with the original unit and replaced by '?' when
//                they differ: so every unit above 127 ends up as '?'. The scan of the source continues after the output
//                is full.
//        h       the next slot is the address of a byte block and the one after it a count: each byte as two upper-case
//                hex digits, with one space after every fourth byte that is followed by another byte.
//        any other character (unreachable, the table has no entry) calls _VIAssertf after storing the file name and line 791.
//  3. The output is NUL terminated at the position reached.
// Inferred: this is a stripped-down printf whose number formatting is delegated to sprintf; the 'h' and 'S' conversions are
// the project's own. Unknown: the owner of the argument-block layout (callers build it with a compiler varargs save area) and
// the meaning of the assertion text beyond what it prints.
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

static inline const unsigned char* slot_bytes(void* args) { return static_cast<const unsigned char*>(args); }
static inline int slot_int(const void* slot) { return *static_cast<const int*>(slot); }
static inline long long slot_long(const void* slot) { return *static_cast<const long long*>(slot); }
static inline unsigned slot_word(const void* slot) { return *static_cast<const unsigned*>(slot); }

static void append_text(char* destination, int capacity, int& count, const char* text) {
    for (; *text != 0; ++text) {
        if (count + 1 < capacity) destination[count++] = *text;
    }
}


void VI_vsnprintf__H1Zc_PX01iPCX01Pv_v(char* destination, int capacity, const char* format, void* args) {
    static const char kSpecialTerminators[] = "cCdiIouxXeEfgGnpsSh%";
    static const char kHexDigits[] = "0123456789ABCDEF";
    const unsigned char* next_slot = slot_bytes(args);
    int count = 0;
    int position = 0;
    if (capacity <= 0) _VIAssert("DstCount > 0", "../core/vistring.cpp", 662);
    while (count + 1 < capacity && format[position] != 0) {
        const char current = format[position];
        if (current != '%') {
            destination[count++] = current;
            ++position;
            continue;
        }
        char specification[64];
        char terminator;
        int length = 0;
        for (;;) {
            const char ch = format[position];
            specification[length++] = ch;
            ++position;
            bool found = false;
            if (length >= 2) {
                for (const char* member = kSpecialTerminators; *member != 0; ++member) {
                    if (*member == ch) {
                        found = true;
                        break;
                    }
                }
            }
            if (found) {
                terminator = ch;
                break;
            }
            if (length == 63 || ch == 0) goto finish;
        }
        specification[length] = 0;
        {
            char text[64];
            switch (terminator) {
                case 'c':
                case 'C':
                    if (count + 1 >= capacity) goto finish;
                    destination[count++] = static_cast<char>(*next_slot);
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
                    append_text(destination, capacity, count, text);
                    break;
                }
                case 'I': {
                    const long long value = slot_long(next_slot);
                    next_slot += 8;
                    sprintf(text, "%lli", value);
                    append_text(destination, capacity, count, text);
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
                    append_text(destination, capacity, count, text);
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
                    append_text(destination, capacity, count, text);
                    break;
                }
                case 's': {
                    const char* string = static_cast<const char*>(NORRATH_WORD_TO_POINTER(slot_word(next_slot)));
                    next_slot += 8;
                    if (string == nullptr) string = "(null)";
                    append_text(destination, capacity, count, string);
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
                            char stored;
                            if (unit < 256) {
                                stored = static_cast<char>(unit);
                            } else {
                                stored = '?';
                                for (int index = 0; index < norrath_conversion_table_size; ++index) {
                                    if (unit == _8VIString$ConversionTable1252[index]) {
                                        stored = static_cast<char>(index - 128);
                                        break;
                                    }
                                }
                            }
                            destination[count] = stored;
                            if (destination[count] != static_cast<int>(*wide)) destination[count] = '?';
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
                        destination[count++] = kHexDigits[block[index] >> 4];
                        if (count + 1 == capacity) break;
                        destination[count++] = kHexDigits[block[index] & 15];
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
