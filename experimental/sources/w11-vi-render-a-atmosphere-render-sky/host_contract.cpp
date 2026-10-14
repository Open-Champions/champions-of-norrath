#include "render_sky.cpp"
#include <string.h>

static int order[64]; static int n;
static float view_at_set[8][16]; static void* set_raster[8]; static int set_n;
static void* copy_src; static void* copy_dst;
static int sprite_idx[8]; static int sprite_n;
static void* mul_left[4]; static int mul_n;
static void* stars_args[3]; static int stars_n;
static void* dtor_arg[4]; static int dtor_flags[4]; static int dtor_n;
struct Call { void* self; void* camera; void* scene; void* raster; int mode; };
static Call calls[8]; static int call_n;
static unsigned char sprites[8][128];
static unsigned char vtable[64];

static void draw(void* self, void* camera, void* scene, void* raster, int mode) {
    calls[call_n++] = Call{self, camera, scene, raster, mode}; order[n++] = 6;
}
extern "C" void sceVu0CopyMatrix(void* d, const void* s) { copy_dst = d; copy_src = const_cast<void*>(s); memcpy(d, s, 64); order[n++] = 1; }
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* r, const void* m) { set_raster[set_n] = r; memcpy(view_at_set[set_n++], m, 64); order[n++] = 2; }
extern "C" void* Sprite__7VIScenei(void*, int idx) { sprite_idx[sprite_n] = idx; order[n++] = 3; return sprites[sprite_n++ % 8]; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* r, const void* a, const void* b) {
    mul_left[mul_n++] = const_cast<void*>(a);
    const float* x = static_cast<const float*>(a); const float* y = static_cast<const float*>(b); float* o = static_cast<float*>(r);
    for (int i = 0; i < 16; ++i) o[i] = x[i] + 100.0f * y[i];
    order[n++] = 4;
}
extern "C" void RenderStars__12VIAtmosphereR7VISceneR8VICamera(void* o, void* s, void* c) { stars_args[0] = o; stars_args[1] = s; stars_args[2] = c; ++stars_n; order[n++] = 5; }
extern "C" void _$_10VIMatrix44(void* m, int f) { dtor_arg[dtor_n] = m; dtor_flags[dtor_n++] = f; order[n++] = 7; }

static void put(unsigned char* b, int o, unsigned int v) { memcpy(b + o, &v, 4); }
static unsigned int addr(const void* p) { return static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static void reset() { n = set_n = sprite_n = mul_n = stars_n = dtor_n = call_n = 0; }

int main() {
    static unsigned char object[1700], scene[19400];
    static unsigned char camera[128];
    static int raster;
    for (int i = 0; i < 16; ++i) { float f = static_cast<float>(i + 1); memcpy(camera + 48 + 4 * i, &f, 4); }
    put(scene, 19356, addr(&raster));
    put(vtable, 24, 0);
    unsigned short d = 12; memcpy(vtable + 24, &d, 2);
    put(vtable, 28, addr(reinterpret_cast<void*>(&draw)));
    for (int i = 0; i < 8; ++i) put(sprites[i], 60, addr(vtable));
    float ident[16]; for (int i = 0; i < 16; ++i) ident[i] = static_cast<float>(i) * 0.5f;
    memcpy(object + 1280, ident, 64); memcpy(object + 1344, ident, 64); memcpy(object + 1408, ident, 64);

    // inactive: nothing happens
    reset(); memset(object + 1228, 0, 4);
    RenderSky__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    if (n != 0) return 1;

    // active, no extra bodies (word 1476 zero): sky sprite only, then stars
    put(object, 1228, 1); put(object, 1248, 7); put(object, 1260, 8); put(object, 1264, 9); put(object, 1268, 10);
    put(object, 1560, 0); put(object, 1476, 0);
    reset();
    RenderSky__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    int ex1[] = {1, 2, 3, 6, 2, 5, 7};
    if (n != 7) return 2;
    for (int i = 0; i < 7; ++i) if (order[i] != ex1[i]) return 3;
    if (copy_src != camera + 48) return 4;
    // the first model view is the camera matrix with the translation row cleared
    for (int i = 0; i < 16; ++i) {
        float want = (i >= 12 && i < 15) ? 0.0f : static_cast<float>(i + 1);
        if (view_at_set[0][i] != want || view_at_set[1][i] != want) return 5;
    }
    if (set_raster[0] != &raster || sprite_idx[0] != 7) return 6;
    if (call_n != 1 || calls[0].self != sprites[0] + 12 || calls[0].camera != camera || calls[0].scene != scene || calls[0].raster != &raster || calls[0].mode != 3) return 7;
    if (stars_n != 1 || stars_args[0] != object || stars_args[1] != scene || stars_args[2] != camera) return 8;
    if (dtor_n != 1 || dtor_flags[0] != 2) return 9;

    // with the extra bodies
    put(object, 1476, 1);
    reset();
    RenderSky__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    int ex2[] = {1, 2, 3, 6, 4, 2, 3, 6, 4, 2, 3, 6, 4, 2, 3, 6, 7, 2, 5, 7};
    if (n != 20) return 10;
    for (int i = 0; i < 20; ++i) if (order[i] != ex2[i]) return 11;
    if (sprite_idx[1] != 8 || sprite_idx[2] != 9 || sprite_idx[3] != 10) return 12;
    if (mul_left[0] != object + 1280 || mul_left[1] != object + 1344 || mul_left[2] != object + 1408) return 13;
    for (int i = 0; i < 16; ++i) {
        float m = static_cast<float>(i) * 0.5f;
        float v = view_at_set[0][i];
        if (view_at_set[1][i] != m + 100.0f * v) return 14;
    }

    // stars suppressed when the word at 1560 is set; the extra bodies are skipped as well
    put(object, 1560, 1);
    reset();
    RenderSky__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    int ex3[] = {1, 2, 3, 6, 2, 7};
    if (n != 6 || stars_n != 0) return 20;
    for (int i = 0; i < 6; ++i) if (order[i] != ex3[i]) return 21;
    return 0;
}
