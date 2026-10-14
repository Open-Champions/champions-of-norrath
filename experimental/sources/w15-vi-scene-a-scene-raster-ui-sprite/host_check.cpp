// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "raster.cpp"
#include <cstdint>
#include <cstring>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static unsigned W(unsigned off) { return word_of(arena + off); }
static void setu(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned getu(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void seth(unsigned off, short v) { std::memcpy(arena + off, &v, 2); }
extern "C" void* norrath_host_function(unsigned);
static int calls; static unsigned char *gA0, *gA1, *gA2, *gA3; static unsigned gFlags; static int result;
static int fn(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned e) { ++calls; gA0 = a; gA1 = b; gA2 = c; gA3 = d; gFlags = e; return result; }
extern "C" void* norrath_host_function(unsigned w) { return w == 7 ? (void*)fn : nullptr; }
int main() {
    const int results[] = {-9, -1, 0, 3};
    const unsigned raster[] = {0, 1};
    const int saved[] = {0, 5, -1, 77};
    for (int index = 0; index < 3; ++index)
        for (int r : results)
            for (unsigned hasRaster : raster)
                for (int old : saved) {
                    std::memset(arena, 0, sizeof arena);
                    unsigned scene = 0x100, camera = 0x900;
                    setu(scene + 1004, W(0x1000));
                    setu(scene + 1092, static_cast<unsigned>(old));
                    setu(scene + 19356, hasRaster ? W(0x5000) : 0);
                    for (int k = 0; k < 3; ++k) {
                        setu(0x1000 + 12 * k, W(0x2000 + 0x100 * k));
                        setu(0x2000 + 0x100 * k + 60, W(0x3000 + 0x40 * k));
                        seth(0x3000 + 0x40 * k + 24, static_cast<short>(16 * k - 8));
                        setu(0x3000 + 0x40 * k + 28, 7);
                    }
                    result = r; calls = 0;
                    int got = RasterUISprite__7VISceneR8VICameraiUi(arena + scene, arena + camera, index, 0xabcd0000u + index);
                    int word = static_cast<int>(getu(scene + 1092));
                    if (!hasRaster) {
                        if (got != -1 || calls != 0 || word != -1) return 1;
                        continue;
                    }
                    if (calls != 1) return 2;
                    if (gA0 != arena + 0x2000 + 0x100 * index + 16 * index - 8 || gA1 != arena + camera || gA2 != arena + scene || gA3 != arena + 0x5000) return 3;
                    if (gFlags != 0xabcd0000u + index) return 4;
                    if (r < 0) { if (got != -1 || word != -1) return 5; }
                    else if (got != 0 || word != old) return 6;
                }
    return 0;
}
