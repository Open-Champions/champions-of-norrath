#include "global_init.cpp"
static int calls;
static void* self;
static int ints[8], tail[3];
static float floats[3];
static void (*cb)(void*) = reinterpret_cast<void (*)(void*)>(1);
extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* s, int a, int b, int c, int d, int e, int f, int g, int h,
                                                    float x, float y, float z, int i, int j, int k,
                                                    void (*callback)(void*)) {
    ++calls; self = s;
    int v[8] = {a, b, c, d, e, f, g, h};
    for (int n = 0; n < 8; ++n) ints[n] = v[n];
    floats[0] = x; floats[1] = y; floats[2] = z;
    tail[0] = i; tail[1] = j; tail[2] = k;
    cb = callback;
}
int main() {
    _GLOBAL_$I$playerStartPos();
    if (calls != 1 || self != trailParticleSpark || sizeof(trailParticleSpark) != 80) return 1;
    const int want[8] = {200, 200, 150, 300, 200, 175, 100, 0};
    for (int n = 0; n < 8; ++n) if (ints[n] != want[n]) return 2;
    if (floats[0] != 2.0f || floats[1] != 5.0f || floats[2] != -20.0f) return 3;
    if (tail[0] != 1 || tail[1] != 2 || tail[2] != 2 || cb != 0) return 4;
    return 0;
}
