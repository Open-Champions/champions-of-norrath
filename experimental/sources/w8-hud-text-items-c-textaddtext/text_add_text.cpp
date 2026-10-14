// Observed textAddText__FPv at 0x001670c8, 304 bytes.
// The argument is a text block: repeated records of a NUL-terminated narrow key,
// padding to the next even address, then a 16-bit string ended by a zero unit.
// A block ends at a record whose first key byte is zero. For each record the
// key is hashed and the key address is stored in the first empty slot of an
// 8192-entry pointer table (0x006d8fa0), probing forward one slot at a time and
// wrapping. The hash starts at 0 and, for each key byte read as signed char,
// adds the byte, adds h << 10, and xors h >> 6 (arithmetic shift). After the
// last byte it adds h << 3 and xors h >> 11; the result is masked with 0x1fff.
// The records are not copied and the table is never checked for being full.
extern "C" unsigned strlen(const char* text);

extern "C" char* norrath_text_hash_slots[8192];

extern "C" void textAddText__FPv(void* block) {
    char* record = static_cast<char*>(block);
    while (*record != 0) {
        int length = static_cast<int>(strlen(record));
        int hash = 0;
        for (int i = 0; i < length; i++) {
            hash += record[i];
            hash += hash << 10;
            hash ^= hash >> 6;
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        unsigned slot = static_cast<unsigned>(hash) & 0x1fff;
        while (norrath_text_hash_slots[slot] != 0) slot = (slot + 1) & 0x1fff;
        norrath_text_hash_slots[slot] = record;
        unsigned short* wide = reinterpret_cast<unsigned short*>(
            (reinterpret_cast<unsigned long>(record) + length + 2) & ~1ul);
        while (*wide != 0) wide++;
        record = reinterpret_cast<char*>(wide + 1);
    }
}
