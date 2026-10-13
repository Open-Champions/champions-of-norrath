// Host check only; not a build input. sinf and cosf are stubbed with logging.
#include "transform.cpp"
#include <cmath>
#include <cstdio>
static char order[16];
static int n;
static float seenAngle[6];
extern "C" float sinf(float x) { order[n] = 's'; seenAngle[n++] = x; return (float)__builtin_sin((double)x); }
extern "C" float cosf(float x) { order[n] = 'c'; seenAngle[n++] = x; return (float)__builtin_cos((double)x); }
int main() {
    float angles[3] = {0.5f, 0.25f, -1.0f}, t[3] = {4, 5, 6};
    float m[17];
    for (int i = 0; i < 17; ++i) m[i] = -9.0f;
    Transform__10VIMatrix44RC7VIVect3fT1(m, angles, 2.0f, t);
    bool ok = n == 6 && order[0] == 's' && order[1] == 'c' && order[2] == 's' && order[3] == 'c' && order[4] == 's' && order[5] == 'c' &&
              seenAngle[0] == 0.5f && seenAngle[1] == 0.5f && seenAngle[2] == 0.25f && seenAngle[3] == 0.25f && seenAngle[4] == -1.0f &&
              seenAngle[5] == -1.0f && m[16] == -9.0f;
    float s0 = (float)__builtin_sin(0.5), c0 = (float)__builtin_cos(0.5), s1 = (float)__builtin_sin(0.25), c1 = (float)__builtin_cos(0.25), s2 = (float)__builtin_sin(-1.0), c2 = (float)__builtin_cos(-1.0);
    float want[16] = {(c2 * c0 + (s2 * s1) * s0) * 2.0f, (s2 * c1) * 2.0f, ((-c2) * s0 + (s2 * s1) * c0) * 2.0f, 0,
                      ((-s2) * c0 + (c2 * s1) * s0) * 2.0f, (c2 * c1) * 2.0f, (s2 * s0 + (c2 * s1) * c0) * 2.0f, 0,
                      (c1 * s0) * 2.0f, (-s1) * 2.0f, (c1 * c0) * 2.0f, 0,
                      4, 5, 6, 1};
    for (int i = 0; i < 16; ++i) ok = ok && m[i] == want[i];
    // identity angles and unit scale give the identity rotation with the translation row
    n = 0;
    float zero[3] = {0, 0, 0}, id[16];
    Transform__10VIMatrix44RC7VIVect3fT1(id, zero, 1.0f, t);
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) {
        float e = r == c ? 1.0f : 0.0f;
        if (r == 3 && c < 3) e = t[c];
        ok = ok && id[4 * r + c] == e;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
