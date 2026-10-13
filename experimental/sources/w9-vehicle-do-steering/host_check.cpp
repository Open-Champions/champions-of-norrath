// Host contract for doSteering__7Vehicle. Not an EE plan input.
#include "do_steering.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

unsigned char pad[6 * 88];
unsigned char player[5 * 23984];
int gameControlPaused;
signed char cutMode;

static int failures = 0;
static int event_calls; static unsigned char* event_arg;
static float plane_result; static int plane_calls;
static void steer_event(unsigned char* s) { ++event_calls; event_arg = s; }

extern "C" float distanceToPlane__FP6Point3N20(const Point3*, const Point3*, const Point3*) { ++plane_calls; return plane_result; }
extern "C" float atan2f(float y, float x) { return (float)std::atan2((double)y, (double)x); }
extern "C" float acosf(float x) { return (float)std::acos((double)x); }

static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static int& wd(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-3f * (1.0f + std::fabs(b)); }

static unsigned char v[2500];
static unsigned char vtable[256];
static unsigned char blocker[2400], node[64];
static unsigned char record[64];

static void setup(int driver) {
    std::memset(v, 0, sizeof v); std::memset(vtable, 0, sizeof vtable); std::memset(blocker, 0, sizeof blocker);
    std::memset(pad, 0, sizeof pad); std::memset(player, 0, sizeof player); std::memset(record, 0, sizeof record);
    short delta = 0x30; void (*fn)(unsigned char*) = steer_event;
    std::memcpy(vtable + 220, &fn, sizeof fn); std::memcpy(vtable + 216, &delta, 2);
    unsigned char* vt = vtable; std::memcpy(v + 72, &vt, sizeof vt);
    wd(v, 956) = driver; at(v, 860) = 20; at(v, 864) = 40; at(v, 840) = 15;   // L = 20 + 20 * min(1, 15/15) = 40 (K1 = 1/15)
    at(v, 856) = 120; at(v, 1116) = 1; at(v, 1120) = 0;
    at(v, 1188) = 0; at(v, 1192) = 0; at(v, 1196) = 0;
    gameControlPaused = 0; cutMode = 0; event_calls = 0; plane_calls = 0;
}
static int delta_of_event() { short d; std::memcpy(&d, vtable + 216, 2); return d; }

int main() {
    // Early out.
    setup(0); wd(v, 1024) = 1; at(v, 800) = 3.0f; doSteering__7Vehicle(v);
    expect(at(v, 800) == 3.0f, "word +1024 set: nothing changes");
    // Auto mode with a blocker: target is +-L by the plane side.
    setup(0); wd(v, 2148) = 1; at(v, 800) = 0;
    unsigned char* n = node; std::memcpy(blocker + 2320, &n, sizeof n);
    unsigned char* bl = blocker; std::memcpy(v + 2268, &bl, sizeof bl);
    plane_result = 1.0f; doSteering__7Vehicle(v);
    expect(plane_calls == 1 && near(at(v, 800), 40.0f * 0.25f * 0.99999994f), "blocker, plane > 0: target L");
    at(v, 800) = 0; plane_result = -1.0f; doSteering__7Vehicle(v);
    expect(near(at(v, 800), -40.0f * 0.25f * 0.99999994f), "blocker, plane <= 0: target -L");
    // Auto mode, no blocker: target = atan2(aim.x, -aim.y) - atan2(T.x, -T.y).
    setup(0); wd(v, 2148) = 1; at(v, 800) = 0; at(v, 852) = 0; wd(v, 1020) = 1;
    at(v, 1980) = 0; at(v, 1984) = -1;      // atan2(0, 1) = 0
    at(v, 596) = 0.5f; at(v, 600) = -0.8660254f;   // atan2(0.5, 0.866) = 30 degrees
    doSteering__7Vehicle(v);
    expect(near(at(v, 800), 30.0f * 0.25f * 0.99999994f), "auto aim 30 degrees from heading");
    // Wrap past 180 degrees and the clamp.
    at(v, 800) = 0; at(v, 596) = 0; at(v, 600) = 1;    // atan2(0, -1) = 180 -> target 180 (not > 180) -> clamped to L = 40
    doSteering__7Vehicle(v);
    expect(near(at(v, 800), 40.0f * 0.25f * 0.99999994f), "180 degrees is clamped to L");
    // Manual mode: stick -> event on a sign change, stick stored unclamped.
    setup(1); *reinterpret_cast<float*>(pad + 20 + 88) = 0.5f; at(v, 952) = -0.5f; at(v, 800) = 0;
    doSteering__7Vehicle(v);
    expect(event_calls == 1 && event_arg == v + delta_of_event() && v[108] == 10, "sign change calls the steering event and sets the counter");
    expect(at(v, 952) == 0.5f, "stored stick");
    // Stick beyond 1.0: stored raw, used clamped. Heading speed 0 -> y = 0; target = 40 * -1.0 = -40 (|stick| > 0.125); step R = 120/60 = 2.
    setup(1); *reinterpret_cast<float*>(pad + 20 + 88) = 3.0f; at(v, 952) = 3.0f; at(v, 800) = 0;
    doSteering__7Vehicle(v);
    expect(at(v, 952) == 3.0f && event_calls == 0 && near(at(v, 800), -2.0f), "moves toward -L by one step; raw stick kept");
    // Small stick: no steering input, target stays at the speed-derived angle (0).
    setup(1); *reinterpret_cast<float*>(pad + 20 + 88) = 0.1f; at(v, 952) = 0.1f; at(v, 800) = 1.0f;
    doSteering__7Vehicle(v);
    expect(near(at(v, 800), 0.0f), "dead zone: moves toward 0 and does not overshoot");
    // Paused zeroes the stick.
    setup(1); *reinterpret_cast<float*>(pad + 20 + 88) = 1.0f; at(v, 952) = 1.0f; at(v, 800) = 0; gameControlPaused = 1;
    doSteering__7Vehicle(v);
    expect(near(at(v, 800), 0.0f), "paused: stick ignored");
    // cutMode takes +752.
    setup(1); *reinterpret_cast<float*>(pad + 20 + 88) = 0.0f; at(v, 752) = 1.0f; cutMode = 1; at(v, 800) = 0;
    doSteering__7Vehicle(v);
    expect(near(at(v, 800), -2.0f), "cutMode: +752 replaces the stick");
    // Clamp to L.
    setup(1); *reinterpret_cast<float*>(pad + 20 + 88) = 1.0f; at(v, 952) = 1.0f; at(v, 800) = -41.0f;
    doSteering__7Vehicle(v);
    expect(at(v, 800) >= -40.0f, "cur is clamped to [-L, L]");
    return failures ? 1 : 0;
}
