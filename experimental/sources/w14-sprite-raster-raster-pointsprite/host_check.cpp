#include <cstdio>
#include <cstring>
#include "raster.cpp"
static int failures, n, order[16], getter_calls, destroy_flags;
static int getter_answer = 5;
static void *scene_seen[2], *ui_scene, *ui_camera, *clamp_self, *clamp_matrix, *set_raster, *set_matrix, *copy_dest, *destroy_self, *model_raster;
static unsigned ui_flags;
static int ui_index;
static float model_view[16], clamp_in[16], set_in[16];
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" int GetPointUISprite__7VIScene(void* scene) {
    if (getter_calls < 2) scene_seen[getter_calls] = scene;
    order[n++] = 1;
    ++getter_calls;
    return getter_answer == -1 ? -1 : getter_answer + getter_calls;
}
extern "C" const void* GetModelView__C8VIRaster(const void* raster) { order[n++] = 2; model_raster = const_cast<void*>(raster); return model_view; }
extern "C" void sceVu0CopyMatrix(void* dest, const void* source) {
    order[n++] = 3; copy_dest = dest; std::memcpy(dest, source, 64);
}
extern "C" void ClampScale__13VIPointSpriteR10VIMatrix44(void* self, void* matrix) {
    order[n++] = 4; clamp_self = self; clamp_matrix = matrix; std::memcpy(clamp_in, matrix, 64);
    static_cast<float*>(matrix)[3] = 99.0f;
}
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix) {
    order[n++] = 5; set_raster = raster; set_matrix = const_cast<void*>(matrix); std::memcpy(set_in, matrix, 64);
}
extern "C" int RasterUISprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags) {
    order[n++] = 6; ui_scene = scene; ui_camera = camera; ui_index = index; ui_flags = flags; return 123;
}
extern "C" void _$_10VIMatrix44(void* self, int flags) { order[n++] = 7; destroy_self = self; destroy_flags = flags; }
int main() {
    unsigned char sprite[8], camera[4], scene[4], raster[4];
    for (int i = 0; i < 16; ++i) model_view[i] = 1.0f + i;
    int r = Raster__13VIPointSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x55u);
    expect(r == 0, "returns 0");
    const int want[8] = {1, 2, 3, 4, 5, 1, 6, 7};
    bool ok = n == 8;
    for (int i = 0; ok && i < 8; ++i) ok = order[i] == want[i];
    expect(ok, "call order: lookup, model view, copy, clamp, set, lookup, draw, destroy");
    expect(scene_seen[0] == scene && scene_seen[1] == scene && model_raster == raster && set_raster == raster, "scene and raster");
    expect(clamp_self == sprite, "clamp gets the sprite");
    expect(copy_dest == clamp_matrix && clamp_matrix == set_matrix && set_matrix == destroy_self, "one local matrix throughout");
    expect(std::memcmp(clamp_in, model_view, 64) == 0, "clamp sees the copied model view");
    expect(set_in[3] == 99.0f && set_in[0] == 1.0f, "set sees the clamped matrix");
    expect(ui_scene == scene && ui_camera == camera && ui_flags == 0x55u, "UI draw arguments");
    expect(ui_index == 7, "UI draw uses the second lookup answer");
    expect(destroy_flags == 2, "destructor flag 2");
    expect((reinterpret_cast<unsigned long>(clamp_matrix) & 15u) == 0, "local is 16-aligned");
    n = 0; getter_calls = 0; getter_answer = -1;
    r = Raster__13VIPointSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x55u);
    expect(r == 0 && n == 1 && order[0] == 1, "-1: only the lookup, returns 0");
    return failures ? 1 : 0;
}
