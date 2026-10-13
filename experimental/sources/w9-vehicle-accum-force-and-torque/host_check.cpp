// Host contract for accumForceAndTorque__7VehicleP6Point3T1i. Not an EE plan input.
#include "accum_force_and_torque.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static int failures = 0;
static const void* seen_matrix;
static const Point3* seen_input;

// Stand-in for the matrix un-rotation: local = 2 * input, so a missing or
// misplaced use of the local force shows up in the torque.
extern "C" void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* m, const Point3* in) {
    seen_matrix = m; seen_input = in;
    out->x = in->x * 2; out->y = in->y * 2; out->z = in->z * 2;
}
static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static bool close(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }

int main() {
    static unsigned char v[2304];
    const float S = 0x1.a41a40p-6f;
    const Point3 point = {1.0f, 2.0f, 3.0f};
    const Point3 force = {4.0f, -5.0f, 6.0f};
    // local = (8,-10,12); scaled = S*(1,2,3)
    const float tx = S * 2 * 12 - S * 3 * -10, ty = S * 3 * 8 - S * 1 * 12, tz = S * 1 * -10 - S * 2 * 8;
    for (int flag = 0; flag < 2; ++flag) {
        std::memset(v, 0, sizeof v);
        for (int off = 1992; off < 2040; off += 4) at(v, off) = 100.0f + (off - 1992);
        unsigned char before[sizeof v]; std::memcpy(before, v, sizeof v);
        accumForceAndTorque__7VehicleP6Point3T1i(v, &point, &force, flag == 0 ? 0 : 7);
        expect(seen_matrix == v + 1116 && seen_input == &force, "matrix at +1116 applied to the original force");
        const int foff = flag ? 2016 : 2004, toff = flag ? 2028 : 1992;
        expect(close(at(v, foff), at(before, foff) + 4) && close(at(v, foff + 4), at(before, foff + 4) - 5) && close(at(v, foff + 8), at(before, foff + 8) + 6), "force accumulated");
        expect(close(at(v, toff), at(before, toff) + tx) && close(at(v, toff + 4), at(before, toff + 4) + ty) && close(at(v, toff + 8), at(before, toff + 8) + tz), "torque accumulated");
        for (int off = 1992; off < 2040; off += 4) {
            const bool target = (off >= foff && off < foff + 12) || (off >= toff && off < toff + 12);
            if (!target) expect(at(v, off) == at(before, off), "other accumulator pair untouched");
        }
    }
    return failures ? 1 : 0;
}
