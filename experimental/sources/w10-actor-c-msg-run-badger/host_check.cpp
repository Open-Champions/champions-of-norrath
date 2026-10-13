// Build with -DNORRATH_W10AC_HOST (32-bit address words become offsets from
// norrath_w10ac_host_base; vtable function words map through norrath_w10ac_host_function).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <deque>
#include <map>
#include <string>
#include <vector>

extern "C" void msg_run__6Badger(void*);
unsigned char* norrath_w10ac_host_base;
static unsigned char memory[0x4000];
static unsigned char* object;
static int failures = 0;
static std::vector<std::string> events;
static std::map<std::string, std::deque<int>> script;  // scripted return values per callee
static int next(const std::string& name, int fallback) {
    auto& q = script[name];
    if (q.empty()) return fallback;
    int v = q.front();
    q.pop_front();
    return v;
}
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - memory); }
static std::string fl(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static const unsigned O = 0x10, MODEL = 0x900;

extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* s, void* m) { events.push_back("damageHandler:" + off(m)); }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void* m) {
    events.push_back("everyFrame:" + off(m));
    return next("everyFrame", 0);
}
extern "C" int objectActivationCheck__8Creaturei(void*, int range) {
    events.push_back("activation:" + std::to_string(range));
    return next("activation", 1);
}
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float step) {
    events.push_back("advance:" + fl(step));
    return next("advance", 0);
}
extern "C" void slideBack__8Creaturef(void*, float a) { events.push_back("slideBack:" + fl(a)); }
extern "C" void move2__8CreatureG6Point3(void*, const void* p) {
    float v[3];
    std::memcpy(v, p, 12);
    events.push_back("move2:" + fl(v[0]) + "," + fl(v[1]) + "," + fl(v[2]));
}
extern "C" void smartWander__8Creature(void*) { events.push_back("wander"); }
extern "C" void turnToFaceEnemy__8Creature(void*) { events.push_back("turn"); }
extern "C" void steerAndRouteToEnemy__8Creature(void*) { events.push_back("steer"); }
extern "C" int bExitStatePersueIdle__8Creature(void*) { events.push_back("exitIdle"); return next("exitIdle", 0); }
extern "C" int enemyInZone__8Creaturefs(void*, float range, short zone) {
    events.push_back("zone:" + fl(range) + ":" + std::to_string(zone));
    return next("zone", 0);
}
extern "C" int enemyInZone2__8Creatureffssfi(void*, float a, float b, short angle, short zone, float c, int d) {
    events.push_back("zone2:" + fl(a) + ":" + fl(b) + ":" + std::to_string(angle) + ":" + std::to_string(zone) + ":" + fl(c) + ":" + std::to_string(d));
    return next("zone2", 0);
}
extern "C" int alternateEnemyInZone__8Creatureffss(void*, float a, float b, short angle, short zone) {
    events.push_back("alt:" + fl(a) + ":" + fl(b) + ":" + std::to_string(angle) + ":" + std::to_string(zone));
    return next("alt", 0);
}
extern "C" int produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void*, float a, float b, short angle, short zone,
                                                                              int p, int q, int r, int flags, void* d) {
    events.push_back("damage:" + fl(a) + ":" + fl(b) + ":" + std::to_string(angle) + ":" + std::to_string(zone) + ":" +
                     std::to_string(p) + ":" + std::to_string(q) + ":" + std::to_string(r) + ":" + std::to_string(flags) + ":" + (d ? "?" : "null"));
    return 0;
}
extern "C" void objectAddToSlowRunList__FP10GameObject(void*) { events.push_back("slowRun"); }
extern "C" void floorAdd__FG6Point3sfii(const void* p, short angle, float size, int a, int b) {
    float v[3];
    std::memcpy(v, p, 12);
    events.push_back("floor:" + fl(v[0]) + "," + fl(v[1]) + "," + fl(v[2]) + ":" + std::to_string(angle) + ":" + fl(size) + ":" + std::to_string(a) + ":" + std::to_string(b));
}
extern "C" int eRand__10GameObject(void*) {
    int v = next("rand", 0);
    events.push_back("rand:" + std::to_string(v));
    return v;
}
static void slot168(void* self) { events.push_back("v168:" + off(self)); }
static void slot184(void* self, int state) { events.push_back("setState:" + off(self) + ":" + std::to_string(state)); }
extern "C" void* norrath_w10ac_host_function(unsigned word) {
    return word == 1 ? reinterpret_cast<void*>(&slot168) : reinterpret_cast<void*>(&slot184);
}

static void put(unsigned o, unsigned v) { std::memcpy(memory + o, &v, 4); }
static void put16(unsigned o, unsigned v) { std::memcpy(memory + o, &v, 2); }
static void putf(unsigned o, float v) { std::memcpy(memory + o, &v, 4); }
static unsigned get(unsigned o) { unsigned v; std::memcpy(&v, memory + o, 4); return v; }
static void reset(int state) {
    std::memset(memory, 0, sizeof memory);
    events.clear();
    script.clear();
    object = memory + O;
    put(O + 36, 0x800); put(0x800 + 12, MODEL);
    put(O + 72, 0x2000);
    put16(0x2000 + 168, 0x10); put(0x2000 + 172, 1);
    put16(0x2000 + 184, 0x20); put(0x2000 + 188, 2);
    put16(O + 572, static_cast<unsigned>(state));
    putf(O + 28, 5.0f);
    put16(O + 56, 100);
    put16(O + 294, 0);
}
static std::vector<std::string> prologue() {
    return {"damageHandler:" + std::to_string(MODEL), "everyFrame:" + std::to_string(MODEL), "activation:600", "advance:1",
            "slideBack:0.2", "v168:" + std::to_string(O + 0x10)};
}
static std::string setstate(int n) { return "setState:" + std::to_string(O + 0x20) + ":" + std::to_string(n); }
static std::vector<std::string> tail(const std::vector<std::string>& all, size_t from) {
    return std::vector<std::string>(all.begin() + from, all.end());
}
static void check(const char* message, const std::vector<std::string>& want) {
    std::vector<std::string> got = tail(events, prologue().size());
    expect(got == want, message);
    if (got != want) {
        for (auto& e : got) std::fprintf(stderr, "  got  %s\n", e.c_str());
        for (auto& e : want) std::fprintf(stderr, "  want %s\n", e.c_str());
    }
}
int main() {
    norrath_w10ac_host_base = memory;
    // Prologue exits.
    reset(0); script["everyFrame"] = {1}; msg_run__6Badger(object);
    expect(events.size() == 2, "everyFrame nonzero stops after two calls");
    reset(0); script["activation"] = {0}; msg_run__6Badger(object);
    expect(events.size() == 3, "activation zero stops");
    reset(17); msg_run__6Badger(object);
    expect(events.size() == prologue().size(), "state 17 does nothing"); expect(events == prologue(), "prologue calls and order");
    reset(-1); msg_run__6Badger(object);
    expect(events.size() == prologue().size(), "negative state does nothing");
    for (int s : {2, 4, 7, 11, 12}) { reset(s); msg_run__6Badger(object); expect(events.size() == prologue().size(), "idle state"); }
    // State 0.
    reset(0); script["zone"] = {1}; msg_run__6Badger(object);
    check("state 0 near", {"zone:400:32767", setstate(3)});
    reset(0); script["zone"] = {0, 1}; msg_run__6Badger(object);
    check("state 0 second zone", {"zone:400:32767", "zone:500:16000", setstate(3)});
    reset(0); put(O + 256, 4); put(O + 808, 5); msg_run__6Badger(object);
    check("state 0 timer", {"zone:400:32767", "zone:500:16000", setstate(1)});
    reset(0); put(O + 256, 5); put(O + 808, 5); msg_run__6Badger(object);
    check("state 0 timer not below", {"zone:400:32767", "zone:500:16000"});
    reset(0); put(O + 256, 0xffffffffu); put(O + 808, 0); msg_run__6Badger(object);
    check("state 0 signed compare", {"zone:400:32767", "zone:500:16000", setstate(1)});
    // State 1.
    reset(1); script["zone"] = {0, 1}; msg_run__6Badger(object);
    check("state 1 zone", {"zone:400:32767", "zone:500:16000", setstate(0)});
    reset(1); msg_run__6Badger(object);
    check("state 1 wander", {"zone:400:32767", "zone:500:16000", "wander"});
    // State 3.
    reset(3); msg_run__6Badger(object);
    check("state 3 no target", {setstate(0)});
    for (int pick : {0, 1, 2, 3, 4, 5, 9}) {
        reset(3); put(O + 576, 1); script["zone2"] = {1}; script["rand"] = {pick};
        msg_run__6Badger(object);
        int want_state = (pick % 5) == 0 ? 14 : (pick % 5) == 1 ? 15 : 6;
        check("state 3 first zone2 rolls a state", {"zone2:0:60:100:3000:0:0", "rand:" + std::to_string(pick), setstate(want_state)});
    }
    reset(3); put(O + 576, 1); script["zone2"] = {0, 1}; msg_run__6Badger(object);
    check("state 3 second zone2", {"zone2:0:60:100:3000:0:0", "zone2:30:100:100:3000:0:0", setstate(16)});
    reset(3); put(O + 576, 1); script["zone2"] = {0, 0, 1}; msg_run__6Badger(object);
    check("state 3 third zone2", {"zone2:0:60:100:3000:0:0", "zone2:30:100:100:3000:0:0", "zone2:0:60:8100:3000:0:0", setstate(14)});
    reset(3); put(O + 576, 1); script["zone2"] = {0, 0, 0, 1}; msg_run__6Badger(object);
    check("state 3 fourth zone2", {"zone2:0:60:100:3000:0:0", "zone2:30:100:100:3000:0:0", "zone2:0:60:8100:3000:0:0",
                                   "zone2:0:60:-7900:3000:0:0", setstate(15)});
    {
        std::vector<std::string> zone2s = {"zone2:0:60:100:3000:0:0", "zone2:30:100:100:3000:0:0", "zone2:0:60:8100:3000:0:0", "zone2:0:60:-7900:3000:0:0"};
        reset(3); put(O + 576, 1); script["alt"] = {1}; script["rand"] = {1}; msg_run__6Badger(object);
        auto want = zone2s; want.push_back("alt:0:30:100:3000"); want.push_back("rand:1"); want.push_back(setstate(15));
        check("state 3 alternate zone with a roll", want);
        reset(3); put(O + 576, 1); script["alt"] = {0, 1}; msg_run__6Badger(object);
        want = zone2s; want.push_back("alt:0:30:100:3000"); want.push_back("alt:0:30:8100:3000"); want.push_back(setstate(14));
        check("state 3 alternate +8000", want);
        reset(3); put(O + 576, 1); script["alt"] = {0, 0, 1}; msg_run__6Badger(object);
        want = zone2s; want.push_back("alt:0:30:100:3000"); want.push_back("alt:0:30:8100:3000"); want.push_back("alt:0:30:-7900:3000"); want.push_back(setstate(15));
        check("state 3 alternate -8000", want);
        reset(3); put(O + 576, 1); script["zone"] = {1}; msg_run__6Badger(object);
        want = zone2s; want.push_back("alt:0:30:100:3000"); want.push_back("alt:0:30:8100:3000"); want.push_back("alt:0:30:-7900:3000");
        want.push_back("zone:25:32767"); want.push_back("turn");
        check("state 3 turns toward the enemy", want);
        reset(3); put(O + 576, 1); msg_run__6Badger(object);
        want.pop_back(); want.push_back("steer");
        check("state 3 steers", want);
    }
    // State 5.
    reset(5); msg_run__6Badger(object);
    check("state 5 no target, no exit", {"exitIdle"});
    reset(5); script["exitIdle"] = {1}; msg_run__6Badger(object);
    check("state 5 no target, exit", {"exitIdle", setstate(3)});
    reset(5); put(O + 576, 1); script["zone2"] = {1}; msg_run__6Badger(object);
    check("state 5 zone2", {"zone2:0:30:100:3000:0:0", setstate(6)});
    reset(5); put(O + 576, 1); script["alt"] = {1}; msg_run__6Badger(object);
    check("state 5 alternate", {"zone2:0:30:100:3000:0:0", "alt:0:30:100:3000", setstate(6)});
    reset(5); put(O + 576, 1); script["exitIdle"] = {1}; msg_run__6Badger(object);
    check("state 5 exit after tests", {"zone2:0:30:100:3000:0:0", "alt:0:30:100:3000", "exitIdle", setstate(3)});
    // States 6, 13..16.
    for (int s : {6, 13, 14, 15, 16}) {
        reset(s); put16(O + 294, 8); msg_run__6Badger(object);
        check("state 6/13-16 quiet", {"move2:0,0,0"});
    }
    reset(14); msg_run__6Badger(object);
    check("state 14 with bit 3 clear sets state 3", {"move2:0,0,0", setstate(3)});
    reset(14); put16(O + 294, 8); script["advance"] = {1}; put16(O + 200, 0xfff0); put16(O + 202, 7); put16(O + 204, 9);
    msg_run__6Badger(object);
    check("state 14 damage flags 8", {"move2:0,0,0", "damage:0:30:100:3000:-16:7:9:8:null"});
    expect(get(O + 960) == 1, "damage sets +960");
    reset(6); put16(O + 294, 8); script["advance"] = {1}; script["rand"] = {4}; msg_run__6Badger(object);
    check("state 6 damage rolled flags", {"move2:0,0,0", "rand:4", "damage:0:30:100:3000:0:0:0:262152:null"});
    reset(16); put16(O + 294, 8); script["advance"] = {1}; script["rand"] = {3}; msg_run__6Badger(object);
    check("state 16 damage rolled flags (divisible by 3)", {"move2:0,0,0", "rand:3", "damage:0:30:100:3000:0:0:0:524296:null"});
    reset(13); put16(O + 294, 8); script["advance"] = {1}; script["rand"] = {5}; msg_run__6Badger(object);
    check("state 13 damage rolled flags", {"move2:0,0,0", "rand:5", "damage:0:30:100:3000:0:0:0:262152:null"});
    reset(6); put16(O + 294, 8); script["advance"] = {1}; memory[O + 964] = 1; msg_run__6Badger(object);
    check("byte 964 equal to 1 keeps flags 8 without a roll", {"move2:0,0,0", "damage:0:30:100:3000:0:0:0:8:null"});
    // Turn toward the enemy blocks.
    reset(6); put(O + 576, 1); put16(O + 294, 8); script["zone"] = {1}; msg_run__6Badger(object);
    check("state 6 turn", {"move2:0,0,0", "zone:15:3000", "turn"});
    reset(6); put(O + 576, 1); put(O + 960, 1); put16(O + 294, 8); msg_run__6Badger(object);
    check("state 6 with +960 set skips the zone test", {"move2:0,0,0"});
    reset(16); put(O + 576, 1); put16(O + 294, 8); script["zone"] = {1}; msg_run__6Badger(object);
    check("state 16 turn", {"move2:0,0,0", "zone:15:3000", "turn"});
    reset(13); put(O + 576, 1); put16(O + 294, 8); script["zone"] = {1}; msg_run__6Badger(object);
    check("state 13 turn", {"move2:0,0,0", "zone:15:3000", "turn"});
    reset(14); put(O + 576, 1); put16(O + 294, 8); script["zone"] = {1}; msg_run__6Badger(object);
    check("state 14 never turns", {"move2:0,0,0"});
    // Bit 1 of the animation result.
    reset(14); put16(O + 294, 8); script["advance"] = {2}; script["zone2"] = {1}; msg_run__6Badger(object);
    check("advance bit 1 with a zone hit", {"move2:0,0,0", "zone2:0:60:100:3000:0:0", setstate(6)});
    reset(6); put16(O + 294, 8); script["advance"] = {2}; script["zone2"] = {1}; msg_run__6Badger(object);
    check("advance bit 1 is ignored in state 6", {"move2:0,0,0"});
    // State 8, 9.
    reset(8); msg_run__6Badger(object); check("state 8 sets 3", {setstate(3)});
    reset(8); put16(O + 294, 0x10); msg_run__6Badger(object); check("state 8 bit 4 stops", {});
    reset(9); put(O + 804, 1); put(O + 808, 2); msg_run__6Badger(object); check("state 9 sets 3", {setstate(3)});
    reset(9); put(O + 804, 2); put(O + 808, 2); msg_run__6Badger(object); check("state 9 not below", {});
    reset(9); put(O + 804, 0xffffffffu); put(O + 808, 0); msg_run__6Badger(object); check("state 9 signed", {setstate(3)});
    // State 10.
    reset(10); put16(O + 294, 0x80); put(O + 12, 0); script["advance"] = {4}; msg_run__6Badger(object);
    check("state 10 slow run list", {"slowRun"});
    expect(get(O + 12) == 1, "state 10 sets +12 bit 0");
    reset(10); script["advance"] = {1}; script["rand"] = {0x12345, 14};
    putf(O + 16, 1.5f); putf(O + 20, 2.5f); putf(O + 24, 3.5f); msg_run__6Badger(object);
    check("state 10 floor", {"rand:74565", "rand:14", "floor:1.5,2.5,3.5:9029:39:0:1"});
    return failures ? 1 : 0;
}
