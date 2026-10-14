#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include "raster.cpp"
using namespace vslots_b;
static int failures, calls, seen_a, seen_b;
static void* seen_raster;
static unsigned seen_flags;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" int DrawPrimBuffer__8VIRasteriiUi(void* raster, int a, int b, unsigned flags) {
    ++calls; seen_raster = raster; seen_a = a; seen_b = b; seen_flags = flags;
    return 0x1234;
}
int main() {
    unsigned char sprite[80] = {0}, camera[4], scene[4], raster[4];
    write_word(address(sprite) + 64u, 0xfffffff0u);
    write_word(address(sprite) + 68u, 77u);
    const int r = Raster__13VIFloraSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0xcafe0001u);
    expect(calls == 1, "one draw");
    expect(seen_raster == raster, "raster is the fourth argument");
    expect(seen_a == -16 && seen_b == 77, "words +64 and +68, signed");
    expect(seen_flags == 0xcafe0001u, "flags");
    expect(r == 0x1234, "result returned");
    return failures ? 1 : 0;
}
