// Host contract for msg_run__4Soul; not an EE input. Address-holding words are offsets from a host base.
// Callees append tokens to a log and are deterministic; the expected token string is written out independently
// per state, and the numeric effects of the travelling state are recomputed here.
#define W10_ACTOR_M_HOST 1
#include <cmath>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
static std::string log_; static unsigned char* g_self; static unsigned char* g_arena;
struct Part { Point3 pos, vel; float v; }; struct Shadow { int index; Point3 src; short angle; float scale; };
struct Atk { void* t; void* a; float f, g; int x, y, z, w; void* d; bool b; };
static std::vector<Part> parts; static std::vector<Shadow> shadows; static std::vector<Atk> attacks; static std::vector<Point3> transformed_in;
static int ef_ret, act_ret, adv_ret, dn_ret, far_ret, exit_ret, steer_ret, count_ret, roll; static float height_ret; static int frame; 
static Point3 steer_arg, turn_arg; static int turn_a1; static int nsetstate; static float height_arg; static Point3 height_pt; static void* world_seen;
unsigned w10_actor_m_host_call(unsigned fn, void* o, int, unsigned long a1, unsigned long a2) {
    unsigned char* b = static_cast<unsigned char*>(o);
    if (fn == 0xaa000017u) { log_ += "S" + std::to_string(static_cast<int>(a1)) + ";"; if (b != g_self + 16) log_ += "BADOBJ;"; }
    else if (fn == 0xaa000015u) { log_ += "U;"; if (b != g_self + 8) log_ += "BADOBJ;"; }
    else if (fn == 0xaa000016u) { log_ += "K;"; turn_a1 = static_cast<int>(a1); std::memcpy(&turn_arg, reinterpret_cast<void*>(a2), 12); if (b != g_self + 24) log_ += "BADOBJ;"; }
    else if (fn == 0xaa000010u) { log_ += "g;"; if (b != g_self + 4) log_ += "BADOBJ;"; return 0x3800; }
    else log_ += "BADFN;";
    return 0;
}
extern "C" unsigned char soulParticle[8]; unsigned char soulParticle[8];
extern "C" int frameNm; int frameNm;
extern "C" int nmPlayers; int nmPlayers;
extern "C" unsigned char player[0x20000]; unsigned char player[0x20000];
extern "C" void* world; void* world;
extern "C" int demonNear__4Soul(void*) { log_ += "d;"; return dn_ret; }
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { log_ += "N;"; if (m != g_arena + 0x3800) log_ += "BADMODEL;"; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void* m) { log_ += "E;"; if (m != g_arena + 0x3800) log_ += "BADMODEL;"; return ef_ret; }
extern "C" int objectActivationCheck__8Creaturei(void*, int r) { log_ += "C" + std::to_string(r) + ";"; return act_ret; }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float a) { log_ += "V" + std::to_string(static_cast<int>(a)) + ";"; return adv_ret; }
extern "C" void slideBack__8Creaturef(void*, float a) { log_ += "B" + std::to_string(static_cast<int>(a)) + ";"; }
extern "C" void move2__8CreatureG6Point3(void*, Point3 p) { log_ += "M;"; if (p.x != 0.0f || p.y != 0.0f || p.z != 0.0f) log_ += "BADVEC;"; }
extern "C" int steerAndRouteToPosition__8CreatureG6Point3(void*, Point3 p) { log_ += "T;"; steer_arg = p; return steer_ret; }
extern "C" int bFarFromHome__8Creature(void*) { log_ += "F;"; return far_ret; }
extern "C" int bExitStatePersueIdle__8Creature(void*) { log_ += "X;"; return exit_ret; }
extern "C" int modelGetNmShadowPoints__FP12_modelHeader(void* m) { log_ += "n;"; if (m != g_arena + 0x3800) log_ += "BADMODEL;"; return count_ret; }
extern "C" void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* m, void* an, Point3 src, short ang, int idx, Point3* out, float* ov, void* mat, float sc) {
    log_ += "w;"; shadows.push_back({idx, src, ang, sc}); if (m != g_arena + 0x3800 || an != g_self + 288 || mat != nullptr) log_ += "BADSHADOW;";
    out->x = 1000.0f + idx; out->y = 2000.0f + static_cast<float>(shadows.size()); out->z = 3000.0f; *ov = 9.0f; }
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) { log_ += "p;"; if (d != soulParticle) log_ += "BADDEF;"; parts.push_back({*p, *v, f}); }
extern "C" int eRand__10GameObject(void*) { log_ += "r;"; return roll; }
extern "C" void objectAddToDeleteList__FP10GameObject(void*) { log_ += "A;"; }
extern "C" void objectRemoveFromRunList__FP10GameObject(void*) { log_ += "R;"; }
extern "C" void getSoulAttachMatrix__6MinionP8Matrix34(void* m, void*) { log_ += "a;"; if (m != g_arena + 0x3900) log_ += "BADMINION;"; }
extern "C" void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void*, void*, int i, void*, float s) { log_ += "m;"; if (i != 0 || s != 1.0f) log_ += "BADATT;"; }
extern "C" void matInvert__FP8Matrix34T0(void* a, void* b) { log_ += "i;"; if (a != b) log_ += "BADINV;"; }
extern "C" void matMul__FP8Matrix34N20(void* a, void* b, void* c) { log_ += "u;"; if (a != c || a == b) log_ += "BADMUL;"; }
extern "C" void matTransform__FP6Point3P8Matrix34T0(Point3* o, void*, Point3* i) { log_ += "t;"; if (o != i) log_ += "BADTR;"; transformed_in.push_back(*i); o->x += 0.5f; }
extern "C" void matMakeTransYawScale__FP8Matrix34G6Point3sf(void*, Point3 p, short y, float s) { log_ += "k;"; shadows.push_back({-1, p, y, s}); }
extern "C" void matRot__FP8Matrix34is(void*, int axis, short a) { log_ += "o;"; if (axis != 0 || a != 20384) log_ += "BADROT;"; }
extern "C" float icos__Fi(int) { log_ += "c;"; return 0.5f; }
extern "C" float isin__Fi(int) { log_ += "s;"; return 0.25f; }
extern "C" float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, Point3 p, float f) { log_ += "h;"; world_seen = w; height_pt = p; height_arg = f; return height_ret; }
extern "C" void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* t, void* a, float f, float g, int x, int y, int z, int w, void* d, bool b) { log_ += "G;"; attacks.push_back({t, a, f, g, x, y, z, w, d, b}); }
#include "msg_run.cpp"
static bool near_(float a, float b) { return std::fabs(a - b) < 2e-3f * (1.0f + std::fabs(b)); }
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x8000];
    g_arena = arena; w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x2000, VT = 0x2800, MN = 0x4100, L = 0x4300, TG = 0x4500, EN = 0x4800;
    g_self = arena + O; world = arena + 0x5000;
    auto setup = [&](int st, unsigned w12, int h294, unsigned w960, unsigned w964, int w256, int w808, int b570, int b569, int h56) {
        std::memset(arena, 0, sizeof arena);
        unsigned tb = T, vt = VT; std::memcpy(arena + O + 36, &tb, 4); std::memcpy(arena + O + 72, &vt, 4);
        unsigned m = 0x3800; std::memcpy(arena + T + 28, &m, 4);
        struct { unsigned off; short adj; unsigned fn; } sl[] = {{128, 4, 0xaa000010u}, {168, 8, 0xaa000015u}, {176, 24, 0xaa000016u}, {184, 16, 0xaa000017u}};
        for (auto& s : sl) { std::memcpy(arena + VT + s.off, &s.adj, 2); std::memcpy(arena + VT + s.off + 4, &s.fn, 4); }
        short s16 = static_cast<short>(st); std::memcpy(arena + O + 572, &s16, 2);
        std::memcpy(arena + O + 12, &w12, 4); unsigned short u294 = static_cast<unsigned short>(h294); std::memcpy(arena + O + 294, &u294, 2);
        std::memcpy(arena + O + 960, &w960, 4); std::memcpy(arena + O + 964, &w964, 4);
        std::memcpy(arena + O + 256, &w256, 4); std::memcpy(arena + O + 808, &w808, 4);
        arena[O + 570] = static_cast<unsigned char>(b570); arena[O + 569] = static_cast<unsigned char>(b569);
        short hh = static_cast<short>(h56); std::memcpy(arena + O + 56, &hh, 2);
        Point3 pos{10.0f, 20.0f, 30.0f}; std::memcpy(arena + O + 16, &pos, 12); float f28 = 5.0f, f32 = 2.0f, f224 = 3.0f;
        std::memcpy(arena + O + 28, &f28, 4); std::memcpy(arena + O + 32, &f32, 4); std::memcpy(arena + O + 224, &f224, 4);
        Point3 dest{7.0f, 8.0f, 9.0f}; std::memcpy(arena + O + 272, &dest, 12);
        unsigned wm12 = 0x3900; std::memcpy(arena + MN + 12, &wm12, 4);
        log_.clear(); parts.clear(); shadows.clear(); attacks.clear(); transformed_in.clear();
        ef_ret = 0; act_ret = 1; adv_ret = 1; roll = 40960; frame = 0x35; frameNm = frame; nmPlayers = 0;
    };
    // ---- simple states
    for (int st : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 18, 20, 21, 22, 23, 25, -1}) for (int tickv : {0, 1}) for (int dn : {0, 1}) for (int far : {0, 1}) for (int steer : {0, 1})
    for (int w576 : {0, 5}) for (int h294 : {0, 0x10}) for (int w256 : {-5, 3}) for (int w808 : {-5, 2}) for (int w960 : {0, 1}) {
        setup(st, 0, h294, w960 ? MN : 0, 0, w256, w808, 0, tickv ? 0x35 : 0x36, 0);
        std::memcpy(arena + O + 576, &w576, 4);
        dn_ret = dn; far_ret = far; steer_ret = steer; exit_ret = far;
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        msg_run__4Soul(arena + O);
        std::string e = "g;N;g;E;C600;V1;B0;U;";
        if (!w960 && st != 17) e += "M;";
        auto less = w256 < w808; (void)less;
        if (st >= 0 && st <= 24) switch (st) {
        case 0: if (tickv) { e += "d;"; if (dn) { e += "S14;"; break; } } if (w256 < w808) e += "S1;"; else { e += "F;"; if (far) e += "S1;"; } break;
        case 1: if (tickv) { e += "d;"; if (dn) { e += "S14;"; break; } } e += "T;"; if (steer || w256 < w808) e += "S0;"; break;
        case 3: if (!w576) e += "S0;"; break;
        case 5: e += "X;"; if (exit_ret) e += "S3;"; break;
        case 8: case 15: if (!(h294 & 0x10)) e += "S14;"; break;
        case 13: if (w256 < w808) e += "S1;"; break;
        case 14: if (tickv) { e += "d;"; if (!dn) e += "S0;"; } break;
        default: break;
        }
        if (log_ != e) { bad = 1; continue; }
        if (st == 1 && (steer_arg.x != 7.0f || steer_arg.y != 8.0f || steer_arg.z != 9.0f)) bad = 1;
        if (std::memcmp(arena, ref, sizeof arena)) bad = 1;
    }
    // early exits
    for (int mode : {0, 1}) {
        setup(0, 0, 0, 0, 0, 0, 0, 0, 0, 0); ef_ret = mode == 0; act_ret = mode == 1 ? 0 : 1;
        msg_run__4Soul(arena + O);
        if (log_ != (mode == 0 ? std::string("g;N;g;E;") : std::string("g;N;g;E;C600;"))) bad = 1;
    }
    // ---- states 10 and 24: shadow bursts
    for (int st : {10, 24}) for (unsigned w12 : {0u, 2u, 8u}) for (int count : {0, 1, 3}) for (int cond : {0, 1, 2}) for (int b570 : {0, 1}) {
        const int w256 = cond == 1 ? 1 : 5, w808 = 2, h294 = cond == 2 ? 0x80 : 0;
        setup(st, w12, h294, MN, 0, w256, w808, b570, 0x35, 40); count_ret = count;
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        msg_run__4Soul(arena + O);
        std::string e = "g;N;g;E;C600;V1;B0;U;";
        if (!(w12 & 2)) {
            e += "g;n;";  // note: model_of is fetched before n
            if (count != 0) for (int i = 0; i < 3; ++i) { e += "g;r;w;r;r;r;"; if (!b570) e += "p;"; }
            if (cond) {
                if (count > 0) for (int i = 0; i < count; ++i) { e += "g;w;"; for (int s = 0; s < 2; ++s) { e += "r;r;r;"; if (!b570) e += "p;"; } }
                e += st == 10 ? "A;" : "R;";
            }
        }
        // the 'g;' before 'n;' is the getModel call feeding modelGetNmShadowPoints; remove the duplicate
        if (log_ != e) { bad = 1; continue; }
        unsigned g12; std::memcpy(&g12, arena + O + 12, 4);
        unsigned want12 = w12;
        if (!(w12 & 2) && cond) want12 = st == 10 ? (w12 | 2u) : (w12 | 0xbu);
        if (g12 != want12) bad = 1;
        if (!(w12 & 2)) {
            size_t pi = 0;
            if (count != 0 && !b570) for (int i = 0; i < 3 && pi < parts.size(); ++i, ++pi) {
                const Part& p = parts[pi];
                if (!near_(p.vel.x, 0.25f * 15.0f) || !near_(p.vel.y, 0.25f * 15.0f) || p.vel.z != 0.0f || p.v != 0.0f) bad = 1;
                if (shadows[i].index != 40960 % count) bad = 1;
                if (shadows[i].angle != 40 || shadows[i].scale != 3.0f) bad = 1;
            }
        }
        unsigned char want[sizeof arena]; std::memcpy(want, ref, sizeof arena); std::memcpy(want + O + 12, &g12, 4);
        if (std::memcmp(arena, want, sizeof arena)) bad = 1;
    }
    // ---- state 19
    for (int adv : {0, 1}) for (int count : {0, 1, 2}) for (int minion : {0, 1}) {
        setup(19, 0, 0, minion ? MN : 0, 0, 0, 0, 0, 0x35, 0); adv_ret = adv; count_ret = count;
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        msg_run__4Soul(arena + O);
        std::string e = "g;N;g;E;C600;V1;B0;U;"; if (!minion) e += "M;";
        if (adv) {
            e += "g;n;";
            for (int i = 0; i < count; ++i) { e += "g;w;"; if (minion) e += "a;m;i;u;t;"; e += "r;r;r;p;r;r;r;p;"; }
            e += "A;";
        }
        if (log_ != e) { bad = 1; continue; }
        unsigned g12; std::memcpy(&g12, arena + O + 12, 4);
        if (g12 != (adv ? 2u : 0u)) bad = 1;
        if (adv) for (int i = 0; i < count; ++i) for (int s = 0; s < 2; ++s) {
            const Part& p = parts[2 * i + s];
            if (!near_(p.vel.x, 3.75f) || !near_(p.vel.y, 3.75f) || p.vel.z != 0.0f) bad = 1;
            if (!near_(p.pos.x, 1000.0f + i + (minion ? 0.5f : 0.0f)) || !near_(p.pos.z, 3000.0f)) bad = 1;
            if (shadows[i].angle != -16384 || shadows[i].src.x != 0.0f) bad = 1;
        }
        unsigned char want[sizeof arena]; std::memcpy(want, ref, sizeof arena); std::memcpy(want + O + 12, &g12, 4);
        if (std::memcmp(arena, want, sizeof arena)) bad = 1;
    }
    // ---- state 17: travelling
    for (int linked : {0, 1}) for (int count : {0, 2}) for (int nplayers : {0, 2}) for (int near_player : {0, 1}) for (int w256 : {5, 0}) for (float ground : {-100.0f, 500.0f}) {
        setup(17, 0, 0, 0, linked ? L : 0, w256, 2, 0, 0x35, 100); count_ret = count; height_ret = ground; nmPlayers = nplayers;
        unsigned lw12 = TG; std::memcpy(arena + L + 12, &lw12, 4);
        Point3 tpos{50.0f, 60.0f, 70.0f}; std::memcpy(arena + TG + 16, &tpos, 12); float f816 = 10.0f, f820 = 20.0f; std::memcpy(arena + TG + 816, &f816, 4); std::memcpy(arena + TG + 820, &f820, 4);
        for (int i = 0; i < nplayers; ++i) {
            unsigned ent = EN + 0x100 * i; std::memcpy(player + 23984 * i + 21872, &ent, 4);
            Point3 ep = near_player ? Point3{12.0f, 20.0f, 30.0f} : Point3{5000.0f, 0.0f, 0.0f}; std::memcpy(arena + ent + 16, &ep, 12);
            float e28 = 20.0f, e32 = 4.0f; std::memcpy(arena + ent + 28, &e28, 4); std::memcpy(arena + ent + 32, &e32, 4);
        }
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        msg_run__4Soul(arena + O);
        std::string e = "g;N;g;E;C600;V1;B0;U;";
        e += "g;n;k;o;";
        if (count != 0) for (int i = 0; i < 3; ++i) e += "g;r;w;t;r;r;r;p;";
        if (linked) e += "K;c;s;"; else e += "c;s;";
        // sub-steps and players
        Point3 push = linked ? Point3{0.5f * 400.0f, 0.25f * 400.0f, 0.0f} : Point3{200.0f, 100.0f, -80.0f};
        Point3 got; std::memcpy(&got, arena + O + 16, 12);
        Point3 p{10.0f, 20.0f, 30.0f}; bool hit = false;
        if (linked) {
            const float dx = 50.0f - 10.0f, dy = 60.0f - 20.0f, dz = 70.0f - 30.0f; const float len = std::sqrt(dx * dx + dy * dy + dz * dz);
            const float z = dz / len * 400.0f; push.z = z > -60.0f ? -60.0f : z;
        }
        for (int sub = 0; sub < 4; ++sub) {
            p.x += push.x * (1.0f / 240.0f); p.y += push.y * (1.0f / 240.0f); p.z += push.z * (1.0f / 240.0f);
            for (int i = 0; i < nplayers; ++i) {
                e += "c;s;";
                const Point3 ep = near_player ? Point3{12.0f, 20.0f, 30.0f + 2.0f} : Point3{5000.0f, 0.0f, 0.0f};
                const Point3 probe{p.x + 0.5f * 36.0f, p.y + 0.25f * 36.0f, p.z};
                const float d = std::sqrt((probe.x - ep.x) * (probe.x - ep.x) + (probe.y - ep.y) * (probe.y - ep.y) + (probe.z - ep.z) * (probe.z - ep.z));
                if (d <= 25.0f) { e += "G;"; hit = true; break; }
            }
        }
        const bool less = w256 < 2; bool despawn = hit || less;
        if (!despawn) { e += "h;"; if (p.z < ground) despawn = true; }
        if (despawn) {
            e += "A;";
            for (int i = 0; i < count; ++i) e += "g;w;t;r;r;r;p;r;r;r;p;";
        }
        if (log_ != e) { bad = 1; continue; }
        if (!near_(got.x, p.x) || !near_(got.y, p.y) || !near_(got.z, p.z)) bad = 1;
        if (shadows.empty() || shadows[0].index != -1 || shadows[0].angle != 100 + 24384 || shadows[0].scale != 3.0f || shadows[0].src.x != 10.0f || shadows[0].src.y != 20.0f || shadows[0].src.z != 30.0f) bad = 1;
        if (log_.find("h;") != std::string::npos && (height_arg != 6.0f || world_seen != world || !near_(height_pt.x, got.x) || !near_(height_pt.z, got.z))) bad = 1;
        if (linked) {
            if (!near_(turn_arg.x, 50.0f + 10.0f * 0.2f) || !near_(turn_arg.y, 60.0f + 20.0f * 0.2f) || turn_arg.z != 70.0f || turn_a1 != 0) bad = 1;
            float f168; std::memcpy(&f168, arena + O + 168, 4); if (!near_(f168, 0.4f)) bad = 1;
        }
        for (const Atk& a : attacks) if (a.f != 10.0f || a.g != 50.0f || a.x != 100000 || a.y != 0 || a.z != 0 || a.w != 0x04000140 || a.d != nullptr || a.b || a.a != arena + O) bad = 1;
        
        unsigned char want[sizeof arena]; std::memcpy(want, ref, sizeof arena); std::memcpy(want + O + 16, &got, 12);
        if (linked) { unsigned f168 = 0x3ecccccc; std::memcpy(want + O + 168, &f168, 4); }
        if (std::memcmp(arena, want, sizeof arena)) bad = 1;
    }
    return bad;
}
