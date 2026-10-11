// Observed strrchr at 0x003052d8. The search byte is the low eight bits of the
// second argument, sign-extended. Each string byte is sign-extended before the
// compare. A zero search byte returns the terminator; no match returns null.
extern "C" char* norrath_archive_last_character(const char* text, int character) {
    const int wanted = static_cast<signed char>(static_cast<unsigned char>(character));
    const char* cursor = text;
    const char* found = nullptr;
    unsigned char raw = static_cast<unsigned char>(*cursor);
    while (raw != 0u) {
        const int value = static_cast<signed char>(raw);
        if ((value ^ wanted) == 0) found = cursor;
        cursor += 1;
        raw = static_cast<unsigned char>(*cursor);
    }
    char* terminator = const_cast<char*>(cursor);
    if ((wanted ^ 0) != 0) return const_cast<char*>(found);
    return terminator;
}
