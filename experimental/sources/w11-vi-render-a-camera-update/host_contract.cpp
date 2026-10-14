#include "update.cpp"
#include <string.h>

static int order[16]; static int n;
static unsigned char* g_obj;
static void* a_copy_dst; static const void* a_copy_src;
static void* mul_args[3]; static void* inv_args[2]; static void* frustum_args[3]; static void* dtor_arg; static int dtor_flags;
static void* init_arg;
static unsigned char at_copy[512], at_mul[512], at_frustum[512];
static float local_at_inv[16];
extern "C" void Init__9VIFrustum(void* f) { init_arg = f; order[n++] = 1; }
extern "C" void sceVu0CopyMatrix(void* d, const void* s) { a_copy_dst = d; a_copy_src = s; memcpy(at_copy, g_obj, 400); order[n++] = 2; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* r, const void* a, const void* b) { mul_args[0] = r; mul_args[1] = const_cast<void*>(a); mul_args[2] = const_cast<void*>(b); memcpy(at_mul, g_obj, 400); order[n++] = 3; }
extern "C" void Invert__10VIMatrix44RC10VIMatrix44(void* r, const void* s) { inv_args[0] = r; inv_args[1] = const_cast<void*>(s); memcpy(local_at_inv, r, 64); order[n++] = 4; }
extern "C" void CalcFrustum__8VICameraRC10VIMatrix44T1(void* o, const void* i, const void* c) { frustum_args[0] = o; frustum_args[1] = const_cast<void*>(i); frustum_args[2] = const_cast<void*>(c); memcpy(at_frustum, g_obj, 400); order[n++] = 5; }
extern "C" void _$_10VIMatrix44(void* m, int f) { dtor_arg = m; dtor_flags = f; order[n++] = 6; }

static float F(unsigned char* b, int o) { float v; memcpy(&v, b + o, 4); return v; }

int main() {
    static unsigned char object[400];
    g_obj = object;
    memset(object, 0x5a, sizeof object);
    float p[3] = {1.0f, 2.0f, 3.0f}, x[3] = {0.5f, 0.25f, 0.125f}, y[3] = {4.0f, 5.0f, 6.0f}, z[3] = {-1.0f, 7.0f, 8.0f};
    unsigned char m[64]; memset(m, 0x11, sizeof m);
    int r = Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(object, p, x, y, z, m);
    if (r != 0) return 1;
    int ex[] = {1, 2, 3, 4, 5, 6};
    if (n != 6) return 2;
    for (int i = 0; i < 6; ++i) if (order[i] != ex[i]) return 3;
    if (init_arg != object + 304 || a_copy_dst != object + 112 || a_copy_src != m) return 4;
    if (mul_args[0] != object + 240 || mul_args[1] != object + 48 || mul_args[2] != object + 112) return 5;
    if (inv_args[1] != object + 112 || frustum_args[0] != object || frustum_args[1] != inv_args[0] || frustum_args[2] != object + 176) return 6;
    if (dtor_arg != inv_args[0] || dtor_flags != 2) return 7;
    for (int i = 0; i < 16; ++i) if (local_at_inv[i] != 0.0f) return 8;
    // the view rows
    float view[12] = {0.5f, 4.0f, -1.0f, 0, 0.25f, 5.0f, 7.0f, 0, 0.125f, 6.0f, 8.0f, 0};
    for (int i = 0; i < 12; ++i) {
        unsigned char* b = object + 48 + 4 * i;
        int w; memcpy(&w, b, 4);
        if (i % 4 == 3) { if (w != 0) return 20 + i; } else if (F(object, 48 + 4 * i) != view[i]) return 20 + i;
    }
    float t0 = -1.0f * 0.5f + -2.0f * 0.25f + -3.0f * 0.125f;
    float t1 = -1.0f * 4.0f + -2.0f * 5.0f + -3.0f * 6.0f;
    float t2 = -1.0f * -1.0f + -2.0f * 7.0f + -3.0f * 8.0f;
    if (F(object, 96) != t0 || F(object, 100) != t1 || F(object, 104) != t2 || F(object, 108) != 1.0f) return 40;
    if (F(object, 28) != 1.0f || F(object, 32) != 2.0f || F(object, 36) != 3.0f) return 41;
    float world[16] = {0.5f, 0.25f, 0.125f, 0, 4, 5, 6, 0, -1, 7, 8, 0, 1, 2, 3, 1};
    for (int i = 0; i < 16; ++i) if (F(object, 176 + 4 * i) != world[i]) return 50 + i;
    // the calls see the stored state
    if (memcmp(at_copy + 176, object + 176, 64) != 0 || memcmp(at_copy + 48, object + 48, 64) != 0) return 70;
    // untouched bytes
    if (object[0] != 0x5a || object[27] != 0x5a || object[40] != 0x5a || object[47] != 0x5a || object[112] != 0x5a || object[175] != 0x5a || object[240] != 0x5a) return 71;
    return 0;
}
