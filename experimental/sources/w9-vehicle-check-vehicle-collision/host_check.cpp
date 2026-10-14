// Host contract for checkVehicleCollision__7Vehicle. Not an EE plan input.
#include "check_vehicle_collision.cpp"

#include <cstdio>
#include <cstring>

int vehicleCount;
unsigned char* vehicleList[32];

static int failures = 0;
static int seg_calls, dual_calls;
struct SegCall { const Point3 *a, *b, *c, *d, *e; };
static SegCall seg_log[256];
static int hit_on = -1;
struct DualCall { unsigned char *self, *b, *a; const Point3 *p, *q; };
static DualCall dual_log[16];

extern "C" int segmentLineHit__FP6Point3N50(const Point3* a, const Point3* b, const Point3* c, const Point3* d, const Point3* e, Point3*) {
    seg_log[seg_calls] = {a, b, c, d, e};
    return seg_calls++ == hit_on;
}
extern "C" void dualImpulse__7VehicleP7VehicleT1P6Point3T3(unsigned char* s, unsigned char* b, unsigned char* a, const Point3* p, const Point3* q) {
    dual_log[dual_calls++] = {s, b, a, p, q};
}

static unsigned char veh[3][2304];
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }

static void setup(int count, float x1, float reach) {
    std::memset(veh, 0, sizeof veh);
    vehicleCount = count;
    for (int i = 0; i < 3; ++i) {
        vehicleList[i] = veh[i];
        std::memcpy(veh[i] + 2156, &i, 4);
        put(veh[i], 836, reach);
    }
    put(veh[1], 16, x1);
    seg_calls = dual_calls = 0; hit_on = -1;
}

int main() {
    // Not the last registered vehicle: nothing happens.
    setup(2, 1.0f, 5.0f);
    expect(checkVehicleCollision__7Vehicle(veh[0]) == 0 && seg_calls == 0, "only the vehicle with index count-1 runs the test");
    // Last vehicle, apart: no segment tests.
    setup(2, 100.0f, 5.0f);
    expect(checkVehicleCollision__7Vehicle(veh[1]) == 0 && seg_calls == 0, "far apart: no segment tests");
    // Touching exactly (distance == reach) does not count.
    setup(2, 10.0f, 5.0f);
    checkVehicleCollision__7Vehicle(veh[1]);
    expect(seg_calls == 0, "distance equal to the reach sum is not a hit");
    // Close: 2 ordered pairs x 2 passes x 4 x 4 segment tests, no impulses.
    setup(2, 1.0f, 5.0f);
    expect(checkVehicleCollision__7Vehicle(veh[1]) == 0, "returns 0");
    expect(seg_calls == 64 && dual_calls == 0, "64 segment tests for two close vehicles");
    // First call: i = 0 (B = veh0), j = 1 (A = veh1), pass 0, s = 0, k = 0.
    expect(seg_log[0].a == (Point3*)(veh[1] + 452 + 12) && seg_log[0].b == (Point3*)(veh[1] + 452) && seg_log[0].c == (Point3*)(veh[1] + 548), "edge 0 of A, companion at +548");
    expect(seg_log[0].d == (Point3*)(veh[0] + 452) && seg_log[0].e == (Point3*)(veh[0] + 16), "B's first value and B's position");
    // Edge 3 wraps to edge 0; next value of B after four edges.
    expect(seg_log[3].a == (Point3*)(veh[1] + 452) && seg_log[3].b == (Point3*)(veh[1] + 452 + 36), "edge 3 ends at edge 0's start");
    expect(seg_log[4].d == (Point3*)(veh[0] + 452 + 12), "next value of B after four edges");
    // Second pass swaps A and B.
    expect(seg_log[16].b == (Point3*)(veh[0] + 452) && seg_log[16].d == (Point3*)(veh[1] + 452) && seg_log[16].e == (Point3*)(veh[1] + 16), "second pass has A and B exchanged");
    // A hit calls dualImpulse with the caller vehicle, B, A, B's value, companion.
    setup(2, 1.0f, 5.0f); hit_on = 5;
    checkVehicleCollision__7Vehicle(veh[1]);
    expect(dual_calls == 1 && dual_log[0].self == veh[1] && dual_log[0].b == veh[0] && dual_log[0].a == veh[1], "dualImpulse (self, B, A)");
    expect(dual_log[0].p == (Point3*)(veh[0] + 452 + 12) && dual_log[0].q == (Point3*)(veh[1] + 548 + 12), "value and companion of call 5");
    // Three vehicles all close: 6 ordered pairs.
    setup(3, 1.0f, 5.0f);
    checkVehicleCollision__7Vehicle(veh[2]);
    expect(seg_calls == 6 * 32, "six ordered pairs of three vehicles");
    return failures ? 1 : 0;
}
