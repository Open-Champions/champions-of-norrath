// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

#include "msg_run.cpp"

struct Query { char kind; float a, b; int angle, range; };
static std::vector<Query> queries;
static std::string log_text;
static std::deque<int> hits;  // results for zone2 / alt queries in order
static int enemy1_result, every_result, advance_result, exit_result;
static unsigned rolls[4]; static int roll_index;
static unsigned char object[1200], group[64], vtable[256];
static std::vector<int> states_entered;
static float melee_amount; static int melee_ints[4]; static int melee_count;
static float general_floats[2]; static int general_ints[8]; static int general_count;
static float floor_point[3]; static int floor_angle, floor_kind, floor_extra; static float floor_extent;
static int fade_arg;
static unsigned char* current;

extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model) { log_text += self == object && model == group + 12 ? "N" : "N?"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { log_text += "E"; return every_result; }
int objectActivationCheck__8Creaturei(void*, int distance) { log_text += distance == 600 ? "A" : "A?"; return 0; }
int creatureAdvanceAnimation__8Creaturef(void*, float amount) { log_text += amount == 1.0f ? "V" : "V?"; return advance_result; }
void slideBack__8Creaturef(void*, float amount) { log_text += amount == 0.0f ? "S" : "S?"; }
void move2__8CreatureG6Point3(void*, const float* point) { log_text += point[0] == 0.0f && point[1] == 0.0f && point[2] == 0.0f ? "M" : "M?"; }
int enemyInZone__8Creaturefs(void*, float range, short angle) { log_text += "z"; queries.push_back({'1', range, 0.0f, angle, 0}); return enemy1_result; }
int enemyInZone2__8Creatureffssfi(void*, float a, float b, short angle, short range, float c, int d) {
    log_text += (c == rtti_c::single_from_bits(0x3f199999u) && d == 0) ? "Z" : "Z?";
    queries.push_back({'z', a, b, angle, range});
    if (hits.empty()) return 0;
    const int v = hits.front(); hits.pop_front(); return v;
}
void* alternateEnemyInZone__8Creatureffss(void*, float a, float b, short angle, short range) {
    log_text += "Y"; queries.push_back({'a', a, b, angle, range});
    if (hits.empty()) return nullptr;
    const int v = hits.front(); hits.pop_front(); return v ? object : nullptr;
}
void smartWander__8Creature(void*) { log_text += "w"; }
void steerAndRouteToEnemy__8Creature(void*) { log_text += "s"; }
void turnToFaceEnemy__8Creature(void*) { log_text += "f"; }
int bExitStatePersueIdle__8Creature(void*) { log_text += "x"; return exit_result; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float amount, int a, int b, int c, int flags, void* description) {
    log_text += "m"; melee_amount = amount; melee_ints[0] = a; melee_ints[1] = b; melee_ints[2] = c; melee_ints[3] = flags; ++melee_count;
    if (description != nullptr) log_text += "!";
}
void produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void*, float a, float b, short angle, short range, int c, int d, int e, int flags, void* description) {
    log_text += "g"; general_floats[0] = a; general_floats[1] = b; general_ints[0] = angle; general_ints[1] = range; general_ints[2] = c;
    general_ints[3] = d; general_ints[4] = e; general_ints[5] = flags; ++general_count;
    if (description != nullptr) log_text += "!";
}
void soundFadeStream__Fi(int stream) { log_text += "d"; fade_arg = stream; }
void objectRemoveFromRunList__FP10GameObject(void*) { log_text += "u"; }
void floorAdd__FG6Point3sfii(const float* point, short angle, float extent, int kind, int extra) {
    log_text += "l"; std::memcpy(floor_point, point, 12); floor_angle = angle; floor_extent = extent; floor_kind = kind; floor_extra = extra;
}
unsigned eRand__10GameObject(void*) { log_text += "r"; return rolls[roll_index++ & 3]; }
}
static void slot168(void* self) { log_text += self == object + 16 ? "U" : "U?"; }
static void slot184(void*, int state) { log_text += "G" + std::to_string(state) + ";"; states_entered.push_back(state); }

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
    rtti_c::host_note(group); rtti_c::host_note(vtable);
    rtti_c::host_note(reinterpret_cast<void*>(slot168)); rtti_c::host_note(reinterpret_cast<void*>(slot184));
    put32(36, addr(group));
    { const unsigned model = addr(group + 12); std::memcpy(group + 12, &model, 4); rtti_c::host_note(group + 12); }
    put32(72, addr(vtable));
    { const short adj = 16; std::memcpy(vtable + 168, &adj, 2); const unsigned f = addr(reinterpret_cast<void*>(slot168)); std::memcpy(vtable + 172, &f, 4); }
    { const short adj = 0; std::memcpy(vtable + 184, &adj, 2); const unsigned f = addr(reinterpret_cast<void*>(slot184)); std::memcpy(vtable + 188, &f, 4); }
    put16(572, state);
    put32(808, 5000); put32(256, 5000); put32(804, 5000); put32(964, 0x11111111); put32(968, 0x22222222);
    put32(576, 0); put16(294, 0); putf(28, 10.0f); put32(960, 0xffffffffu);
    putf(16, 1.5f); putf(20, -2.5f); putf(24, 3.25f); put16(56, 0x4000);
    put16(200, 0xfff6); put16(202, 25); put16(204, 0x8000); put32(12, 0);
    log_text.clear(); hits.clear(); queries.clear(); states_entered.clear();
    enemy1_result = 0; every_result = 0; advance_result = 0; exit_result = 0; roll_index = 0;
    rolls[0] = 1000; rolls[1] = 0xfffffff0u; rolls[2] = 3; rolls[3] = 4;
    melee_count = 0; general_count = 0; current = object;
}
static const char* prefix = "NEAVSUM";
static short shorten(int v) { return static_cast<short>(v); }

struct Step { char kind; float a, b; int offset, range, guard, target; };  // guard: kind value that skips the step, -1 none
int main() {
    rtti_c::host_note(object);
    reset(2); msg_run__10Arenabeast(object); expect(log_text == prefix, "common start, idle state");
    reset(2); every_result = 1; msg_run__10Arenabeast(object); expect(log_text == "NE", "every-frame result returns early");
    for (int s : {4, 7, 11, 12, 24, 25, 1000}) { reset(static_cast<unsigned>(s)); msg_run__10Arenabeast(object); expect(log_text == prefix, "no-op states", s); }
    reset(0xffff); msg_run__10Arenabeast(object); expect(log_text == prefix, "negative state");
    // 0 and 1.
    reset(0); enemy1_result = 1; msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "zG3;" && queries[0].a == 400.0f && queries[0].angle == 32767, "0: enemy near");
    reset(0); put32(256, 4999); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "zG1;", "0: timer");
    reset(0); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "z", "0: nothing");
    reset(1); enemy1_result = 1; msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "zG3;", "1: enemy near");
    reset(1); put32(256, 4999); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "zG0;", "1: timer");
    reset(1); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "zw", "1: wander");
    // 3: cascade.
    const float r = 10.0f;
    const Step state3[] = {
        {'z', 0, r + 20, 0, 4000, 21, 21}, {'z', 0, r + 20, 16384, 8192, 22, 22}, {'z', 0, r + 20, -16384, 8192, 23, 23},
        {'z', 0, r + 50, 0, 4000, -1, 6}, {'z', 0, 150, 0, 2000, -1, 18}, {'z', 0, 200, 0, 1000, -1, 13},
        {'z', 150, 200, 8192, 2000, 16, 16}, {'z', 150, 200, -8192, 2000, 17, 17},
        {'a', 0, r + 20, 0, 4000, 21, 21}, {'a', 0, r + 20, 16384, 8192, 22, 22}, {'a', 0, r + 20, -16384, 8192, 23, 23},
        {'a', r + 50, 150, 0, 2000, -1, 18}, {'a', 150, 200, 0, 1000, -1, 13},
        {'a', 0, 150, 24767, 2000, 14, 14}, {'a', 0, 150, -24769, 2000, 15, 15},
        {'a', 150, 200, 8192, 4000, 16, 16}, {'a', 150, 200, -8192, 4000, 17, 17}};
    const Step state5[] = {
        {'z', 0, r + 50, 0, 4000, -1, 6}, {'z', 0, 150, 0, 4000, -1, 18}, {'z', 0, 200, 0, 1000, -1, 13},
        {'z', 150, 200, 8192, 4000, -1, 16}, {'z', 150, 200, -8192, 4000, -1, 17},
        {'a', r + 50, 150, 0, 4000, -1, 18}, {'a', 150, 200, 0, 1000, -1, 13},
        {'a', 0, 150, 24767, 4000, 14, 14}, {'a', 0, 150, -24769, 4000, 15, 15},
        {'a', 150, 200, 8192, 4000, -1, 16}, {'a', 150, 200, -8192, 4000, -1, 17}};
    for (int which = 0; which < 2; ++which) {
        const Step* steps = which == 0 ? state3 : state5;
        const int count = which == 0 ? 17 : 11;
        const unsigned state = which == 0 ? 3 : 5;
        for (int kind : {-1, 21, 22, 23, 14, 15, 16, 17, 6}) {
            for (int hit = -1; hit < count; ++hit) {
                reset(state); put32(576, 1); put32(960, static_cast<unsigned>(kind));
                // Expected query list: steps whose guard differs from the kind, up to and including the hit step.
                std::vector<Step> want; int hit_state = -1;
                for (int i = 0; i < count; ++i) {
                    if (steps[i].guard != -1 && steps[i].guard == kind) { continue; }
                    want.push_back(steps[i]);
                    if (i == hit) { hit_state = steps[i].target; break; }
                }
                if (hit >= 0 && hit_state < 0) continue;  // the hit step is skipped by the guard
                hits.clear();
                // result queue: zero for every earlier query and one for the hit step
                for (size_t i = 0; i + 1 < want.size(); ++i) hits.push_back(0);
                if (hit >= 0) hits.push_back(1);
                msg_run__10Arenabeast(object);
                bool ok = queries.size() >= want.size();
                for (size_t i = 0; ok && i < want.size(); ++i) {
                    const Query& q = queries[i];
                    ok = q.kind == want[i].kind && q.a == want[i].a && q.b == want[i].b && q.angle == shorten(0x4000 + want[i].offset) && q.range == want[i].range;
                }
                expect(ok, "cascade queries", static_cast<int>(state), hit);
                if (hit >= 0) expect(states_entered.size() == 1 && states_entered[0] == hit_state && queries.size() == want.size(), "cascade hit", static_cast<int>(state), hit);
                else if (state == 3) {
                    expect(queries.size() == want.size() + 1 && queries.back().kind == '1' && queries.back().a == 300.0f && queries.back().angle == 32767 && log_text.back() == 's', "state 3 end: steer", kind);
                } else {
                    expect(queries.size() == want.size() && log_text.back() == 'x' && states_entered.empty(), "state 5 end: exit check", kind);
                }
            }
        }
    }
    reset(3); put32(576, 1); enemy1_result = 1; msg_run__10Arenabeast(object); expect(log_text.back() == 'f' && states_entered.empty(), "state 3: turn to face");
    reset(3); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "G0;", "3: no target");
    reset(5); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "x", "5: no target");
    reset(5); exit_result = 1; msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "xG3;", "5: no target, exit");
    reset(5); put32(576, 1); exit_result = 1; msg_run__10Arenabeast(object); expect(states_entered.size() == 1 && states_entered[0] == 3, "5: exit goes to 3");
    // Attack states.
    struct Attack { unsigned state; char type; float a, b; int offset; };
    const Attack attacks[] = {{6, 'm', 36, 0, 0}, {18, 'm', 150, 0, 0}, {13, 'm', 200, 0, 0}, {14, 'g', 0, 150, 24767}, {15, 'g', 0, 150, -24769}, {16, 'g', 150, 200, 8192}, {17, 'g', 150, 200, -8192}};
    for (const Attack& t : attacks) for (int done : {0, 1, 0x10, 0x11}) for (unsigned bits : {0u, 8u, 0x10u}) {
        reset(t.state); advance_result = done; put16(294, bits); msg_run__10Arenabeast(object);
        const bool fires = (done & 1) != 0;
        std::string want = std::string(prefix) + (fires ? std::string(1, t.type) : "");
        if ((bits & 8) == 0) want += "G3;";
        expect(log_text == want, "attack sequence", t.state, done);
        if (fires && t.type == 'm') expect(melee_amount == t.a && melee_ints[0] == -10 && melee_ints[1] == 25 && melee_ints[2] == -32768 && melee_ints[3] == 8, "melee arguments", t.state);
        if (fires && t.type == 'g') expect(general_floats[0] == t.a && general_floats[1] == t.b && general_ints[0] == shorten(0x4000 + t.offset) && general_ints[1] == 4000 && general_ints[2] == -10 && general_ints[3] == 25 && general_ints[4] == -32768 && general_ints[5] == 8, "general arguments", t.state);
    }
    for (unsigned state : {21u, 22u, 23u}) for (int done : {0, 1, 0x10, 0x11}) for (unsigned bits : {0u, 8u}) {
        reset(state); advance_result = done; put16(294, bits); msg_run__10Arenabeast(object);
        std::string want = std::string(prefix) + ((done & 1) ? "m" : "");
        if ((bits & 8) == 0) want += "G3;";
        expect(log_text == want && melee_count == (done & 1) && (!(done & 1) || melee_amount == 150.0f) && word(968) == ((done & 0x10) ? 1u : 0x22222222u), "21-23", state, done);
    }
    // 8, 9, 19, 20.
    reset(8); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "G0;", "8");
    reset(8); put16(294, 0x10); msg_run__10Arenabeast(object); expect(log_text == prefix, "8: bit 0x10");
    reset(9); put32(804, 4999); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "G3;", "9");
    reset(9); msg_run__10Arenabeast(object); expect(log_text == prefix, "9: equal");
    reset(19); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "G3;", "19");
    reset(19); put16(294, 0x10); msg_run__10Arenabeast(object); expect(log_text == prefix, "19: bit 0x10");
    reset(20); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "G3;r" && word(964) == 5000u + 1000u % 300u + 600u, "20");
    reset(20); rolls[0] = 0xfffffff0u; msg_run__10Arenabeast(object); expect(word(964) == 5000u + 0xfffffff0u % 300u + 600u, "20: unsigned remainder");
    reset(20); put16(294, 0x10); msg_run__10Arenabeast(object); expect(log_text == prefix && word(964) == 0x11111111u, "20: bit 0x10");
    // 10.
    reset(10); msg_run__10Arenabeast(object); expect(log_text == prefix, "10: nothing");
    reset(10); put16(294, 0x80); msg_run__10Arenabeast(object); expect(log_text == std::string(prefix) + "du" && fade_arg == 0, "10: fade and leave");
    reset(10); advance_result = 0x10; put32(12, 2); msg_run__10Arenabeast(object); expect(log_text == prefix, "10: flag 2 blocks the decal");
    reset(10); advance_result = 1; msg_run__10Arenabeast(object); expect(log_text == prefix, "10: bit 0x10 required");
    reset(10); advance_result = 0x10; put32(12, 0xfffffffd); rolls[0] = 0x12345; rolls[1] = 27; msg_run__10Arenabeast(object);
    expect(log_text == std::string(prefix) + "rrl" && floor_angle == 0x2345 && floor_extent == 47.0f && floor_kind == 0 && floor_extra == 1 && floor_point[0] == 1.5f && floor_point[1] == -2.5f && floor_point[2] == 3.25f, "10: decal");
    return failures ? 1 : 0;
}
