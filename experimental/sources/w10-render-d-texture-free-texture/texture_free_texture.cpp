// Observed textureFreeTexture__FP8_texture at 0x001729c8, 92 bytes. The texture holds a pointer at byte
// 20 into the uncompressed-texture cache (uncompCache, 0x00486974, the base of 4096-byte blocks). A zero
// pointer ends the function. Otherwise the block index is (pointer - uncompCache) >> 12 (arithmetic shift),
// the texture's pointer is cleared, and the eight-byte records of uncompMap (0x006e5e80) are walked from
// that index: the first word of the record at the index is cleared, then the second word of each record is
// followed as a pointer to the next record, whose first word is cleared, until the pointer is zero. The
// original also tests the computed record address for zero before the stores; that address is the array
// base plus an offset and the test is kept as a pointer test here. Unknown: what the chain links are
// (the records after the first are assumed to be blocks of the same cache) and the real record type.
struct UncompRecord {
    int owner;
    UncompRecord* next;
};
extern char* uncompCache;
extern UncompRecord uncompMap[];

extern "C" void textureFreeTexture__FP8_texture(unsigned char* texture) {
    char* block = *reinterpret_cast<char**>(texture + 20);
    if (block != 0) {
        UncompRecord* record = &uncompMap[(block - uncompCache) >> 12];
        *reinterpret_cast<char**>(texture + 20) = 0;
        if (record != 0) {
            record->owner = 0;
            for (record = record->next; record != 0; record = record->next) {
                record->owner = 0;
            }
        }
    }
}
