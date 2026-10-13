#include <cstdio>
#include <cstring>
struct Point3 { float x, y, z; };
struct Quat { float x, y, z, w; };
extern "C" {
unsigned char trail[66560];
static int mqCalls, extCalls, qtCalls;
static short mqAngle;
static float mqAxis[3];
static Point3 extFirst[64], extSecond[64];
static int extHandle;
static float extWidth;
static unsigned short countSeenByExtend[64];
void makeQuaternion__FP4Quatsfff(Quat* q, short angle, float x, float y, float z) {
    ++mqCalls; mqAngle = angle; mqAxis[0] = x; mqAxis[1] = y; mqAxis[2] = z;
    q->x = 1.0f; q->y = 2.0f; q->z = 3.0f; q->w = 4.0f;
}
void P_ExtendSwoosh__FiG6Point3T1f(int handle, Point3* a, Point3* b, float width) {
    extHandle = handle; extWidth = width;
    if (extCalls < 64) {
        extFirst[extCalls] = *a; extSecond[extCalls] = *b;
        std::memcpy(&countSeenByExtend[extCalls], trail + (handle & 0xff) * 1040 + 1026, 2);
    }
    ++extCalls;
}
void quatTransform__FP6Point3P4QuatT0(Point3* in, Quat* q, Point3* out) {
    ++qtCalls;
    Point3 r = {in->x + q->x, in->y + q->y, in->z + q->z};
    *out = r;
}
void P_RevolveTrail__FiG6Point3N31i(int handle, Point3* a, Point3* b, Point3* c, Point3* d, int steps);
}
static short half(int slot, int off) { short v; std::memcpy(&v, trail + slot * 1040 + 1024 + off, 2); return v; }
static void setHalf(int slot, int off, short v) { std::memcpy(trail + slot * 1040 + 1024 + off, &v, 2); }
int main() {
    Point3 p0 = {1, 2, 3}, axis = {0, 0, 1}, p2 = {10, 20, 30}, p3 = {-10, -20, -30};
    // Zero handle: nothing.
    P_RevolveTrail__FiG6Point3N31i(0, &p0, &axis, &p2, &p3, 5);
    if (mqCalls || extCalls || qtCalls) return 1;
    // Stale handle: nothing and the header is untouched.
    setHalf(5, 0, 0x0105); setHalf(5, 2, 77);
    P_RevolveTrail__FiG6Point3N31i(0x0205, &p0, &axis, &p2, &p3, 5);
    if (mqCalls || half(5, 2) != 77) return 2;
    // Normal run with 4 steps.
    trail[5 * 1040 + 1024 + 4] = 9; trail[5 * 1040 + 1024 + 5] = 9;
    P_RevolveTrail__FiG6Point3N31i(0x0105, &p0, &axis, &p2, &p3, 4);
    if (mqCalls != 1 || mqAngle != static_cast<short>(65536 / 2)) return 3;
    if (mqAxis[0] != 0 || mqAxis[2] != 1) return 4;
    if (extCalls != 4 || qtCalls != 8 || extHandle != 0x0105 || extWidth != 1.0f) return 5;
    if (trail[5 * 1040 + 1024 + 4] != 0 || trail[5 * 1040 + 1024 + 5] != 0) return 6;
    if (half(5, 8) != 4 || half(5, 2) != 4) return 7;
    // Step i uses the points rotated i times: p2 += (1,2,3) each time, plus the offset p0.
    for (int i = 0; i != 4; ++i) {
        if (extFirst[i].x != 10 + i * 1 + 1 || extFirst[i].y != 20 + i * 2 + 2 || extFirst[i].z != 30 + i * 3 + 3) return 8;
        if (extSecond[i].x != -10 + i * 1 + 1 || extSecond[i].z != -30 + i * 3 + 3) return 9;
        if (countSeenByExtend[i] != i) return 10;
    }
    // Step counts above 60 are clamped; the angle is 65536 / (n - 2).
    mqCalls = extCalls = qtCalls = 0;
    P_RevolveTrail__FiG6Point3N31i(0x0105, &p0, &axis, &p2, &p3, 500);
    if (half(5, 8) != 60 || extCalls != 60 || mqAngle != (short)(65536 / 58)) return 11;
    // Zero or negative counts build the quaternion and stop.
    mqCalls = extCalls = qtCalls = 0;
    P_RevolveTrail__FiG6Point3N31i(0x0105, &p0, &axis, &p2, &p3, -4);
    if (mqCalls != 1 || extCalls != 0 || half(5, 8) != -4 || half(5, 2) != 0) return 12;
    if (mqAngle != (short)(65536 / -6)) return 13;
    std::puts("P_RevolveTrail contract passed");
    return 0;
}
