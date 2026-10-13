// Host contract for msg_run__8Skeleton; not an EE input. Address-holding words are offsets from a host base.
// Callees append tokens to a log; the expected token string is written out independently per state. State 14
// is checked with deterministic stub formulas for the trig and lead routines.
#define W10_ACTOR_M_HOST 1
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
static std::string log_; static unsigned char* g_self; static void* g_model;
struct Call { std::string name; int size; Point3 where; int angle; void* owner; int level; void* skill; };
static std::vector<Call> calls; static int ef_ret, act_ret, adv_ret, exit_ret, roll, crowd_ret, nzone, angle_ret, alloc_fail, lastsize;
static bool zones[4]; static bool flip_968; static unsigned char* g_arena; static int slot176_calls; static void* slot176_p; static unsigned long slot176_a1; static int count_ret;
static float zf[4]; static short zs[4]; static float z2f[2], z2g[2]; static short z2a[2], z2m[2]; static int nz2;
static std::string music_name, sprintf_fmt; static void* sound_header; static float sound_gain; static void* sound_obj; static std::string lump_seen, lump_name;
static Point3 lead_in, lead_out, mw_where, mw_vel; static float lead_range; static int mw_stack[9], mw_regs[3]; static void *mw_model, *mw_tex, *mw_owner;
static float crowd_f[2]; static void* crowd_other[2]; static int crowd_flag[2]; static int ncrowd;
static float melee_amt; static int melee_a[4]; static void* melee_d; static int ang_args[4]; static Point3 iatan_in[2]; static int niatan;
unsigned w10_actor_m_host_call(unsigned fn, void* o, int, unsigned long a1, unsigned long a2) {
    unsigned char* b = static_cast<unsigned char*>(o);
    if (fn == 0xaa000017u) { log_ += "S" + std::to_string(static_cast<int>(a1)) + ";"; if (b != g_self + 16) log_ += "BADOBJ;"; }
    else if (fn == 0xaa000015u) { log_ += "U;"; if (b != g_self + 8) log_ += "BADOBJ;"; }
    else if (fn == 0xaa000016u) { log_ += "K;"; ++slot176_calls; slot176_a1 = a1; slot176_p = reinterpret_cast<void*>(a2); if (b != g_self + 24) log_ += "BADOBJ;"; }
    else if (fn == 0xaa000060u) { log_ += "V;"; return static_cast<unsigned>(count_ret); }
    else log_ += "BADFN;";
    return 0;
}
extern "C" float worldDifficulty; float worldDifficulty;
extern "C" unsigned char skills[256]; unsigned char skills[256];
extern "C" unsigned char _4item$globalItem[32]; unsigned char _4item$globalItem[32];
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* s, void* m) { log_ += "N;"; if (s != g_self || m != g_model) log_ += "BAD;"; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void*) { log_ += "E;"; return ef_ret; }
extern "C" int objectActivationCheck__8Creaturei(void*, int r) { log_ += "C" + std::to_string(r) + ";"; return act_ret; }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float a) { log_ += "A" + std::to_string(static_cast<int>(a)) + ";"; return adv_ret; }
extern "C" void slideBack__8Creaturef(void*, float a) { log_ += "B;"; if (a != w10m::bits(0x3f333333u)) log_ += "BADSLIDE;"; }
extern "C" int enemyInZone__8Creaturefs(void*, float f, short s) { int i = nzone++; if (s == 2000 && flip_968) { unsigned nine = 9; std::memcpy(g_arena + 0x100 + 968, &nine, 4); } zf[i & 3] = f; zs[i & 3] = s; log_ += "Z;"; return zones[i & 3]; }
extern "C" int enemyInZone2__8Creatureffssfi(void*, float a, float b, short c, short d, float e, int f) { int i = nz2++; z2f[i & 1] = a; z2g[i & 1] = b; z2a[i & 1] = c; z2m[i & 1] = d; log_ += "Y;"; if (e != 0.0f || f != 1) log_ += "BADZ2;"; return zones[i & 1]; }
extern "C" int objectIsCrowded__8CreatureP10GameObjectffi(void*, void* other, float a, float b, int flag) { int i = ncrowd++; crowd_other[i & 1] = other; crowd_f[0] = a; crowd_f[1] = b; crowd_flag[i & 1] = flag; log_ += "R;"; return crowd_ret; }
extern "C" void smartWander__8Creature(void*) { log_ += "W;"; }
extern "C" void turnToFaceEnemy__8Creature(void*) { log_ += "T;"; }
extern "C" void steerAndRouteToEnemy__8Creature(void*) { log_ += "Q;"; }
extern "C" int bExitStatePersueIdle__8Creature(void*) { log_ += "X;"; return exit_ret; }
extern "C" void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int x, int y, int z, int w, void* d) { log_ += "M;"; melee_amt = a; melee_a[0] = x; melee_a[1] = y; melee_a[2] = z; melee_a[3] = w; melee_d = d; }
extern "C" void objectRemoveFromRunList__FP10GameObject(void*) { log_ += "D;"; }
extern "C" void findEnemyLeadPosition__8CreaturefP6Point3(void*, float r, Point3* p) { log_ += "L;"; lead_range = r; lead_in = *p; p->x += 1.0f; p->y += 2.0f; p->z += 3.0f; lead_out = *p; }
extern "C" float icos__Fi(int a) { log_ += "c;"; return 0.5f + a * 0.001f; }
extern "C" float isin__Fi(int a) { log_ += "s;"; return 0.25f + a * 0.002f; }
extern "C" int iatan2__Fff(float y, float x) { log_ += "a;"; if (niatan < 2) { iatan_in[niatan].y = y; iatan_in[niatan].x = x; } ++niatan; return angle_ret; }
static std::vector<int> script; static size_t script_i;
extern "C" int eRand__10GameObject(void*) { log_ += "r;"; return script_i < script.size() ? script[script_i++] : roll; }
extern "C" int sprintf(char* b, const char* f, ...) { log_ += "p;"; va_list ap; va_start(ap, f); int n = va_arg(ap, int); va_end(ap); sprintf_fmt = f; std::snprintf(b, 32, f, n); music_name = b; return 0; }
extern "C" void* lumpFindResource__FPcT0(const char* l, const char* n) { log_ += "l;"; lump_seen = l; lump_name = n; return g_self + 0x700; }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float g) { log_ += "m;"; sound_header = h; sound_obj = o; sound_gain = g; }
extern "C" void* blockAlloc__Fi(int size) { log_ += "b" + std::to_string(size) + ";"; lastsize = size; static unsigned char pool[64][8]; static int n; return alloc_fail ? nullptr : pool[n++ & 63]; }
extern "C" void ColdArrowProjectile(void*, Point3* w, int a, void* o, int l, void* s) __asm__("__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill");
extern "C" void ColdArrowProjectile(void*, Point3* w, int a, void* o, int l, void* s) { log_ += "1;"; calls.push_back({"cold", 176, *w, a, o, l, s}); }
extern "C" void ExplodeArrowProjectile(void*, Point3* w, int a, void* o, int l, void* s) __asm__("__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill");
extern "C" void ExplodeArrowProjectile(void*, Point3* w, int a, void* o, int l, void* s) { log_ += "2;"; calls.push_back({"explode", 116, *w, a, o, l, s}); }
extern "C" void FireArrowProjectile(void*, Point3* w, int a, void* o, int l, void* s) __asm__("__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill");
extern "C" void FireArrowProjectile(void*, Point3* w, int a, void* o, int l, void* s) { log_ += "3;"; calls.push_back({"fire", 176, *w, a, o, l, s}); }
extern "C" void MultiFireProjectile(void*, Point3* w, int a, void* o, int l, void* s) __asm__("__19MultiFireProjectileG6Point3sP10GameObjectiP5Skill");
extern "C" void MultiFireProjectile(void*, Point3* w, int a, void* o, int l, void* s) { log_ += "4;"; calls.push_back({"multi", 120, *w, a, o, l, s}); }
extern "C" void MissileWeapon(void*, int zero, void* model, void* tex, Point3* w, Point3* v, int angle, void* creature, void* sa, void* sb, int a, int b, int c, int d, int e, int f, void* desc)
    __asm__("__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description");
extern "C" void MissileWeapon(void*, int zero, void* model, void* tex, Point3* w, Point3* v, int angle, void* creature, void* sa, void* sb, int a, int b, int c, int d, int e, int f, void* desc) {
    log_ += "5;"; mw_where = *w; mw_vel = *v; mw_model = model; mw_tex = tex; mw_owner = creature; mw_regs[0] = zero; mw_regs[1] = angle;
    mw_stack[0] = sa != nullptr; mw_stack[1] = sb != nullptr; mw_stack[2] = a; mw_stack[3] = b; mw_stack[4] = c; mw_stack[5] = d; mw_stack[6] = e; mw_stack[7] = f; mw_stack[8] = desc != nullptr; }
#include "msg_run.cpp"
static bool near_(float a, float b) { return std::fabs(a - b) < 2e-3f * (1.0f + std::fabs(b)); }
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x8000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x2000, VT = 0x2800, B = 0x3000, SKO = 0x4000, VT3 = 0x5000;
    g_self = arena + O; g_model = arena + 0x3800; g_arena = arena;
    for (int st : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, -1}) for (unsigned w576 : {0u, B}) for (int h294 : {0, 8, 0x10})
    for (int w256 : {-5, 1}) for (int w808 : {-5, 2}) for (int adv : {0, 1}) for (int zb = 0; zb < 4; ++zb) for (int cr : {0, 1}) for (int ex : {0, 1}) for (int v968 : {0, 4}) for (int b972 : {0, 7}) for (int flip : {0, 1}) {
        if (flip && st != 5) continue;
        std::memset(arena, 0, sizeof arena);
        unsigned tb = T, vt = VT; std::memcpy(arena + O + 36, &tb, 4); std::memcpy(arena + O + 72, &vt, 4);
        unsigned m = 0x3800; std::memcpy(arena + T + 12, &m, 4);
        short a168 = 8, a184 = 16, a176 = 24; unsigned f168 = 0xaa000015u, f184 = 0xaa000017u, f176 = 0xaa000016u;
        std::memcpy(arena + VT + 168, &a168, 2); std::memcpy(arena + VT + 172, &f168, 4); std::memcpy(arena + VT + 184, &a184, 2); std::memcpy(arena + VT + 188, &f184, 4);
        std::memcpy(arena + VT + 176, &a176, 2); std::memcpy(arena + VT + 180, &f176, 4);
        short s16 = static_cast<short>(st); std::memcpy(arena + O + 572, &s16, 2);
        unsigned short u294 = static_cast<unsigned short>(h294); std::memcpy(arena + O + 294, &u294, 2);
        std::memcpy(arena + O + 576, &w576, 4); std::memcpy(arena + O + 256, &w256, 4); std::memcpy(arena + O + 808, &w808, 4);
        int w804 = w256 + 1; std::memcpy(arena + O + 804, &w804, 4); unsigned u968 = static_cast<unsigned>(v968); std::memcpy(arena + O + 968, &u968, 4);
        arena[O + 972] = static_cast<unsigned char>(b972);
        unsigned wB12 = 0x3900; std::memcpy(arena + B + 12, &wB12, 4);
        short h200 = 11, h202 = -22, h204 = 33; std::memcpy(arena + O + 200, &h200, 2); std::memcpy(arena + O + 202, &h202, 2); std::memcpy(arena + O + 204, &h204, 2);
        float f28 = 3.0f; std::memcpy(arena + O + 28, &f28, 4);
        flip_968 = flip; log_.clear(); script.assign(1, 1000); script_i = 0; ef_ret = 0; act_ret = 1; adv_ret = adv; exit_ret = ex; crowd_ret = cr; roll = 32768; nzone = 0; nz2 = 0; ncrowd = 0; slot176_calls = 0; calls.clear(); niatan = 0;
        for (int i = 0; i < 4; ++i) zones[i] = (zb >> (i % 3)) & 1;
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        msg_run__8Skeleton(arena + O);
        std::string e = "N;E;C600;A1;B;U;";
        if (st < 0 || st > 15) { if (log_ != e) bad = 1; continue; }
        auto tail = [&](std::string& s, int& w, unsigned h, int& dec) { int v = w; if (!(h & 8)) v -= 1; dec = v - w; if (v <= 0) s += "S3;"; };
        int dec = 0; unsigned long long q160 = 0, want160 = 0; std::memcpy(&q160, ref + O + 160, 8); want160 = q160;
        switch (st) {
        case 0: if (b972) { e += "Z;"; if (zones[0]) { e += "S3;"; break; } } if (w576) e += "S3;"; else if (w256 < w808) e += "S1;"; break;
        case 1: if (b972) { e += "Z;"; if (zones[0]) { e += "S3;"; break; } } if (w576) e += "S3;"; else e += "W;"; break;
        case 3:
            if (!w576) { e += "S0;"; break; }
            if (v968 == 4) { e += "Y;"; if (zones[0]) e += "S14;"; else { e += "Y;"; e += zones[1] ? "T;" : "Q;"; } }
            else { e += "Z;"; if (zones[0]) e += "S6;"; else { e += "R;"; if (cr) e += "r;S5;"; else { e += "Z;"; e += zones[1] ? "T;" : "Q;"; } } }
            break;
        case 5:
            if (v968 == 4) {
                e += "Z;"; if (zones[0]) { e += "S14;"; break; }
                if (flip) { e += "Z;"; if (zones[1]) { e += "S6;"; break; } }
            } else { e += "Z;"; if (zones[0]) { e += "S6;"; break; } }
            e += "X;"; if (!ex) break;
            if (w256 < w808) { e += "S3;"; break; }
            if (!w576) break;
            e += "R;"; if (!cr) e += "S3;"; break;
        case 6: {
            if (w576) { if (adv & 1) e += "M;"; if (!(h294 & 8)) want160 |= 0x200ull; else e += "T;"; }
            tail(e, w256, h294, dec); break; }
        case 8: if (!(h294 & 0x10)) e += "S3;"; break;
        case 9: if (w804 < w808) e += "S0;"; break;
        case 10: e += "D;"; break;
        case 15: if (w256 < w808) e += "S3;"; break;
        case 14: if (!w576) { e += "S0;"; tail(e, w256, h294, dec); break; } else continue;  // armed path checked below
        default: break;
        }
        if (st == 14 && w576) continue;
        if (log_ != e) { bad = 1; continue; }
        unsigned got256; std::memcpy(&got256, arena + O + 256, 4); unsigned long long g160; std::memcpy(&g160, arena + O + 160, 8);
        const bool crowd_path = st == 3 && w576 && v968 != 4 && !zones[0] && cr;
        if (static_cast<int>(got256) != (crowd_path ? w808 + 1000 % 240 + 240 : w256 + dec)) bad = 1;
        if (g160 != want160) bad = 1;
        if (st == 3 && w576 && v968 == 4 && (z2a[0] != 0 || z2m[0] != 2000 || z2f[0] != 0.0f || z2g[0] != 400.0f)) bad = 1;
        if (st == 3 && w576 && v968 != 4 && (zf[0] != 23.0f || zs[0] != 6000)) bad = 1;
        if (st == 3 && w576 && v968 != 4 && !zones[0] && (crowd_other[0] != arena + 0x3900 || crowd_f[0] != 300.0f || crowd_f[1] != 9.0f || crowd_flag[0] != 0)) bad = 1;
        if (st == 0 || st == 1) { if (b972 && (zf[0] != 200.0f || zs[0] != 32767)) bad = 1; }
        if (st == 6 && w576 && (adv & 1) && (melee_amt != 36.0f || melee_a[0] != 11 || melee_a[1] != -22 || melee_a[2] != 33 || melee_a[3] != 8 || melee_d != arena + O + 1048)) bad = 1;
        unsigned char want_mem[sizeof arena]; std::memcpy(want_mem, ref, sizeof arena); std::memcpy(want_mem + O + 256, &got256, 4); std::memcpy(want_mem + O + 160, &want160, 8);
        if (flip && st == 5 && v968 == 4) { unsigned nine = 9; std::memcpy(want_mem + O + 968, &nine, 4); }
        if (std::memcmp(arena, want_mem, sizeof arena)) bad = 1;
    }
    // armed state 14: deterministic geometry, one case per projectile kind
    for (int kind : {0, 1, 2, 3, 4, 5}) for (int alloc : {0, 1}) for (int angle_case : {0, 1, 2, 3}) for (int adv : {1, 3, 2}) for (int h294 : {0, 8}) {
        std::memset(arena, 0, sizeof arena);
        unsigned tb = T, vt = VT; std::memcpy(arena + O + 36, &tb, 4); std::memcpy(arena + O + 72, &vt, 4);
        unsigned m = 0x3800; std::memcpy(arena + T + 12, &m, 4);
        short a168 = 8, a184 = 16, a176 = 24; unsigned f168 = 0xaa000015u, f184 = 0xaa000017u, f176 = 0xaa000016u;
        std::memcpy(arena + VT + 168, &a168, 2); std::memcpy(arena + VT + 172, &f168, 4); std::memcpy(arena + VT + 184, &a184, 2); std::memcpy(arena + VT + 188, &f184, 4);
        std::memcpy(arena + VT + 176, &a176, 2); std::memcpy(arena + VT + 180, &f176, 4);
        short s16 = 14; std::memcpy(arena + O + 572, &s16, 2); unsigned w576 = B; std::memcpy(arena + O + 576, &w576, 4);
        unsigned u294 = static_cast<unsigned>(h294); std::memcpy(arena + O + 294, &u294, 2);
        int w256 = 5, w808 = 1; std::memcpy(arena + O + 256, &w256, 4); std::memcpy(arena + O + 808, &w808, 4);
        unsigned k208 = static_cast<unsigned>(kind); std::memcpy(arena + O + 208, &k208, 4);
        unsigned w1124 = 0x77; std::memcpy(arena + O + 1124, &w1124, 4);
        Point3 pos{10.0f, -20.0f, 30.0f}; std::memcpy(arena + O + 16, &pos, 12); float w32 = 8.0f; std::memcpy(arena + O + 32, &w32, 4);
        short yaw = 40; std::memcpy(arena + O + 56, &yaw, 2); short h200 = 11, h202 = -23, h204 = 33; std::memcpy(arena + O + 200, &h200, 2); std::memcpy(arena + O + 202, &h202, 2); std::memcpy(arena + O + 204, &h204, 2);
        worldDifficulty = 2.75f;
        for (unsigned off : {172u, 176u, 184u, 188u}) { unsigned v = SKO + 0x10 * (off / 4); std::memcpy(skills + off, &v, 4); std::memcpy(arena + v + 96, &VT3, 4); }
        unsigned kv = VT3; (void)kv; short a96 = 0; unsigned f96 = 0xaa000060u; std::memcpy(arena + VT3 + 96, &a96, 2); std::memcpy(arena + VT3 + 100, &f96, 4);
        unsigned gm = 0x3a00, gt = 0x3b00; std::memcpy(_4item$globalItem + 8, &gm, 4); std::memcpy(_4item$globalItem + 12, &gt, 4);
        log_.clear(); script.assign({32768, 36864, 28672, 6}); script_i = 0; ef_ret = 0; act_ret = 1; adv_ret = adv; roll = 6; calls.clear(); niatan = 0; alloc_fail = !alloc; count_ret = 3; slot176_calls = 0;
        angle_ret = angle_case == 0 ? 40 + 10 : angle_case == 1 ? 40 + 1000 : angle_case == 2 ? 40 + 1001 : 40 - 5000;
        msg_run__8Skeleton(arena + O);
        const float c = 0.5f + 40 * 0.001f, s = 0.25f + 40 * 0.002f;
        const Point3 P0{10.0f + c * 300.0f, -20.0f + s * 300.0f, 30.0f};
        Point3 P1 = P0; P1.x += 1.0f; P1.y += 2.0f; P1.z += 3.0f;
        std::string e = "N;E;C600;A1;B;U;c;s;L;";
        if (lead_range != 400.0f || std::memcmp(&lead_in, &P0, 12)) bad = 1;
        Point3 Q{10.0f, -20.0f, (30.0f + 8.0f * 0.5f) + 12.0f};
        Point3 D{P1.x - Q.x, P1.y - Q.y, P1.z - Q.z};
        Point3 Pfinal = P1;
        if (adv & 1) {
            const bool redo = angle_case >= 2; e += "a;"; if (redo) { e += "c;s;a;"; Pfinal = P0; }
            if (!(near_(iatan_in[0].y, D.y) && near_(iatan_in[0].x, D.x))) bad = 1;
            if (redo && !(near_(iatan_in[1].y, D.y) && near_(iatan_in[1].x, D.x))) bad = 1;
            e += "r;r;r;r;p;l;m;";
            const char* mark = kind == 2 ? "b116;2;" : kind == 1 ? "b176;1;" : kind == 3 ? "b176;3;" : kind == 4 ? "V;b120;4;b120;4;b120;4;" : "b452;5;";
            std::string add = mark;
            if (!alloc) { if (kind == 4) add = "V;b120;b120;b120;"; else add = std::string(mark).substr(0, std::string(mark).find(';') + 1); }
            e += add;
        }
        if (adv & 2) {}
        if (h294 & 8) { e += "K;"; }
        { int w = w256; (void)w; }
        int dec = (h294 & 8) ? 0 : -1; if (w256 + dec <= 0) e += "S3;";
        if (log_ != e) { bad = 1; continue; }
        unsigned g1124; std::memcpy(&g1124, arena + O + 1124, 4);
        unsigned want1124 = (adv & 1) ? 0u : 0x77u; if (adv & 2) want1124 = 1u;
        if (g1124 != want1124) bad = 1;
        if (h294 & 8) { if (slot176_calls != 1 || slot176_a1 != 0 || slot176_p == nullptr) bad = 1; }
        if (adv & 1) {
            const float len = std::sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
            const Point3 norm{D.x / len * 400.0f, D.y / len * 400.0f, D.z / len * 400.0f};
            const int level = 2;
            for (const Call& cl : calls) {
                if (!near_(cl.where.x, Q.x) || !near_(cl.where.y, Q.y) || !near_(cl.where.z, Q.z) || cl.owner != arena + O || cl.level != level) bad = 1;
            }
            const int angle = angle_ret;
            if (kind == 1 && alloc && (calls.size() != 1 || calls[0].angle != angle || calls[0].skill != arena + (SKO + 0x10 * 44))) bad = 1;
            if (kind == 2 && alloc && (calls.size() != 1 || calls[0].skill != arena + (SKO + 0x10 * 47))) bad = 1;
            if (kind == 3 && alloc && (calls.size() != 1 || calls[0].skill != arena + (SKO + 0x10 * 43))) bad = 1;
            if (kind == 4 && alloc) {
                if (calls.size() != 3) bad = 1;
                else for (int i = 0; i < 3; ++i) if (calls[i].angle != static_cast<short>(angle - 750 + 1000 * i) || calls[i].skill != arena + (SKO + 0x10 * 46)) bad = 1;
            }
            if (kind != 1 && kind != 2 && kind != 3 && kind != 4 && alloc) {
                if (mw_regs[0] != 0 || mw_regs[1] != angle - 16384 || mw_model != arena + 0x3a00 || mw_tex != arena + 0x3b00 || mw_owner != arena + O) bad = 1;
                if (mw_stack[0] || mw_stack[1] || mw_stack[2] != 11 || mw_stack[3] != -11 || mw_stack[4] != 0 || mw_stack[5] != 0 || mw_stack[6] != 33 || mw_stack[7] != 4 || mw_stack[8]) bad = 1;
                if (!near_(mw_where.x, Q.x) || !near_(mw_where.z, Q.z)) bad = 1;
                if (!near_(mw_vel.x, norm.x + 0.0f) || !near_(mw_vel.y, norm.y + 6.0f) || !near_(mw_vel.z, norm.z - 6.0f)) bad = 1;
            }
            if (std::string(sprintf_fmt) != "swsh_arrow%d.anm" || music_name != "swsh_arrow" + std::to_string((6 & 3) + 1) + ".anm" || lump_seen != "fx.lmp" || sound_gain != 1.0f || sound_obj != arena + O) bad = 1;
        }
        (void)Pfinal;
    }
    return bad;
}
