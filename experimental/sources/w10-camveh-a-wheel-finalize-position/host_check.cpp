// Host contract for FinalizePosition__5Wheel. Not an EE plan input.
// The EE keeps 4-byte pointers; the host's are 8 bytes. The car pointer sits at
// wheel+396 and is read once before the matrix copy overwrites its upper half
// (wheel+400); the q pointer slot used is index 2 at car+2064 and nothing else
// is stored near it.
#include "finalize_position.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

static int grid_calls, rot_calls;
static void* grid_arg;
static void* rot_matrix; static int rot_axis; static short rot_angle;
static float matrix_at_rot[9];
static float position_at_grid[3];
extern "C" void objectUpdateInGrid__FP10GameObject(void* o) {
    ++grid_calls; grid_arg = o;
    std::memcpy(position_at_grid, (unsigned char*)o + 16, 12);
    if (rot_calls != 0) std::fprintf(stderr, "fail: grid after rot\n");
}
extern "C" void matRot__FP8Matrix33is(void* m, int axis, short angle) {
    ++rot_calls; rot_matrix = m; rot_axis = axis; rot_angle = angle;
    std::memcpy(matrix_at_rot, m, 36);
    if (grid_calls == 0) std::fprintf(stderr, "fail: rot before grid\n");
}

static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static float get(const unsigned char* o, int off) { float v; std::memcpy(&v, o + off, 4); return v; }

int main() {
    int failures = 0;
    alignas(8) unsigned char car[2400];
    alignas(8) unsigned char wheel[480];
    float q[2] = {2.0f, 0.0f};
    struct Case { float amount, steer, turn, speed, angle; };
    const Case cases[] = {
        {0.0f, 0.5f, 0.0f, 0.0f, 10.0f},
        {0.25f, 0.5f, 2.0f, 30.0f, 355.0f},
        {1.0f, -1.0f, 0.0f, 60.0f, 359.9f},
        {0.5f, 2.0f, -3.0f, -90.0f, 5.0f},
    };
    for (const Case& c : cases) {
        std::memset(car, 0, sizeof car);
        std::memset(wheel, 0, sizeof wheel);
        for (int k = 0; k < 9; ++k) put(car, 1116 + 4 * k, 0.125f * (k + 1));
        put(car, 676 + 8, c.amount);
        put(car, 1332 + 24, 10.0f); put(car, 1332 + 28, 20.0f); put(car, 1332 + 32, 30.0f);
        put(car, 800, 0.5f);
        unsigned char* qp = (unsigned char*)q;
        std::memcpy(car + 2056 + 8, &qp, sizeof qp);
        unsigned char* cp = car;
        std::memcpy(wheel + 396, &cp, sizeof cp);
        int idx = 2; std::memcpy(wheel + 384, &idx, 4);
        put(wheel, 376, c.steer); put(wheel, 372, c.turn); put(wheel, 364, c.speed); put(wheel, 368, c.angle);
        put(wheel, 16, -1); put(wheel, 20, -1); put(wheel, 24, -1);
        grid_calls = rot_calls = 0;
        FinalizePosition__5Wheel(wheel);
        const float u[3] = {0.125f * 7, 0.125f * 8, 0.125f * 9};
        const float kk = ((-q[0]) * 39.0f) * (1.0f - c.amount) + c.steer * 39.0f;
        const float want[3] = {10.0f + u[0] * kk, 20.0f + u[1] * kk, 30.0f + u[2] * kk};
        bool ok = grid_calls == 1 && grid_arg == wheel;
        for (int k = 0; k < 3; ++k) ok = ok && get(wheel, 16 + 4 * k) == want[k] && position_at_grid[k] == want[k];
        // matrix copy: the copy is made before matRot; the wheel+400 words that overlap the car pointer are clobbered by design on the host
        if (c.turn != 0.0f) {
            const float scaled = (0.5f * 32767.0f) * (c.turn * 0x1.6c16c0p-8f);
            ok = ok && rot_calls == 1 && rot_matrix == wheel + 400 && rot_axis == 2 && rot_angle == (short)(int)scaled;
            for (int k = 0; k < 9; ++k) ok = ok && matrix_at_rot[k] == 0.125f * (k + 1);
        } else {
            ok = ok && rot_calls == 0;
        }
        float t = c.angle + (c.speed * 0x1.111110p-6f) * 0x1.7ffffep+2f;
        if (360.0f <= t) t = t - 360.0f;
        ok = ok && get(wheel, 368) == t;
        if (!ok) { std::fprintf(stderr, "fail: case amount %g turn %g speed %g angle %g\n", c.amount, c.turn, c.speed, c.angle); ++failures; }
    }
    return failures ? 1 : 0;
}
