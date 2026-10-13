// Host contract for getBlockingVehicle__7VehiclePf. Not an EE plan input.
#include "get_blocking_vehicle.cpp"

#include <cmath>
#include <cstdio>
#include <initializer_list>
#include <cstring>

int vehicleCount;
unsigned char* vehicleList[32];

static float script[8];
static int calls;
static struct { Point3 a, b, c; } seen[8];
extern "C" float distanceToPlane__FP6Point3N20(const Point3* a, const Point3* b, const Point3* c) {
    seen[calls] = {*a, *b, *c};
    return script[calls++];
}

static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static void putw(unsigned char* o, int off, int v) { std::memcpy(o + off, &v, 4); }

static unsigned char self[2304], v1[2304], v2[2304];

static void reset() {
    for (unsigned char* o : {self, v1, v2}) std::memset(o, 0, 2304);
    // matrix rows: side = (1,0,0), back = -(0,1,0) = (0,-1,0)
    put(self, 1116, 1); put(self, 1128 + 4, 1);
    put(self, 16, 5); put(self, 20, 6); put(self, 24, 7);
    put(self, 836, 2.0f); put(self, 840, 0.0f); putw(self, 2156, 0);
    put(v1, 836, 1.0f); put(v1, 16, 50); put(v1, 20, 60); put(v1, 24, 70);
    put(v2, 836, 1.0f); put(v2, 16, 51); put(v2, 20, 61); put(v2, 24, 71);
    vehicleList[0] = self; vehicleList[1] = v1; vehicleList[2] = v2; vehicleCount = 3;
    calls = 0;
}

int main() {
    int failures = 0;
    float out = -77.0f;
    reset();
    // v1: gap = 4 - 1 = 3, limit = 2 * (0 * K + 2.1) = 4.2 -> passes; side 0.5 < (2+1)*0.75 -> match
    script[0] = 4.0f; script[1] = 0.5f;
    unsigned char* r = getBlockingVehicle__7VehiclePf(self, &out);
    if (r != v1 || out != 0.5f || calls != 2) { std::fprintf(stderr, "fail: match v1 (%p calls %d out %g)\n", (void*)r, calls, out); ++failures; }
    if (seen[0].a.x != 5 || seen[0].b.x != 0 || seen[0].b.y != -1 || seen[0].c.x != 50 || seen[1].b.x != 1 || seen[1].b.y != 0 || seen[1].c.z != 70) {
        std::fprintf(stderr, "fail: plane arguments\n"); ++failures;
    }
    // null output pointer is allowed
    reset(); script[0] = 4.0f; script[1] = -0.5f;
    if (getBlockingVehicle__7VehiclePf(self, 0) != v1) { std::fprintf(stderr, "fail: null out\n"); ++failures; }
    // too far behind: gap not below the limit, falls through to the second vehicle
    reset(); out = -77.0f; script[0] = 100.0f; script[1] = 2.0f; script[2] = 0.25f;
    r = getBlockingVehicle__7VehiclePf(self, &out);
    if (r != v2 || out != 0.25f || calls != 3) { std::fprintf(stderr, "fail: second vehicle\n"); ++failures; }
    // gap zero or negative is skipped; too wide is skipped; nothing found returns 0 and leaves out
    reset(); out = -77.0f; script[0] = 4.0f; script[1] = 2.5f; script[2] = 1.0f;
    if (getBlockingVehicle__7VehiclePf(self, &out) != 0 || out != -77.0f || calls != 3) { std::fprintf(stderr, "fail: nothing blocks (calls %d)\n", calls); ++failures; }
    // the faster the vehicle, the bigger the limit: speed 10 -> min(1, 1.0) * 1.9 + 2.1 = 4 -> limit 8
    reset(); put(self, 840, 10.0f); script[0] = 8.5f; script[1] = 0.0f;
    if (getBlockingVehicle__7VehiclePf(self, &out) != v1) { std::fprintf(stderr, "fail: speed scaled limit\n"); ++failures; }
    reset(); put(self, 840, 10.0f); script[0] = 9.5f; script[1] = 0.0f; script[2] = 9.5f; script[3] = 0.0f;
    if (getBlockingVehicle__7VehiclePf(self, &out) != 0) { std::fprintf(stderr, "fail: limit clamp\n"); ++failures; }
    // an other vehicle moving along the back vector faster than our own value is skipped before any query
    reset(); put(v1, 1188 + 4, -5.0f); put(v2, 1188 + 4, -5.0f); put(self, 840, 1.0f);
    // back = (0,-1,0): dot = 5 > 1 -> skipped
    if (getBlockingVehicle__7VehiclePf(self, &out) != 0 || calls != 0) { std::fprintf(stderr, "fail: velocity skip\n"); ++failures; }
    // own index is skipped even when it is not first
    reset(); putw(self, 2156, 1); vehicleList[0] = v2; vehicleList[1] = self; vehicleList[2] = v1; script[0] = 4.0f; script[1] = 0.0f;
    if (getBlockingVehicle__7VehiclePf(self, &out) != v2) { std::fprintf(stderr, "fail: own index\n"); ++failures; }
    reset(); vehicleCount = 0;
    if (getBlockingVehicle__7VehiclePf(self, &out) != 0) { std::fprintf(stderr, "fail: empty\n"); ++failures; }
    return failures ? 1 : 0;
}
