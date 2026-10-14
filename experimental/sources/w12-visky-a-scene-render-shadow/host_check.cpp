// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// The triangle array pointer is a 32-bit word mapped onto an arena (base 0x100000). All VI calls are
// recording stubs; expected values are recomputed here from the notes at the top of render_shadow.cpp.
#define NORRATH_HOST_ARENA
#include <cstdio>
#include <cstring>
#include <string>
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "render_shadow.cpp"

static const unsigned SCENE = 0x100, TRI = 0x4000, LOC = 0x6000;
static std::string trace;
static unsigned char *seen_m0, *seen_m1, *seen_m2;
static unsigned sprite_result;
static int slot_calls;
static std::string S(long v) { return std::to_string(v); }
static std::string Fs(float f) { char b[40]; std::snprintf(b, sizeof b, "%.9g", f); return b; }
static float bf(unsigned u) { float f; std::memcpy(&f, &u, 4); return f; }
static unsigned char* dtor_log[16];
static int dtor_n;
static std::string which(unsigned char* p) { return p == seen_m0 ? "M0" : p == seen_m1 ? "M1" : p == seen_m2 ? "M2" : "?"; }
static std::string vec(const float* f, int n) { std::string s = "("; for (int i = 0; i < n; ++i) { if (i) s += ","; s += Fs(f[i]); } return s + ")"; }


extern "C" {
void __13VIActorFilter(unsigned char*) { trace += "ctor;"; }
void _$_10VIMatrix44(unsigned char* m, int flags) { dtor_log[dtor_n++] = m; trace += "dtor" + which(m) + "," + S(flags) + ";"; }
unsigned GetActorSpritePtr__C7VIScenei(unsigned char*, int actor) { trace += "sprite" + S(actor) + ";"; return sprite_result; }
void GetActorTransform__7VISceneiR10VIMatrix44(unsigned char*, int actor, unsigned char* m) {
    trace += "xf" + S(actor) + ";"; seen_m0 = m;
    float* f = reinterpret_cast<float*>(m);
    for (int i = 0; i < 16; ++i) f[i] = 100.0f + i;
}
void Invert__10VIMatrix44RC10VIMatrix44(unsigned char* to, unsigned char* from) {
    seen_m1 = to;
    trace += "inv(" + Fs(reinterpret_cast<float*>(from)[0]) + ");";
    float* f = reinterpret_cast<float*>(to);
    for (int i = 0; i < 16; ++i) f[i] = 0.0f;
    // u = x * 1 + y * 0.5 + z * 0.25 + 10; v = x * 2 + y * 1 + z * 0.5 + 20 (byte offsets 0/16/32/48 and 8/24/40/56)
    f[0] = 1.0f; f[4] = 0.5f; f[8] = 0.25f; f[12] = 10.0f;
    f[2] = 2.0f; f[6] = 1.0f; f[10] = 0.5f; f[14] = 20.0f;
}
void CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(unsigned sprite, int node, int actor, unsigned char*, unsigned char* m) {
    trace += "slot" + S(sprite) + "," + S(node) + "," + S(actor) + ";"; seen_m2 = m;
    float* f = reinterpret_cast<float*>(m);
    f[12] = 1.0f + slot_calls; f[13] = 10.0f + slot_calls; f[14] = 20.0f + slot_calls; ++slot_calls;
}
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(unsigned char* m, unsigned char* in, unsigned char* out) {
    const float* v = reinterpret_cast<const float*>(in);
    trace += "mul" + which(m) + vec(v, 3) + ";";
    float* o = reinterpret_cast<float*>(out);
    o[0] = v[0] * 2.0f; o[1] = v[1] * 2.0f; o[2] = v[2] * 2.0f;
}
void Init__6VIBBoxiP7VIVect3(unsigned char* box, int count, unsigned char* points) {
    const float* p = reinterpret_cast<const float*>(points);
    trace += "box" + S(count) + vec(p, 3) + vec(p + 12, 3) + vec(p + 15, 3) + ";";
    float* b = reinterpret_cast<float*>(box);
    b[0] = -2.0f; b[1] = -3.0f; b[2] = -4.0f; b[3] = 6.0f; b[4] = 5.0f; b[5] = 8.0f;
}
void Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(unsigned char* box, unsigned char* m, unsigned char* out) {
    trace += "transform" + vec(reinterpret_cast<float*>(box), 6) + which(m) + vec(reinterpret_cast<float*>(out), 6) + ";";
    float* o = reinterpret_cast<float*>(out);
    o[0] = -7.0f; o[2] = -8.0f; o[3] = 9.0f; o[5] = 11.0f;  // y entries are set by the caller afterwards
}
unsigned char* GetActorLocation__C7VIScenei(unsigned char*, int actor) { trace += "loc" + S(actor) + ";"; return arena + LOC; }
void Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f(unsigned char* list, unsigned char* box, unsigned char* dir, float d) {
    trace += "reset" + S(list - arena) + vec(reinterpret_cast<float*>(box), 6) + vec(reinterpret_cast<float*>(dir), 3) + Fs(d) + ";";
}
void QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList(unsigned char*, unsigned char* box, unsigned char* filter, unsigned char* list) {
    int nodes[6]; std::memcpy(nodes, filter + 16, 24);
    trace += "query" + vec(reinterpret_cast<float*>(box), 6) + "[" + S(nodes[0]) + S(nodes[1]) + S(nodes[2]) + S(nodes[3]) + S(nodes[4]) + S(nodes[5]) + "]" + S(list - arena) + ";";
}
void SetMaterial__8VIRasteri(unsigned r, int m) { trace += "mat" + S(r) + "," + S(m) + ";"; }
void SetZBias__8VIRasteri(unsigned r, int b) { trace += "zb" + S(r) + "," + S(b) + ";"; }
void BeginTriFan__8VIRaster(unsigned) { trace += "begin;"; }
void Color__8VIRasterRC10VIColor32F(unsigned, const float* c) { trace += "c" + vec(c, 4); }
void UV__8VIRasterRC7VIVect2(unsigned, const float* uv) { trace += "uv" + vec(uv, 2); }
void Vertex__8VIRasterRC7VIVect3(unsigned, const float* p) { trace += "v" + vec(p, 3) + ";"; }
void EndTriFan__8VIRaster(unsigned) { trace += "end;"; }
}

static void sw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void sf(unsigned off, float f) { std::memcpy(arena + off, &f, 4); }
static void arm(int material, int count) {
    std::memset(arena, 0, sizeof arena);
    trace.clear(); dtor_n = 0; slot_calls = 0; sprite_result = 0x1c900000; seen_m0 = seen_m1 = seen_m2 = nullptr;
    sw(SCENE + 6272, static_cast<unsigned>(material)); sw(SCENE + 19356, 77);
    sw(SCENE + 6128, static_cast<unsigned>(count)); sw(SCENE + 6136, arena_base + TRI);
    sf(LOC + 0, 1.0f); sf(LOC + 4, 0.25f); sf(LOC + 8, 3.0f);
}
static int go(int actor) { return RenderShadow__7VIScenei(arena + SCENE, actor); }

static void put_tri(int i, const float v[9]) { for (int k = 0; k < 9; ++k) sf(TRI + 40 * i + 4 + 4 * k, v[k]); sw(TRI + 40 * i, 0xdeadbeefu); }

int main() {
    // The full path with one triangle.
    arm(7, 1);
    const float tri[9] = {1.0f, 2.0f, 4.0f, 3.0f, 5.0f, 6.0f, -1.0f, 0.5f, 2.0f};
    put_tri(0, tri);
    if (go(3) != 0) return 1;
    unsigned char* const order[3] = {seen_m2, seen_m1, seen_m0};
    if (dtor_n != 3 || dtor_log[0] != order[0] || dtor_log[1] != order[1] || dtor_log[2] != order[2]) return 2;
    std::string want = "ctor;sprite3;xf3;inv(100);";
    float corners[18];
    for (int i = 0; i < 6; ++i) {
        want += "slot" + S(0x1c900000) + "," + S(i == 0 ? 5 : i == 1 ? 6 : i == 2 ? 4 : i == 3 ? 7 : i == 4 ? 0 : 1) + ",3;";
        const float in[3] = {1.0f + i, 10.0f + i, 20.0f + i};
        want += "mulM1" + vec(in, 3) + ";";
        for (int k = 0; k < 3; ++k) corners[3 * i + k] = in[k] * 2.0f;
        if (i >= 4) for (int k = 0; k < 3; ++k) corners[3 * i + k] = corners[3 * i + k] * 0.5f;
    }
    want += "box6" + vec(corners, 3) + vec(corners + 12, 3) + vec(corners + 15, 3) + ";";
    const float box[6] = {-2.0f - bf(0x3e4ccccc), -3.0f - 0.0f, -4.0f - bf(0x3e4ccccc), 6.0f + bf(0x3e4ccccc), 5.0f + 0.0f, 8.0f + bf(0x3e4ccccc)};
    const float zeros[6] = {0, 0, 0, 0, 0, 0};
    want += "transform" + vec(box, 6) + "M0" + vec(zeros, 6) + ";loc3;";
    const float region[6] = {-7.0f, 0.25f - 1.0f, -8.0f, 9.0f, 0.25f + 0.5f, 11.0f};
    const float up[3] = {0.0f, 1.0f, 0.0f};
    want += "reset" + S(SCENE + 6128) + vec(region, 6) + vec(up, 3) + "80;";
    want += "query" + vec(region, 6) + "[564701]" + S(SCENE + 6128) + ";";
    want += "mat77,7;zb77,15;begin;";
    const float inv_dx = 1.0f / (box[3] - box[0]), inv_dz = 1.0f / (box[5] - box[2]);
    for (int k = 0; k < 3; ++k) {
        const float x = tri[3 * k], y = tri[3 * k + 1] + bf(0x3d23d70a), z = tri[3 * k + 2];
        const float u = (((((x * 1.0f) + (y * 0.5f)) + (z * 0.25f)) + 10.0f) - box[0]) * inv_dx;
        const float v = (((((x * 2.0f) + (y * 1.0f)) + (z * 0.5f)) + 20.0f) - box[2]) * inv_dz;
        float a = 0.25f - y;
        if (a < 0.0f) a = 0.0f;
        if (a > 1.0f) a = 1.0f;
        const float color[4] = {1.0f, 1.0f, 1.0f, (1.0f - a) * bf(0x3f266666)};
        const float uv[2] = {u, v};
        const float pos[3] = {x, y, z};
        want += "c" + vec(color, 4) + "uv" + vec(uv, 2) + "v" + vec(pos, 3) + ";";
    }
    want += "end;zb77,0;dtorM2,2;dtorM1,2;dtorM0,2;";
    if (trace != want) { std::printf("%s\n%s\n", trace.c_str(), want.c_str()); return 3; }
    // No material: only the filter, then the destructors in the same order. Nothing else is called.
    arm(-1, 1);
    put_tri(0, tri);
    if (go(3) != -1 || trace != "ctor;dtor?,2;dtor?,2;dtor?,2;") return 4;
    if (dtor_n != 3 || dtor_log[0] != order[0] || dtor_log[1] != order[1] || dtor_log[2] != order[2]) return 5;
    // No sprite: one more call, then the same exit.
    arm(7, 1);
    sprite_result = 0;
    if (go(9) != -1 || trace != "ctor;sprite9;dtor?,2;dtor?,2;dtor?,2;") return 6;
    if (dtor_n != 3 || dtor_log[0] != order[0] || dtor_log[1] != order[1] || dtor_log[2] != order[2]) return 7;
    // No triangles: the bias is set and cleared, nothing is drawn.
    arm(7, 0);
    if (go(3) != 0 || trace.find("begin") != std::string::npos || trace.find("mat77,7;zb77,15;zb77,0;dtorM2,2;") == std::string::npos) return 8;
    // A negative count is also empty. Two triangles draw twice; the count is read from the list each pass.
    arm(7, -2);
    if (go(3) != 0 || trace.find("begin") != std::string::npos) return 9;
    arm(7, 2);
    put_tri(0, tri); put_tri(1, tri);
    if (go(3) != 0) return 10;
    {
        size_t first = trace.find("begin;"), second = trace.find("begin;", first + 1), third = trace.find("begin;", second + 1);
        if (first == std::string::npos || second == std::string::npos || third != std::string::npos) return 11;
    }
    // Alpha clamps: a vertex far below the actor gives alpha 0 colour = 0.65 exactly; far above gives alpha 1 -> 0.
    arm(7, 1);
    {
        const float low[9] = {0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 0.1f, 0.0f};
        put_tri(0, low);
        go(3);
        const float c0[4] = {1.0f, 1.0f, 1.0f, (1.0f - 1.0f) * bf(0x3f266666)};  // a = 0.25 + 50 clamps to 1
        const float c1[4] = {1.0f, 1.0f, 1.0f, (1.0f - 0.0f) * bf(0x3f266666)};  // a = 0.25 - 50 clamps to 0
        if (trace.find("c" + vec(c0, 4)) == std::string::npos || trace.find("c" + vec(c1, 4)) == std::string::npos) return 12;
    }
    return 0;
}
