// Host contract for BuildMatrix__6CamerafffP6Point3. Not an EE plan input.
// The reciprocal length is 1/sqrtf here, so float comparisons use a tolerance.
#include "build_matrix.cpp"

#include <cmath>
#include <initializer_list>
#include <cstdio>
#include <cstring>

static int failures = 0;
static int ident_calls, rot_calls, view_calls, paused_calls, dlg_calls;
static int rot_axis[4];
static short rot_angle[4];
static float seen_point[3], seen_matrix[12];
static int paused_v, dlg_v;
static bool matrix_was_ident_before_rot;

float gameCameraShake;
unsigned eRandState;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static bool close(double a, double b) { return std::fabs(a - b) <= 2e-5 * (1.0 + std::fabs(b)); }

extern "C" void matIdent__FP8Matrix34(float* m) {
    ++ident_calls;
    for (int i = 0; i < 12; ++i) m[i] = (i == 0 || i == 4 || i == 8) ? 1.0f : 0.0f;
    // diagonal of a 3x3 stored in nine consecutive floats is 0, 4, 8
}
extern "C" void matRot__FP8Matrix34is(float* m, int axis, short angle) {
    if (rot_calls < 4) { rot_axis[rot_calls] = axis; rot_angle[rot_calls] = angle; }
    ++rot_calls;
    m[9] += 1.0f;
}
extern "C" void worldSetViewMatricies__FP6Point3P8Matrix34(float* p, float* m) {
    ++view_calls;
    std::memcpy(seen_point, p, 12);
    std::memcpy(seen_matrix, m, 48);
}
extern "C" int gameIsPaused__Fv() { ++paused_calls; return paused_v; }
extern "C" int DLG_Active__Fv() { ++dlg_calls; return dlg_v; }

static void reset_counts() { ident_calls = rot_calls = view_calls = paused_calls = dlg_calls = 0; }

static void put_half(unsigned char* c, int off, unsigned short v) { std::memcpy(c + off, &v, 2); }

int main() {
    static unsigned char camera[512];
    // Null target.
    const unsigned pitches[] = {0u, 1u, 0x7fffu, 0x8000u, 0xffffu, 0x4000u};
    const short yaws[] = {0, 1, -1, 0x7fff, -0x8000, 12345};
    for (unsigned i = 0; i < 6; ++i) {
        std::memset(camera, 0, sizeof camera);
        put_half(camera, 106, static_cast<unsigned short>(pitches[i]));
        put_half(camera, 104, static_cast<unsigned short>(yaws[i]));
        reset_counts();
        gameCameraShake = 0.0f;
        BuildMatrix__6CamerafffP6Point3(camera, 1.0f, 2.0f, 3.0f, nullptr);
        expect(ident_calls == 1 && rot_calls == 2 && view_calls == 1, "identity, two rotations, one view call");
        expect(rot_axis[0] == 0 && rot_axis[1] == 2, "axes 0 then 2");
        expect(rot_angle[0] == static_cast<short>(pitches[i] + 16339u), "first angle is the pitch halfword plus 16339 as a short");
        expect(rot_angle[1] == yaws[i], "second angle is the signed yaw halfword");
        expect(seen_point[0] == 1.0f && seen_point[1] == 2.0f && seen_point[2] == 3.0f, "point unchanged without shake");
        expect(seen_matrix[9] == 2.0f && seen_matrix[0] == 1.0f, "matrix is the one that was rotated");
        expect(paused_calls == 0, "no pause query without shake");
    }
    // Non-null target.
    const float targets[][3] = {{1, 0, 0}, {0.3f, -0.4f, 0.8f}, {-5, 2, 7}, {0, 0, 1}, {0, 3, -2}};
    for (const auto& t : targets) {
        reset_counts();
        gameCameraShake = 0.0f;
        BuildMatrix__6CamerafffP6Point3(camera, -4.0f, 5.0f, 6.0f, t);
        expect(ident_calls == 1 && rot_calls == 0 && view_calls == 1, "identity only, one view call");
        double tx = t[0], ty = t[1], tz = t[2];
        double bx = ty, by = -tx, bz = 0;
        double bl = std::sqrt(bx * bx + by * by + bz * bz);
        bx /= bl; by /= bl; bz /= bl;
        double cx = ty * bz - tz * by, cy = tz * bx - tx * bz, cz = tx * by - ty * bx;
        double cl = std::sqrt(cx * cx + cy * cy + cz * cz);
        cx /= cl; cy /= cl; cz /= cl;
        const double want[12] = {bx, cx, tx, by, cy, ty, bz, cz, tz, 0, 0, 0};
        if (t[0] == 0 && t[1] == 0) continue;  // B has zero length: not a valid input
        for (int i = 0; i < 9; ++i) expect(close(seen_matrix[i], want[i]), "matrix element");
        for (int i = 9; i < 12; ++i) expect(seen_matrix[i] == 0.0f, "translation words stay zero");
        expect(seen_point[0] == -4.0f && seen_point[1] == 5.0f && seen_point[2] == 6.0f, "point unchanged without shake");
    }
    // Shake: gates and query order.
    struct Gate { int paused, dlg; bool applies; int dlg_calls; };
    const Gate gates[] = {{0, 0, true, 0}, {0, 1, true, 0}, {1, 0, false, 1}, {1, 1, true, 1}};
    for (const auto& g : gates) {
        reset_counts();
        paused_v = g.paused; dlg_v = g.dlg;
        gameCameraShake = 3.0f;
        eRandState = 12345u;
        BuildMatrix__6CamerafffP6Point3(camera, 10.0f, 20.0f, 30.0f, nullptr);
        expect(paused_calls == 1 && dlg_calls == g.dlg_calls, "DLG_Active is asked only after a nonzero pause");
        expect((gameCameraShake == 2.5f) == g.applies, "shake decays by one half only when applied");
        expect((eRandState != 12345u) == g.applies, "random state advances only when applied");
        if (!g.applies) expect(seen_point[0] == 10.0f && seen_point[1] == 20.0f && seen_point[2] == 30.0f, "no offset when blocked");
    }
    // Shake values against an independent model.
    const unsigned seeds[] = {0u, 1u, 12345u, 0xffffffffu, 0x80000000u};
    const float amounts[] = {0.25f, 1.0f, 6.0f};
    for (unsigned seed : seeds) for (float s : amounts) {
        paused_v = 0; dlg_v = 0;
        gameCameraShake = s;
        eRandState = seed;
        reset_counts();
        BuildMatrix__6CamerafffP6Point3(camera, 10.0f, 20.0f, 30.0f, nullptr);
        unsigned long long state = seed;
        double offsets[3];
        for (int i = 0; i < 3; ++i) {
            state = (state * 1664525ull + 1013904223ull) & 0xffffffffull;
            offsets[i] = ((double)(state >> 16) / 65536.0 - 0.5) * 2.0 * s;
        }
        expect(eRandState == state, "state after three LCG steps");
        expect(close(seen_point[0], 10.0 + offsets[0]) && close(seen_point[1], 20.0 + offsets[1]) && close(seen_point[2], 30.0 + offsets[2]), "offsets applied to x, y, z in step order");
        expect(gameCameraShake == s - 0.5f, "amount decremented by 0.5 (may go negative)");
    }
    // Zero or negative amounts do not shake and do not ask about pause.
    for (float s : {0.0f, -1.0f}) {
        gameCameraShake = s; eRandState = 7; reset_counts();
        BuildMatrix__6CamerafffP6Point3(camera, 1, 2, 3, nullptr);
        expect(paused_calls == 0 && eRandState == 7 && gameCameraShake == s, "no shake when amount is not above zero");
    }
    return failures ? 1 : 0;
}
