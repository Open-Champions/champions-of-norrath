// Observed strticmp__FPCcT0 at 0x00155ab8, 120 bytes (the original C++ name of a function
// taking two C strings). The loop is controlled by the second string: while its current byte
// is not zero, one signed byte is taken from each string and both advance. A byte is folded to
// lower case by adding 32 when bit 0 of the entry at _ctype_ + 1 + byte (_ctype_ is at
// 0x0047e1f0) is set. The first pair that differs after folding returns 1. When the second
// string ends the result is 0, whether or not the first string has ended.
// So the result is 0 exactly when the second string is a case-insensitive prefix of the first
// one, and 1 otherwise; it never returns -1. A first string that ends early differs at that
// byte from any nonzero second byte and returns 1.
// Inferred: bit 0 of the ctype table is the upper-case class. The byte is loaded sign-extended
// before the table index is formed, so a byte above 127 indexes below the table's second entry;
// what lies there is not established here and the host contract does not exercise it.
extern "C" {
extern const unsigned char _ctype_[];

static int fold(signed char value) {
    const int widened = value;
    return (_ctype_[1 + widened] & 1) != 0 ? widened + 32 : widened;
}

int norrath_strticmp(const char* text, const char* prefix) {
    while (*prefix != 0) {
        const signed char left = static_cast<signed char>(*text++);
        const signed char right = static_cast<signed char>(*prefix++);
        if (fold(left) != fold(right)) return 1;
    }
    return 0;
}
}
