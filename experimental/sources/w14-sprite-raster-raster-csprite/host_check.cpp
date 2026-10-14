#include <cstdio>
#include <cstring>
#include "raster.cpp"
static int failures, n, order[16], destroy_flag;
static float view[16];
static void *h_self, *h_cam, *h_scene, *h_raster, *t_self, *t_cam, *t_scene, *t_matrix, *u_self, *u_cam, *u_scene, *u_raster, *u_matrix, *copy_dst, *destroyed;
static unsigned h_flags;
static float matrix_at_trails[16];
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" const void* GetModelView__C8VIRaster(const void*) { order[n++] = 1; return view; }
extern "C" void sceVu0CopyMatrix(void* dest, const void* source) { order[n++] = 2; copy_dst = dest; std::memcpy(dest, source, 64); }
extern "C" int Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(void* self, void* camera, void* scene, void* raster, unsigned flags) {
    order[n++] = 3; h_self = self; h_cam = camera; h_scene = scene; h_raster = raster; h_flags = flags; return 41;
}
extern "C" int ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(void* self, void* camera, void* scene, void* matrix) {
    order[n++] = 4; t_self = self; t_cam = camera; t_scene = scene; t_matrix = matrix; std::memcpy(matrix_at_trails, matrix, 64); return 99;
}
extern "C" void UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44(void* self, void* camera, void* scene, void* raster, void* matrix) {
    order[n++] = 5; u_self = self; u_cam = camera; u_scene = scene; u_raster = raster; u_matrix = matrix;
}
extern "C" void _$_10VIMatrix44(void* self, int flags) { order[n++] = 6; destroyed = self; destroy_flag = flags; }
int main() {
    unsigned char sprite[4], camera[4], scene[4], raster[4];
    for (int i = 0; i < 16; ++i) view[i] = 1.0f + i;
    const int r = Raster__9VICSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x1234u);
    expect(r == 41, "result of the H sprite call");
    const int want[6] = {1, 2, 3, 4, 5, 6};
    bool ok = n == 6;
    for (int i = 0; ok && i < 6; ++i) ok = order[i] == want[i];
    expect(ok, "order: model view, copy, H sprite, trails, emitters, destroy");
    expect(h_self == sprite && h_cam == camera && h_scene == scene && h_raster == raster && h_flags == 0x1234u, "H sprite arguments");
    expect(t_self == sprite && t_cam == camera && t_scene == scene, "trails arguments");
    expect(u_self == sprite && u_cam == camera && u_scene == scene && u_raster == raster, "emitter arguments");
    expect(t_matrix == copy_dst && u_matrix == copy_dst && destroyed == copy_dst && destroy_flag == 2, "one local, flag 2");
    expect(std::memcmp(matrix_at_trails, view, 64) == 0, "local holds the model view");
    expect((reinterpret_cast<unsigned long>(copy_dst) & 15u) == 0, "aligned");
    return failures ? 1 : 0;
}
