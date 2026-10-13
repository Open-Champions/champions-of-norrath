// Host contract for objectMoveWithWorldCollision__FP6Point3G6Point3fiT0. Not an EE plan input.
// worldClipMotion is replaced by a scripted stand-in that records its arguments.
#include <cmath>
#include <cstdio>
#include <cstring>

extern "C" int objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(float*, const float*, float, int, float*);
static int dummy_world;
extern "C" void* world = &dummy_world;

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

enum Mode { Wall, Constant };
static Mode mode;
static float constant_fraction;
static float constant_normal[3];
static int calls, last_mask;
static float last_radius, first_start[3], first_step[3];
static void* last_world;

extern "C" float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void* w, const float* p, const float* m, float r, int mask,
                                                                      float* out) {
    if (calls == 0) { std::memcpy(first_start, p, 12); std::memcpy(first_step, m, 12); }
    ++calls; last_world = w; last_radius = r; last_mask = mask;
    if (mode == Constant) {
        std::memcpy(out, constant_normal, 12);
        return constant_fraction;
    }
    // Wall: solid plane x = 10, normal (-1, 0, 0), reached only when moving in +x.
    if (m[0] > 0.0f && p[0] + m[0] > 10.0f) {
        out[0] = -1.0f; out[1] = 0.0f; out[2] = 0.0f;
        return (10.0f - p[0]) / m[0];
    }
    return 1.0f;
}

int main() {
    {   // Deflection off a wall: pass 1 stops at x=10 after half the step, the motion is turned
        // to (-0.1, 10, 0), pass 2 moves freely and uses up the rest.
        mode = Wall; calls = 0;
        float pos[3] = {0, 0, 0}, move[3] = {20, 10, 0}, out[3] = {7, 7, 7};
        const int r = objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(pos, move, 4.5f, 3, out);
        expect(r == 1, "deflected result is 1");
        expect(calls == 2, "two passes");
        expect(near(pos[0], 9.95f) && near(pos[1], 10.0f) && near(pos[2], 0.0f), "final position");
        expect(out[0] == -1.0f && out[1] == 0.0f && out[2] == 0.0f, "normal copied to output");
        expect(last_world == &dummy_world && last_radius == 4.5f && last_mask == 3, "world, radius and mask passed through");
        expect(first_start[0] == 0 && first_step[0] == 20 && first_step[1] == 10, "first call arguments");
        expect(move[0] == 20 && move[1] == 10, "motion argument not modified");
    }
    {   // No block: one pass, position moved by the whole motion, result 0, null output allowed.
        mode = Wall; calls = 0;
        float pos[3] = {1, 2, 3}, move[3] = {-4, 5, 6};
        const int r = objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(pos, move, 1.0f, 0, nullptr);
        expect(r == 0 && calls == 1 && pos[0] == -3 && pos[1] == 7 && pos[2] == 9, "free move");
    }
    {   // Pass limit: no progress gives eleven calls; a zero normal still gives push = 0.1 - 0 > 0.
        mode = Constant; constant_fraction = 0.0f; std::memset(constant_normal, 0, 12); calls = 0;
        float pos[3] = {0, 0, 0}, move[3] = {1, 1, 1};
        const int r = objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(pos, move, 1.0f, 0, nullptr);
        expect(calls == 11 && r == 1, "eleven passes, flag set by the positive push");
        expect(pos[0] == 0 && pos[1] == 0 && pos[2] == 0, "no motion applied");
    }
    {   // Fractions of 0.4: remaining 0.6, 0.2, then below zero after three calls.
        mode = Constant; constant_fraction = 0.4f; constant_normal[0] = 0; constant_normal[1] = 0; constant_normal[2] = 1; calls = 0;
        float pos[3] = {0, 0, 0}, move[3] = {1, 0, -2};
        const int r = objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(pos, move, 1.0f, 0, nullptr);
        // Pass 1: dot = -2, push = 2.1 so z motion becomes about 0.1; pass 3 ends on remaining -0.2.
        expect(r == 1, "deflected");
        expect(calls == 3, "three passes");
    }
    {   // Stops once remaining is at or below 0.1: fraction 0.95 leaves 0.05 after one call.
        mode = Constant; constant_fraction = 0.95f; std::memset(constant_normal, 0, 12); calls = 0;
        float pos[3] = {0, 0, 0}, move[3] = {10, 0, 0};
        objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(pos, move, 1.0f, 0, nullptr);
        expect(calls == 1 && near(pos[0], 9.5f), "one pass at 0.95");
    }
    {   // A push is applied only when remaining is above 0.1 after the pass: fraction 0.95, normal facing the motion.
        mode = Constant; constant_fraction = 0.95f; constant_normal[0] = -1; constant_normal[1] = 0; constant_normal[2] = 0; calls = 0;
        float pos[3] = {0, 0, 0}, move[3] = {10, 0, 0};
        const int r = objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(pos, move, 1.0f, 0, nullptr);
        expect(r == 0, "no deflection flag when little remains");
    }
    {   // Motion (3,0,0) with normal (-1,0,0), fraction 0.5: pass 2 step shows the turned motion.
        mode = Constant; constant_fraction = 0.5f; constant_normal[0] = -1; constant_normal[1] = 0; constant_normal[2] = 0; calls = 0;
        float pos[3] = {0, 0, 0}, move[3] = {3, 0, 0};
        const int r = objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(pos, move, 1.0f, 0, nullptr);
        // pass 1 moves 1.5, motion becomes (3 - 3.1) = -0.1; pass 2 (frac 0.5, remaining 0): moves -0.05*0.5.
        expect(r == 1 && calls == 2 && near(pos[0], 1.5f - 0.025f), "turned motion used by the second pass");
    }
    if (failures) return 1;
    std::printf("objectMoveWithWorldCollision contract passed\n");
    return 0;
}
