// Host contract for msg_run__10SeaMonster; not an EE input. Address-holding words are offsets from a host base.
// Every callee appends a token to a log; the expected token string per state is written out independently.
#define W10_ACTOR_M_HOST 1
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
static std::string log_; static void* g_self; static void* g_model;
unsigned w10_actor_m_host_call(unsigned fn, void* o, int, unsigned long a1, unsigned long) {
    if (fn == 0xaa000017u) { log_ += "S" + std::to_string(static_cast<int>(a1)) + ";"; if (o != static_cast<unsigned char*>(g_self) + 16) log_ += "BADOBJ;"; }
    else if (fn == 0xaa000015u) { log_ += "U;"; if (o != static_cast<unsigned char*>(g_self) + 8) log_ += "BADOBJ;"; }
    else log_ += "BADFN;";
    return 0;
}
static int ef_ret, act_ret, adv_ret, exit_ret, roll; static bool zones[3]; static int nzone; static float zone_f[3]; static short zone_s[3];
static int melee_n; static float melee_amt; static int melee_a[4]; static void* melee_d; static std::string music, fmt_seen; static int music_mode, reset_arg, fmt_arg;
extern "C" void soundResetStream__Fi(int a) { log_ += "Q;"; reset_arg = a; }
extern "C" int eRand__10GameObject(void*) { log_ += "R;"; return roll; }
extern "C" int sprintf(char* b, const char* f, ...) { log_ += "P;"; va_list ap; va_start(ap, f); fmt_arg = va_arg(ap, int); va_end(ap); fmt_seen = f; std::snprintf(b, 32, f, fmt_arg); return 0; }
extern "C" void soundStartMusic__FPci(char* n, int m) { log_ += "H;"; music = n; music_mode = m; }
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* s, void* m) { log_ += "N;"; if (s != g_self || m != g_model) log_ += "BAD;"; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void* s, void* m) { log_ += "E;"; if (s != g_self || m != g_model) log_ += "BAD;"; return ef_ret; }
extern "C" int objectActivationCheck__8Creaturei(void*, int r) { log_ += "C" + std::to_string(r) + ";"; return act_ret; }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float a) { log_ += "V" + std::to_string(static_cast<int>(a)) + ";"; return adv_ret; }
extern "C" void slideBack__8Creaturef(void*, float a) { log_ += "B;"; if (a != w10m::bits(0x3e4ccccc)) log_ += "BADSLIDE;"; }
extern "C" int enemyInZone__8Creaturefs(void*, float f, short s) { int i = nzone++; zone_f[i % 3] = f; zone_s[i % 3] = s; log_ += "Z;"; return zones[i % 3]; }
extern "C" void move2__8CreatureG6Point3(void*, Point3 p) { log_ += "G;"; if (p.x != 0.0f || p.y != 0.0f || p.z != 0.0f) log_ += "BADVEC;"; }
extern "C" void smartWander__8Creature(void*) { log_ += "W;"; }
extern "C" void turnToFaceEnemy__8Creature(void*) { log_ += "T;"; }
extern "C" void steerAndRouteToEnemy__8Creature(void*) { log_ += "Y;"; }
extern "C" int bExitStatePersueIdle__8Creature(void*) { log_ += "X;"; return exit_ret; }
extern "C" void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int x, int y, int z, int w, void* d) { log_ += "M;"; melee_amt = a; melee_a[0] = x; melee_a[1] = y; melee_a[2] = z; melee_a[3] = w; melee_d = d; ++melee_n; }
extern "C" void objectRemoveFromRunList__FP10GameObject(void*) { log_ += "D;"; }
#include "msg_run.cpp"
static std::string state_tail(int st, unsigned w576, int h294, int w256, int w804, int w808, int adv, const bool* z, int ex, int& dec) {
    std::string e; dec = 0;
    switch (st) {
    case 0: e += "Z;"; if (z[0]) e += "S3;"; else if (w256 < w808) e += "S1;"; break;
    case 1: e += "G;Z;"; if (z[0]) e += "S3;"; else if (w256 < w808) e += "S0;"; else e += "W;"; break;
    case 3: if (!w576) e += "S0;"; else { e += "Z;"; if (z[0]) e += "S6;"; else { e += "Z;"; if (z[1]) e += "S15;"; else { e += "Z;"; e += z[2] ? "T;" : "Y;"; } } } break;
    case 5: e += "Z;"; if (z[0]) e += "S6;"; else { e += "Z;"; if (z[1]) e += "S15;"; else { e += "X;"; if (ex) e += "S3;"; } } break;
    case 6: case 15: {
        e += "G;"; if (w576 && (adv & 1)) e += "M;";
        int v = w256; if (!(h294 & 8)) v -= 1; dec = v - w256; if (v <= 0) e += "S3;"; break; }
    case 8: if (!(h294 & 0x10)) e += "S0;"; break;
    case 9: if (w804 < w808) e += "S3;"; break;
    case 10: e += "G;"; if (h294 & 0x80) e += "D;"; break;
    case 13: case 14: e += "G;"; if (!(h294 & 8)) e += "S3;"; break;
    default: break;
    }
    return e;
}
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x4000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x2000, VT = 0x2800;
    g_self = arena + O; g_model = arena + 0x3000;
    for (int st : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, -1, 100}) for (unsigned w576 : {0u, 5u}) for (int h294 : {0, 8, 0x10, 0x80, 0x98})
    for (int w256 : {-5, 0, 1, 2}) for (int w808 : {-5, 1, 2}) for (int adv : {0, 1}) for (int zb = 0; zb < 8; ++zb) for (int ex : {0, 1}) for (int pre : {0, 1, 2, 3, 4}) {
        std::memset(arena, 0, sizeof arena);
        unsigned tb = T, vt = VT; std::memcpy(arena + O + 36, &tb, 4); std::memcpy(arena + O + 72, &vt, 4);
        unsigned m = 0x3000; std::memcpy(arena + T + 12, &m, 4);
        short a168 = 8, a184 = 16; unsigned f168 = 0xaa000015u, f184 = 0xaa000017u;
        std::memcpy(arena + VT + 168, &a168, 2); std::memcpy(arena + VT + 172, &f168, 4); std::memcpy(arena + VT + 184, &a184, 2); std::memcpy(arena + VT + 188, &f184, 4);
        short s16 = static_cast<short>(st); std::memcpy(arena + O + 572, &s16, 2);
        unsigned short u294 = static_cast<unsigned short>(h294); std::memcpy(arena + O + 294, &u294, 2);
        std::memcpy(arena + O + 576, &w576, 4); std::memcpy(arena + O + 256, &w256, 4); std::memcpy(arena + O + 808, &w808, 4);
        int w804 = w256; std::memcpy(arena + O + 804, &w804, 4);
        short h200 = 11, h202 = -22, h204 = 33; std::memcpy(arena + O + 200, &h200, 2); std::memcpy(arena + O + 202, &h202, 2); std::memcpy(arena + O + 204, &h204, 2);
        float f28 = 3.0f; std::memcpy(arena + O + 28, &f28, 4);
        // pre: 0 none; 1 music condition true; 2 w236 only; 3 w960 set; 4 bit0 clear
        unsigned w236 = (pre == 1 || pre == 3 || pre == 4) ? 9u : 0u, w960 = pre == 3 ? 4u : 0u; unsigned long long q160 = pre == 4 ? 2ull : 1ull;
        if (pre == 2) q160 = 1ull;
        std::memcpy(arena + O + 236, &w236, 4); std::memcpy(arena + O + 960, &w960, 4); std::memcpy(arena + O + 160, &q160, 8);
        log_.clear(); ef_ret = 0; act_ret = 1; adv_ret = adv; exit_ret = ex; roll = 0x12345679; nzone = 0; for (int i = 0; i < 3; ++i) zones[i] = (zb >> i) & 1;
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        msg_run__10SeaMonster(arena + O);
        std::string e; const bool music_on = w236 && !w960 && (q160 & 1);
        if (music_on) e += "Q;R;P;H;";
        e += "N;E;C600;V1;B;U;";
        if (st < 0 || st > 15) { if (log_ != e) bad = 1; continue; }
        int dec = 0; e += state_tail(st, w576, h294, w256, w804, w808, adv, zones, ex, dec);
        if (log_ != e) { bad = 1; continue; }
        unsigned got256, got960; std::memcpy(&got256, arena + O + 256, 4); std::memcpy(&got960, arena + O + 960, 4);
        if (static_cast<int>(got256) != w256 + dec) bad = 1;
        if (music_on) {
            if (got960 != 1 || music != "mx_boss_generic" + std::to_string((0x12345679 & 3) + 1) + "_l.vag" || music_mode != 259 || reset_arg != 0 || fmt_seen != "mx_boss_generic%d_l.vag" || fmt_arg != 2) bad = 1;
        } else if (got960 != w960) bad = 1;
        if (st == 6 && w576 && (adv & 1) && (melee_amt != 36.0f || melee_a[0] != 11 || melee_a[1] != -22 || melee_a[2] != 33 || melee_a[3] != 8 || melee_d != nullptr)) bad = 1;
        if (st == 15 && w576 && (adv & 1) && melee_amt != 60.0f) bad = 1;
        if (st == 3 && w576 && (zone_f[0] != 23.0f || zone_s[0] != 5000)) bad = 1;
        if (st == 3 && w576 && !zones[0] && (zone_f[1] != 63.0f || zone_s[1] != 5000)) bad = 1;
        if (st == 3 && w576 && !zones[0] && !zones[1] && (zone_f[2] != 63.0f || zone_s[2] != 32767)) bad = 1;
        if ((st == 0 || st == 1) && (zone_f[0] != 400.0f || zone_s[0] != 32767)) bad = 1;
        unsigned char want[sizeof arena]; std::memcpy(want, ref, sizeof arena); std::memcpy(want + O + 256, &got256, 4); std::memcpy(want + O + 960, &got960, 4);
        if (std::memcmp(arena, want, sizeof arena)) bad = 1;
    }
    // early exits: everyFrame nonzero, activation zero
    for (int mode : {0, 1}) {
        std::memset(arena, 0, sizeof arena);
        unsigned tb = T; std::memcpy(arena + O + 36, &tb, 4); unsigned m = 0x3000; std::memcpy(arena + T + 12, &m, 4);
        log_.clear(); ef_ret = mode == 0; act_ret = mode == 1 ? 0 : 1;
        msg_run__10SeaMonster(arena + O);
        if (log_ != (mode == 0 ? std::string("N;E;") : std::string("N;E;C600;"))) bad = 1;
    }
    return bad;
}
