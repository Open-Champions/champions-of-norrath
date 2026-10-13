#include "render_weather.cpp"
#include <string.h>

static int order[256]; static int n;
static int materials[8]; static int material_n;
static int begin_remaining[16]; static int begin_n; static int batch_capacity = 3;
static unsigned char* batch_buffer;
static unsigned char batches[16][3 * 48]; static int batch_entries_used[16]; static int end_n;
static unsigned char** begin_cursor_arg;
static float plane_storage[4];
static void* mv_matrix; static void* splash_args[3]; static int splash_n; static int mv_n;
static float normalized_in[3];

extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void*, const void* m) { mv_matrix = const_cast<void*>(m); ++mv_n; order[n++] = 1; }
extern "C" void* Plane__C9VIFrustumi(const void* f, int i) { (void)f; if (i != 0) order[n++] = 99; order[n++] = 2; return plane_storage; }
extern "C" int Normalize__7VIVect3(void* v) { memcpy(normalized_in, v, 12); float* f = static_cast<float*>(v); f[0] *= 0.5f; f[1] *= 0.5f; f[2] *= 0.5f; order[n++] = 3; return 1; }
extern "C" void SetMaterial__8VIRasteri(void*, int m) { materials[material_n++] = m; order[n++] = 4; }
extern "C" int BeginBillboards__8VIRasterPP11VIBillboardi(void*, unsigned char** cursor, int count) {
    begin_remaining[begin_n] = count;
    *cursor = batches[begin_n]; begin_cursor_arg = cursor;
    ++begin_n; order[n++] = 5;
    return batch_capacity < count ? batch_capacity : count;
}
extern "C" void EndBillboards__8VIRaster(void*) { ++end_n; order[n++] = 6; }
extern "C" void RenderSplashes__12VIAtmosphereR7VISceneR8VICamera(void* o, void* s, void* c) { splash_args[0] = o; splash_args[1] = s; splash_args[2] = c; ++splash_n; order[n++] = 7; }

static void putf(unsigned char* b, int o, float v) { memcpy(b + o, &v, 4); }
static void putw(unsigned char* b, int o, unsigned int v) { memcpy(b + o, &v, 4); }
static float getf(const unsigned char* b, int o) { float v; memcpy(&v, b + o, 4); return v; }
static unsigned int getw(const unsigned char* b, int o) { unsigned int v; memcpy(&v, b + o, 4); return v; }
static void reset() { n = material_n = begin_n = end_n = splash_n = mv_n = 0; memset(batches, 0, sizeof batches); }

int main() {
    static unsigned char object[1700], scene[19400], camera[400], particles[10 * 36];
    static int raster;
    putw(scene, 19356, static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(&raster)));
    plane_storage[0] = 0.0f; plane_storage[1] = 1.0f; plane_storage[2] = 0.0f; plane_storage[3] = 0.0f;
    for (int i = 0; i < 10; ++i) {
        putf(particles, 36 * i + 0, static_cast<float>(i));
        putf(particles, 36 * i + 4, i < 5 ? -10.0f : 10.0f);   // y: far below the plane for the first five
        putf(particles, 36 * i + 8, 1.0f);
        for (int b = 12; b < 28; ++b) particles[36 * i + b] = static_cast<unsigned char>(i * 7 + b);
    }
    putw(object, 12, static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(particles)));
    putw(object, 1228, 1); putw(object, 8, 10); putf(object, 72, 1.0f); putf(object, 128, 0.5f); putf(object, 16, 2.0f);
    putf(object, 60, 100.0f); putf(object, 64, 0.0f); putf(object, 68, 0.0f);
    putw(object, 160, 7); putw(object, 164, 8);
    putf(object, 136, 1.0f); putf(object, 140, 3.0f);
    putf(object, 144, 0.5f); putf(object, 148, 0.25f); putf(object, 152, 0.125f); putf(object, 156, 0.0625f);

    // inactive and zero-density cases touch nothing
    reset(); putw(object, 1228, 0);
    RenderWeather__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    if (n != 0) return 1;
    putw(object, 1228, 1); putf(object, 72, 0.0f);
    RenderWeather__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    if (n != 0) return 2;

    // two groups: n = 10, split = 5
    putf(object, 72, 1.0f);
    reset();
    RenderWeather__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    if (mv_matrix != camera + 48) return 3;
    if (material_n != 2 || materials[0] != 8 || materials[1] != 7) return 4;
    if (splash_n != 1 || splash_args[0] != object || splash_args[1] != scene || splash_args[2] != camera) return 5;
    if (normalized_in[0] != 0.0f || normalized_in[1] != 0.0f || normalized_in[2] != 0.0f) return 6;
    // plane (0,1,0,0): u = (0,0,0) so dot = 0.5*0... = 0 and k = 1
    // first group: base 3*2 = 6, width 6-6*0.5 = 3, height 6-6*0.125 = 5.25; capacity 3 -> batches of 3 and 2
    if (begin_n != 4 || begin_remaining[0] != 5 || begin_remaining[1] != 2 || begin_remaining[2] != 5 || begin_remaining[3] != 2) return 7;
    if (end_n != 4) return 8;
    unsigned char* e0 = batches[0];
    if (getf(e0, 0) != 100.0f || getf(e0, 4) != -10.0f || getf(e0, 8) != 1.0f) return 9;
    // y = -10 gives distance -10 which is below -1.5, so the sizes stay
    if (getf(e0, 16) != 3.0f || getf(e0, 20) != 5.25f || getw(e0, 12) != 0) return 10;
    if (memcmp(e0 + 32, particles + 12, 16) != 0) return 11;
    // second group: base 1*2 = 2, width 2-2*0.25 = 1.5, height 2-2*0.0625 = 1.875; y = 10 gives distance 10 > -1.5, sizes zero
    unsigned char* g1 = batches[2];
    if (getf(g1, 0) != 105.0f || getw(g1, 16) != 0 || getw(g1, 20) != 0) return 12;
    // the order: material 164 group first, then 160
    int expect_head[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) if (order[i] != expect_head[i]) return 13;
    if (order[n - 1] != 7) return 14;

    // single group, split == 0: material 160 with the 136/152/144 group
    putf(object, 128, 0.0f);
    reset();
    RenderWeather__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    if (material_n != 1 || materials[0] != 7 || begin_n != 4 || begin_remaining[0] != 10) return 20;
    if (getf(batches[0], 16) != 2.0f - 2.0f * 0.5f || getf(batches[0], 20) != 2.0f - 2.0f * 0.125f) return 21;
    // single group, split >= n: material 164 with the 140/156/148 group
    putf(object, 128, 1.0f);
    reset();
    RenderWeather__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    if (material_n != 1 || materials[0] != 8) return 22;
    if (getf(batches[0], 16) != 6.0f - 6.0f * 0.25f || getf(batches[0], 20) != 6.0f - 6.0f * 0.0625f) return 23;
    // n clamps to the capacity and to zero; zero particles still set the material
    putf(object, 72, 5.0f); putf(object, 128, 0.0f);
    reset();
    RenderWeather__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    if (begin_remaining[0] != 10) return 24;
    putf(object, 72, -1.0f);
    reset();
    RenderWeather__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    if (material_n != 1 || begin_n != 0 || end_n != 0 || splash_n != 1) return 25;
    return 0;
}
