// Host contract for msg_run__19ParticleMorphEffect; not an EE input. Words that hold addresses are
// offsets from a host base; table calls and every callee are stubs that count or log. For frame
// distances in each phase the contract checks which calls ran and how often, the colour
// halfwords written, the byte flags, the seed update and the particle arguments, and that the
// finished case calls the table entry at +8 with 3.
#define W10_ACTOR_K_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_k_host_base;
extern "C" {
int frameNm;
unsigned char morpheffectParticle1[8], morpheffectParticle2[8];
}
#include "msg_run.cpp"
alignas(16) static unsigned char arena[0x8000];
static std::string calls;
static int count_p1, count_p2, count_pos, count_vcall, count_np, grid_u, grid_r;
static int counts_to_return[2] = {3, 5};
static unsigned entry8_arg;
static Point3 last_vel;
unsigned w10_actor_k_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    if (fn == 0x100) { ++count_vcall; return static_cast<unsigned>(static_cast<unsigned char*>(adjusted) - arena); }
    if (fn == 0x200) { entry8_arg = static_cast<unsigned>(a1); calls += "entry8;"; (void)n; return 0; }
    return 0;
}
extern "C" int modelGetNmShadowPoints__FP12_modelHeader(void* h) { ++count_np; return counts_to_return[(static_cast<unsigned char*>(h) - arena) == 0x2000 ? 0 : 1]; }
extern "C" void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* h, void* an, Point3 p, short ang, int idx, Point3* out, float* f, void* m, float s) {
    (void)an; (void)p; (void)ang; (void)f; (void)m; (void)s;
    ++count_pos;
    const int which = (static_cast<unsigned char*>(h) - arena) == 0x2000 ? 0 : 1;
    if (idx < 0 || idx >= counts_to_return[which]) calls += "badindex;";
    out->x = static_cast<float>(idx); out->y = which ? 10.0f : 0.0f; out->z = 1.0f;
}
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) {
    (void)p;
    if (d == morpheffectParticle1) ++count_p1; else if (d == morpheffectParticle2) ++count_p2; else calls += "baddef;";
    if (f != 0.0f) calls += "badf;";
    last_vel = *v;
    if (d == morpheffectParticle1 && v->y != 10.0f * bits(0x3e89b9f9u)) calls += "badvel1;";
    if (d == morpheffectParticle2 && !(v->x * v->x + v->y * v->y + v->z * v->z <= 16.0f * 16.0f + 0.01f)) calls += "badvel2;";
}
extern "C" void objectUpdateInGrid__FP10GameObject(void*) { ++grid_u; }
extern "C" void objectRemoveFromGrid__FP10GameObject(void*) { ++grid_r; }
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static short h16(unsigned at) { short v; std::memcpy(&v, arena + at, 2); return v; }
int main() {
    w10_actor_k_host_base = arena;
    int bad = 0;
    const unsigned self = 0x100, A = 0x2000, B = 0x4000, vt = 0x1000, avt = 0x1800;
    struct Case { int el; int end_gap; int p1, p2, u, r, set_a, set_b; const char* tag; };
    // set_a: A's colours changed by phase one/two; set_b: B's colours changed by phase two/three.
    const Case cases[] = {
        {10, 500, 20, 0, 0, 0, 1, 0, "phase one only"},
        {60, 500, 20, 40, 1, 0, 1, 1, "phases one and two"},
        {101, 500, 0, 0, 0, 1, 0, 1, "phase three only"},
        {80, -1, 0, 0, 0, 0, 0, 0, "finished"},
    };
    for (const Case& c : cases) {
        std::memset(arena, 0, sizeof arena);
        calls.clear(); count_p1 = count_p2 = count_pos = count_vcall = count_np = grid_u = grid_r = 0; entry8_arg = 0;
        frameNm = 1000;
        put32(self + 72, vt); put32(self + 84, A); put32(self + 88, B); put32(self + 80, 1000 - c.el);
        put32(self + 76, c.end_gap < 0 ? 900 : 1000 + c.end_gap);
        put32(self + 68, 0x2468ace0u);
        put32(A + 72, avt); put32(B + 72, avt);
        put32(vt + 8, 0x200);           // adjustment 0 in the low halfword
        put32(vt + 12, 0x200);          // function word at +4 of the entry
        put32(avt + 128 + 4, 0x100);    // function word of the entry at +128
        // Entry layout is {halfword adjustment, padding, function}: set the function word at +4.
        put32(vt + 8 + 4, 0x200);
        put32(avt + 128, 0);
        put32(avt + 128 + 4, 0x100);
        msg_run__19ParticleMorphEffect(arena + self);
        if (c.end_gap < 0) {
            if (calls != "entry8;" || entry8_arg != 3 || count_p1 || count_p2) { std::printf("%s: finished path wrong\n", c.tag); bad = 1; }
            continue;
        }
        if (count_p1 != c.p1 || count_p2 != c.p2 || grid_u != c.u || grid_r != c.r) { std::printf("%s: counts p1 %d p2 %d u %d r %d\n", c.tag, count_p1, count_p2, grid_u, grid_r); bad = 1; }
        if (!calls.empty()) { std::printf("%s: %s\n", c.tag, calls.c_str()); bad = 1; }
        if (c.set_a && (h16(A + 140) != (c.p2 ? 220 : static_cast<short>(static_cast<float>(c.el) * bits(0x3ca3d70bu) * 220.0f)))) { std::printf("%s: A red %d\n", c.tag, h16(A + 140)); bad = 1; }
        if (c.p2 && (h16(B + 140) != 220 || h16(B + 146) != 250 || h16(B + 142) != 20 || h16(A + 154) != 250)) { std::printf("%s: final colours\n", c.tag); bad = 1; }
        if (c.r && (arena[A + 570] & 8) == 0) { std::printf("%s: A flag\n", c.tag); bad = 1; }
        if (c.u && (arena[B + 570] & 8) != 0) { std::printf("%s: B flag\n", c.tag); bad = 1; }
        unsigned seed; std::memcpy(&seed, arena + self + 68, 4);
        if (seed == 0x2468ace0u && (c.p1 || c.p2)) { std::printf("%s: seed not stepped\n", c.tag); bad = 1; }
    }
    return bad;
}
