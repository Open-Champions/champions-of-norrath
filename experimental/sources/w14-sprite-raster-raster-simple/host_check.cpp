#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include "raster.cpp"
using namespace vslots_b;
static int failures, color_calls, four_calls, five_calls, color_result;
static int a4, b4, a5, b5, c5;
static void *scene_seen, *raster4, *raster5;
static unsigned flags4, flags5;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" int GetNextColorBuffer__7VIScene(void* scene) { ++color_calls; scene_seen = scene; return color_result; }
extern "C" int DrawPrimBuffer__8VIRasteriiUi(void* raster, int a, int b, unsigned flags) {
    ++four_calls; raster4 = raster; a4 = a; b4 = b; flags4 = flags; return 4;
}
extern "C" int DrawPrimBuffer__8VIRasteriiiUi(void* raster, int a, int b, int c, unsigned flags) {
    ++five_calls; raster5 = raster; a5 = a; b5 = b; c5 = c; flags5 = flags; return 5;
}
int main() {
    unsigned char sprite[80] = {0}, camera[4], scene[4], raster[4];
    write_word(address(sprite) + 64u, 0xfffffff0u);
    write_word(address(sprite) + 72u, 21u);
    color_result = 6;
    int r = Raster__14VISimpleSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x77u);
    expect(color_calls == 1 && scene_seen == scene, "color buffer asked of the scene");
    expect(five_calls == 1 && four_calls == 0 && r == 5, "non-negative color: five-argument draw");
    expect(raster5 == raster && a5 == -16 && b5 == 6 && c5 == 21 && flags5 == 0x77u, "five-argument values");
    color_result = 0;
    r = Raster__14VISimpleSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x78u);
    expect(five_calls == 2 && c5 == 21 && b5 == 0, "zero counts as non-negative");
    color_result = -1;
    r = Raster__14VISimpleSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x79u);
    expect(four_calls == 1 && five_calls == 2 && r == 4, "negative color: four-argument draw");
    expect(raster4 == raster && a4 == -16 && b4 == 21 && flags4 == 0x79u, "four-argument values");
    return failures ? 1 : 0;
}
