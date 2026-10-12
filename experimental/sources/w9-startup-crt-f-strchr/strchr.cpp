// Observed strchr at 0x0030436c, 400 bytes. It returns a pointer to the first
// byte equal to the low eight bits of `character`, comparing bytes as
// unsigned, or a null pointer. The terminating zero counts as part of the
// string, so searching for zero returns the terminator. Aligned inputs are
// scanned in 8- or 16-byte blocks that look for either the target or a zero;
// the returned pointer is the same as this byte loop.
extern "C" char* strchr(const char* text, int character) {
    const unsigned char wanted = static_cast<unsigned char>(character);
    for (;; ++text) {
        const unsigned char byte = static_cast<unsigned char>(*text);
        if (byte == wanted) return const_cast<char*>(text);
        if (byte == 0) return 0;
    }
}
