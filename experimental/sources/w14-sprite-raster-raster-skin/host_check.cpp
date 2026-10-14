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
    return 9;
}
int main() {
    unsigned char sprite[80] = {0}, camera[4], scene[4], raster[4];
    write_word(address(sprite) + 68u, 0xfffffffeu);
    write_word(address(sprite) + 72u, 5u);
    write_word(address(sprite) + 64u, 1u);
    int r = Raster__12VISkinSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 3u);
    expect(calls == 0 && r == 0, "bit 0 set: nothing drawn, 0 returned");
    write_word(address(sprite) + 64u, 0xfffffffeu);  // every other bit set
    r = Raster__12VISkinSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 3u);
    expect(calls == 1 && r == 9, "bit 0 clear: drawn, result returned");
    expect(seen_raster == raster && seen_a == -2 && seen_b == 5 && seen_flags == 3u, "arguments from +68, +72, flags");
    write_word(address(sprite) + 64u, 0u);
    r = Raster__12VISkinSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 4u);
    expect(calls == 2 && seen_flags == 4u, "zero word draws");
    return failures ? 1 : 0;
}
