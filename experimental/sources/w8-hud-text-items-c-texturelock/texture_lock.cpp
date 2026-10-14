// Observed textureLock__FP8_texture at 0x00172650, 60 bytes.
// When the texture's signed halfword at offset 24 is nonzero, the signed
// halfword at offset 26 selects a 12-byte entry of the texture cache table at
// 0x006e5d80 and the unsigned halfword at entry offset 8 is incremented.
// A zero halfword at offset 24 leaves the table untouched.
struct NorrathCacheEntry {
    unsigned char before[8];
    unsigned short lockCount;
    unsigned char after[2];
};

struct NorrathTextureLockView {
    unsigned char before[24];
    short cached;
    short cacheIndex;
};

extern "C" NorrathCacheEntry cache[];

extern "C" void textureLock__FP8_texture(NorrathTextureLockView* texture) {
    if (texture->cached != 0) cache[texture->cacheIndex].lockCount++;
}
