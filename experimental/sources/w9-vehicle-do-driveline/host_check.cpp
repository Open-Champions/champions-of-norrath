// Host contract for doDriveline__7Vehicle. Not an EE plan input. The pointer
// arrays at +1052 and +2056 have a 4-byte stride and the spec pointer at +2052
// sits 4 bytes before the first q pointer, which 8-byte host pointers would
// overlap. One wheel is used (the paired run against the original covers up to
// four), and the spec and q objects are mapped at 0x300000000 (high half 3) and
// 0x400000003 (low half 3) so the high half of the first is the low half of
// the second. The soft double routines are modelled with host doubles.
#include "do_driveline.cpp"

#include <sys/mman.h>


#include <cstdio>
#include <cstring>

unsigned char slideDustParticle[16], slideDustParticleDark[16], slideDustParticleBig[16], slideDustParticleBigDark[16], lavaGoopParticle[16];

static int failures = 0;
// Newton iteration: the host symbols named acos and sqrt are the stand-ins below, so libm must not be called for them.
static double newton_sqrt(double x) { if (x <= 0) return 0; double r = x > 1 ? x : 1; for (int i = 0; i < 80; ++i) r = 0.5 * (r + x / r); return r; }
static double as_double(Double b) { double d; std::memcpy(&d, &b, 8); return d; }
static Double as_bits(double d) { Double b; std::memcpy(&b, &d, 8); return b; }
extern "C" Double fptodp(float v) { return as_bits(v); }
extern "C" int dpcmp(Double a, Double b) { double x = as_double(a), y = as_double(b); return x < y ? -1 : (x > y ? 1 : 0); }
extern "C" Double dpsub(Double a, Double b) { return as_bits(as_double(a) - as_double(b)); }
extern "C" Double dpmul(Double a, Double b) { return as_bits(as_double(a) * as_double(b)); }
extern "C" float dptofp(Double a) { return (float)as_double(a); }
// The source names these two through assembler labels, which are used literally (no leading
// underscore added) by the host linker, so the stand-ins carry the same labels.
extern "C" Double host_dp_acos(Double a) __asm__("acos");
extern "C" Double host_dp_acos(Double a) { { const double x = as_double(a); return as_bits(__builtin_atan2(newton_sqrt(1.0 - x * x), x)); } }
extern "C" Double host_dp_sqrt(Double a) __asm__("sqrt");
extern "C" Double host_dp_sqrt(Double a) { return as_bits(newton_sqrt(as_double(a))); }

static int diff_calls, accum_calls, particle_calls, rand_calls, scale_calls;
static float torque_value = 2.0f;
static Point3 accum_force; static const Point3* accum_point; static int accum_flag;
struct Particle { void* def; Point3 position, velocity; float f; };
static Particle particles[8];
static int rand_value;

extern "C" void doDifferential__7Vehicle(unsigned char*) { ++diff_calls; }
extern "C" float getEngineTorque__7Vehicle(unsigned char*) { return torque_value; }
extern "C" float scalePatchLimit__7VehicleiP6Point3(unsigned char*, int, Point3*) { ++scale_calls; return 1.0f; }
extern "C" void accumForceAndTorque__7VehicleP6Point3T1i(unsigned char*, const Point3* p, const Point3* f, int flag) { ++accum_calls; accum_force = *f; accum_point = p; accum_flag = flag; }
extern "C" int eRand__10GameObject(void*) { ++rand_calls; return rand_value; }
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, const Point3* p, const Point3* v, float f) { particles[particle_calls++] = {d, *p, *v, f}; }

static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static int& wd(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static bool near(float a, float b) { return __builtin_fabsf(a - b) <= 1e-4f * (1.0f + __builtin_fabsf(b)); }

static unsigned char v[2600];
static unsigned char* spec;
static unsigned char wheel[512];
static unsigned char* q;

static void setup(float a, int driven) {
    std::memset(v, 0, sizeof v); std::memset(spec, 0, 128); std::memset(wheel, 0, sizeof wheel); std::memset(q, 0, 128);
    diff_calls = accum_calls = particle_calls = rand_calls = scale_calls = 0; rand_value = 0; torque_value = 2.0f;
    wd(v, 1000) = 1;
    unsigned char* sp = spec; unsigned char* wp = wheel; unsigned char* qp = q;
    std::memcpy(v + 2052, &sp, sizeof sp); std::memcpy(v + 1052, &wp, sizeof wp); std::memcpy(v + 2056, &qp, sizeof qp);
    at(spec, 0) = 100; at(spec, 4) = 5; at(spec, 56) = 1;
    at(q, 4) = 2.0f;
    at(v, 676) = a; at(v, 868) = 1.0f; at(v, 948) = 10; at(v, 944) = 30; at(v, 912) = 2; at(v, 820) = 4; at(v, 908) = 8; at(v, 756) = 1; at(v, 872) = 1;
    at(v, 824) = 1.0f; at(v, 748) = 0.5f;
    at(v, 1740) = 0; at(v, 1744) = 0; at(v, 1748) = 1;                   // N = (0, 0, 1)
    at(wheel, 400) = 1; at(wheel, 404) = 0; at(wheel, 408) = 0;          // u = (1, 0, 0): N.u = 0, so the angle is 90 degrees
    at(wheel, 412) = 0; at(wheel, 416) = -1; at(wheel, 420) = 0;         // a = (0, 1, 0)
    at(v, 1836) = 3; at(v, 1840) = 0; at(v, 1844) = 5;                   // velocity
    at(v, 1440) = 10; at(v, 1444) = 20; at(v, 1448) = 30;                // H
    wd(wheel, 388) = driven; wd(wheel, 392) = 0;
    at(wheel, 360) = 2.0f; at(wheel, 364) = 0.0f; at(wheel, 380) = 0.0f; at(wheel, 376) = 1.0f;
}

int main() {
    void* a3 = mmap(reinterpret_cast<void*>(0x300000000ull), 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    void* a4 = mmap(reinterpret_cast<void*>(0x400000000ull), 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    if (a3 != reinterpret_cast<void*>(0x300000000ull) || a4 != reinterpret_cast<void*>(0x400000000ull)) { std::fprintf(stderr, "fail: cannot map the spec and q pages\n"); return 2; }
    spec = static_cast<unsigned char*>(a3);
    q = static_cast<unsigned char*>(a4) + 3;
    // No contact: E and R cleared, nothing accumulated.
    setup(0.0f, 0);
    at(v, 768) = 9; at(v, 1644) = 9; at(v, 1648) = 9; at(v, 1652) = 9;
    doDriveline__7Vehicle(v);
    expect(at(v, 768) == 0 && at(v, 1644) == 0 && at(v, 1648) == 0 && at(v, 1652) == 0 && accum_calls == 0, "A == 0: cleared, no force");
    expect(diff_calls == 1 && near(at(v, 748), 0.5f) && near(at(v, 824), 1.0f), "no driven wheels: +824 kept, +748 unchanged");

    // Contact, free wheel: angle 90 degrees -> g = 0 -> keep = 1 -> E = A * q1 * C1.
    setup(0.5f, 0);
    doDriveline__7Vehicle(v);
    const float C1 = 1.0f * 0x1.d89d88p-4f * 1.0f;
    const float E = 0.5f * 2.0f * C1;
    expect(at(v, 768) == E, "E = A * q[1] * C1 with keep = 1 (exact)");
    expect(near(at(v, 1644), 0) && near(at(v, 1648), 1) && near(at(v, 1652), 0), "R = (0, 1, 0)");
    // side velocity (3,0,0) -> F = -(3,0,0) * E, longer than E so it is cut to E and dust is emitted.
    expect(accum_calls == 1 && accum_flag == 0 && accum_point == (Point3*)(v + 1536), "force accumulated at the wheel point, flag 0");
    expect(near(accum_force.x, -E) && near(accum_force.y, 0) && near(accum_force.z, 0), "F cut to length E");
    expect(scale_calls == 1 && rand_calls == 4 && particle_calls == 1, "dust: three position draws and one choice, one particle");
    expect(particles[0].def == slideDustParticle && particles[0].f == 0.0f, "small dust definition on an even draw");
    expect(near(particles[0].position.x, 10 - 6) && near(particles[0].position.y, 20 - 6) && near(particles[0].position.z, 30 - 6), "position = H + (draw % 12 - 6)");
    expect(near(particles[0].velocity.x, 3.0f * -0.25f * -1.0f), "dust velocity = unit force direction * -0.25 (along +x here)");
    // An odd draw picks the dark one.
    setup(0.5f, 0); rand_value = 1;
    doDriveline__7Vehicle(v);
    expect(particles[0].def == slideDustParticleDark, "odd draw: dark dust");
    // |F| = 8.5 * E (side velocity 8.5): big dust; 7.5 * E: small dust.
    setup(0.5f, 0); at(v, 1836) = 8.5f;
    doDriveline__7Vehicle(v);
    expect(particles[0].def == slideDustParticleBig, "force above 8 * E: big dust");
    setup(0.5f, 0); at(v, 1836) = 7.5f;
    doDriveline__7Vehicle(v);
    expect(particles[0].def == slideDustParticle, "force below 8 * E: small dust");

    // Driven wheel, torque path.
    setup(0.5f, 1);
    doDriveline__7Vehicle(v);
    // average = w360 * (spec0 / +912) = 2 * 50 = 100, limited to spec float +4 = 5; +824 becomes 5; +748 = 0.5 + (5 - 1) clamped to 5.
    expect(near(at(v, 824), 5.0f) && near(at(v, 748) , 5.0f), "driven wheel: average limited to spec +4; +748 follows");
    expect(accum_calls == 1, "force accumulated");
    // Word +2152 negates the average first, so it clamps to 0.
    setup(0.5f, 1); wd(v, 2152) = 1;
    doDriveline__7Vehicle(v);
    expect(near(at(v, 824), 0.0f), "reverse: average negated and floored at zero");
    return failures ? 1 : 0;
}
