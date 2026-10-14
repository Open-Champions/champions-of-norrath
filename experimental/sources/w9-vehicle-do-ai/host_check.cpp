// Host contract for doAI__7Vehicle. Not an EE plan input. The two pointer
// fields (+2320, +2324) are 4 bytes apart but host pointers are 8 bytes, so
// the high half of the first is the low half of the second. The two node
// objects are therefore mapped at 0x300000000 (high half 3) and 0x400000003
// (low half 3) to make both reads see a real pointer.
#include "do_ai.cpp"

#include <sys/mman.h>

#include <cmath>
#include <cstdio>
#include <cstring>

static int failures = 0;
static float dist_script[2];
static int dist_calls;
static const void* dist_a[2];
static const Point3* dist_b[2];
static const Point3* dist_c[2];
static int blocking_result;
static float* blocking_arg;
static float pow_base, pow_exp;

extern "C" float distanceToPlane__FP6Point3N20(const Point3* a, const Point3* b, const Point3* c) {
    dist_a[dist_calls] = a; dist_b[dist_calls] = b; dist_c[dist_calls] = c; return dist_script[dist_calls++];
}
extern "C" int getBlockingVehicle__7VehiclePf(unsigned char*, float* p) { blocking_arg = p; return blocking_result; }
extern "C" float powf(float b, float e) { pow_base = b; pow_exp = e; return 0.5f; }

static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static int& wd(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }

static unsigned char v[2400];
static unsigned char* node;
static unsigned char* other;

static void setup(float d0, float d1, float w852, float w832, float w848, int blocking, float size) {
    std::memset(v, 0, sizeof v); std::memset(node, 0, 64); std::memset(other, 0, 64);
    unsigned char* n = node; unsigned char* m = other;
    std::memcpy(v + 2320, &n, sizeof n); std::memcpy(v + 2324, &m, sizeof m);
    for (int i = 0; i < 3; ++i) { at(node, 12 + 4 * i) = float(i + 1); at(node, 24 + 4 * i) = float(10 * (i + 1)); }
    at(other, 56) = size;
    at(v, 852) = w852; at(v, 832) = w832; at(v, 848) = w848;
    at(v, 1128) = -3.0f; at(v, 1132) = -4.0f;
    dist_script[0] = d0; dist_script[1] = d1; dist_calls = 0; blocking_result = blocking; pow_base = -99;
    wd(v, 1020) = 7;
}

int main() {
    void* a = mmap(reinterpret_cast<void*>(0x300000000ull), 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    void* b = mmap(reinterpret_cast<void*>(0x400000000ull), 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    if (a != reinterpret_cast<void*>(0x300000000ull) || b != reinterpret_cast<void*>(0x400000000ull)) { std::fprintf(stderr, "fail: cannot map the node pages\n"); return 2; }
    node = static_cast<unsigned char*>(a);
    other = static_cast<unsigned char*>(b) + 3;
    // Far from the plane (852 < |d|), d > 0: Q2 negated, +1020 = 1.
    setup(5.0f, 0.0f, 2.0f, 0.5f, 3.0f, 0, 0.25f);
    doAI__7Vehicle(v);
    expect(near(at(v, 828), 5.0f) && wd(v, 1020) == 1, "d stored, flag set");
    expect(at(v, 596) == -10 && at(v, 600) == -20 && at(v, 604) == -30, "aim is the negated Q2 when d > 0");
    expect(dist_a[0] == node && dist_b[0] == (Point3*)(node + 24) && dist_c[0] == (Point3*)(v + 656), "first plane query");
    expect(dist_a[1] == other && dist_b[1] == (Point3*)(node + 12) && dist_c[1] == (Point3*)(v + 656), "second plane query");
    expect(blocking_arg == (float*)(v + 804) && wd(v, 2268) == 0, "blocking vehicle result stored");
    expect(near(at(v, 1980), 0.6f) && near(at(v, 1984), 0.8f) && at(v, 1988) == 0.0f, "T is the normalised (-m3,-m4,0)");
    // s = min(1, 0.5) = 0.5 ; e = 0 -> u = 0 -> k = 0.5 ; 852 = 0.5*240 ; 848 = 0.5 + 0.5*0.5 = 0.75
    expect(near(at(v, 852), 120.0f), "852 = (1 - k) * 240");
    expect(near(pow_base, 1.0f * 0.6f + 2.0f * 0.8f) && pow_exp == 3.0f, "powf(max(0, Q . T), 3)");
    expect(near(at(v, 848), 0.75f * 0.5f) || at(v, 848) == 2.0f, "848 scaled by powf, then floored at 2");
    expect(at(v, 848) == 2.0f, "floor of 2.0");
    // Near the plane, d < 0 branch of the aim and a blocking vehicle: no powf.
    setup(-1.0f, -100.0f, 2.0f, 5.0f, 7.0f, 1, 0.25f);
    doAI__7Vehicle(v);
    expect(wd(v, 1020) == 0 && at(v, 596) == 1 && at(v, 600) == 2 && at(v, 604) == 3, "aim is Q when near");
    expect(pow_base == -99 && wd(v, 2268) == 1, "no powf with a blocking vehicle");
    // s = min(1, 0.5), u = 100, k = s * (1 - 100*0.0016667); 848 = 5 + (1 - 5)*k stays above the 2.0 floor
    const float k = 0.5f * (1.0f - 100.0f * 0x1.b4e81ap-10f);
    expect(near(at(v, 848), 5.0f + (1.0f - 5.0f) * k) && near(at(v, 852), (1.0f - k) * 240.0f), "848 and 852 from k");
    // d < 0 and far: Q2 not negated.
    setup(-5.0f, 0.0f, 2.0f, 0.5f, 3.0f, 1, 0.25f);
    doAI__7Vehicle(v);
    expect(at(v, 596) == 10 && wd(v, 1020) == 1, "d < 0: Q2 kept");
    return failures ? 1 : 0;
}
