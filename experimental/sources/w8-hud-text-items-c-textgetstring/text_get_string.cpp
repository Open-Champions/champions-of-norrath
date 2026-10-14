// Observed textGetString__FPc at 0x001671f8, 464 bytes.
// The key is copied into a local buffer with every '"' byte dropped, then
// hashed exactly as textAddText hashes a key (see that function) and looked up
// in the 8192-entry pointer table at 0x006d8fa0 with forward probing. A slot
// whose stored key compares equal under strcmp yields the first even address at
// or after the end of the key and its terminator, which is where the 16-bit
// string of that record starts. The first empty slot ends the probe as a miss.
// On a miss a counter byte (0x00484038) is advanced modulo 16, the matching
// 40-byte entry of a 16-entry buffer (0x006e0fa0) is cleared, and it receives
// the units 'N', 'F', ':' followed by the first 16 bytes of the stripped key
// buffer sign-extended to 16-bit units (bytes past the terminator included).
// The entry address is returned. A full table with no match would not end.
extern "C" unsigned strlen(const char* text);
extern "C" int strcmp(const char* a, const char* b);
extern "C" void* memset(void* destination, int value, unsigned count);

extern "C" char* norrath_text_hash_slots[8192];
extern "C" unsigned short norrath_text_missing_entries[16][20];
extern "C" unsigned char norrath_text_missing_cursor;

extern "C" unsigned short* textGetString__FPc(char* key) {
    char stripped[512];
    char* out = stripped;
    while (*key != 0) {
        char c = *key++;
        if (c != '"') *out++ = c;
    }
    *out = 0;
    int length = static_cast<int>(strlen(stripped));
    int hash = 0;
    for (int i = 0; i < length; i++) {
        hash += stripped[i];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    unsigned slot = static_cast<unsigned>(hash) & 0x1fff;
    for (;;) {
        char* stored = norrath_text_hash_slots[slot];
        if (stored == 0) break;
        if (strcmp(stripped, stored) == 0) {
            return reinterpret_cast<unsigned short*>(
                (reinterpret_cast<unsigned long>(stored) + length + 2) & ~1ul);
        }
        slot = (slot + 1) & 0x1fff;
    }
    norrath_text_missing_cursor = (norrath_text_missing_cursor + 1) & 15;
    unsigned short* entry = norrath_text_missing_entries[norrath_text_missing_cursor];
    memset(entry, 0, 40);
    entry[0] = 'N';
    entry[1] = 'F';
    entry[2] = ':';
    for (int i = 0; i < 16; i++) entry[3 + i] = static_cast<unsigned short>(static_cast<signed char>(stripped[i]));
    return entry;
}
