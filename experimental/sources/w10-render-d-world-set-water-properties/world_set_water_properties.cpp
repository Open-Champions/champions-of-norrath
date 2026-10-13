// Observed worldSetWaterProperties__FP8_texture at 0x0017daa8, 208 bytes. It stores its argument in
// waterTexture (0x004841a0) and sets the water globals: worldWaterWavyness to the float with bit pattern
// 0x3b999999 (a constant loaded from memory), worldWaterBumpAmount 41.0f, worldWaterBumpRadius 10.0f,
// worldWaterBumpTimer 4, worldWaterAlpha 102 and worldWaterColor 0x006d6b5f. Then, only when bit 8 of
// levelFlags is set, it walks the world's 60-byte records (count at world+0, table at world+36) and for
// each record whose flag word at +36 has bit 0x80 set fills 128 bytes starting 16 bytes into the block
// that record's first word points at with 0xff. The world pointer and count are re-read every
// iteration. Nothing is returned. Unknown: what the filled block is.
extern "C" {
extern void* waterTexture;
extern float worldWaterWavyness;
extern float worldWaterBumpAmount;
extern float worldWaterBumpRadius;
extern int worldWaterBumpTimer;
extern int worldWaterAlpha;
extern unsigned worldWaterColor;
extern int levelFlags;
extern unsigned char* world;
void* memset(void* destination, int value, unsigned count);
}

extern "C" void worldSetWaterProperties__FP8_texture(void* texture) {
    waterTexture = texture;
    worldWaterWavyness = __builtin_bit_cast(float, 0x3b999999u);
    worldWaterBumpAmount = 41.0f;
    worldWaterBumpRadius = 10.0f;
    worldWaterBumpTimer = 4;
    worldWaterAlpha = 102;
    worldWaterColor = 0x006d6b5fu;
    if (levelFlags & 8) {
        for (int i = 0; i < *reinterpret_cast<int*>(world); ++i) {
            unsigned char* record = *reinterpret_cast<unsigned char**>(world + 36) + i * 60;
            if (*reinterpret_cast<unsigned*>(record + 36) & 0x80) {
                memset(*reinterpret_cast<unsigned char**>(record) + 16, 255, 128);
            }
        }
    }
}
