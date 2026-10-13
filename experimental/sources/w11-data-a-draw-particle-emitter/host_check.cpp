// Host contract for drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25.
// Not an EE plan input. The callees are recording stubs: matrices are not changed and
// cos and sin of every angle are 1 and 0.
#include <cstdio>
#include <cstring>

struct Point3 { float x, y, z; };
extern "C" {
int eRandState;
void drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(
    Point3 origin, void* def, void* data, int frame, short angle, bool a, bool b, bool c);
void matIdent__FP8Matrix34(void*) {}
void matRot__FP8Matrix34is(void*, int, short) {}
void matTransform__FP6Point3P8Matrix34T0(void*, void*, void*) {}
float icos__Fi(int) { return 1.0f; }
float isin__Fi(int) { return 0.0f; }
}
static int adds;
static Point3 seenPos, seenVel;
static void* seenDef;
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, void* pos, void* vel, float extra) {
    ++adds;
    seenDef = def;
    std::memcpy(&seenPos, pos, 12);
    std::memcpy(&seenVel, vel, 12);
    if (extra != 0.0f) std::fprintf(stderr, "extra\n");
}

static unsigned char data[128];
template <typename T> static void put(unsigned off, T v) { std::memcpy(data + off, &v, sizeof v); }
template <typename T> static T get(unsigned off) { T v; std::memcpy(&v, data + off, sizeof v); return v; }
static int fail(const char* what) { std::fprintf(stderr, "fail: %s\n", what); return 1; }

static void reset() {
    std::memset(data, 0, sizeof data);
    put<signed char>(17, 3);
    put<short>(18, 10); put<short>(20, 20); put<short>(22, 5);
    put<short>(24, 3); put<short>(26, 4);
    put<signed char>(60, 1);
    put<int>(56, 100);
    adds = 0;
    eRandState = 0;
}

int main() {
    static int definition;
    const Point3 origin = {100.0f, 200.0f, 300.0f};
    reset();
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 10, 0, false, false, false);
    if (adds != 3) return fail("burst count");
    if (seenDef != &definition) return fail("definition");
    if (seenPos.x != 110.0f || seenPos.y != 220.0f || seenPos.z != 305.0f) return fail("position");
    if (seenVel.x != 3.0f || seenVel.y != 4.0f || seenVel.z != 0.0f) return fail("velocity");
    if (get<int>(56) != 100) return fail("schedule untouched");

    reset();
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 10, 0, true, false, true);
    if (seenPos.x != 120.0f || seenPos.y != 190.0f) return fail("flip and swap position");
    if (seenVel.x != 4.0f || seenVel.y != -3.0f) return fail("flip and swap velocity");

    reset();
    put<unsigned char>(52, 1);
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 10, 0, true, true, true);
    if (seenVel.x != 3.0f || seenVel.y != 4.0f) return fail("turned velocity is not flipped");

    reset();
    put<signed char>(17, 0);
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 10, 0, false, false, false);
    if (adds != 0) return fail("zero burst");

    reset();
    put<signed char>(17, -4);
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 10, 0, false, false, false);
    if (adds != 0) return fail("off period");
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 12, 0, false, false, false);
    if (adds != 1) return fail("on period");

    reset();
    put<signed char>(60, 0);
    put<int>(44, 100);
    put<int>(48, 40);
    put<int>(56, 5);
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 10, 0, false, false, false);
    // first random word is 0x3c6ef35f: high half 15470, so (15470 / 65536 - 0.5) * 100 = -26.39
    if (get<int>(56) != 83 || get<signed char>(60) != 1 || adds != 3) return fail("short interval");
    put<int>(56, 5);
    eRandState = 0;
    put<short>(34, 0);
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 10, 0, false, false, false);
    // the flag is set, so the second interval d[48] = 40 is used: 50 + (-0.2639 * 40) = 39.44
    if (get<int>(56) != 39 || get<signed char>(60) != 0) return fail("long interval");

    reset();
    put<int>(44, 0);
    put<signed char>(60, 0);
    put<int>(56, 5);
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 10, 0, false, false, false);
    if (get<signed char>(60) != 1 || get<int>(56) != 5 || adds != 3) return fail("zero interval");

    reset();
    put<signed char>(60, 0);
    drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(origin, &definition, data, 10, 0, false, false, false);
    if (adds != 0) return fail("flag off");
    return 0;
}
