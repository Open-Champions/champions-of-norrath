// Host contract for buildCollisionBox__7Vehicle. Not an EE plan input.
#include "build_collision_box.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static int calls;
static const void* seen_matrix;
static const Point3* seen_in[4];
static Point3* seen_out[4];
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* m, const Point3* in) {
    seen_matrix = m; seen_in[calls] = in; seen_out[calls] = out;
    out->x = in->x * 2.0f + 3.0f; out->y = in->y - 1.0f; out->z = 9.0f;
    ++calls;
}

static float get(const unsigned char* o, int off) { float v; std::memcpy(&v, o + off, 4); return v; }
static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static float inv(float s) { return 1.0f / __builtin_sqrtf(s); }

int main() {
    int failures = 0;
    unsigned char v[2400];
    std::memset(v, 0, sizeof v);
    // rows (x, y, z): (3, 4, 9), (0, 2, 5), (-6, 8, 1)
    const float mrows[9] = {3, 4, 9, 0, 2, 5, -6, 8, 1};
    for (int k = 0; k < 9; ++k) put(v, 1116 + 4 * k, mrows[k]);
    put(v, 16, 100); put(v, 20, 200); put(v, 24, 300);
    for (int j = 0; j < 4; ++j) {
        put(v, 404 + 12 * j, 1.0f + j); put(v, 408 + 12 * j, 10.0f * (j + 1)); put(v, 412 + 12 * j, 0.5f * (j + 1));
        put(v, 500 + 12 * j, 7.0f * j); put(v, 504 + 12 * j, -2.0f * j); put(v, 508 + 12 * j, 4.0f);
    }
    unsigned char before[sizeof v];
    std::memcpy(before, v, sizeof v);
    buildCollisionBox__7Vehicle(v);
    float bx[3], by[3];
    for (int k = 0; k < 3; ++k) {
        const float r = inv(mrows[3 * k] * mrows[3 * k] + mrows[3 * k + 1] * mrows[3 * k + 1]);
        bx[k] = mrows[3 * k] * r; by[k] = mrows[3 * k + 1] * r;
    }
    unsigned char expect[sizeof v];
    std::memcpy(expect, before, sizeof v);
    for (int j = 0; j < 4; ++j) {
        const float px = 1.0f + j, py = 10.0f * (j + 1), pz = 0.5f * (j + 1);
        put(expect, 452 + 12 * j, ((bx[0] * px + bx[1] * py) + bx[2] * pz) + 100.0f);
        put(expect, 456 + 12 * j, ((by[0] * px + by[1] * py) + by[2] * pz) + 200.0f);
        put(expect, 460 + 12 * j, 0.0f + 300.0f);
        const float ox = (7.0f * j) * 2.0f + 3.0f, oy = (-2.0f * j) - 1.0f;
        const float r = inv(ox * ox + oy * oy + 0.0f * 0.0f);
        put(expect, 548 + 12 * j, ox * r); put(expect, 552 + 12 * j, oy * r); put(expect, 556 + 12 * j, 0.0f * r);
    }
    if (calls != 4 || seen_matrix != v + 1116) { std::fprintf(stderr, "fail: transform calls\n"); ++failures; }
    for (int j = 0; j < 4 && j < calls; ++j) {
        if (seen_in[j] != (const Point3*)(v + 500 + 12 * j) || seen_out[j] != (Point3*)(v + 548 + 12 * j)) { std::fprintf(stderr, "fail: transform args %d\n", j); ++failures; }
    }
    for (int i = 0; i < (int)sizeof v; ++i) {
        if (v[i] != expect[i]) { std::fprintf(stderr, "fail: byte %d differs (offset class %d)\n", i, i); ++failures; break; }
    }
    (void)get;
    return failures ? 1 : 0;
}
