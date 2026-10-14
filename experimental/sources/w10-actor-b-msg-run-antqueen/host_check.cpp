// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

extern "C" unsigned skills[64];
unsigned skills[64];
#include "msg_run.cpp"

static std::string log_text;
static std::deque<int> zone_results, alt_results, exit_results, steer_results, roll_results;
static int every_result, advance_result, alloc_succeeds, skill_amount;
static unsigned char object[1200];
static unsigned char group[64];
static unsigned char vtable[256];
static unsigned char skill_object[128], skill_table[256];
static unsigned char node[32], target[32], enemy_storage[16], wave_storage[16];
struct Zone { float range; int angle; };
static std::vector<Zone> zones;
static float alt_args[2]; static int alt_angle, alt_range;
static std::vector<int> states_entered;
static std::vector<float> floats_seen;
static float melee_amount; static int melee_ints[4]; static void* melee_description;
static float general_floats[2]; static int general_ints[8]; static void* general_description;
static unsigned char description_copy[64];
static float steer_point[3];
static float wave_point[3]; static void* wave_owner; static int wave_angle, wave_argument, alloc_bytes;
static int fade_arg, floor_angle, floor_kind, floor_extra; static float floor_extent, floor_point[3];
static int skill_calls; static int skill_args[2]; static void* skill_this;
static unsigned char* current;

static int take(std::deque<int>& q, int fallback) {
    if (q.empty()) return fallback;
    const int v = q.front(); q.pop_front(); return v;
}
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model) { log_text += self == object && model == group + 12 ? "N" : "N?"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { log_text += "E"; return every_result; }
int objectActivationCheck__8Creaturei(void*, int distance) { log_text += distance == 600 ? "A" : "A?"; return 0; }
int creatureAdvanceAnimation__8Creaturef(void*, float amount) { log_text += amount == 1.0f ? "V" : "V?"; return advance_result; }
void slideBack__8Creaturef(void*, float amount) { log_text += amount == 0.5f ? "S" : "S?"; }
void subTurnAnimation__8AntQueen(void*) { log_text += "t"; }
void addTurnAnimation__8AntQueen(void*) { log_text += "a"; }
int enemyInZone__8Creaturefs(void*, float range, short angle) { log_text += "z"; zones.push_back({range, angle}); return take(zone_results, 0); }
void smartWander__8Creature(void*) { log_text += "w"; }
void* alternateEnemyInZone__8Creatureffss(void*, float a, float b, short angle, short range) {
    log_text += "y"; alt_args[0] = a; alt_args[1] = b; alt_angle = angle; alt_range = range;
    return take(alt_results, 0) ? enemy_storage : nullptr;
}
void steerAndRouteToEnemy__8Creature(void*) { log_text += "s"; }
int bExitStatePersueIdle__8Creature(void*) { log_text += "x"; return take(exit_results, 0); }
void turnToFaceEnemy__8Creature(void*) { log_text += "f"; }
int steerAndRouteToPosition__8CreatureG6Point3(void*, const float* point) { log_text += "p"; std::memcpy(steer_point, point, 12); return take(steer_results, 0); }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float amount, int a, int b, int c, int flags, void* description) {
    log_text += "m"; melee_amount = amount; melee_ints[0] = a; melee_ints[1] = b; melee_ints[2] = c; melee_ints[3] = flags; melee_description = description;
    if (description) std::memcpy(description_copy, description, 64);
}
void produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void*, float a, float b, short angle, short range, int c, int d, int e, int flags, void* description) {
    log_text += "g"; general_floats[0] = a; general_floats[1] = b;
    general_ints[0] = angle; general_ints[1] = range; general_ints[2] = c; general_ints[3] = d; general_ints[4] = e; general_ints[5] = flags; general_description = description;
    std::memcpy(description_copy, description, 64);
}
float icos__Fi(int angle) { log_text += "c"; floats_seen.push_back(static_cast<float>(angle)); return 0.5f; }
float isin__Fi(int angle) { log_text += "i"; floats_seen.push_back(static_cast<float>(angle)); return 0.25f; }
void* blockAlloc__Fi(int bytes) { log_text += "b"; alloc_bytes = bytes; return alloc_succeeds ? wave_storage : nullptr; }
void __7AntWaveP10GameObjectG6Point3si(void* block, void* owner, const float* point, short angle, int argument) {
    log_text += block == wave_storage ? "W" : "W?"; wave_owner = owner; std::memcpy(wave_point, point, 12); wave_angle = angle; wave_argument = argument;
}
void soundFadeStream__Fi(int stream) { log_text += "d"; fade_arg = stream; }
void objectRemoveFromRunList__FP10GameObject(void*) { log_text += "u"; }
void floorAdd__FG6Point3sfii(const float* point, short angle, float extent, int kind, int extra) {
    log_text += "l"; std::memcpy(floor_point, point, 12); floor_angle = angle; floor_extent = extent; floor_kind = kind; floor_extra = extra;
}
unsigned eRand__10GameObject(void*) { log_text += "r"; return static_cast<unsigned>(take(roll_results, 0)); }
}
static void slot168(void* self) { log_text += self == object + 16 ? "U" : "U?"; }
static void slot184(void* self, int state) { log_text += "G" + std::to_string(state) + ";"; states_entered.push_back(state); (void)self; }
static int skill_function(void* self, int a, int b) { ++skill_calls; skill_this = self; skill_args[0] = a; skill_args[1] = b; log_text += "K"; return skill_amount; }

static int failures;
static void expect(bool ok, const char* what, int a = 0, int b = 0) {
    if (!ok) { std::fprintf(stderr, "fail: %s (%d, %d) log=%s\n", what, a, b, log_text.c_str()); ++failures; }
}
static void put32(unsigned off, unsigned v) { std::memcpy(object + off, &v, 4); }
static void put16(unsigned off, unsigned v) { unsigned short s = static_cast<unsigned short>(v); std::memcpy(object + off, &s, 2); }
static void putf(unsigned off, float v) { std::memcpy(object + off, &v, 4); }
static unsigned word(unsigned off) { unsigned v; std::memcpy(&v, object + off, 4); return v; }
static unsigned addr(void* p) { return rtti_c::address_bits(p); }
static void reset(unsigned state) {
    std::memset(object, 0x3c, sizeof object);
    std::memset(group, 0, sizeof group); std::memset(vtable, 0, sizeof vtable);
    std::memset(skill_object, 0, sizeof skill_object); std::memset(skill_table, 0, sizeof skill_table);
    std::memset(node, 0, sizeof node);
    rtti_c::host_note(group); rtti_c::host_note(vtable); rtti_c::host_note(skill_object); rtti_c::host_note(skill_table);
    rtti_c::host_note(node); rtti_c::host_note(reinterpret_cast<void*>(slot168)); rtti_c::host_note(reinterpret_cast<void*>(slot184));
    rtti_c::host_note(reinterpret_cast<void*>(skill_function)); rtti_c::host_note(target);
    put32(36, addr(group));
    put32(12 + 0, 0);
    { const unsigned model = addr(group + 12); std::memcpy(group + 12, &model, 4); rtti_c::host_note(group + 12); }
    put32(72, addr(vtable));
    { const short adj = 16; std::memcpy(vtable + 168, &adj, 2); const unsigned f = addr(reinterpret_cast<void*>(slot168)); std::memcpy(vtable + 172, &f, 4); }
    { const short adj = 0; std::memcpy(vtable + 184, &adj, 2); const unsigned f = addr(reinterpret_cast<void*>(slot184)); std::memcpy(vtable + 188, &f, 4); }
    skills[26] = addr(skill_object);
    { const unsigned t = addr(skill_table); std::memcpy(skill_object + 96, &t, 4); }
    { const short adj = 8; std::memcpy(skill_table + 96, &adj, 2); const unsigned f = addr(reinterpret_cast<void*>(skill_function)); std::memcpy(skill_table + 100, &f, 4); }
    put16(572, state);
    put32(808, 5000); put32(256, 5000); put32(972, 5000); put32(804, 5000); put32(716, 0); put32(964, 0);
    put32(576, 0); object[292] = 0; put16(294, 0); putf(28, 10.0f);
    putf(16, 1.5f); putf(20, -2.5f); putf(24, 3.25f); put16(56, 0x4000);
    put16(200, 0xfff6); put16(202, 25); put16(204, 0x8000);
    log_text.clear(); zone_results.clear(); alt_results.clear(); exit_results.clear(); steer_results.clear(); roll_results.clear();
    every_result = 0; advance_result = 0; alloc_succeeds = 1; skill_amount = 77; zones.clear(); states_entered.clear(); floats_seen.clear();
    skill_calls = 0; current = object;
}
static void with_target() {
    const unsigned n = addr(node);
    put32(576, n);
    const unsigned t = addr(target);
    std::memcpy(node + 12, &t, 4);
}
static const char* prefix = "NEAVSU";  // common start: damage handler, every-frame, activation, advance, slide, entry 168

int main() {
    rtti_c::host_note(enemy_storage); rtti_c::host_note(wave_storage);
    // Common start.
    reset(2); msg_run__8AntQueen(object); expect(log_text == prefix, "common start, idle state");
    reset(2); every_result = 1; msg_run__8AntQueen(object); expect(log_text == "NE", "every-frame result returns early");
    for (int s : {4, 7, 11, 12, 20, 21, 1000}) { reset(static_cast<unsigned>(s)); msg_run__8AntQueen(object); expect(log_text == prefix, "no-op states", s); }
    reset(0xffff); msg_run__8AntQueen(object); expect(log_text == prefix, "negative state is out of range");
    // State 0.
    reset(0); zone_results = {1}; object[292] = 3; msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "tzG3;" && zones[0].range == 400.0f && zones[0].angle == 32767, "0: near enemy, low threat");
    reset(0); zone_results = {1, 1}; object[292] = 4; msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "tzz", "0: high threat falls through both zones (nothing else: +256 equals +808)");
    reset(0); zone_results = {0, 1}; object[292] = 0xff; msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "tzzG3;" && zones[1].range == 500.0f && zones[1].angle == 16000, "0: second zone, negative byte is below 4");
    reset(0); put32(256, 4999); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tzzG1;", "0: timer below the limit goes to 1");
    reset(0); put32(256, 0xffffffff); msg_run__8AntQueen(object); expect(states_entered.size() == 1 && states_entered[0] == 1, "0: signed timer compare");
    // State 1.
    reset(1); zone_results = {1}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "azG0;", "1: first zone");
    reset(1); zone_results = {0, 1}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "azzG0;", "1: second zone");
    reset(1); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "azzw", "1: wander");
    // State 3.
    reset(3); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "aG0;", "3: no target");
    for (int roll : {1, 2, 3, 5}) {
        reset(3); with_target(); roll_results = {roll}; zone_results = {1}; msg_run__8AntQueen(object);
        expect(log_text == std::string(prefix) + "arzG6;" && zones[0].range == 30.0f && zones[0].angle == 6000, "3: roll with zone", roll);
        reset(3); with_target(); roll_results = {roll}; msg_run__8AntQueen(object);
        expect(log_text == std::string(prefix) + "arzs", "3: roll without zone steers (timer equal)", roll);
    }
    reset(3); with_target(); roll_results = {4}; alt_results = {1}; msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "aryG19;" && alt_args[0] == 0.0f && alt_args[1] == 50.0f && alt_angle == static_cast<short>(0x4000 - 32767) && alt_range == 6000, "3: alternate zone");
    reset(3); with_target(); roll_results = {0}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "arys", "3: no alternate enemy steers");
    reset(3); with_target(); roll_results = {1}; put32(972, 4999); object[292] = 3; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "arzsG14;", "3: low timer and low threat goes to 14");
    reset(3); with_target(); roll_results = {1}; put32(972, 4999); object[292] = 4; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "arzs", "3: low timer, high threat stays");
    // State 5.
    reset(5); roll_results = {1}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "rx", "5: no target, exit check");
    reset(5); roll_results = {1}; exit_results = {1}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "rxG3;", "5: exit goes to 3");
    reset(5); with_target(); roll_results = {2}; zone_results = {1}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "rzG6;", "5: zone goes to 6");
    reset(5); with_target(); roll_results = {2}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "rzx", "5: no zone, exit");
    reset(5); with_target(); roll_results = {4}; alt_results = {1}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "ryG19;", "5: alternate goes to 19");
    reset(5); roll_results = {4}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "rx", "5: roll 0 without target skips the alternate test");
    // State 13.
    reset(13); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "aG0;", "13: no target");
    reset(13); with_target(); put32(972, 4999); zone_results = {1}; msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "afzG17;" && zones[0].range == 500.0f && zones[0].angle == 3000, "13: in time, enemy near");
    reset(13); with_target(); put32(972, 4999); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "afz", "13: in time, no enemy");
    reset(13); with_target(); put16(294, 8); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "a", "13: bit 8 blocks steering");
    reset(13); with_target(); put32(272, 0x11111111); put32(276, 0x7fc00003); put32(280, 0x33333333); steer_results = {1}; put32(716, 40);
    msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "ap" && word(972) == 0u, "13: arrival clears the timer");
    { unsigned bits[3]; std::memcpy(bits, steer_point, 12); expect(bits[0] == 0x11111111u && bits[1] == 0x7fc00003u && bits[2] == 0x33333333u, "13: steering point copied"); }
    for (unsigned stuck : {0u, 30u, 31u, 32u, 0xffffffffu}) {
        reset(13); with_target(); put32(716, stuck); msg_run__8AntQueen(object);
        const bool clears = static_cast<int>(stuck) >= 31;
        expect(log_text == std::string(prefix) + "ap" && word(972) == (clears ? 0u : 5000u), "13: stuck counter", static_cast<int>(stuck));
    }
    // State 14.
    reset(14); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "aG0;", "14: no target");
    reset(14); with_target(); put32(972, 4999); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "aG13;", "14: timer low goes to 13");
    reset(14); with_target(); put16(294, 8); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "a", "14: bit 8");
    reset(14); with_target(); zone_results = {1}; roll_results = {4}; msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "azG15;r" && word(972) == 5000u && zones[0].range == 30.0f && zones[0].angle == 6000, "14: near enemy, roll not divisible");
    reset(14); with_target(); zone_results = {1}; roll_results = {6}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "azG15;r" && word(972) == 0u, "14: near enemy, roll divisible by 3");
    reset(14); with_target(); zone_results = {0, 1}; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "azzf" && zones[1].angle == 32767, "14: far enemy turns");
    reset(14); with_target(); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "azzs", "14: nobody steers");
    // 17, 8, 9.
    reset(17); object[292] = 3; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tG3;", "17");
    reset(17); object[292] = 4; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "t", "17: high threat");
    reset(17); put16(294, 8); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "t", "17: bit 8 does stop it");
    reset(8); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tG3;", "8");
    reset(8); put16(294, 0x10); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "t", "8: bit 0x10");
    reset(8); put16(294, 0x08); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tG3;", "8: bit 8 is ignored");
    reset(9); put32(804, 4999); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "G3;", "9");
    reset(9); msg_run__8AntQueen(object); expect(log_text == prefix, "9: equal timers");
    // 6.
    reset(6); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tG3;", "6: no target");
    reset(6); with_target(); advance_result = 1; msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "tmG3;" && melee_amount == 36.0f && melee_ints[0] == -10 && melee_ints[1] == 25 && melee_ints[2] == -32768 && melee_ints[3] == 0x40008 && melee_description == nullptr && word(964) == 1u, "6: melee on animation end");
    reset(6); with_target(); advance_result = 2; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tfG3;", "6: not done, no earlier hit: face");
    reset(6); with_target(); put32(964, 5); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tG3;", "6: already hit");
    reset(6); with_target(); put16(294, 8); advance_result = 3; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tm", "6: bit 8 blocks the exit");
    // 19.
    reset(19); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tG3;", "19: not done");
    reset(19); advance_result = 1; skill_amount = 0x12345; put16(56, 0xffff); msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "tKgG3;", "19: done");
    expect(skill_this == skill_object + 8 && skill_args[0] == 0 && skill_args[1] == 1, "19: skill call");
    expect(general_floats[0] == 0.0f && general_floats[1] == 40.0f && general_ints[0] == 32766 && general_ints[1] == 3000 && general_ints[2] == -10 && general_ints[3] == 25 && general_ints[4] == -32768 && general_ints[5] == 0x40008, "19: damage arguments");
    {
        const unsigned char* d = description_copy;
        auto h = [&](unsigned o) { unsigned short v; std::memcpy(&v, d + o, 2); return v; };
        bool ok = d[32] == 0xff && d[33] == 0xff;
        for (unsigned o : {16u, 18u, 20u, 22u, 24u, 26u, 28u, 30u}) ok = ok && h(o) == 0;
        ok = ok && h(36) == 23 && h(38) == 0x2345 && h(40) == 0x2345 && h(42) == 0 && h(44) == 0xffff && h(52) == 0xffff && h(60) == 0xffff;
        expect(ok, "19: description");
    }
    reset(19); advance_result = 1; put16(294, 8); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tKg", "19: bit 8");
    // 15, 16.
    reset(15); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tG14;", "15: no target");
    reset(16); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tG13;", "16: no target");
    reset(16); with_target(); advance_result = 1; msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "tKmfG13;" && melee_amount == 40.0f && melee_ints[3] == 0x10004 && melee_description != nullptr, "16: melee with a description");
    reset(15); with_target(); advance_result = 0; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tfG14;", "15: face only");
    reset(15); with_target(); put16(294, 8); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tf", "15: bit 8");
    // 18.
    reset(18); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "t", "18: timer equal, no target");
    reset(18); put32(256, 4999); object[292] = 3; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tG3;", "18: no target, timer below");
    reset(18); with_target(); put32(256, 5120); msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "tcib" + "W" && alloc_bytes == 976 && wave_owner == target && wave_angle == 0x4000 && wave_argument == 600, "18: wave");
    expect(wave_point[0] == 1.5f + 0.5f * 20.0f && wave_point[1] == -2.5f + 0.25f * 20.0f && wave_point[2] == 3.25f && floats_seen[0] == 16384.0f, "18: wave position");
    reset(18); with_target(); put32(256, 5120); alloc_succeeds = 0; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tcib", "18: allocation fails");
    reset(18); with_target(); put32(256, 5121); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "t", "18: not exactly 120 ahead");
    reset(18); with_target(); put32(256, 5119); object[292] = 0; msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "t", "18: 119 ahead and the timer is not below");
    // 10.
    reset(10); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "t", "10: nothing");
    reset(10); put16(294, 0x80); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "tdu" && fade_arg == 0, "10: fade and leave");
    reset(10); advance_result = 1; put32(12, 2); msg_run__8AntQueen(object); expect(log_text == std::string(prefix) + "t", "10: flag 2 blocks the decal");
    reset(10); advance_result = 1; put32(12, 0xfffffffd); roll_results = {0x12345, 6}; put16(56, 0x1000); msg_run__8AntQueen(object);
    expect(log_text == std::string(prefix) + "tcirrl" && floor_angle == 0x2345 && floor_extent == 17.0f && floor_kind == 6 && floor_extra == 1, "10: decal");
    expect(floor_point[0] == 1.5f + 0.5f && floor_point[1] == -2.5f + 0.25f && floor_point[2] == 3.25f && floats_seen[0] == 4096.0f, "10: decal position");
    return failures ? 1 : 0;
}
