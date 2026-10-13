// Host contract for doMisc__7Vehicle. Not an EE plan input. The wheel pointer
// array (+1052.., 4-byte stride) would overlap with 8-byte host pointers, so
// the wheel part uses one wheel; the paired run against the original covers more.
#include "do_misc.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

Point3 nullp;
unsigned char lavaSteamParticle[80];
int frameNm;

static int failures = 0;
struct Accum { const Point3* point; Point3 force; };
static Accum accum[16]; static int accum_calls;
static int rand_script[64], rand_len, rand_calls;
static int particle_calls; static Point3 particle_pos, particle_vel; static float particle_f;
static int seat_calls;

extern "C" void accumForceAndTorque__7VehicleP6Point3T1i(unsigned char*, const Point3* p, const Point3* f, int flag) {
    if (flag != 0) ++failures;
    accum[accum_calls++] = {p, *f};
}
extern "C" void getSeatPosition__7VehicleiP6Point3(unsigned char*, int seat, Point3* out) { ++seat_calls; *out = {float(seat), 1.0f, 2.0f}; }
extern "C" void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void*, const Point3* in) { *out = {in->x * 3, in->y * 3, in->z * 3}; }
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void*, const Point3* in) { *out = {in->x * 2, in->y * 2, in->z * 2}; }
extern "C" int eRand__10GameObject(void*) { return rand_script[rand_calls++ % rand_len]; }
extern "C" void getParticleVelocity__7VehicleP6Point3(Point3* r, unsigned char*, const Point3*) { *r = {100.0f, 200.0f, 300.0f}; }
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, const Point3* p, const Point3* v, float f) {
    ++particle_calls; particle_pos = *p; particle_vel = *v; particle_f = f; if (d != lavaSteamParticle) ++failures;
}

static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static int& wd(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }

static unsigned char v[2600];
static unsigned char wheel[400];

static void setup() {
    std::memset(v, 0, sizeof v); std::memset(wheel, 0, sizeof wheel);
    accum_calls = particle_calls = seat_calls = rand_calls = 0;
    wd(v, 1044) = 1; wd(v, 2144) = 1; wd(v, 1000) = 0; wd(v, 976) = 0; wd(v, 2168) = 0; wd(v, 956) = -1; wd(v, 2148) = 0;
    at(v, 668) = 2.0f; frameNm = 1;
    rand_script[0] = 0; rand_len = 1;
}

int main() {
    // Gravity: bit 0 of +1044 clear.
    setup(); wd(v, 1044) = 0; at(v, 668) = 3.0f;
    doMisc__7Vehicle(v);
    expect(accum_calls == 1 && accum[0].point == &nullp && accum[0].force.x == 0 && accum[0].force.y == 0 && accum[0].force.z == -30.0f, "gravity force");
    // Gravity skipped when the bit is set.
    setup(); doMisc__7Vehicle(v); expect(accum_calls == 0, "no gravity with bit 0 set");
    // One wheel force, word 392 chooses +880 over +876.
    setup(); wd(v, 2144) = 0; wd(v, 1000) = 1; at(v, 876) = 10.0f; at(v, 880) = 20.0f;
    unsigned char* w = wheel; std::memcpy(v + 1052, &w, sizeof w);
    at(v, 1836) = 1; at(v, 1840) = 2; at(v, 1844) = 3;
    doMisc__7Vehicle(v);
    const float C = 0x1.a41a40p-6f;
    expect(accum_calls == 1 && accum[0].point == (Point3*)(v + 1536) && near(accum[0].force.x, 1 * (10.0f * C * 2.0f)) && near(accum[0].force.z, 3 * (10.0f * C * 2.0f)), "wheel force uses +876 when word 392 is zero");
    wd(wheel, 392) = 5; accum_calls = 0; doMisc__7Vehicle(v);
    expect(near(accum[0].force.y, 2 * (20.0f * C * 2.0f)), "wheel force uses +880 when word 392 is set");
    // Seat loads: a non-zero word applies -word * D at the un-rotated seat point and clears the word.
    setup(); wd(v, 976) = 2; wd(v, 984) = 0; wd(v, 988) = 4;
    doMisc__7Vehicle(v);
    expect(seat_calls == 1 && accum_calls == 1 && wd(v, 988) == 0, "one seat applied and cleared");
    expect(near(accum[0].force.z, -4.0f * 0x1.8dba2ep+10f) && accum[0].force.x == 0 && accum[0].force.y == 0, "seat force");
    expect(near(accum[0].point->x, 3.0f) && near(accum[0].point->y, 3.0f) && near(accum[0].point->z, 6.0f), "applied at the un-rotated seat position");
    // Exhaust: no driver and +2148 clear: nothing.
    setup(); wd(v, 2168) = 2; at(v, 740) = 1.0f;
    doMisc__7Vehicle(v); expect(particle_calls == 0 && rand_calls == 0, "no exhaust with no driver and +2148 zero");
    // x == 1.0, driver present, frameNm % 2 == 0 selects exhaust 0.
    setup(); wd(v, 2168) = 2; wd(v, 956) = 0; at(v, 740) = 1.0f; frameNm = 4;
    at(v, 2172) = 1; at(v, 2176) = 2; at(v, 2180) = 3; at(v, 2184) = 1; at(v, 2188) = 1; at(v, 2192) = 1;
    at(v, 16) = 10; at(v, 20) = 20; at(v, 24) = 30;
    int script[] = {32768, 32768, 32768, 32768};   // strength 0.5*50+10 = 35, then r = (0,0,0)
    std::memcpy(rand_script, script, sizeof script); rand_len = 4;
    doMisc__7Vehicle(v);
    expect(particle_calls == 1 && particle_f == 0.0f, "one particle");
    expect(near(particle_pos.x, 2 + 10) && near(particle_pos.y, 4 + 20) && near(particle_pos.z, 6 + 30), "position = transform + vehicle position");
    expect(near(particle_vel.x, 2 * 35.0f + 0 + 100.0f) && near(particle_vel.y, 2 * 35.0f + 200.0f), "velocity = transform * strength + random + reported");
    // frameNm % 2 == 1 selects exhaust 1, which has zeroed fields but still emits.
    setup(); wd(v, 2168) = 2; wd(v, 956) = 0; at(v, 740) = 1.0f; frameNm = 5;
    std::memcpy(rand_script, script, sizeof script); rand_len = 4;
    doMisc__7Vehicle(v);
    expect(particle_calls == 1, "frameNm % count picks one exhaust per frame");
    // x == 0.0 emits only when frameNm % 7 == 0.
    setup(); wd(v, 2168) = 1; wd(v, 956) = 0; at(v, 740) = 0.0f; frameNm = 7;
    std::memcpy(rand_script, script, sizeof script); rand_len = 4;
    doMisc__7Vehicle(v); expect(particle_calls == 1, "idle: emits on frames divisible by 7");
    setup(); wd(v, 2168) = 1; wd(v, 956) = 0; at(v, 740) = 0.0f; frameNm = 8;
    doMisc__7Vehicle(v); expect(particle_calls == 0 && rand_calls == 0, "idle: no draw otherwise");
    // Mid throttle: never.
    setup(); wd(v, 2168) = 1; wd(v, 956) = 0; at(v, 740) = 0.5f; frameNm = 7;
    doMisc__7Vehicle(v); expect(particle_calls == 0, "mid throttle: no exhaust");
    return failures ? 1 : 0;
}
