// Observed textRemoveText__FPv at 0x00166f98, 300 bytes; the inverse of textAddText__FPv. The argument is
// the same text block: records of a NUL-terminated narrow key, padding to the next even address, and a
// 16-bit string ended by a zero unit, with the block ended by a record whose first key byte is zero. Each
// key is hashed exactly as the adder hashes it (signed bytes; add, add h << 10, xor h >> 6; then add
// h << 3, xor h >> 11; mask 0x1fff). The 8192-entry pointer table (0x006d8fa0) is probed forward one
// slot at a time, wrapping, until the slot holds the key's own address, and that slot is cleared. The
// probe has no bound: a key that is not in the table makes it run on. Nothing is returned.
extern "C" unsigned strlen(const char* text);

extern "C" char* norrath_text_hash_slots[8192];

extern "C" void textRemoveText__FPv(void* block) {
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
        while (norrath_text_hash_slots[slot] != record) slot = (slot + 1) & 0x1fff;
        norrath_text_hash_slots[slot] = 0;
        unsigned short* wide = reinterpret_cast<unsigned short*>(
            (reinterpret_cast<unsigned long>(record) + length + 2) & ~1ul);
        while (*wide != 0) wide++;
        record = reinterpret_cast<char*>(wide + 1);
    }
}
