// Host contract for msg_run__14WoodElfSoldier; not an EE input. Pointer words are offsets from a host base, every
// callee is a recording stub whose results are set per case, and the three virtual slots go through the shared host
// slot tables. Each case names the state, the variant V (word +964) and the stub results and lists the calls the
// run must make, following the table in the comment of the source.
#define W11_ACTOR_C_HOST 1
#include "msg_run.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
unsigned char* w11_actor_c_host_base;
int syncRandState;
unsigned char globalItem[64];
static std::string calls;
static std::map<std::string, int> zone;          // "range/angle" -> result
static int ret_every, ret_activation = 1, ret_cv, ret_edge, ret_far, ret_steer, ret_wander, ret_exit;
static unsigned advance_result;
static int lead_yaw;
static unsigned roll;
static bool alloc_fails;
static float missile_vel[3], missile_pos[3]; static int missile_angle;
static void* slot_plain(void* o) { calls += "slot168(" + std::to_string(static_cast<unsigned char*>(o) - w11_actor_c_host_base) + ");"; return nullptr; }
static void* slot_state(void* o, int n) { calls += "state(" + std::to_string(n) + ");"; return nullptr; }
static void* slot_point(void* o, int zero, const void* p) { const float* f = static_cast<const float*>(p); char b[100]; std::snprintf(b, sizeof b, "point(%d,%g,%g,%g);", zero, f[0], f[1], f[2]); calls += b; return nullptr; }
W11ActorCHostSlot w11_actor_c_host_slots[] = {slot_plain};
W11ActorCHostSlotInt w11_actor_c_host_slots_int[] = {slot_state};
W11ActorCHostSlotIntPtr w11_actor_c_host_slots_int_ptr[] = {slot_point};
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - w11_actor_c_host_base); }
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* h) { calls += "damage(" + off(h) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { calls += "every;"; return ret_every; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "activation(" + std::to_string(r) + ");"; return ret_activation; }
unsigned creatureAdvanceAnimation__8Creaturef(void*, float a) { calls += "advance(" + std::to_string(a) + ");"; return advance_result; }
void slideBack__8Creaturef(void*, float a) { calls += "slide(" + std::to_string(a) + ");"; }
void moveArrow__14WoodElfSoldier(void*) { calls += "arrow;"; }
int commanderValid__14WoodElfSoldier(void*) { calls += "valid;"; return ret_cv; }
void joinCommander__14WoodElfSoldier(void*) { calls += "join;"; }
int bNextToEdge__14WoodElfSoldier(void*) { calls += "edge;"; return ret_edge; }
void TurnToFaceEdge__14WoodElfSoldier(void*) { calls += "faceedge;"; }
int enemyInZone__8Creaturefs(void*, float r, short a) { const std::string k = std::to_string(static_cast<int>(r)) + "/" + std::to_string(a); calls += "zone(" + k + ");"; return zone.count(k) ? zone[k] : 0; }
int bFarFromHome__8Creature(void*) { calls += "far;"; return ret_far; }
int steerAndRouteToPosition__8CreatureG6Point3(void*, const float* p) { char b[100]; std::snprintf(b, sizeof b, "steerpos(%g,%g,%g);", p[0], p[1], p[2]); calls += b; return ret_steer; }
int smartWander__8Creature(void*) { calls += "wander;"; return ret_wander; }
void flee2__8CreatureP10GameObject(void* s, void* o) { calls += std::string("flee(") + (s == o ? "self" : "other") + ");"; }
void turnToFaceEnemy__8Creature(void*) { calls += "face;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steerenemy;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit;"; return ret_exit; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int d, int e, void* desc) {
    calls += "melee(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + (desc ? "desc" : "none") + ");"; }
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int f) { calls += "chunks(" + off(m) + "," + std::to_string(f) + ");"; return 0x1122334455667788ull; }
int creatureFindLeadYaw__8CreatureP10GameObjectf(void*, void* o, float r) { calls += "lead(" + off(o) + "," + std::to_string(r) + ");"; return lead_yaw; }
int eRand__10GameObject(void*) { calls += "rand;"; return static_cast<int>(roll); }
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return 0.25f; }
int sprintf(char* d, const char* f, ...) { __builtin_va_list ap; __builtin_va_start(ap, f); int v = __builtin_va_arg(ap, int); __builtin_va_end(ap);
    std::string s = f; s.replace(s.find("%d"), 2, std::to_string(v)); std::memcpy(d, s.c_str(), s.size() + 1); return static_cast<int>(s.size()); }
void* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + l + "," + n + ");"; return globalItem + 40; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float g) { calls += "sound(" + std::to_string(g) + ");"; }
void* blockAlloc__Fi(int n) { calls += "alloc" + std::to_string(n) + ";"; return alloc_fails ? nullptr : globalItem + 48; }
void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void*, int, void*, void*, const float* p, const float* v, int angle, void*, void*, void*, int a, int b, int c, int d, int e, int f, void*) {
    std::memcpy(missile_pos, p, 12); std::memcpy(missile_vel, v, 12); missile_angle = angle; calls += "missile(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(e) + "," + std::to_string(f) + ");"; }
void objectAddToSlowRunList__FP10GameObject(void*) { calls += "slowrun;"; }
void floorAdd__FG6Point3sfii(const float* p, short a, float b, int c, int d) { char s[120]; std::snprintf(s, sizeof s, "floor(%g,%g,%g,%d,%g,%d,%d);", p[0], p[1], p[2], a, b, c, d); calls += s; }
}

static int failures;
static unsigned char* self;
static void put(int offset, unsigned v) { std::memcpy(self + offset, &v, 4); }
static void putf(int offset, float v) { std::memcpy(self + offset, &v, 4); }
static void puth(int offset, unsigned v) { std::memcpy(self + offset, &v, 2); }
static unsigned get(int offset) { unsigned v; std::memcpy(&v, self + offset, 4); return v; }
static std::string head() { return "damage(2048);every;activation(600);advance(1.000000);slide(0.200000);slot168(64);arrow;"; }
static void reset(int state, unsigned v) {
    static unsigned char base[16384];
    std::memset(base, 0, sizeof base);
    w11_actor_c_host_base = base; self = base + 64;
    put(36, 1024); std::memset(base + 1024, 0, 0);
    { const unsigned model = 2048; std::memcpy(base + 1024 + 12, &model, 4); }
    puth(572, state); put(964, v); put(236, 1);
    put(72, 5000); { const short adj = 0; for (int o : {168, 176, 184}) { std::memcpy(base + 5000 + o, &adj, 2); const unsigned fn = 0; std::memcpy(base + 5000 + o + 4, &fn, 4); } }
    put(576, 3000); { const unsigned enemy = 3500; std::memcpy(base + 3000 + 12, &enemy, 4); const float p[3] = {100.0f, 200.0f, 300.0f}; std::memcpy(base + 3500 + 16, p, 12); const float h = 10.0f; std::memcpy(base + 3500 + 32, &h, 4); }
    { const float p[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(self + 16, p, 12); const float q[3] = {7.0f, 8.0f, 9.0f}; std::memcpy(self + 272, q, 12); }
    put(256, 5); put(808, 10); put(804, 1); put(244, 0x400); put(12, 0);
    puth(56, 1000); puth(200, 11); puth(202, 22); puth(204, 33);
    zone.clear(); ret_every = 0; ret_activation = 1; ret_cv = ret_edge = ret_far = ret_steer = ret_wander = ret_exit = 0; advance_result = 0; alloc_fails = false;
    roll = 7; lead_yaw = 0; calls.clear();
    const unsigned item8 = 7000, item12 = 7100; std::memcpy(globalItem + 8, &item8, 4); std::memcpy(globalItem + 12, &item12, 4);
    syncRandState = 12345;
}
static void run(const char* label, const std::string& want) {
    msg_run__14WoodElfSoldier(self);
    if (calls != want) { std::fprintf(stderr, "%s:\n got  %s\n want %s\n", label, calls.c_str(), want.c_str()); ++failures; }
}
int main() {
    // early exits
    reset(0, 0); ret_every = 1; run("every", "damage(2048);every;");
    reset(0, 0); put(236, 0); run("inactive", "damage(2048);every;activation(600);");
    reset(16, 0); run("state 16", head()); reset(4, 0); run("state 4", head()); reset(11, 0); run("state 11", head()); reset(12, 0); run("state 12", head());
    // state 0
    reset(0, 0); putf(856, 1.0f); run("0 fear", head() + "state(7);");
    reset(0, 2); run("0 join", head() + "valid;join;zone(400/32767);zone(500/16000);far;state(1);");
    reset(0, 2); ret_cv = 1; run("0 no join", head() + "valid;zone(400/32767);zone(500/16000);far;state(1);");
    reset(0, 0); zone["400/32767"] = 1; run("0 zone a", head() + "zone(400/32767);state(3);");
    reset(0, 0); zone["500/16000"] = 1; run("0 zone b", head() + "zone(400/32767);zone(500/16000);state(3);");
    reset(0, 0); ret_far = 1; run("0 far", head() + "zone(400/32767);zone(500/16000);far;state(1);");
    reset(0, 0); put(256, 20); run("0 wait", head() + "zone(400/32767);zone(500/16000);far;");
    reset(0, 1); ret_edge = 1; run("0 edge", head() + "zone(400/32767);zone(500/16000);far;edge;state(14);");
    reset(0, 1); run("0 no edge", head() + "zone(400/32767);zone(500/16000);far;edge;state(1);");
    reset(0, 3); run("0 other", head() + "zone(400/32767);zone(500/16000);far;state(1);");
    // state 1
    reset(1, 0); putf(856, 0.5f); run("1 fear", head() + "state(7);");
    reset(1, 0); zone["500/16000"] = 1; run("1 zone", head() + "zone(400/32767);zone(500/16000);state(0);");
    reset(1, 0); ret_steer = 1; run("1 arrived", head() + "zone(400/32767);zone(500/16000);steerpos(7,8,9);state(0);");
    reset(1, 0); put(256, 20); run("1 keep", head() + "zone(400/32767);zone(500/16000);steerpos(7,8,9);");
    reset(1, 0); run("1 timer", head() + "zone(400/32767);zone(500/16000);steerpos(7,8,9);state(0);");
    // state 2, 7
    reset(2, 0); zone["400/32767"] = 1; run("2 zone", head() + "zone(400/32767);state(0);");
    reset(2, 0); ret_wander = 1; run("2 wander", head() + "zone(400/32767);zone(500/16000);wander;");
    reset(2, 0); run("2 stop", head() + "zone(400/32767);zone(500/16000);wander;state(0);");
    reset(7, 0); put(256, 30); run("7 flee", head() + "flee(self);"); reset(7, 0); put(256, 5); run("7 end", head() + "state(0);");
    // state 3
    reset(3, 0); put(576, 0); run("3 no enemy", head() + "state(0);");
    reset(3, 0); putf(856, 3.0f); run("3 fear", head() + "state(7);");
    reset(3, 3); zone["60/3000"] = 1; run("3 V3 melee", head() + "zone(60/3000);state(6);");
    reset(3, 3); zone["400/6000"] = 1; run("3 V3 shoot", head() + "zone(60/3000);zone(400/6000);state(15);");
    reset(3, 1); run("3 V1 steer", head() + "zone(400/6000);steerenemy;");
    reset(3, 0); zone["60/3000"] = 1; run("3 V0 melee", head() + "zone(60/3000);state(6);");
    reset(3, 0); zone["60/32767"] = 1; run("3 V0 face", head() + "zone(60/3000);zone(60/32767);face;");
    reset(3, 0); run("3 V0 steer", head() + "zone(60/3000);zone(60/32767);steerenemy;");
    reset(3, 2); zone["70/2000"] = 1; run("3 V2 melee", head() + "valid;zone(70/2000);state(6);");
    reset(3, 2); run("3 V2 steer", head() + "valid;zone(70/2000);zone(70/32767);steerenemy;");
    reset(3, 2); ret_cv = 1; ret_steer = 1;
    { unsigned char* base = w11_actor_c_host_base; const unsigned commander = 4000, leader = 4500; std::memcpy(self + 960, &commander, 4); std::memcpy(base + 4000 + 12, &leader, 4);
      const float lp[3] = {100.0f, 200.0f, 200.0f}; std::memcpy(base + 4500 + 16, lp, 12);   // enemy at (100,200,300): direction (0,0,1)
      run("3 V2 commander", head() + "valid;steerpos(100,200,250);state(13);"); }
    // state 5
    reset(5, 3); zone["60/3000"] = 1; run("5 V3", head() + "zone(60/3000);state(6);");
    reset(5, 1); zone["400/6000"] = 1; run("5 V1", head() + "zone(400/6000);state(15);");
    reset(5, 2); zone["70/2000"] = 1; run("5 V2", head() + "zone(70/2000);state(6);");
    reset(5, 0); zone["60/3000"] = 1; run("5 V0", head() + "zone(60/3000);state(6);");
    reset(5, 0); ret_exit = 1; run("5 exit", head() + "zone(60/3000);exit;state(3);");
    reset(5, 0); run("5 stay", head() + "zone(60/3000);exit;");
    // state 13
    reset(13, 0); put(576, 0); run("13 none", head() + "state(0);");
    reset(13, 0); run("13 wait", head() + "face;state(3);");
    reset(13, 0); put(256, 99); run("13 busy", head() + "face;");
    // state 6
    reset(6, 0); advance_result = 1; puth(294, 0); put(256, 3); run("6 hit", head() + "melee(60.000000,11,22,33,262152,none);");
    { std::string h = head(); reset(6, 0); put(256, 1); run("6 end", h + "state(3);"); if (get(256) != 0) ++failures; }
    reset(6, 0); advance_result = 0; puth(294, 8); put(256, 3); run("6 face", head() + "face;");
    if (get(256) != 3) { std::fprintf(stderr, "6: counter changed during frame bit\n"); ++failures; }
    reset(6, 0); put(576, 0); put(256, 3); run("6 none", head()); if (get(256) != 2) ++failures;
    // state 9, 8
    reset(9, 0); put(804, 1); put(808, 10); run("9 go", head() + "state(0);"); reset(9, 0); put(804, 10); put(808, 1); run("9 wait", head());
    reset(8, 0); puth(294, 0x10); run("8 busy", head()); reset(8, 0); puth(294, 0); run("8 done", head() + "state(3);");
    // state 14
    reset(14, 1); advance_result = 1; puth(294, 8); run("14 R1", head() + "chunks(2048,0);rand;lump(fx.lmp,swsh_arrow4.anm);sound(1.000000);faceedge;");
    if (get(988) != 1 || get(244) != 0 || get(256) != 5) { std::fprintf(stderr, "14 fields %u %u\n", get(988), get(244)); ++failures; }
    { float v[3]; std::memcpy(v, self + 972, 12); if (v[0] != 1.0f || v[1] != 2.0f || v[2] != 61.0f) ++failures; unsigned short h; std::memcpy(&h, self + 984, 2); if (h != 1000) ++failures; }
    reset(14, 1); advance_result = 2; puth(294, 0); put(244, 0); put(256, 1); run("14 R2", head() + "chunks(2048,1024);state(0);");
    // state 10
    reset(10, 0); puth(294, 0x80); advance_result = 0x10; run("10 slow", head() + "slowrun;cos(1000);sin(1000);rand;rand;floor(-9,-3,3,7,27,0,1);");
    // state 15
    reset(15, 0); advance_result = 2; puth(294, 8); put(244, 0); put(256, 3); run("15 R2", head() + "chunks(2048,1024);point(0,100,200,300);");
    reset(15, 0); advance_result = 1; puth(294, 0); lead_yaw = 5000; roll = 900; alloc_fails = false; put(256, 9);
    msg_run__14WoodElfSoldier(self);
    {
        // the lead yaw 5000 is clamped to the current heading 1000 + 1000; the jitter 900 % 1800 - 900 leaves 2000; the name roll 900 & 3 = 0 gives arrow 1
        const std::string want = head() + "chunks(2048,0);lead(3500,400.000000);rand;cos(2000);sin(2000);rand;lump(fx.lmp,swsh_arrow1.anm);sound(1.000000);alloc452;missile(11,11,33,4);";
        if (calls != want) { std::fprintf(stderr, "15 R1:\n got  %s\n want %s\n", calls.c_str(), want.c_str()); ++failures; }
        if (missile_angle != 2000 - 16384 || missile_vel[0] != 200.0f || missile_vel[1] != 100.0f) { std::fprintf(stderr, "15 missile angle %d velocity (%g,%g,%g)\n", missile_angle, missile_vel[0], missile_vel[1], missile_vel[2]); ++failures; }
        if (missile_pos[0] != 1.0f || missile_pos[1] != 2.0f || missile_pos[2] != 61.0f) ++failures;
    }
    if (!failures) std::puts("WoodElfSoldier msg_run ok");
    return failures != 0;
}
