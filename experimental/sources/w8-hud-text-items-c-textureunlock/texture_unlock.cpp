// Observed textureUnlock__FP8_texture at 0x00172620, 44 bytes.
// The texture's signed halfword at offset 26 selects a 12-byte entry of the
// texture cache table at 0x006e5d80. The unsigned halfword at entry offset 8
// is decremented by one without a floor check.
struct NorrathCacheEntry {
    unsigned char before[8];
    unsigned short lockCount;
    unsigned char after[2];
};

struct NorrathTextureLockView {
    unsigned char before[26];
    short cacheIndex;
};

extern "C" NorrathCacheEntry norrath_texture_cache_slots[];

extern "C" void textureUnlock__FP8_texture(NorrathTextureLockView* texture) {
    norrath_texture_cache_slots[texture->cacheIndex].lockCount--;
}
