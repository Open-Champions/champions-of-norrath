// Host contract for gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA -ffp-contract=off host_check.cpp
// Record words are arena addresses; the gameLines array holds host pointers.
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include "../w10-world-b-common/host_arena.hpp"
#include "../w10-world-b-common/host_check.hpp"
#include <cmath>
unsigned char* gameLines[100];
int gameLineCount;
static int plane_calls;
// Independent plane distance: dot(second, hit - first).
extern "C" float distanceToPlane__FP6Point3N20(const unsigned char* a, const unsigned char* b, const unsigned char* c) {
    ++plane_calls;
    float r = 0;
    for (int k = 0; k != 3; ++k) r += lf_(b + 4 * k) * (lf_(c + 4 * k) - lf_(a + 4 * k));
    return r;
}
#include "game_line_ray_collision.cpp"
static unsigned char* P = arena_at(0x100);
static unsigned char* Q = arena_at(0x110);
static unsigned char* OUT = arena_at(0x1000);
static unsigned char* rec(int i) { return arena_at(0x2000 + 0x100 * i); }
static unsigned char* verts(int i) { return arena_at(0x4000 + 0x400 * i); }
static void vec(unsigned char* p, float x, float y, float z) { sf_(p, x); sf_(p + 4, y); sf_(p + 8, z); }
static void setvertex(int line, int i, float px, float py, float pz, float nx, float ny, float nz, float thick, float sx = 0, float sy = 1, float sz = 0) {
    unsigned char* v = verts(line) + 64 * i;
    vec(v, px, py, pz); vec(v + 12, sx, sy, sz); vec(v + 24, nx, ny, nz); sf_(v + 48, thick);
}
static void setline(int line, unsigned flags, unsigned count, unsigned closed) {
    sw_(rec(line) + 12, flags); sw_(rec(line) + 4, count); sw_(rec(line) + 8, closed);
    sw_(rec(line) + 20, norrath_host_address(verts(line)));
    gameLines[line] = rec(line);
}
static void reset() {
    std::memset(arena_at(0), 0, 0x8000);
    std::memset(OUT, 0xaa, 24 * 8);
    gameLineCount = 0; plane_calls = 0;
    vec(P, 0, 0, 0); vec(Q, 10, 0, 0);
}
static int call(int max) { return gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi(P, Q, OUT, max); }
int main() {
    // A wall at x = 5, normal +x: denominator -10, numerator -5, t = 0.5, hit (5,0,0).
    reset(); gameLineCount = 1; setline(0, 1, 2, 0);
    setvertex(0, 0, 5, 0, 0, 1, 0, 0, 1);
    if (call(4) != 1) return 1;
    if (hw(OUT) != norrath_host_address(rec(0)) || hw(OUT + 4) != norrath_host_address(verts(0))) return 2;
    if (lf_(OUT + 8) != 5.f || lf_(OUT + 12) != 0.f || lf_(OUT + 16) != 0.f) return 3;
    if (hw(OUT + 20) != 0xaaaaaaaau) return 4;                   // fourth word of the record untouched
    if (hw(OUT + 24) != 0xaaaaaaaau) return 5;
    // Segment count is count - 1 for an open line, count when the closed word is set.
    reset(); gameLineCount = 1; setline(0, 1, 1, 0); setvertex(0, 0, 5, 0, 0, 1, 0, 0, 1);
    if (call(4) != 0) return 6;
    setline(0, 1, 1, 7);
    if (call(4) != 1) return 7;
    reset(); gameLineCount = 1; setline(0, 1, 3, 0);
    setvertex(0, 0, 5, 0, 0, 1, 0, 0, 1); setvertex(0, 1, 6, 0, 0, 1, 0, 0, 1); setvertex(0, 2, 7, 0, 0, 1, 0, 0, 1);
    if (call(9) != 2) return 8;                                  // two segments of three vertices
    if (lf_(OUT + 8) != 5.f || lf_(OUT + 24 + 8) != 6.f) return 9;
    if (hw(OUT + 24 + 4) != norrath_host_address(verts(0) + 64)) return 10;
    // The maximum ends the walk at once.
    reset(); gameLineCount = 1; setline(0, 1, 3, 0);
    setvertex(0, 0, 5, 0, 0, 1, 0, 0, 1); setvertex(0, 1, 6, 0, 0, 1, 0, 0, 1);
    if (call(1) != 1 || hw(OUT + 24) != 0xaaaaaaaau) return 11;
    // Flag bit 0 clear skips the whole line; other bits do not matter.
    reset(); gameLineCount = 1; setline(0, 0xfffffffe, 2, 0); setvertex(0, 0, 5, 0, 0, 1, 0, 0, 1);
    if (call(4) != 0 || plane_calls != 0) return 12;
    setline(0, 0x101, 2, 0); if (call(4) != 1) return 13;
    // gameLineCount bounds the lines.
    reset(); gameLineCount = 1; setline(0, 1, 2, 0); setline(1, 1, 2, 0);
    setvertex(0, 0, 5, 0, 0, 1, 0, 0, 1); setvertex(1, 0, 6, 0, 0, 1, 0, 0, 1);
    if (call(4) != 1) return 14;
    gameLineCount = 2; if (call(4) != 2) return 15;
    if (hw(OUT + 24) != norrath_host_address(rec(1))) return 16;
    // t outside [0,1] skips: the wall at x = 11 (t = -0.1) and at x = -1 (t = 1.1).
    reset(); gameLineCount = 1; setline(0, 1, 3, 0);
    setvertex(0, 0, 11, 0, 0, 1, 0, 0, 1); setvertex(0, 1, -1, 0, 0, 1, 0, 0, 1);
    if (call(4) != 0 || plane_calls != 0) return 17;
    // t exactly 0 and exactly 1 are kept (walls at x = 10 and x = 0).
    reset(); gameLineCount = 1; setline(0, 1, 3, 0);
    setvertex(0, 0, 10, 0, 0, 1, 0, 0, 1); setvertex(0, 1, 0, 0, 0, 1, 0, 0, 1);
    if (call(4) != 2) return 18;
    // Small denominators are clamped away from zero: d.N = -10 * 0.0005 = -0.005 is lowered to
    // -0.01; numerator -5 * 0.0005 = -0.0025; t = 0.25 and the hit is 10 - 10 * 0.25 = 7.5.
    reset(); gameLineCount = 1; setline(0, 1, 2, 0);
    setvertex(0, 0, 5, 0, 0, 0.0005f, 0, 0, 1);
    if (call(4) != 1 || std::fabs(lf_(OUT + 8) - 7.5f) > 1e-4f) return 19;
    // Positive denominator: d = +10 gives 0.005, raised to 0.01; t = 0.25 and the hit is 12.5.
    reset(); gameLineCount = 1; setline(0, 1, 2, 0); vec(P, 20, 0, 0);
    setvertex(0, 0, 15, 0, 0, 0.0005f, 0, 0, 1);
    if (call(4) != 1 || std::fabs(lf_(OUT + 8) - 12.5f) > 1e-4f) return 20;
    // Zero denominator with a nonzero numerator gives an infinite t, which is skipped.
    reset(); gameLineCount = 1; setline(0, 1, 2, 0);
    setvertex(0, 0, 5, 3, 0, 0, 1, 0, 1);
    if (call(4) != 0) return 21;
    // Zero over zero is NaN; neither range test rejects it, so the segment is kept (as in the image).
    setvertex(0, 0, 5, 0, 0, 0, 1, 0, 1);
    if (call(4) != 1) return 26;
    // Distance window: distance = second . (hit - first). With second (0,1,0) and the wall point at y = -0.3 the hit at y = 0 is 0.3 away.
    reset(); gameLineCount = 1; setline(0, 1, 2, 0);
    setvertex(0, 0, 5, -0.3f, 0, 1, 0, 0, 0.0f);                // 0.3 > 0 + 0.25: skipped
    if (call(4) != 0) return 22;
    setvertex(0, 0, 5, -0.3f, 0, 1, 0, 0, 0.1f);                // 0.3 > 0.35? no: kept
    if (call(4) != 1) return 23;
    setvertex(0, 0, 5, 0.3f, 0, 1, 0, 0, 0.0f);                 // -0.3 < -0.25: skipped
    if (call(4) != 0) return 24;
    setvertex(0, 0, 5, 0.2f, 0, 1, 0, 0, 0.0f);                 // -0.2: kept
    if (call(4) != 1) return 25;
    std::puts("gameLineRayCollision check passed");
    return 0;
}
