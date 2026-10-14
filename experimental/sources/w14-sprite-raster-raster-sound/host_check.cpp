#include <cmath>
#include <cstdio>
#include <cstring>
#include "raster.cpp"
static int failures, n, order[16], getter_calls, destroy_flag, getter_answer = 3;
static float view[16], set_in[16];
static void *copy_dst, *set_matrix, *set_raster, *ui_scene, *ui_camera, *destroyed;
static int ui_index;
static unsigned ui_flags;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" int GetSoundUISprite__7VIScene(void*) {
    order[n++] = 1; ++getter_calls; return getter_answer == -1 ? -1 : getter_answer + getter_calls;
}
extern "C" const void* GetModelView__C8VIRaster(const void*) { order[n++] = 2; return view; }
extern "C" void sceVu0CopyMatrix(void* dest, const void* source) { order[n++] = 3; copy_dst = dest; std::memcpy(dest, source, 64); }
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix) {
    order[n++] = 4; set_raster = raster; set_matrix = const_cast<void*>(matrix); std::memcpy(set_in, matrix, 64);
}
extern "C" int RasterUISprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags) {
    order[n++] = 5; ui_scene = scene; ui_camera = camera; ui_index = index; ui_flags = flags; return 9;
}
extern "C" void _$_10VIMatrix44(void* self, int flags) { order[n++] = 6; destroyed = self; destroy_flag = flags; }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main() {
    unsigned char sprite[4], camera[4], scene[4], raster[4];
    // row 0 length 10 (6,8,0), row 1 length 4 (0,0,4), row 2 length 2 (2,0,0), row 3 arbitrary
    const float rows[16] = {6, 8, 0, 11, 0, 0, 4, 12, 2, 0, 0, 13, 100, 200, 300, 14};
    std::memcpy(view, rows, sizeof rows);
    int r = Raster__13VISoundSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x66u);
    expect(r == 0, "returns 0");
    const int want[8] = {1, 2, 3, 4, 1, 5, 6};
    bool ok = n == 7;
    for (int i = 0; ok && i < 7; ++i) ok = order[i] == want[i];
    expect(ok, "order: lookup, model view, copy, set, lookup, UI draw, destroy");
    expect(near(set_in[0], 2.4f) && near(set_in[1], 3.2f) && set_in[2] == 0.0f, "row 0 brought to length 4: (2.4, 3.2, 0)");
    expect(set_in[3] == 11.0f, "w of row 0 untouched");
    expect(set_in[4] == 0.0f && set_in[5] == 0.0f && set_in[6] == 4.0f && set_in[7] == 12.0f, "length exactly 4 untouched");
    expect(set_in[8] == 2.0f && set_in[11] == 13.0f, "short row untouched");
    expect(std::memcmp(set_in + 12, rows + 12, 16) == 0, "row 3 untouched");
    expect(set_matrix == copy_dst && destroyed == copy_dst && destroy_flag == 2 && set_raster == raster, "one local, flag 2");
    expect(ui_scene == scene && ui_camera == camera && ui_flags == 0x66u && ui_index == 5, "UI draw uses the second answer");
    // zero row: infinite scale, not below 0.25
    const float rows2[16] = {0, 0, 0, 1, 0, 0, 8, 2, 0, 0, 0, 3, 7, 7, 7, 7};
    std::memcpy(view, rows2, sizeof rows2);
    n = getter_calls = 0;
    Raster__13VISoundSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(set_in[0] == 0.0f && set_in[1] == 0.0f && near(set_in[6], 4.0f) && set_in[3] == 1.0f && set_in[7] == 2.0f, "zero row left alone, row 1 halved");
    // row 2 clamped uses its own scale
    const float rows3[16] = {0, 0, 1, 0, 0, 0, 1, 0, 0, 5, 0, 0, 0, 0, 0, 0};
    std::memcpy(view, rows3, sizeof rows3);
    Raster__13VISoundSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(near(set_in[9], 4.0f) && set_in[2] == 1.0f && set_in[6] == 1.0f, "row 2 uses its own scale");
    // -1: nothing else
    n = getter_calls = 0; getter_answer = -1;
    r = Raster__13VISoundSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(r == 0 && n == 1 && order[0] == 1, "-1: lookup only");
    return failures ? 1 : 0;
}
