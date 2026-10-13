// Host contract for msg_run__8Skelight; not an EE input. Address-holding words are offsets from a host base.
// Every callee appends a token to a log; the expected token string per state is written out independently.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
static std::string log_; static void* g_self;
unsigned w10_actor_m_host_call(unsigned fn, void* o, int, unsigned long a1, unsigned long) {
    if (fn == 0xaa000017u ? o != static_cast<unsigned char*>(g_self) + 16 : o != static_cast<unsigned char*>(g_self) + 8) log_ += "BADOBJ;";
    if (fn == 0xaa000017u) log_ += "S" + std::to_string(static_cast<int>(a1)) + ";";
    else if (fn == 0xaa000015u) log_ += "U;";
    else log_ += "BADFN;";
    return 0;
}
static int ef_ret, adv_ret, far_ret, exit_ret; static int zone_ret[2]; static int nzone; static float zone_f[2]; static short zone_s[2];
static float adv_arg, melee_amt; static int act_arg; static int melee_args[4]; static void* melee_desc; static Point3 eff_pos; static int eff_kind; static void* g_model;
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* s, void* m) { log_ += "N;"; if (s != g_self || m != g_model) log_ += "BAD;"; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void* s, void* m) { log_ += "E;"; if (s != g_self || m != g_model) log_ += "BAD;"; return ef_ret; }
extern "C" void objectActivationCheck__8Creaturei(void*, int r) { log_ += "C;"; act_arg = r; }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float a) { log_ += "V;"; adv_arg = a; return adv_ret; }
extern "C" void slideBack__8Creaturef(void*, float a) { log_ += "B;"; if (a != w10m::bits(0x3f333333u)) log_ += "BADSLIDE;"; }
extern "C" int bFarFromHome__8Creature(void*) { log_ += "F;"; return far_ret; }
extern "C" void smartWander__8Creature(void*) { log_ += "W;"; }
extern "C" int enemyInZone__8Creaturefs(void*, float f, short s) { zone_f[nzone % 2] = f; zone_s[nzone % 2] = s; log_ += "Z;"; return zone_ret[nzone++ % 2]; }
extern "C" void turnToFaceEnemy__8Creature(void*) { log_ += "T;"; }
extern "C" void steerAndRouteToEnemy__8Creature(void*) { log_ += "R;"; }
extern "C" int bExitStatePersueIdle__8Creature(void*) { log_ += "X;"; return exit_ret; }
extern "C" void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int x, int y, int z, int w, void* d) { log_ += "M;"; melee_amt = a; melee_args[0] = x; melee_args[1] = y; melee_args[2] = z; melee_args[3] = w; melee_desc = d; }
extern "C" void addEffect__FiG6Point3(int k, Point3 p) { log_ += "A;"; eff_kind = k; eff_pos = p; }
#include "msg_run.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x4000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x2000, VT = 0x2800;
    g_self = arena + O; g_model = arena + 0x3000;
    for (int st : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, -1, 100}) for (unsigned w576 : {0u, 5u}) for (int h294 : {0, 8, 0x80, 0x88})
    for (int w256 : {-5, 0, 1, 2}) for (int w808 : {-5, 1, 2}) for (int adv : {0, 1, 2}) for (int ef : {0, 1}) for (int far : {0, 1}) for (int z1 : {0, 1}) for (int z2 : {0, 1}) for (int ex : {0, 1}) {
        std::memset(arena, 0, sizeof arena);
        unsigned tb = T, vt = VT; std::memcpy(arena + O + 36, &tb, 4); std::memcpy(arena + O + 72, &vt, 4);
        unsigned m = 0x3000; std::memcpy(arena + T + 12, &m, 4);
        short a168 = 8, a184 = 16; unsigned f168 = 0xaa000015u, f184 = 0xaa000017u;
        std::memcpy(arena + VT + 168, &a168, 2); std::memcpy(arena + VT + 172, &f168, 4); std::memcpy(arena + VT + 184, &a184, 2); std::memcpy(arena + VT + 188, &f184, 4);
        short s16 = static_cast<short>(st); std::memcpy(arena + O + 572, &s16, 2);
        unsigned short u294 = static_cast<unsigned short>(h294); std::memcpy(arena + O + 294, &u294, 2);
        std::memcpy(arena + O + 576, &w576, 4); std::memcpy(arena + O + 256, &w256, 4); std::memcpy(arena + O + 808, &w808, 4);
        short h200 = 11, h202 = -22, h204 = 33; std::memcpy(arena + O + 200, &h200, 2); std::memcpy(arena + O + 202, &h202, 2); std::memcpy(arena + O + 204, &h204, 2);
        Point3 pos{1.5f, -2.0f, 7.0f}; std::memcpy(arena + O + 16, &pos, 12); float f28 = 3.0f; std::memcpy(arena + O + 28, &f28, 4);
        log_.clear(); ef_ret = ef; adv_ret = adv; far_ret = far; exit_ret = ex; zone_ret[0] = z1; zone_ret[1] = z2; nzone = 0;
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        msg_run__8Skelight(arena + O);
        std::string e;
        if (st != 13) e += "N;";
        e += "E;";
        if (ef) { if (log_ != e) bad = 1; continue; }
        e += "C;V;B;U;";
        int dec = 0;  // expected change to word 256
        switch (st) {
        case 0: if (w576) e += "S3;"; else { e += "F;"; if (far) e += "S1;"; else if (w256 < w808) e += "S1;"; } break;
        case 1: if (w256 < w808) e += "S0;"; else e += "W;"; break;
        case 3: if (!w576) e += "S0;"; else { e += "Z;"; if (z1) e += "S6;"; else { e += "Z;"; e += z2 ? "T;" : "R;"; } } break;
        case 5: e += "Z;"; if (z1) e += "S6;"; else { e += "X;"; if (ex) e += "S3;"; } break;
        case 6: {
            int v = w256;
            if (w576) { if (adv & 1) e += "M;"; if (!(h294 & 8)) v -= 1; else e += "T;"; }
            if (!(w576 && !(h294 & 8)) && !(h294 & 8)) v -= 1;
            dec = v - w256; if (v <= 0) e += "S3;"; break;
        }
        case 8: e += "S3;"; break;
        case 13: if (h294 & 0x80) e += "S0;"; e += "A;"; break;
        default: break;
        }
        if (log_ != e) { bad = 1; continue; }
        int got256; std::memcpy(&got256, arena + O + 256, 4);
        if (got256 != w256 + dec) bad = 1;
        if (adv_arg != 1.0f || act_arg != 600) bad = 1;
        if (st == 3 && w576) { if (zone_f[0] != 53.0f || zone_s[0] != 6000) bad = 1; if (!z1 && (zone_f[1] != 33.0f || zone_s[1] != 32767)) bad = 1; }
        if (st == 5 && (zone_f[0] != 53.0f || zone_s[0] != 6000)) bad = 1;
        if (st == 6 && w576 && (adv & 1) && (melee_amt != 36.0f || melee_args[0] != 11 || melee_args[1] != -22 || melee_args[2] != 33 || melee_args[3] != 8 || melee_desc != arena + O + 1032)) bad = 1;
        if (st == 13 && (eff_kind != 7 || std::memcmp(&eff_pos, &pos, 12))) bad = 1;
        unsigned char want[sizeof arena]; std::memcpy(want, ref, sizeof arena); std::memcpy(want + O + 256, &got256, 4);
        if (std::memcmp(arena, want, sizeof arena)) bad = 1;
    }
    return bad;
}
