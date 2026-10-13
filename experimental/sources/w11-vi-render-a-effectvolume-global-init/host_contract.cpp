#include "global_init.cpp"
#include <string.h>

unsigned char sFogCard[8 * 48];
unsigned char sDustCard[60 * 48];
unsigned char sFallEffect[60 * 48];
void* _14VIEffectVolume$mScene;

enum { RX = 1, RY, RZ, MUL, DTOR, SIN, COS };
static int events[4096]; static int event_n;
static float angles[4096]; static int angle_n;
static float trig_args[4096]; static int trig_n;
static int first_matrix_zero = 1; static int rotate_seen;
static int dtor_flags_ok = 1;

static void note_rotate(int kind, void* matrix, float angle) {
    const unsigned int* w = static_cast<const unsigned int*>(matrix);
    if (!rotate_seen++) for (int i = 0; i < 16; ++i) if (w[i] != 0) first_matrix_zero = 0;
    if ((reinterpret_cast<__UINTPTR_TYPE__>(matrix) & 15) != 0) first_matrix_zero = 0;
    events[event_n++] = kind; angles[angle_n++] = angle;
}
extern "C" void RotateX__10VIMatrix44f(void* m, float a) { note_rotate(RX, m, a); }
extern "C" void RotateY__10VIMatrix44f(void* m, float a) { note_rotate(RY, m, a); }
extern "C" void RotateZ__10VIMatrix44f(void* m, float a) { note_rotate(RZ, m, a); }
extern "C" void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void*, const void* in, void* out) {
    const float* a = static_cast<const float*>(in); float* o = static_cast<float*>(out);
    o[0] = a[0] + 10.0f; o[1] = a[1] * 2.0f; o[2] = a[2] - 10.0f; events[event_n++] = MUL;
}
extern "C" void _$_10VIMatrix44(void*, int flags) { if (flags != 2) dtor_flags_ok = 0; events[event_n++] = DTOR; }
extern "C" float sinf(float x) { trig_args[trig_n++] = x; events[event_n++] = SIN; return 0.5f; }
extern "C" float cosf(float x) { trig_args[trig_n++] = x; events[event_n++] = COS; return 0.25f; }

static float getf(const unsigned char* b, int o) { float v; memcpy(&v, b + o, 4); return v; }

int main() {
    memset(sFogCard, 0x55, sizeof sFogCard); memset(sDustCard, 0x55, sizeof sDustCard); memset(sFallEffect, 0x55, sizeof sFallEffect);
    int scene;
    if (GlobalInit__14VIEffectVolumeP7VIScene(&scene) != 0) return 1;
    if (_14VIEffectVolume$mScene != &scene) return 2;
    if (!first_matrix_zero) return 3;
    if (!dtor_flags_ok) return 4;
    const float pi = float_from_bits(0x40490fdau);
    // 8 + 60 records of rotate, four multiplies, destroy; then 30 passes of 56 events
    // (ry, 4 mul, sin, rx, 4 mul, cos, rz, 4 mul, sin, rx, 4 mul, sin, ry, 4 mul, dtor)
    int total = 68 * 6 + 30 * (1 + 4 + 1 + 1 + 4 + 1 + 1 + 4 + 1 + 1 + 4 + 1 + 1 + 4 + 1);
    if (event_n != total) return 5;
    // first card record: RotateZ, then four Mul, then the destructor
    static const int card[] = {RZ, MUL, MUL, MUL, MUL, DTOR};
    for (int r = 0; r < 68; ++r) for (int k = 0; k < 6; ++k) if (events[6 * r + k] != card[k]) return 6;
    for (int i = 0; i < 8; ++i) {
        float expected = ((static_cast<float>(i) * 0.125f) + (static_cast<float>(i) * 0.125f)) * pi;
        if (angles[i] != expected) return 7;
    }
    for (int i = 0; i < 60; ++i) {
        float base = static_cast<float>(-i) * float_from_bits(0x3c888888u);
        if (angles[8 + i] != (base + base) * pi) return 8;
    }
    // card vertices pass through the stub multiply (x + 10, y * 2, z - 10)
    static const float cx[4] = {-0.5f, 0.5f, 0.5f, -0.5f}, cy[4] = {0.5f, 0.5f, -0.5f, -0.5f};
    for (int i = 0; i < 8; ++i) for (int k = 0; k < 4; ++k) {
        const unsigned char* r = sFogCard + 48 * i + 12 * k;
        if (getf(r, 0) != cx[k] + 10.0f || getf(r, 4) != cy[k] * 2.0f || getf(r, 8) != 0.0f - 10.0f) return 9;
    }
    for (int i = 0; i < 60; ++i) for (int k = 0; k < 4; ++k) {
        const unsigned char* r = sDustCard + 48 * i + 12 * k;
        if (getf(r, 0) != cx[k] + 10.0f || getf(r, 4) != cy[k] * 2.0f || getf(r, 8) != 0.0f - 10.0f) return 10;
    }
    // falling-effect passes
    const int pass[] = {RY, MUL, MUL, MUL, MUL, SIN, RX, MUL, MUL, MUL, MUL, COS, RZ, MUL, MUL, MUL, MUL,
                        SIN, RX, MUL, MUL, MUL, MUL, SIN, RY, MUL, MUL, MUL, MUL, DTOR};
    int base = 68 * 6;
    for (int j = 0; j < 30; ++j) {
        for (int k = 0; k < 30; ++k) if (events[base + 30 * j + k] != pass[k]) return 11;
    }
    // angle and trig argument of pass 7
    float theta7 = ((static_cast<float>(7) * float_from_bits(0x3d088888u)) + (static_cast<float>(7) * float_from_bits(0x3d088888u))) * pi;
    int a = 68 + 5 * 7;
    if (angles[a] != theta7) return 12;
    // 5 rotations per pass: ry(theta), rx(sin*k), rz(-cos*k), rx(sin*k), ry(sin*c)
    const float k = float_from_bits(0x3ecccccc);
    if (angles[a + 1] != 0.5f * k || angles[a + 2] != -0.25f * k || angles[a + 3] != 0.5f * k) return 13;
    if (angles[a + 4] != 0.5f * float_from_bits(0x3e6b851eu)) return 14;
    if (trig_args[4 * 7] != theta7 || trig_args[4 * 7 + 1] != theta7 || trig_args[4 * 7 + 2] != theta7 || trig_args[4 * 7 + 3] != theta7) return 15;
    // first record of a pass: y is -0.5 + stub (*2) ... the stub doubles y, so (-0.5f*2) + 0.5f
    for (int j = 0; j < 30; ++j) for (int v = 0; v < 4; ++v) {
        const unsigned char* up = sFallEffect + 48 * j + 12 * v;
        const unsigned char* lo = sFallEffect + 1440 + 48 * j + 12 * v;
        // upper: three stub multiplies the second and third are in place (y*2 each), the final one doubles again
        float uy = -0.5f * 2.0f * 2.0f * 2.0f + 0.5f;
        if (getf(up, 4) != uy) return 16;
        float lyy = float_from_bits(0xbecccccc) * 2.0f * 2.0f + k;
        if (getf(lo, 4) != lyy) return 17;
    }
    // x positions: -0.1, 0.1, 0.1, -0.1 plus 10 per multiply (3 for the upper record, 2 for the lower)
    const float bx[4] = {float_from_bits(0xbdccccccu), float_from_bits(0x3dccccccu), float_from_bits(0x3dccccccu), float_from_bits(0xbdccccccu)};
    for (int v = 0; v < 4; ++v) {
        float x = bx[v] + 10.0f + 10.0f + 10.0f;
        if (getf(sFallEffect + 12 * v, 0) != x) return 18;
        if (getf(sFallEffect + 1440 + 12 * v, 0) != bx[v] + 10.0f + 10.0f) return 19;
    }
    // z: -0.1, -0.1, 0.1, 0.1 minus 10 per multiply
    const float bz[4] = {bx[0], bx[0], bx[1], bx[1]};
    for (int v = 0; v < 4; ++v) if (getf(sFallEffect + 12 * v, 8) != bz[v] - 10.0f - 10.0f - 10.0f) return 20;
    for (int v = 0; v < 4; ++v) if (getf(sFallEffect + 1440 + 12 * v, 8) != bz[v] - 10.0f - 10.0f) return 21;
    return 0;
}
