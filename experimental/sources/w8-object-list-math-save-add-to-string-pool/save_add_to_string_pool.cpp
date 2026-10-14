// Observed objectSaveAddToStringPool__FPCc at 0x143c38, extent 212.
// The pool is a run of entries between poolStart (0x48689c) and poolEnd
// (0x4868a0). Each entry is a length byte, the characters and a NUL, so it
// occupies length + 2 bytes. Returns 0 when the pool does not exist (poolStart
// is zero) or the text is null. Otherwise it walks the entries: the first
// character (as a signed byte) is compared before strcmp is called, and an exact
// match returns the offset of its characters from poolStart. With no match the
// text is appended at poolEnd: length byte (low 8 bits of strlen), strcpy of the
// text, poolEnd advanced by strlen + 2, and the offset of the new characters is
// returned. Offset 0 can never be a real result because entries start with a
// length byte. The walk re-reads poolEnd on every step; there is no capacity
// check. Unknown: the pool's capacity and who allocates it.
extern "C" int strcmp(const char*, const char*);
extern "C" unsigned strlen(const char*);
extern "C" char* strcpy(char*, const char*);

extern "C" char* poolStart;
extern "C" char* poolEnd;

extern "C" int objectSaveAddToStringPool__FPCc(const char* text) {
    char* entry = poolStart;
    if (entry == nullptr || text == nullptr) return 0;
    while (entry != poolEnd) {
        const signed char length = static_cast<signed char>(*entry);
        ++entry;
        if (*entry == *text && strcmp(entry, text) == 0) return static_cast<int>(entry - poolStart);
        entry += length + 1;
    }
    const unsigned length = strlen(text);
    *entry = static_cast<char>(length);
    ++entry;
    strcpy(entry, text);
    poolEnd = poolEnd + length + 2;
    return static_cast<int>(entry - poolStart);
}
