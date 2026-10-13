// Observed addpmlist__FPc at 0x002686a8, 240 bytes in SLUS_205.65. It interns a text string in
// the character pool pmlistchars (0x00435f68, 0x1000 bytes) whose fill level is the signed word
// pmlistcharsused (0x00485e20), and returns a pointer to the interned copy.
//  1. Scan pool offsets i = 0 .. pmlistcharsused - 1 (signed compare; the limit is read again at
//     every step). A running match length m counts how many leading source characters have been
//     seen: at each pool byte the source byte at m is read and m is advanced. If that source byte
//     is the terminating zero and the pool byte is also zero, the whole string was found (as a
//     whole entry or as the tail of a longer entry, since every pool byte is tried) and the
//     result is the pool address of offset i - strlen(source).
//  2. Otherwise, when the source byte differs from the pool byte, m restarts at 0 and this pool
//     byte is not tried again as a first character. (This is not a full substring search.)
//  3. Not found: strcpy the string to the pool at the current fill level, take the returned
//     address from a second read of the fill level, then add strlen(source) + 1 to a third read
//     of the fill level and store it.
// There is no capacity check: nothing stops the fill level passing 0x1000.
// Inferred: the pool holds the names used by the megatile level data (a neighbouring routine,
// addpmlistptrs, stores results into pmlistptrs). Unknown: the pool's owner and the effect of
// strlen/strcpy aliasing the pool, which this body does not guard against.
extern "C" {
extern char pmlistchars[];
extern int pmlistcharsused;
unsigned strlen(const char*);
char* strcpy(char*, const char*);

char* addpmlist__FPc(char* text) {
    int matched = 0;
    for (int i = 0; i < pmlistcharsused; i++) {
        char current = text[matched];
        matched++;
        if (current == 0 && pmlistchars[i] == 0) {
            return pmlistchars + (i - static_cast<int>(strlen(text)));
        }
        if (text[matched - 1] != pmlistchars[i]) matched = 0;
    }
    strcpy(pmlistchars + pmlistcharsused, text);
    char* result = pmlistchars + pmlistcharsused;
    unsigned length = strlen(text);
    pmlistcharsused = pmlistcharsused + length + 1;
    return result;
}
}
