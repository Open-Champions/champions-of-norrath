// Host contract for setState__10SeaMonsteri; not an EE input. Address-holding words are offsets from a host
// base; the animation, roll, routing and treasure routines are recorded stubs.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>
#include <cstdio>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
static int death_calls; static void* death_obj; static unsigned death_fn;
unsigned w10_actor_m_host_call(unsigned fn, void* o, int, unsigned long, unsigned long) { ++death_calls; death_fn = fn; death_obj = o; return 0; }
static std::string order; static int proc_ret, proc_new = -1, proc_seen; static std::vector<unsigned> rolls; static size_t ri;
struct Anim { char kind; void* hdr; float a, b; int flags; };
static std::vector<Anim> anims; static unsigned char* g_obj; static unsigned word256_at_anim[8]; static int nsync, nreset, ndest, dest_range; static void* dest_self;
static unsigned treasure_args_ok; static Point3 tr_pos; static int tr_angle; static float tr_f[6]; static char* tr_text; static void* tr_self;
extern "C" int setStateProcessing__8CreaturePi(void*, int* st) { order += "P;"; proc_seen = *st; if (proc_new >= 0) *st = proc_new; return proc_ret; }
extern "C" int eRand__10GameObject(void*) { order += "R;"; return static_cast<int>(rolls[ri++ % rolls.size()]); }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float r, int f) { order += "T;"; std::memcpy(&word256_at_anim[anims.size()], g_obj + 256, 4); anims.push_back(Anim{'t', h, r, r, f}); if (s != g_obj + 288) order += "BADANIM;"; }
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) { order += "O;"; std::memcpy(&word256_at_anim[anims.size()], g_obj + 256, 4); anims.push_back(Anim{'o', h, a, b, f}); if (s != g_obj + 288) order += "BADANIM;"; }
extern "C" void animGlobalSync__FP14AnimationState(void* s) { order += "G;"; ++nsync; if (s != g_obj + 288) order += "BADANIM;"; }
extern "C" void resetRouting__8Creature(void* s) { order += "Z;"; ++nreset; if (s != g_obj) order += "BADSELF;"; }
extern "C" void getSmartRandomDestination__8Creaturei(Point3* r, void* s, int range) { order += "D;"; ++ndest; dest_range = range; dest_self = s; r->x = 11.0f; r->y = 12.0f; r->z = 13.0f; }
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void* s, Point3 p, int ang, float a, float b, float c, float d, float e, float f, char* t) {
    order += "X;"; tr_self = s; tr_pos = p; tr_angle = ang; tr_f[0] = a; tr_f[1] = b; tr_f[2] = c; tr_f[3] = d; tr_f[4] = e; tr_f[5] = f; tr_text = t; }
#include "set_state.cpp"
static float F(unsigned b) { float f; std::memcpy(&f, &b, 4); return f; }
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x4000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x2000, VT = 0x2800;
    g_obj = arena + O;
    for (int st : {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0x10005}) for (int cur : {6, 9, 0, 5}) for (unsigned f294 : {0u, 8u}) for (int procret : {0, 1}) for (int newst : {-1, 3}) {
        std::memset(arena, 0, sizeof arena);
        unsigned tb = T; std::memcpy(arena + O + 36, &tb, 4);
        for (unsigned off = 0; off < 128; off += 4) { unsigned v = 0x3000 + off; std::memcpy(arena + T + off, &v, 4); }
        unsigned vt = VT; std::memcpy(arena + O + 72, &vt, 4);
        short adj = 8; unsigned fn = 0xfeed0018u; std::memcpy(arena + VT + 192, &adj, 2); std::memcpy(arena + VT + 196, &fn, 4);
        short cs = static_cast<short>(cur); std::memcpy(arena + O + 572, &cs, 2);
        unsigned short h294 = static_cast<unsigned short>(f294); std::memcpy(arena + O + 294, &h294, 2);
        unsigned w808 = 1000; std::memcpy(arena + O + 808, &w808, 4);
        Point3 pos{1.5f, -2.0f, 7.0f}; std::memcpy(arena + O + 16, &pos, 12); short ang = -33; std::memcpy(arena + O + 56, &ang, 2);
        unsigned w256 = 0x5555; std::memcpy(arena + O + 256, &w256, 4);
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        rolls = {0xfffffff5u, 0x12345678u, 7u, 8u}; ri = 0; order.clear(); anims.clear(); nsync = nreset = ndest = death_calls = 0; proc_ret = procret; proc_new = newst;
        setState__10SeaMonsteri(arena + O, st);
        int eff = (procret == 0 && newst >= 0) ? newst : st;
        short stored; std::memcpy(&stored, arena + O + 572, 2);
        if (proc_seen != st) bad = 1;
        if (procret) { if (order != "P;" || stored != cs || std::memcmp(arena, ref, sizeof arena)) bad = 1; continue; }
        // expected word 256 and call order per state
        unsigned r0 = 0xfffffff5u, r1 = 0x12345678u, r2 = 7u;
        unsigned char want[sizeof arena]; std::memcpy(want, ref, sizeof arena);
        std::string eo = "P;"; short want_state = static_cast<short>(eff); unsigned w = w256;
        auto H = [&](unsigned off) { return arena + 0x3000 + off; };
        std::vector<Anim> ea; unsigned w256_seen[8] = {0};
        switch (static_cast<unsigned>(eff)) {
        case 0: w = 1000 + r0 % 120u; eo += "R;T;"; ea.push_back({'t', H(60), F(0x3e999999), F(0x3e999999), 3}); w256_seen[0] = w; break;
        case 1: eo += "Z;D;R;T;G;"; w = 1000 + r0 % 120u + 300u; ea.push_back({'t', H(124), F(0x3e999999), F(0x3e999999), 2051}); w256_seen[0] = w; break;
        case 3: eo += "Z;T;G;"; ea.push_back({'t', H(92), F(0x3dcccccc), F(0x3dcccccc), 2051}); w256_seen[0] = w; break;
        case 5: eo += "T;"; ea.push_back({'t', H(68), F(0x3dcccccc), F(0x3dcccccc), 3}); w256_seen[0] = w; break;
        case 6: eo += "T;R;O;R;"; ea.push_back({'t', H(68), F(0x3dcccccc), F(0x3dcccccc), 3}); ea.push_back({'o', H(44 + 8 * (r0 & 1)), 0.5f, 0.5f, 8}); w = r1 % 12u; w256_seen[0] = w256_seen[1] = w256; break;
        case 8: if ((cur == 6 && (f294 & 8)) || cur == 9) want_state = static_cast<short>(cur); else { eo += "O;"; ea.push_back({'o', H(84), F(0x3dcccccc), F(0x3dcccccc), 16}); w256_seen[0] = w; } break;
        case 9: eo += "T;"; ea.push_back({'t', H(60), F(0x3e999999), F(0x3e999999), 3}); w256_seen[0] = w; break;
        case 10: eo += "X;T;"; ea.push_back({'t', H(76), F(0x3e999999), F(0x3e999999), 6}); w256_seen[0] = w; break;
        case 13: eo += "T;O;"; ea.push_back({'t', H(68), F(0x3dcccccc), F(0x3dcccccc), 3}); ea.push_back({'o', H(108), F(0x3dcccccc), F(0x3dcccccc), 2056}); w256_seen[0] = w256_seen[1] = w; break;
        case 14: eo += "T;O;"; ea.push_back({'t', H(68), F(0x3dcccccc), F(0x3dcccccc), 3}); ea.push_back({'o', H(116), F(0x3dcccccc), F(0x3dcccccc), 2056}); w256_seen[0] = w256_seen[1] = w; break;
        case 15: eo += "T;O;R;"; ea.push_back({'t', H(68), F(0x3dcccccc), F(0x3dcccccc), 3}); ea.push_back({'o', H(36), 0.5f, 0.5f, 8}); w = r0 % 20u; w256_seen[0] = w256_seen[1] = w256; break;
        default: break;
        }
        (void)r2;
        if (order != eo) bad = 1;
        if (anims.size() != ea.size()) { bad = 1; continue; }
        for (size_t i = 0; i < ea.size(); ++i) {
            if (anims[i].kind != ea[i].kind || anims[i].hdr != ea[i].hdr || anims[i].a != ea[i].a || anims[i].b != ea[i].b || anims[i].flags != ea[i].flags) bad = 1;
        }
        if (stored != want_state) bad = 1;
        unsigned got256; std::memcpy(&got256, arena + O + 256, 4);
        if (got256 != w) bad = 1;
        // the +256 store precedes the animation call for states 0 and 1
        if ((eff == 0 || eff == 1) && word256_at_anim[0] != w) bad = 1;
        if (eff == 1) {
            Point3 d; std::memcpy(&d, arena + O + 272, 12);
            if (d.x != 11.0f || d.y != 12.0f || d.z != 13.0f || dest_range != 200 || dest_self != arena + O) bad = 1;
        }
        if (eff == 10) {
            if (tr_self != arena + O || std::memcmp(&tr_pos, &pos, 12) || tr_angle != ang || tr_f[0] != 36.0f || tr_f[1] != 2.0f || tr_f[2] != F(0x3fe66666) || tr_f[3] != F(0x3f199999) ||
                tr_f[4] != 2.0f || tr_f[5] != 0.25f || tr_text != nullptr) bad = 1;
            if (death_calls != 1 || death_fn != 0xfeed0018u || death_obj != arena + O + 8) bad = 1;
        } else if (death_calls) bad = 1;
        if (eff == 1 && (nreset != 1 || ndest != 1 || nsync != 1)) bad = 1;
        // nothing else in the object changed
        std::memcpy(want + O + 256, &w, 4); std::memcpy(want + O + 572, &want_state, 2);
        if (eff == 1) { Point3 d{11.0f, 12.0f, 13.0f}; std::memcpy(want + O + 272, &d, 12); }
        if (std::memcmp(arena, want, sizeof arena)) bad = 1;
    }
    return bad;
}
