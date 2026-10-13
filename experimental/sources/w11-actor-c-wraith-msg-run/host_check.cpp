// Host contract for msg_run__6Wraith; not an EE input. Pointer words are offsets from a host base, every callee is
// a recording stub with per-case results, and the virtual slots go through the shared host slot tables. The
// soft-float helpers of the image (fptodp, dpcmp, dpsub, dpmul, dptofp) are stubbed with host doubles.
#define W11_ACTOR_C_HOST 1
#include "msg_run.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
unsigned char* w11_actor_c_host_base;
unsigned skills[64];
float worldDifficulty;
void* world = reinterpret_cast<void*>(0x4000);
unsigned char norrath_w11_actor_c_view_7d7110[80];
static char skeletonText[] = "Skeleton";
char* norrath_w11_actor_c_view_4852b0 = skeletonText;
int norrath_w11_actor_c_view_4852ac;
static std::string calls;
static std::map<std::string, int> zone;
static int ret_every, ret_activation = 1, ret_exit, ret_melee, skill_value;
static unsigned advance_result;
static float found_height;
static int shadow_points = 2;
static float shadow_weight;
static int atan_result;
static unsigned roll_value;
static bool alloc_fails;
static unsigned char blockMem[1200];
static void* slot_plain(void*) { calls += "slot168;"; return nullptr; }
static void* slot_state(void*, int n) { calls += "state(" + std::to_string(n) + ");"; return nullptr; }
static int slot_skill(void*, int a, int b) { calls += "skill(" + std::to_string(a) + "," + std::to_string(b) + ");"; return skill_value; }
W11ActorCHostSlot w11_actor_c_host_slots[] = {slot_plain};
W11ActorCHostSlotInt w11_actor_c_host_slots_int[] = {slot_state};
W11ActorCHostSlotIntPtr w11_actor_c_host_slots_int_ptr[] = {nullptr};
W11ActorCHostSlotIntInt w11_actor_c_host_slots_int_int[] = {slot_skill};
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - w11_actor_c_host_base); }
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void*) { calls += "damage;"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { calls += "every;"; return ret_every; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "activation;"; return ret_activation; }
unsigned creatureAdvanceAnimation__8Creaturef(void*, float) { calls += "advance;"; return advance_result; }
void slideBack__8Creaturef(void*, float a) { calls += "slide(" + std::to_string(a) + ");"; }
int iatan2__Fff(float y, float x) { calls += "atan(" + std::to_string(y) + "," + std::to_string(x) + ");"; return atan_result; }
int enemyInZone__8Creaturefs(void*, float r, short a) { const std::string k = std::to_string(static_cast<int>(r)) + "/" + std::to_string(a); calls += "zone(" + k + ");"; return zone.count(k) ? zone[k] : 0; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void turnToFaceEnemy__8Creature(void*) { calls += "face;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit;"; return ret_exit; }
int produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int d, int e, void* desc) {
    unsigned char* x = static_cast<unsigned char*>(desc);
    std::string ds = desc ? ("desc[" + std::to_string(static_cast<short>(x[36] | x[37] << 8)) + "," + std::to_string(static_cast<short>(x[38] | x[39] << 8)) + "," + std::to_string(static_cast<short>(x[40] | x[41] << 8)) + "," + std::to_string(static_cast<short>(x[42] | x[43] << 8)) + "]") : "none";
    calls += "melee(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + ds + ");"; return ret_melee; }
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return 0.25f; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, const float* p, float r) { char b[100]; std::snprintf(b, sizeof b, "height(%g,%g,%g,%g);", p[0], p[1], p[2], r); calls += b; return found_height; }
unsigned long long fptodp(float v) { double d = v; unsigned long long r; std::memcpy(&r, &d, 8); return r; }
int dpcmp(unsigned long long a, unsigned long long b) { double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); return x < y ? -1 : x > y ? 1 : 0; }
unsigned long long dpsub(unsigned long long a, unsigned long long b) { double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); double z = x - y; unsigned long long r; std::memcpy(&r, &z, 8); return r; }
unsigned long long dpmul(unsigned long long a, unsigned long long b) { double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); double z = x * y; unsigned long long r; std::memcpy(&r, &z, 8); return r; }
float dptofp(unsigned long long v) { double d; std::memcpy(&d, &v, 8); return static_cast<float>(d); }
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void* blockAlloc__Fi(int n) { calls += "alloc" + std::to_string(n) + ";"; return alloc_fails ? nullptr : blockMem; }
void __8SkeletonG6Point3sPPcPci(void* self, const float* p, short heading, char** names, char* name, int value) {
    char b[160]; std::snprintf(b, sizeof b, "skeleton(%g,%g,%g,%d,%s,%s,%d);", p[0], p[1], p[2], heading, names ? "names" : "null", name, value); calls += b; }
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* second, void* value) { calls += "link(" + off(head) + "," + (second == value ? "same" : "diff") + ");"; }
void* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + l + "," + n + ");"; return blockMem + 100; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void* o, float g) { calls += std::string("sound(") + (o == blockMem ? "skeleton" : "other") + "," + std::to_string(g) + ");"; }
int eRand__10GameObject(void*) { calls += "rand;"; return static_cast<int>(roll_value); }
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y, float z, int i, int j, int k, void (*cb)(void*)) {
    char buf[200]; std::snprintf(buf, sizeof buf, "pdef(%d,%d,%d,%d,%d,%d,%d,%d,%g,%g,%g,%d,%d,%d,%d);", a, b, c, d, e, f, g, h, x, y, z, i, j, k, cb != nullptr); calls += buf; }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { calls += "points;"; return shadow_points; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void*, void*, const float* pt, short heading, int index, float* position, float* weight, void* matrix, float scale) {
    calls += "shadow(" + std::to_string(index) + "," + std::to_string(heading) + "," + std::to_string(matrix == nullptr) + ");"; position[0] = 10.0f; position[1] = 20.0f; position[2] = 30.0f; *weight = shadow_weight; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, float* p, float* v, float s) { char b[160]; std::snprintf(b, sizeof b, "particle(%s,%g,%g,%g|%g,%g,%g|%g);", d == norrath_w11_actor_c_view_7d7110 ? "wraith" : "other", p[0], p[1], p[2], v[0], v[1], v[2], s); calls += b; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "delete;"; }
}

static int failures;
static unsigned char* self;
static void put(int offset, unsigned v) { std::memcpy(self + offset, &v, 4); }
static void putf(int offset, float v) { std::memcpy(self + offset, &v, 4); }
static void puth(int offset, unsigned v) { std::memcpy(self + offset, &v, 2); }
static unsigned get(int offset) { unsigned v; std::memcpy(&v, self + offset, 4); return v; }
static float getf(int offset) { float v; std::memcpy(&v, self + offset, 4); return v; }
static unsigned long long get64(int offset) { unsigned long long v; std::memcpy(&v, self + offset, 8); return v; }
static const std::string head = "damage;every;activation;advance;";
static void reset(int state, int fade, unsigned flags12 = 0) {
    static unsigned char base[16384];
    std::memset(base, 0, sizeof base);
    w11_actor_c_host_base = base; self = base + 64;
    put(36, 1024); { const unsigned model = 2048; std::memcpy(base + 1024 + 12, &model, 4); }
    puth(572, state); self[960] = static_cast<unsigned char>(fade); put(236, 1);
    put(72, 5000); { const short adj = 0; for (int o : {168, 184}) { std::memcpy(base + 5000 + o, &adj, 2); const unsigned fn = 0; std::memcpy(base + 5000 + o + 4, &fn, 4); } }
    put(576, 3000); { const unsigned enemy = 3500; std::memcpy(base + 3000 + 12, &enemy, 4); const float p[3] = {100.0f, 200.0f, 300.0f}; std::memcpy(base + 3500 + 16, p, 12); }
    { const float p[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(self + 16, p, 12); }
    put(256, 5); put(808, 10); put(804, 1); put(964, 3); put(968, 0); put(12, flags12); putf(28, 10.0f); putf(176, 60.0f);
    puth(56, 1000); puth(200, 11); puth(202, 22); puth(204, 33);
    zone.clear(); ret_every = 0; ret_activation = 1; ret_exit = 0; ret_melee = 0; skill_value = 0; advance_result = 0; alloc_fails = false;
    found_height = 3.0f; shadow_points = 2; shadow_weight = 0.0f; atan_result = 0; roll_value = 32768; calls.clear();
    worldDifficulty = 2.0f; skills[14] = 6000; { const unsigned t = 6500; std::memcpy(base + 6000 + 96, &t, 4); const short adj = 0; std::memcpy(base + 6500 + 96, &adj, 2); const unsigned fn = 0; std::memcpy(base + 6500 + 100, &fn, 4); }
}
static void run(const char* label, const std::string& want) {
    msg_run__6Wraith(self);
    if (calls != want) { std::fprintf(stderr, "%s:\n got  %s\n want %s\n", label, calls.c_str(), want.c_str()); ++failures; }
}
static void expectFade(const char* label, int want) { if (self[960] != want) { std::fprintf(stderr, "%s: fade %d want %d\n", label, self[960], want); ++failures; } }
int main() {
    norrath_w11_actor_c_view_4852ac = 0;
    const std::string pre = head + "slot168;slide(0.000000);";
    reset(0, 5); ret_every = 1; run("every", "damage;every;");
    reset(0, 5); ret_activation = 0; run("inactive", "damage;every;activation;");
    // header: flags and fade handling
    for (int fade : {0, 20, 21, 98, 99, 127, 128, 255}) {
        reset(2, fade, 1u); put(160, 0x1u); put(164, 0xdeadbeefu);
        const int L = static_cast<signed char>(fade);
        msg_run__6Wraith(self);
        const unsigned long long want = L >= 21 ? (0xdeadbeef00000001ull | 0x54ull) : (0xdeadbeef00000001ull & ~0x54ull);
        if (get64(160) != want) { std::fprintf(stderr, "flags64 for L=%d\n", L); ++failures; }
        if (get(12) != (L < 99 ? 0u : 1u)) { std::fprintf(stderr, "word 12 for L=%d: %u\n", L, get(12)); ++failures; }
        const std::string wantCalls = pre + (L >= 99 ? "atan(198.000000,99.000000);" : "");
        if (calls != wantCalls) { std::fprintf(stderr, "header for L=%d:\n got  %s\n want %s\n", L, calls.c_str(), wantCalls.c_str()); ++failures; }
    }
    reset(2, 5, 1u); msg_run__6Wraith(self); if (get(12) != 0u) ++failures;      // low fade clears bit 0 in states other than 10
    reset(10, 5, 1u); shadow_points = 1; msg_run__6Wraith(self); if (get(12) != 1u) ++failures;   // ... but not in state 10
    reset(2, 120, 0u); put(576, 0); run("no enemy", pre); if (get(12) != 1u) ++failures;       // fade 120: no turn without an enemy
    reset(16, 5); run("state 16", pre); reset(7, 5); run("state 7", pre); reset(4, 5); run("state 4", pre);
    // states 0 and 1
    reset(0, 50); zone["400/32767"] = 1; run("0 zone", pre + "zone(400/32767);state(3);"); expectFade("0 zone", 51);
    reset(0, 100); zone["500/16000"] = 1; run("0 zone b", pre + "atan(198.000000,99.000000);zone(400/32767);zone(500/16000);state(3);"); expectFade("0 cap", 100);
    reset(0, 50); run("0 timer", pre + "zone(400/32767);zone(500/16000);state(1);");
    reset(0, 50); put(256, 30); run("0 wait", pre + "zone(400/32767);zone(500/16000);");
    reset(1, 50); zone["400/32767"] = 1; run("1 zone", pre + "zone(400/32767);state(0);");
    reset(1, 50); run("1 wander", pre + "zone(400/32767);zone(500/16000);wander;"); expectFade("1", 51);
    // state 3
    reset(3, 70); put(576, 0); run("3 none", pre + "state(0);"); expectFade("3 down", 69);
    reset(3, 50); run("3 up", pre + "zone(30/6000);zone(30/32767);cos(1000);sin(1000);height(16,9.5,3,15);state(15);"); expectFade("3 up", 51);
    reset(3, 50); zone["30/6000"] = 1; put(964, 20); run("3 melee", pre + "zone(30/6000);state(6);");
    reset(3, 50); zone["30/6000"] = 1; put(964, 5); run("3 shoot", pre + "zone(30/6000);state(13);");
    reset(3, 60); zone["30/32767"] = 1; run("3 face", pre + "zone(30/6000);zone(30/32767);face;");
    reset(3, 60); found_height = 20.0f;
    run("3 walk", pre + "zone(30/6000);zone(30/32767);cos(1000);sin(1000);height(16,9.5,3,15);face;cos(1000);sin(1000);grid;");
    { float p[3]; std::memcpy(p, self + 16, 12); if (std::fabs(p[0] - 1.5f) > 1e-4 || std::fabs(p[1] - 2.25f) > 1e-4 || std::fabs(p[2] - 3.1666667f) > 1e-4) { std::fprintf(stderr, "3 walk position %g %g %g\n", p[0], p[1], p[2]); ++failures; } }
    reset(3, 60); found_height = 20.0f; put(968, 4);
    run("3 no probe", pre + "zone(30/6000);zone(30/32767);face;cos(1000);sin(1000);grid;");
    // state 5, 14, 9, 8
    reset(5, 50); zone["30/6000"] = 1; run("5 zone", pre + "zone(30/6000);state(13);"); reset(5, 50); zone["30/6000"] = 1; put(964, 20); run("5 zone b", pre + "zone(30/6000);state(6);"); reset(5, 50); run("5 exit no", pre + "zone(30/6000);exit;"); reset(5, 50); ret_exit = 1; run("5 exit", pre + "zone(30/6000);exit;state(3);");
    reset(14, 90); run("14 down", pre + "state(3);"); expectFade("14 down", 88);
    reset(14, 40); put(256, 30); run("14 up", pre); expectFade("14 up", 42);
    reset(9, 50); run("9 go", pre + "state(0);"); reset(9, 50); put(804, 30); run("9 wait", pre);
    reset(8, 50); run("8 go", pre + "state(3);"); reset(8, 50); puth(294, 0x10); run("8 busy", pre);
    // state 6
    reset(6, 50); advance_result = 1; puth(294, 0); run("6 hit", pre + "melee(36.000000,11,22,33,8,none);face;grid;state(3);"); expectFade("6", 48);
    reset(6, 50); advance_result = 0; puth(294, 8); run("6 frame bit", pre + "face;grid;"); reset(6, 50); put(576, 0); run("6 none", pre + "state(3);");
    reset(6, 0); put(576, 0); run("6 fade zero", pre + "state(3);"); expectFade("6 zero", 0);
    // state 13
    reset(13, 50); advance_result = 1; skill_value = 17; ret_melee = 1; putf(560, 3.0f); putf(564, 30.0f);
    run("13 hit", pre + "skill(0,2);melee(36.000000,11,22,33,1028,desc[22,17,17,0]);face;grid;state(3);");
    if (std::fabs(getf(560) - 5.0f) > 1e-4) { std::fprintf(stderr, "13 gauge %g\n", getf(560)); ++failures; }
    reset(13, 50); advance_result = 1; ret_melee = 1; putf(560, 29.0f); putf(564, 30.0f); msg_run__6Wraith(self); if (getf(560) != 30.0f) { std::fprintf(stderr, "13 cap %g\n", getf(560)); ++failures; }
    reset(13, 50); advance_result = 1; ret_melee = 0; putf(560, 3.0f); putf(564, 30.0f); msg_run__6Wraith(self); if (getf(560) != 3.0f) ++failures;
    // state 15
    reset(15, 50); advance_result = 1; found_height = 7.0f;
    run("15 spawn", pre + "cos(1000);sin(1000);height(16,9.5,3,15);alloc1128;skeleton(16,9.5,8,1000,null,Skeleton,0);link(1032,same);lump(fx.lmp,spel_he_heal.anm);sound(skeleton,1.000000);state(3);");
    reset(15, 50); advance_result = 1; alloc_fails = true; run("15 alloc fails", pre + "cos(1000);sin(1000);height(16,9.5,3,15);alloc1128;state(3);");
    reset(15, 50); advance_result = 0; run("15 no spawn", pre + "state(3);");
    // state 10: death particles
    norrath_w11_actor_c_view_4852ac = 0; reset(10, 100); shadow_points = 2; run("10 delete", head + "slide(0.000000);atan(198.000000,99.000000);pdef(10,10,30,10,10,10,30,0,10.62,50.75,-10,2,0,2,0);delete;");
    if (norrath_w11_actor_c_view_4852ac != 1) ++failures;
    for (float w : {0.0f, 1000.0f, 5000.0f, 20000.0f}) {
        reset(10, 50); norrath_w11_actor_c_view_4852ac = 1; shadow_weight = w; shadow_points = 2; roll_value = 32768;
        std::string want = head + "slide(0.000000);points;";
        float accumulated = 0.0f;
        for (int n = 0; n < 3; ++n) {
            want += "rand;shadow(" + std::to_string(32768 % 2) + ",1000,1);";
            accumulated = accumulated + w * w * w;
            while (0.0f < accumulated) {
                accumulated = accumulated - __builtin_bit_cast(float, 0x5196b020u);
                want += "rand;rand;rand;rand;rand;rand;particle(wraith,10,20,30|0,0,0|0);";
            }
        }
        run("10 particles", want); expectFade("10 up", 51);
    }
    if (!failures) std::puts("Wraith msg_run ok");
    return failures != 0;
}
