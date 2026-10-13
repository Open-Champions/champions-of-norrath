#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
struct Line { float from[3], to[3]; unsigned char c[8]; float width; };
static std::vector<Line> lines;
struct Pos { unsigned model, anim, index; float at[3]; short angle; float scale; unsigned matrix; };
static std::vector<Pos> poses;
static unsigned shadowCount;
extern "C" {
unsigned modelGetNmShadowPoints__FP12_modelHeader(unsigned char*) { return shadowCount; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    unsigned char* model, unsigned char* anim, const float* position, short angle, int index, float* out,
    float* outFloat, unsigned char* matrix, float scale) {
    Pos p = {norrath_host_address(model), norrath_host_address(anim), (unsigned)index, {position[0], position[1], position[2]}, angle, scale, matrix != nullptr};
    poses.push_back(p);
    out[0] = 100.0f + index; out[1] = 200.0f; out[2] = 300.0f; *outFloat = 1.0f;
}
void P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(const float* from, const float* to, unsigned char a, unsigned char b,
                                         unsigned char c, unsigned char d, unsigned char e, unsigned char f,
                                         unsigned char g, unsigned char h, float width) {
    Line l = {{from[0], from[1], from[2]}, {to[0], to[1], to[2]}, {a, b, c, d, e, f, g, h}, width};
    lines.push_back(l);
}
void DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
    const float* start, int a, const float* end, int b, int count, int sub, float step0, unsigned char* obj,
    unsigned char* model, unsigned char* anim);
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
static bool at(const float* v, float x, float y, float z) { return near(v[0], x) && near(v[1], y) && near(v[2], z); }
#define DRAW(...) DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(__VA_ARGS__)
int main() {
    unsigned char* obj = arena_at(0x100);
    unsigned char* model = arena_at(0x400);
    unsigned char* anim = arena_at(0x800);
    float p0[3] = {0, 0, 0}, p1[3] = {10, 0, 0};
    // Nothing for a == b == 0, or count 0 or negative.
    DRAW(p0, 0, p1, 0, 3, 2, 2.0f, nullptr, nullptr, nullptr); if (!lines.empty()) return 1;
    DRAW(p0, 9, p1, 9, 0, 2, 2.0f, nullptr, nullptr, nullptr); if (!lines.empty()) return 2;
    DRAW(p0, 9, p1, 9, -3, 2, 2.0f, nullptr, nullptr, nullptr); if (!lines.empty()) return 3;
    // Three main lines on alternating sides of p1, no bundle (sub 0). Side step: (-dy, dx, dz) normalised
    // and scaled by step0 / (count - 1) = 1. End points p1 + (0, 0, 0), p1 + (0, 1, 0), p1 - (0, 1, 0).
    DRAW(p0, 300, p1, -4, 3, 0, 2.0f, nullptr, nullptr, nullptr);
    if (lines.size() != 3) return 4;
    const float ys[3] = {0.0f, 1.0f, -1.0f};
    for (int i = 0; i < 3; ++i) {
        if (!at(lines[i].from, 0, 0, 0) || !at(lines[i].to, 10, ys[i], 0)) return 5 + i;
        const unsigned char want[8] = {255, 255, 255, 300 & 255, 255, 255, 255, (unsigned char)-4};
        if (std::memcmp(lines[i].c, want, 8) != 0 || lines[i].width != 0.0f) return 8 + i;
    }
    // A bundle of sub = 2 lines after the main line of step 1 (none after step 0): end points are
    // pulled back from A = (10, 1, 0) and Q = step 0's end point (10, 0, 0). Level = max(a, b) / 4.
    lines.clear();
    DRAW(p0, 40, p1, 100, 2, 2, 2.0f, nullptr, nullptr, nullptr);
    if (lines.size() != 4) return 12;
    // Count 2: step = 2 / 1; w = (0, 2, 0); step 1 has k = 1, sign +1 -> A = (10, 2, 0), Q = (10, 0, 0).
    // e1 = (5, 1, 0), e2 = (5, 0, 0); A -= e1 / 4, Q -= e2 / 4; then twice e *= 0.8 and subtract.
    if (!at(lines[1].to, 10, 2, 0)) return 13;
    if (!at(lines[2].from, 8.75f - 4.0f, 1.75f - 0.8f, 0) || !at(lines[2].to, 8.75f - 4.0f, 0, 0)) return 14;
    if (!at(lines[3].from, 4.75f - 3.2f, 0.95f - 0.64f, 0) || !at(lines[3].to, 4.75f - 3.2f, 0, 0)) return 15;
    for (int i = 2; i < 4; ++i) {
        const unsigned char want[8] = {255, 255, 255, 25, 255, 255, 255, 25};
        if (std::memcmp(lines[i].c, want, 8) != 0) return 16;
    }
    // Negative level: (-9 + 3) >> 2 = -2 -> 254 (the image rounds negative values toward zero).
    lines.clear();
    DRAW(p0, -9, p1, -9, 2, 1, 2.0f, nullptr, nullptr, nullptr);
    if (lines.size() != 3 || lines[2].c[3] != (unsigned char)-2 || lines[2].c[7] != (unsigned char)-2) return 17;
    // With an object and a model that reports 3 shadow points, end points come from modelGetShadowPos.
    lines.clear(); poses.clear(); shadowCount = 3;
    sf_(obj + 16, 1.0f); sf_(obj + 20, 2.0f); sf_(obj + 24, 3.0f); sh_(obj + 56, (unsigned short)-5);
    DRAW(p0, 9, p1, 9, 2, 0, 2.0f, obj, model, anim);
    unsigned seed = norrath_host_address(obj);
    if (poses.size() != 2 || lines.size() != 2) return 18;
    if (poses[0].index != (seed >> 16) % 3 || poses[0].model != norrath_host_address(model) || poses[0].anim != norrath_host_address(anim)) return 19;
    seed = seed * 0x19660du + 0x3c6ef35fu;
    if (poses[1].index != (seed >> 16) % 3 || poses[0].angle != -5 || poses[0].scale != 1.0f || poses[0].matrix != 0) return 20;
    if (!at(poses[0].at, 1, 2, 3)) return 21;
    if (!at(lines[0].to, 100.0f + poses[0].index, 200, 300) || !at(lines[1].to, 100.0f + poses[1].index, 200, 300)) return 22;
    // No shadow points: the side-step path again.
    lines.clear(); poses.clear(); shadowCount = 0;
    DRAW(p0, 9, p1, 9, 1, 0, 2.0f, obj, model, anim);
    if (!poses.empty() || lines.size() != 1) return 23;
    return 0;
}
