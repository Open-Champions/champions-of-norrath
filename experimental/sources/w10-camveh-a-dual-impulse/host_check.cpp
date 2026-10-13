// Host contract for dualImpulse__7VehicleP7VehicleT1P6Point3T3. Not an EE plan input.
#include "dual_impulse.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

static std::vector<int> order;       // 1 particle velocity, 2 transform, 3 backup, 4 add impulse
static unsigned char* velocity_for[2]; static int velocity_calls;
static Point3 velocities[2];
static Point3 transform_in[2]; static const void* transform_matrix[2]; static int transform_calls;
static Point3 transform_result = {1, 2, 3};
static unsigned char* backup_for[2]; static int backup_calls;
static unsigned char* add_for[2]; static Point3 add_impulse[2], add_point[2]; static int add_flag[2]; static int add_calls;

extern "C" Point3* getParticleVelocity__7VehicleP6Point3(Point3* out, unsigned char* v, const Point3* p) {
    order.push_back(1); velocity_for[velocity_calls] = v; *out = velocities[velocity_calls]; ++velocity_calls; (void)p; return out;
}
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* m, const Point3* in) {
    order.push_back(2); transform_in[transform_calls] = *in; transform_matrix[transform_calls] = m; ++transform_calls; *out = transform_result;
}
extern "C" void backupJustForces__7Vehicle(unsigned char* v) { order.push_back(3); backup_for[backup_calls++] = v; }
extern "C" void addImpulse__7VehicleP6Point3T1i(unsigned char* v, const Point3* i, const Point3* p, int flag) {
    order.push_back(4); add_for[add_calls] = v; add_impulse[add_calls] = *i; add_point[add_calls] = *p; add_flag[add_calls] = flag; ++add_calls;
}

static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static void putw(unsigned char* o, int off, int v) { std::memcpy(o + off, &v, 4); }
static bool same(const Point3& a, const Point3& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }

static void reset() { order.clear(); velocity_calls = transform_calls = backup_calls = add_calls = 0; }

int main() {
    int failures = 0;
    const float k = 0x1.a41a40p-6f;
    unsigned char a[2304], b[2304];
    std::memset(a, 0, sizeof a); std::memset(b, 0, sizeof b);
    put(a, 16, 1); put(a, 20, 2); put(a, 24, 3);
    put(b, 16, -4); put(b, 20, 6); put(b, 24, 9);
    put(a, 672, 0.5f); put(b, 672, 0.25f);
    put(a, 1224, 1); put(a, 1228, 1); put(a, 1232, 1);
    const Point3 point = {11, 22, 33};
    const Point3 normal = {1, 0, 0};
    velocities[0] = {9, 5, 7}; velocities[1] = {1, 2, 3};
    // rel = (8, 3, 0); scale = -1.5 -> s = -12 < 0 -> returns 0 with no impulses
    reset();
    if (dualImpulse__7VehicleP7VehicleT1P6Point3T3(nullptr, a, b, &point, &normal) != 0 || order != std::vector<int>({1, 1})) { std::fprintf(stderr, "fail: s < 0\n"); ++failures; }
    // reversed: vA slower than vB -> rel = (-8, -3, 0): s = 12 with scale -1.5
    velocities[0] = {1, 2, 3}; velocities[1] = {9, 5, 7};
    for (int heavy = 0; heavy < 2; ++heavy) {
        putw(a, 2144, 0); putw(b, 2144, heavy);
        reset();
        const int r = dualImpulse__7VehicleP7VehicleT1P6Point3T3(nullptr, a, b, &point, &normal);
        const float scale = heavy ? -2.0f : -1.5f;
        const float s = ((1.0f - 9.0f) * scale) * normal.x + ((2.0f - 5.0f) * scale) * normal.y + 0.0f;
        if (r != 1 || order != std::vector<int>({1, 1, 2, 2, 3, 3, 4, 4})) { std::fprintf(stderr, "fail: call order\n"); ++failures; continue; }
        if (velocity_for[0] != a || velocity_for[1] != b || backup_for[0] != a || backup_for[1] != b || add_for[0] != a || add_for[1] != b) { std::fprintf(stderr, "fail: vehicle arguments\n"); ++failures; }
        if (transform_matrix[0] != a + 1116 || transform_matrix[1] != b + 1116) { std::fprintf(stderr, "fail: matrices\n"); ++failures; }
        const Point3 ra = {(11 - 1) * k, (22 - 2) * k, 0.0f}, rb = {(11 + 4) * k, (22 - 6) * k, 0.0f};
        if (!same(add_point[0], ra) || !same(add_point[1], rb) || add_flag[0] != 0 || add_flag[1] != 0) { std::fprintf(stderr, "fail: lever arms\n"); ++failures; }
        // torque x = r.y * n.z = 0, y = -(r.x * n.z) = -0, z = r.x*n.y - r.y*n.x ; W = T * (1,2,3); U = W x r
        // with transform_result = (1, 2, 3); the d terms are max(0, U . n)
        auto term = [&](const Point3& rr) {
            const Point3 t = {rr.y * normal.z, -(rr.x * normal.z), rr.x * normal.y - rr.y * normal.x};
            const Point3 w = {t.x * 1, t.y * 2, t.z * 3};
            const Point3 u = {w.y * rr.z - w.z * rr.y, w.z * rr.x - w.x * rr.z, w.x * rr.y - w.y * rr.x};
            const float d = (u.x * normal.x + u.y * normal.y) + u.z * normal.z;
            return d > 0.0f ? d : 0.0f;
        };
        const float j = s / (((0.5f + 0.25f) + term(ra)) + term(rb));
        const Point3 want_a = {normal.x * j, normal.y * j, normal.z * j};
        const Point3 want_b = {normal.x * -j, normal.y * -j, normal.z * -j};
        if (!same(add_impulse[0], want_a) || !same(add_impulse[1], want_b)) { std::fprintf(stderr, "fail: impulse values heavy=%d (%g vs %g)\n", heavy, add_impulse[0].x, want_a.x); ++failures; }
    }
    return failures ? 1 : 0;
}
