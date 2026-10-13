// Host check for worldSetWaterProperties: water globals, and the 0xff fill only for flagged records when bit 8 of levelFlags is set.
#include <cstdio>
#include <cstring>
void* waterTexture;
float worldWaterWavyness, worldWaterBumpAmount, worldWaterBumpRadius;
int worldWaterBumpTimer, worldWaterAlpha, levelFlags;
unsigned worldWaterColor;
unsigned char* world;
extern "C" void worldSetWaterProperties__FP8_texture(void* texture);
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
int main() {
    static unsigned char head[64];
    static unsigned char records[3 * 60];
    static unsigned char blocks[3][160];
    world = head;
    *reinterpret_cast<int*>(head) = 3;
    *reinterpret_cast<unsigned char**>(head + 36) = records;
    for (int i = 0; i < 3; ++i) *reinterpret_cast<unsigned char**>(records + i * 60) = blocks[i];
    *reinterpret_cast<unsigned*>(records + 36) = 0x80;
    *reinterpret_cast<unsigned*>(records + 60 + 36) = 0x7f;
    *reinterpret_cast<unsigned*>(records + 120 + 36) = 0xff;
    int tex;
    levelFlags = 4;
    worldSetWaterProperties__FP8_texture(&tex);
    CHECK(waterTexture == &tex && worldWaterBumpAmount == 41.0f && worldWaterBumpRadius == 10.0f);
    CHECK(worldWaterBumpTimer == 4 && worldWaterAlpha == 102 && worldWaterColor == 0x6d6b5f);
    unsigned bits; memcpy(&bits, &worldWaterWavyness, 4);
    CHECK(bits == 0x3b999999u);
    CHECK(blocks[0][16] == 0 && blocks[2][16] == 0);  // flag bit 8 clear: no fill
    levelFlags = 8;
    worldSetWaterProperties__FP8_texture(0);
    CHECK(waterTexture == 0);
    CHECK(blocks[0][15] == 0 && blocks[0][16] == 255 && blocks[0][143] == 255 && blocks[0][144] == 0);
    CHECK(blocks[1][16] == 0 && blocks[2][16] == 255 && blocks[2][143] == 255 && blocks[2][144] == 0);
    if (fails) return 1;
    puts("worldSetWaterProperties ok");
    return 0;
}
