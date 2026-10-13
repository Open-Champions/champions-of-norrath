// Host check for waterInit: clears, links, sets the sentinel word and fills the 33 blocks of each patch.
#include <cstdio>
#include <cstdlib>
#include <cstring>
int nmWaterPatches;
unsigned char* waterPatch;
unsigned char* waterList;
unsigned char* waterListBack;
extern "C" void waterInit__Fv();
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static unsigned word(int patch, int offset) {
    unsigned v; memcpy(&v, waterPatch + patch * 4784 + offset, 4); return v;
}
int main() {
    for (int count = 2; count <= 4; ++count) {
        nmWaterPatches = count;
        waterPatch = (unsigned char*)malloc(count * 4784 + 4784);
        memset(waterPatch, 0x77, count * 4784 + 4784);
        waterInit__Fv();
        CHECK(waterList == waterPatch && waterListBack == waterPatch + (count - 1) * 4784);
        CHECK(word(0, 4760) == 4784);
        for (int i = 1; i < count; ++i) CHECK(word(i, 4764) == (unsigned)((i - 1) * 4784));
        for (int i = 1; i < count - 1; ++i) CHECK(word(i, 4760) == (unsigned)((i + 1) * 4784));
        CHECK(word(0, 4764) == 0 && word(count - 1, 4760) == 0);
        for (int i = 0; i < count; ++i) {
            CHECK(word(i, 4756) == 0xfffe7960u && word(i, 0) == 0 && word(i, 4748) == 0);
            for (int j = 0; j < 33; ++j) {
                int o = 64 + j * 80;
                CHECK(word(i, o) == (j == 0 ? 0u : 0x14000038u) && word(i, o + 4) == 0 && word(i, o + 16) == 0);
                unsigned m = (j + 1) % 3 * 300;
                CHECK(word(i, o + 8) == (j == 32 ? 0u : (m | 0x04000000u)));
                CHECK(word(i, o + 12) == (j == 32 ? 0u : (m | 0x61200000u)));
            }
        }
        free(waterPatch);
    }
    if (fails) return 1;
    puts("waterInit ok");
    return 0;
}
