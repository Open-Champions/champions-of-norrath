// Observed strticmp__FPCcT0 at 0x00155ab8, 120 bytes. It walks the second
// string until its terminating zero, comparing each pair of bytes after
// folding upper-case letters (bit 1 of the _ctype_ table entry, reached through
// the view indexed by character + 128) by adding 32. It returns 1 at the first
// pair that differs after folding and 0 if the second string ends first. The
// first string is read as far as the second runs, so a shorter first string
// differs at its terminator. Bytes are read as signed char.
extern "C" const unsigned char norrath_factory_ctype_minimum_view[];

namespace {
int fold(char character) {
    const int value = character;
    return (norrath_factory_ctype_minimum_view[value + 128] & 1u) ? value + 32 : value;
}
}

extern "C" int strticmp__FPCcT0(const char* left, const char* right) {
    while (*right != '\0') {
        if (fold(*left) != fold(*right)) return 1;
        left++;
        right++;
    }
    return 0;
}
