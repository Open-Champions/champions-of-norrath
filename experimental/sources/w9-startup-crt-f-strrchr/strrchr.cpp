// Observed strrchr at 0x003052d8, 80 bytes. It returns a pointer to the last
// byte whose value equals `character` (both taken as signed char), or a null
// pointer. The terminating zero is examined too, so searching for zero returns
// the terminator. Unlike strchr the comparison is signed and the low byte of
// the argument is sign-extended before comparing.
extern "C" char* strrchr(const char* text, int character) {
    const signed char wanted = static_cast<signed char>(character);
    const char* last = 0;
    while (*text != '\0') {
        if (static_cast<signed char>(*text) == wanted) last = text;
        ++text;
    }
    if (wanted == 0) last = text;
    return const_cast<char*>(last);
}
