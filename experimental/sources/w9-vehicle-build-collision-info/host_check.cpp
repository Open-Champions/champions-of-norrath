// Host contract for buildCollisionInfo__7Vehicle. Not an EE plan input. The
// sphere slot array (+2104.., 4-byte stride) overlaps with 8-byte host
// pointers, so only the last slot written is checked for its pointer value.
#include "build_collision_info.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static int failures = 0;
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static int& wd(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }

static unsigned char v[2400];
static unsigned char spheres[4][128];
static int alloc_calls, ctor_calls;
struct Ctor { void* sphere; unsigned char* vehicle; float y, r; };
static Ctor ctors[8];
static int order[16], steps;
static unsigned char vtable[64];
static int destroy_calls, destroy_flag; static unsigned char* destroy_arg;
static void destroy(unsigned char* self, int flag) { order[steps++] = 3; ++destroy_calls; destroy_arg = self; destroy_flag = flag; }

extern "C" void* blockAlloc__Fi(int size) { order[steps++] = 1; if (size != 84) ++failures; return alloc_calls < 4 ? spheres[alloc_calls++] : nullptr; }
extern "C" void __17VehicleBodySphereP7Vehicleff(void* s, unsigned char* veh, float y, float r) { order[steps++] = 2; ctors[ctor_calls++] = {s, veh, y, r}; }

int main() {
    std::memset(v, 0, sizeof v);
    at(v, 900) = 3; at(v, 904) = 4;
    wd(v, 1000) = 1; at(v, 1236) = 10; at(v, 1240) = 20; at(v, 1244) = 30;
    at(v, 928) = 20; at(v, 932) = 10; at(v, 936) = 20; at(v, 940) = 15;   // r2 = 10, b+d = 25 = 2.5*r2 -> q = 1, m = 1
    buildCollisionInfo__7Vehicle(v);
    expect(at(v, 2040) == 0 && at(v, 2044) == 3 && at(v, 2048) == 4, "offset Point3");
    expect(at(v, 1236) == 10 && at(v, 1240) == 17 && at(v, 1244) == 26, "wheel point shifted by the offset");
    expect(at(v, 404) == 10 && at(v, 408) == -10 && at(v, 412) == 0, "corner 0");
    expect(at(v, 416) == 10 && at(v, 420) == 15 && at(v, 424) == 0, "corner 1");
    expect(at(v, 428) == -10 && at(v, 432) == 15 && at(v, 436) == 0, "corner 2");
    expect(at(v, 440) == -10 && at(v, 444) == -10 && at(v, 448) == 0, "corner 3");
    // Edge 0 runs along +y from (10,-10) to (10,15): direction (0,1,0); cross with up = (1,0,0) -> normal (1,0,0).
    expect(near(at(v, 500), 1) && std::fabs(at(v, 504)) < 1e-6f && near(at(v, 512), 0) && near(at(v, 520 + 4), 1) == false, "normal 0 is (1, 0, 0)");
    // Edge 1 from (10,15) to (-10,15): direction (-1,0,0); cross with up = (0,1,0).
    expect(std::fabs(at(v, 512)) < 1e-6f && near(at(v, 516), 1), "normal 1 is (0, 1, 0)");
    expect(near(at(v, 836), std::sqrt(10.0f * 10.0f + 15.0f * 15.0f)), "radius is the longest corner length");
    expect(near(at(v, 28), std::sqrt(325.0f)) && v[40] == 18, "radius stored as a float and as a byte");
    expect(wd(v, 2140) == 1 && alloc_calls == 1 && ctor_calls == 1, "one sphere built");
    // step divides by m - 1 = 0, so y is -(b+d)/2 + r2 plus 0 * inf: NaN on the host, -2.5 where infinity is clamped.
    expect(ctors[0].vehicle == v && ctors[0].r == 10.0f && (std::isnan(ctors[0].y) || near(ctors[0].y, -12.5f + 10)), "sphere y and radius");
    unsigned char* last; std::memcpy(&last, v + 2104, sizeof last);
    expect(last == spheres[0], "slot 0 holds the new sphere");
    expect(order[0] == 1 && order[1] == 2 && steps == 2, "alloc then constructor, no destroy");
    // Existing spheres are destroyed first with flag 3.
    std::memset(v, 0, sizeof v); std::memset(vtable, 0, sizeof vtable);
    short delta = 0x24; void (*fn)(unsigned char*, int) = destroy;
    std::memcpy(vtable + 12, &fn, sizeof fn); std::memcpy(vtable + 8, &delta, 2);
    unsigned char* old = spheres[3]; unsigned char* vt = vtable; std::memcpy(old + 72, &vt, sizeof vt);
    at(v, 928) = 20; at(v, 932) = 10; at(v, 936) = 20; at(v, 940) = 15;
    std::memcpy(v + 2104, &old, sizeof old);
    alloc_calls = ctor_calls = steps = 0;
    buildCollisionInfo__7Vehicle(v);
    short got; std::memcpy(&got, vtable + 8, 2);
    expect(destroy_calls == 1 && destroy_flag == 3 && destroy_arg == old + got && order[0] == 3, "old sphere destroyed through table offset 8 with flag 3 before allocation");
    return failures ? 1 : 0;
}
