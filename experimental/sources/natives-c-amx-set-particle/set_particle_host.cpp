#include <cstdio>

extern "C" int amxSetParticle__FP5__amxPi(void*, int*);

static int defs[2] = {11, 22};
extern "C" void* scriptParticles[2] = {&defs[0], &defs[1]};

static void* definition;
static int values[8];
static float amount;
static float rise;
static float sink;
static int tail[3];
static void (*callback)(void*);
static int calls;

extern "C" void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
    void* def, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y,
    float z, int mode, int extra, int repeat, void (*fn)(void*)) {
    definition = def;
    values[0] = a;
    values[1] = b;
    values[2] = c;
    values[3] = d;
    values[4] = e;
    values[5] = f;
    values[6] = g;
    values[7] = h;
    amount = x;
    rise = y;
    sink = z;
    tail[0] = mode;
    tail[1] = extra;
    tail[2] = repeat;
    callback = fn;
    calls += 1;
}

int main() {
    int params[16] = {60, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    if (amxSetParticle__FP5__amxPi(nullptr, params) != 0) return 1;
    if (calls != 1 || definition != &defs[1]) return 2;
    for (int i = 0; i < 8; ++i) {
        if (values[i] != i + 2) return 3;
    }
    if (amount != 10.f || rise != 11.f || sink != 12.f) return 4;
    if (tail[0] != 13 || tail[1] != 14 || tail[2] != 15 || callback != nullptr) return 5;
    params[1] = 0;
    params[10] = -3;
    if (amxSetParticle__FP5__amxPi(nullptr, params) != 0) return 6;
    if (definition != &defs[0] || amount != -3.f) return 7;
    std::printf("set particle passed\n");
    return 0;
}
