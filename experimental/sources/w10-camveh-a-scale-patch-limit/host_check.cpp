// Host contract for scalePatchLimit__7VehicleiP6Point3. Not an EE plan input.
#include "scale_patch_limit.cpp"

#include <cstdio>
#include <cstring>

static int dp_calls;
extern "C" Double fptodp(float v) { double d = v; Double r; std::memcpy(&r, &d, 8); ++dp_calls; return r; }
static double asd(Double a) { double d; std::memcpy(&d, &a, 8); return d; }
extern "C" int dpcmp(Double a, Double b) { return asd(a) < asd(b) ? -1 : asd(a) > asd(b) ? 1 : 0; }
extern "C" Double dpsub(Double a, Double b) { double d = asd(a) - asd(b); Double r; std::memcpy(&r, &d, 8); return r; }
extern "C" float dptofp(Double a) { return (float)asd(a); }

static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }

int main() {
    int failures = 0;
    unsigned char o[2304];
    std::memset(o, 0, sizeof o);
    struct Case { int index; float r[3]; float f; Point3 dir; float want; } cases[] = {
        {0, {1, 0, 0}, 0.25f, {3, 0, 0}, 1.0f},
        {0, {-1, 0, 0}, 0.25f, {3, 0, 0}, 1.0f},   // absolute value
        {1, {0, 1, 0}, 0.5f, {0, 4, 0}, 1.0f},
        {2, {0, 0, 1}, 0.5f, {0, 0, -2}, 1.0f},
        {3, {0.5f, 0, 0}, 0.0f, {2, 0, 0}, 0.5f},
        {3, {0.5f, 0, 0}, 1.0f, {2, 0, 0}, 1.0f},  // f = 1: result is f whatever the dot
        {0, {0, 1, 0}, 0.125f, {2, 0, 0}, 0.125f}, // perpendicular
    };
    for (const Case& c : cases) {
        put(o, 764, c.f);
        for (int k = 0; k < 3; ++k) put(o, 1644 + 12 * c.index + 4 * k, c.r[k]);
        const Point3 before = c.dir;
        Point3 dir = c.dir;
        dp_calls = 0;
        float got = scalePatchLimit__7VehicleiP6Point3(o, c.index, &dir);
        if (got != c.want) { std::fprintf(stderr, "fail: index %d got %g want %g\n", c.index, got, c.want); ++failures; }
        if (dir.x != before.x || dir.y != before.y || dir.z != before.z) { std::fprintf(stderr, "fail: caller copy modified\n"); ++failures; }
        if (dp_calls != 1) { std::fprintf(stderr, "fail: fptodp calls %d\n", dp_calls); ++failures; }
    }
    return failures ? 1 : 0;
}
