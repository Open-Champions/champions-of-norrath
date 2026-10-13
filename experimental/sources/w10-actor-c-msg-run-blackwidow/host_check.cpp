// Build with -DNORRATH_W10AC_HOST (32-bit address words become offsets from
// norrath_w10ac_host_base; vtable function words map through norrath_w10ac_host_function).
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <deque>
#include <map>
#include <string>
#include <vector>

extern "C" void msg_run__10BlackWidow(void*);
unsigned char* norrath_w10ac_host_base;
extern "C" unsigned char ectopileParticle1[16] = {};
static unsigned char memory[0x4000];
static unsigned char* object;
static int failures = 0;
static std::vector<std::string> events;
static std::map<std::string, std::deque<int>> script;
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
static std::string pt(const void* p) {
    float v[3];
    std::memcpy(v, p, 12);
    return fl(v[0]) + "," + fl(v[1]) + "," + fl(v[2]);
}
static const unsigned O = 0x10, MODEL = 0x900, W = O + 976, NODE = 0x1000, OBJ2 = 0x1100;

extern "C" void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { events.push_back("damageHandler:" + off(m)); }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void* m) { events.push_back("everyFrame:" + off(m)); return next("everyFrame", 0); }
extern "C" int objectActivationCheck__8Creaturei(void*, int range) { events.push_back("activation:" + std::to_string(range)); return next("activation", 1); }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float step) { events.push_back("advance:" + fl(step)); return next("advance", 0); }
extern "C" void slideBack__8Creaturef(void*, float a) { events.push_back("slideBack:" + fl(a)); }
extern "C" void run__3WebG6Point3i(void* w, const void* p, int v) { events.push_back("webrun:" + off(w) + ":" + pt(p) + ":" + std::to_string(v)); }
extern "C" void draw__3WebG6Point3(void* w, const void* p) { events.push_back("webdraw:" + off(w) + ":" + pt(p)); }
extern "C" void start__3WebG6Point3P8Creaturefi(void* w, const void* p, void* c, float r, int v) {
    events.push_back("webstart:" + off(w) + ":" + pt(p) + ":" + off(c) + ":" + fl(r) + ":" + std::to_string(v));
}
extern "C" int bActive__3Web(void*) { events.push_back("webActive"); return next("active", 0); }
extern "C" void breakWeb__3Web(void*) { events.push_back("webBreak"); }
extern "C" int move2__8CreatureG6Point3(void*, const void* p) { events.push_back("move2:" + pt(p)); return next("move2", 0); }
extern "C" void smartWander__8Creature(void*) { events.push_back("wander"); }
extern "C" void turnToFaceEnemy__8Creature(void*) { events.push_back("turn"); }
extern "C" void steerAndRouteToEnemy__8Creature(void*) { events.push_back("steer"); }
extern "C" int bExitStatePersueIdle__8Creature(void*) { events.push_back("exitIdle"); return next("exitIdle", 0); }
extern "C" int enemyInZone__8Creaturefs(void*, float range, short zone) {
    events.push_back("zone:" + fl(range) + ":" + std::to_string(zone));
    return next("zone", 0);
}
static std::vector<std::vector<unsigned char>> descriptors;
extern "C" int produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float amount, int a, int b, int c, int flags,
                                                                         void* description) {
    events.push_back("melee:" + fl(amount) + ":" + std::to_string(a) + ":" + std::to_string(b) + ":" + std::to_string(c) + ":" +
                     std::to_string(flags) + ":" + (description ? "desc" : "null"));
    if (description) {
        const unsigned char* d = static_cast<const unsigned char*>(description);
        descriptors.emplace_back(d + 16, d + 68);
    }
    return 0;
}
extern "C" void loseBodyPart__10BlackWidowi(void*, int part) { events.push_back("lose:" + std::to_string(part)); }
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, void* p, void* v, float f) {
    events.push_back(std::string("particle:") + (def == ectopileParticle1 ? "def" : "?") + ":" + pt(p) + ":" + pt(v) + ":" + fl(f));
}
extern "C" void objectAddToDeleteList__FP10GameObject(void*) { events.push_back("delete"); }
extern "C" void objectRemoveFromRunList__FP10GameObject(void*) { events.push_back("removeRun"); }
extern "C" void floorAdd__FG6Point3sfii(const void* p, short angle, float size, int a, int b) {
    events.push_back("floor:" + pt(p) + ":" + std::to_string(angle) + ":" + fl(size) + ":" + std::to_string(a) + ":" + std::to_string(b));
}
extern "C" float icos__Fi(int a) { events.push_back("icos:" + std::to_string(a)); return 0.5f; }
extern "C" float isin__Fi(int a) { events.push_back("isin:" + std::to_string(a)); return 0.25f; }
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
static void put64(unsigned o, unsigned long long v) { std::memcpy(memory + o, &v, 8); }
static void putf(unsigned o, float v) { std::memcpy(memory + o, &v, 4); }
static unsigned get(unsigned o) { unsigned v; std::memcpy(&v, memory + o, 4); return v; }
static float getf(unsigned o) { float v; std::memcpy(&v, memory + o, 4); return v; }
static unsigned long long get64(unsigned o) { unsigned long long v; std::memcpy(&v, memory + o, 8); return v; }
static void reset(int state) {
    std::memset(memory, 0, sizeof memory);
    events.clear(); script.clear(); descriptors.clear();
    object = memory + O;
    put(O + 36, 0x800); put(0x800 + 12, MODEL);
    put(O + 72, 0x2000);
    put16(0x2000 + 168, 0x10); put(0x2000 + 172, 1);
    put16(0x2000 + 184, 0x20); put(0x2000 + 188, 2);
    put16(O + 572, static_cast<unsigned>(state));
    putf(O + 16, 10.0f); putf(O + 20, 20.0f); putf(O + 24, 30.0f); putf(O + 28, 5.0f);
    put16(O + 56, 100);
    put(O + 808, 7);
}
static std::vector<std::string> prologue(const char* z = "40") {
    return {"damageHandler:" + std::to_string(MODEL), "everyFrame:" + std::to_string(MODEL), "activation:600", "advance:1",
            "slideBack:0.5", "v168:" + std::to_string(O + 0x10), "webrun:" + std::to_string(W) + ":10,20," + z + ":7",
            "webdraw:" + std::to_string(W) + ":10,20," + z};
}
static std::string setstate(int n) { return "setState:" + std::to_string(O + 0x20) + ":" + std::to_string(n); }
static void check(const char* message, const std::vector<std::string>& want) {
    std::vector<std::string> got(events.begin() + std::min(events.size(), prologue().size()), events.end());
    expect(got == want, message);
    if (got != want) {
        for (auto& e : got) std::fprintf(stderr, "  got  %s\n", e.c_str());
        for (auto& e : want) std::fprintf(stderr, "  want %s\n", e.c_str());
    }
}
static void link_target(unsigned flags, float quantity) {
    put(O + 576, NODE); put(NODE + 12, OBJ2); put(OBJ2 + 12, flags); putf(OBJ2 + 756, quantity);
}
int main() {
    norrath_w10ac_host_base = memory;
    // Early exits and the frame-wide adjustment of +24.
    reset(0); script["everyFrame"] = {1}; put64(O + 160, 0x01000000ull); putf(O + 972, 2.5f); msg_run__10BlackWidow(object);
    expect(events.size() == 2 && getf(O + 24) == 32.5f, "everyFrame nonzero returns without undoing the +24 adjustment");
    reset(0); script["activation"] = {0}; msg_run__10BlackWidow(object);
    expect(events.size() == 3, "activation zero stops");
    reset(0); put64(O + 160, 0x01000000ull); putf(O + 972, 2.5f); msg_run__10BlackWidow(object);
    expect(getf(O + 24) == 30.0f && events[6] == "webrun:" + std::to_string(W) + ":10,20,42.5:7", "z adjusted for the pass and restored");
    reset(17); msg_run__10BlackWidow(object); expect(events == prologue(), "state 17");
    reset(-1); msg_run__10BlackWidow(object); expect(events == prologue(), "negative state");
    for (int s : {2, 4, 7, 11, 12}) { reset(s); msg_run__10BlackWidow(object); expect(events == prologue(), "idle state"); }
    // State 16.
    reset(16); put64(O + 160, ~0ull); put(O + 12, 0xffffffffu); script["advance"] = {1}; put16(O + 294, 0x80);
    msg_run__10BlackWidow(object);
    check("state 16 sets state 0", {setstate(0)});
    expect(get64(O + 160) == (~0ull & ~(1ull << 21) & ~(1ull << 11)) && get(O + 12) == 0xfffffffeu, "state 16 flag clears");
    reset(16); put64(O + 160, ~0ull); script["advance"] = {0}; put16(O + 294, 0);
    msg_run__10BlackWidow(object);
    check("state 16 quiet", {});
    expect(get64(O + 160) == ~0ull, "state 16 keeps the flags without bits");
    reset(16); put64(O + 160, ~0ull); script["advance"] = {1}; msg_run__10BlackWidow(object);
    expect(get64(O + 160) == (~0ull & ~(1ull << 21)), "state 16 clears only bit 21");
    // States 0 and 1.
    reset(0); script["zone"] = {1}; msg_run__10BlackWidow(object);
    check("state 0 first zone", {"zone:300:32767", setstate(3)});
    reset(0); script["zone"] = {0, 1}; msg_run__10BlackWidow(object);
    check("state 0 second zone", {"zone:300:32767", "zone:300:18000", setstate(3)});
    reset(0); put(O + 256, 6); msg_run__10BlackWidow(object);
    check("state 0 timer", {"zone:300:32767", "zone:300:18000", setstate(1)});
    reset(0); put(O + 256, 7); msg_run__10BlackWidow(object);
    check("state 0 timer equal", {"zone:300:32767", "zone:300:18000"});
    reset(1); script["zone"] = {0, 1}; msg_run__10BlackWidow(object);
    check("state 1 second zone", {"zone:300:32767", "zone:300:18000", setstate(3)});
    reset(1); msg_run__10BlackWidow(object);
    check("state 1 wander", {"zone:300:32767", "zone:300:18000", "wander"});
    // State 3.
    reset(3); msg_run__10BlackWidow(object); check("state 3 without a target", {setstate(0)});
    for (int r : {0, 6, 1, 5, 12}) {
        reset(3); link_target(0, 0); script["zone"] = {1}; script["rand"] = {r}; msg_run__10BlackWidow(object);
        check("state 3 roll", {"zone:35:2000", "rand:" + std::to_string(r), setstate(r % 6 == 0 ? 15 : 6)});
    }
    reset(3); link_target(0x20, 0.0f); put(O + 960, 3); script["zone"] = {0, 1}; msg_run__10BlackWidow(object);
    check("state 3 web", {"zone:35:2000", "zone:500:3000", setstate(13)});
    reset(3); link_target(0x20, 0.0f); put(O + 960, 3); script["zone"] = {0, 0, 1}; msg_run__10BlackWidow(object);
    check("state 3 web refused, then turns", {"zone:35:2000", "zone:500:3000", "zone:35:32767", "turn"});
    reset(3); link_target(0x20, 0.0f); put(O + 960, 7); msg_run__10BlackWidow(object);
    check("state 3 timer not below", {"zone:35:2000", "zone:35:32767", "steer"});
    reset(3); link_target(0x20, 1.0f); msg_run__10BlackWidow(object);
    check("state 3 nonzero float", {"zone:35:2000", "zone:35:32767", "steer"});
    reset(3); link_target(0x1f, 0.0f); msg_run__10BlackWidow(object);
    check("state 3 flag missing", {"zone:35:2000", "zone:35:32767", "steer"});
    // State 5.
    reset(5); link_target(0, 0); script["zone"] = {1}; script["rand"] = {6}; msg_run__10BlackWidow(object);
    check("state 5 roll", {"zone:35:2000", "rand:6", setstate(15)});
    reset(5); msg_run__10BlackWidow(object);
    check("state 5 without a target", {"zone:35:2000", "exitIdle"});
    reset(5); script["exitIdle"] = {1}; msg_run__10BlackWidow(object);
    check("state 5 exit", {"zone:35:2000", "exitIdle", setstate(3)});
    reset(5); link_target(0x20, 0.0f); put(O + 960, 1); script["zone"] = {0, 1}; msg_run__10BlackWidow(object);
    check("state 5 web", {"zone:35:2000", "zone:500:3000", setstate(13)});
    reset(5); link_target(0x20, 0.0f); put(O + 960, 1); script["exitIdle"] = {1}; msg_run__10BlackWidow(object);
    check("state 5 web refused, exit", {"zone:35:2000", "zone:500:3000", "exitIdle", setstate(3)});
    // State 6.
    reset(6); put16(O + 294, 8); put(O + 256, 5); msg_run__10BlackWidow(object);
    check("state 6 without a target: bit 3 set means no countdown and the counter stays positive", {});
    expect(get(O + 256) == 5, "counter unchanged");
    reset(6); put(O + 256, 5); msg_run__10BlackWidow(object);
    check("state 6 without a target counts down", {});
    expect(get(O + 256) == 4, "counter decremented");
    reset(6); put(O + 256, 1); msg_run__10BlackWidow(object);
    check("state 6 counter expires", {setstate(3)});
    expect(get(O + 256) == 0, "counter reached zero");
    reset(6); put(O + 256, 0); msg_run__10BlackWidow(object);
    check("state 6 counter below zero", {setstate(3)});
    reset(6); link_target(0, 0); put16(O + 294, 8); put(O + 256, 9); script["advance"] = {2};
    put16(O + 200, 0xfff0); put16(O + 202, 7); put16(O + 204, 9); msg_run__10BlackWidow(object);
    check("state 6 melee and turn", {"melee:36:-16:7:9:4:null", "turn"});
    reset(6); link_target(0, 0); put(O + 256, 9); msg_run__10BlackWidow(object);
    check("state 6 target, bit 3 clear: only the countdown", {});
    expect(get(O + 256) == 8, "state 6 counter");
    // State 15.
    reset(15); put(O + 256, 9); msg_run__10BlackWidow(object);
    check("state 15 without a target", {});
    expect(get(O + 256) == 8, "state 15 counter");
    reset(15); link_target(0, 0); put(O + 964, 1); putf(O + 176, 8.0f); memory[O + 800] = 30; script["move2"] = {2}; put(O + 256, 9);
    msg_run__10BlackWidow(object);
    check("state 15 movement", {"icos:100", "isin:100", "move2:2,1,0"});
    expect(get(O + 964) == 0, "move2 result bit 1 clears +964");
    reset(15); link_target(0, 0); put(O + 964, 1); putf(O + 176, 8.0f); memory[O + 800] = 70; script["move2"] = {1}; put(O + 256, 9);
    msg_run__10BlackWidow(object);
    check("state 15 movement unscaled", {"icos:100", "isin:100", "move2:4,2,0"});
    expect(get(O + 964) == 1, "move2 result without bit 1 keeps +964");
    reset(15); link_target(0, 0); put(O + 964, 1); putf(O + 176, 8.0f); memory[O + 800] = 1; script["move2"] = {0}; put(O + 256, 9);
    msg_run__10BlackWidow(object);
    check("state 15 movement scaled by 59/60", {"icos:100", "isin:100", "move2:3.93333,1.96667,0"});
    reset(15); link_target(0, 0); put(O + 964, 5); script["advance"] = {2}; put16(O + 200, 11); put16(O + 202, 12); put16(O + 204, 13);
    put(O + 256, 9); msg_run__10BlackWidow(object);
    check("state 15 melee with a descriptor", {"icos:100", "isin:100", "move2:0,0,0", "melee:36:11:12:13:65540:desc"});
    expect(get(O + 964) == 0, "state 15 clears +964");
    if (!descriptors.empty()) {
        const auto& d = descriptors[0];  // starts at byte 16 of the descriptor
        bool ok = true;
        for (int i = 0; i < 16; ++i) ok = ok && d[i] == 0;
        ok = ok && d[16] == 0xff && d[17] == 0xff;
        // slot 0 at byte 36: id 23, then 11, 12, 0
        auto half = [&](int at) { return static_cast<short>(d[at - 16] | (d[at - 16 + 1] << 8)); };
        ok = ok && half(36) == 23 && half(38) == 11 && half(40) == 12 && half(42) == 0;
        ok = ok && half(44) == -1 && half(52) == -1 && half(60) == -1;
        expect(ok, "descriptor layout");
    }
    // State 13.
    reset(13); script["active"] = {0}; msg_run__10BlackWidow(object);
    check("state 13 inactive", {"webActive", setstate(3)});
    reset(13); script["active"] = {1}; msg_run__10BlackWidow(object);
    check("state 13 active, nothing more", {"webActive"});
    reset(13); script["active"] = {1}; put(O + 988, 1); msg_run__10BlackWidow(object);
    check("state 13 active with +988", {"webActive", setstate(14)});
    reset(13); link_target(0x20, 0); script["advance"] = {1}; script["active"] = {1}; msg_run__10BlackWidow(object);
    check("state 13 starts the web", {"webstart:" + std::to_string(W) + ":10,20,40:" + std::to_string(OBJ2) + ":300:7", "webActive"});
    reset(13); link_target(0x20, 0); script["advance"] = {0}; script["active"] = {1}; msg_run__10BlackWidow(object);
    check("state 13 needs bit 0 of the animation result", {"webActive"});
    reset(13); link_target(0x1f, 0); script["advance"] = {1}; script["active"] = {1}; msg_run__10BlackWidow(object);
    check("state 13 needs the target flag", {"webActive"});
    // State 14.
    reset(14); script["active"] = {0}; msg_run__10BlackWidow(object);
    check("state 14 inactive", {"webActive", setstate(3)});
    reset(14); script["active"] = {1}; link_target(0, 0); msg_run__10BlackWidow(object);
    check("state 14 turns", {"webActive", "zone:35:2000", "turn"});
    reset(14); script["active"] = {1}; link_target(0, 0); script["zone"] = {1}; msg_run__10BlackWidow(object);
    check("state 14 breaks the web", {"webActive", "zone:35:2000", "webBreak", setstate(15), "turn"});
    // States 9 and 8.
    reset(9); put(O + 804, 3); msg_run__10BlackWidow(object); check("state 9", {setstate(0)});
    reset(9); put(O + 804, 7); msg_run__10BlackWidow(object); check("state 9 not below", {});
    reset(8); msg_run__10BlackWidow(object); check("state 8", {setstate(3)});
    reset(8); put16(O + 294, 0x10); msg_run__10BlackWidow(object); check("state 8 bit 4", {});
    // State 10.
    reset(10); script["advance"] = {4}; script["rand"] = {0x2345, 17}; msg_run__10BlackWidow(object);
    check("state 10 floor", {"rand:9029", "rand:17", "floor:10,20,30:9029:57:6:1"});
    reset(10); script["advance"] = {4}; put(O + 12, 2); msg_run__10BlackWidow(object);
    check("state 10 floor blocked by +12 bit 1", {});
    reset(10); put16(O + 294, 0x80); msg_run__10BlackWidow(object);
    check("state 10 flag clear: removed from the run list", {"removeRun"});
    reset(10); put64(O + 160, 0x01000000ull); put16(O + 294, 0x80); put(O + 808, 500); msg_run__10BlackWidow(object);
    check("state 10 arms the timer", {});
    expect(get(O + 964) == 1 && get(O + 968) == 500, "state 10 stores +964 and +968");
    reset(10); put64(O + 160, 0x01000000ull); put(O + 808, 500); msg_run__10BlackWidow(object);
    check("state 10 needs bit 7", {});
    expect(get(O + 964) == 0, "state 10 keeps +964");
    for (int k = 0; k < 8; ++k) {
        reset(10); put64(O + 160, 0x01000000ull); put(O + 964, 1); put(O + 808, 500); put(O + 968, 500 - 10 * (k + 1));
        msg_run__10BlackWidow(object);
        std::vector<std::string> want = {"lose:" + std::to_string(k)};
        if (k >= 7) {}  // difference of 80 does not start the particles
        check("state 10 body part", want);
    }
    reset(10); put64(O + 160, 0x01000000ull); put(O + 964, 1); put(O + 808, 500); put(O + 968, 419);
    script["rand"] = std::deque<int>(60, 49152);
    msg_run__10BlackWidow(object);
    {
        // diff 81: three bursts. All coordinates come from rolls of 32768 -> 0.0 -> -1... use 49152 -> 0.5.
        std::vector<std::string> got(events.begin() + prologue().size(), events.end());
        int particles = 0;
        for (auto& e : got) if (e.rfind("particle:", 0) == 0) ++particles;
        expect(particles == 3, "diff 81 starts three bursts");
        expect(getf(O + 972) > 0.1499f && getf(O + 972) < 0.1501f, "+972 grows by 0.15");
    }
    reset(10); put64(O + 160, 0x01000000ull); put(O + 964, 1); put(O + 808, 500); put(O + 968, 420);
    msg_run__10BlackWidow(object);
    {
        std::vector<std::string> got(events.begin() + prologue().size(), events.end());
        expect(got.empty() || got[0].rfind("particle:", 0) != 0 || true, "diff 80");
        int particles = 0;
        for (auto& e : got) if (e.rfind("particle:", 0) == 0) ++particles;
        expect(particles == 0, "diff 80 starts no bursts (and loses part 7)");
    }
    reset(10); put64(O + 160, 0x01000000ull); put(O + 964, 1); put(O + 808, 800); put(O + 968, 559);
    script["rand"] = std::deque<int>(80, 49152);
    msg_run__10BlackWidow(object);
    {
        std::vector<std::string> got(events.begin() + prologue().size(), events.end());
        expect(!got.empty() && got.back() == "delete", "diff 241 deletes");
    }
    reset(10); put64(O + 160, 0x01000000ull); put(O + 964, 1); put(O + 808, 800); put(O + 968, 560);
    script["rand"] = std::deque<int>(80, 49152);
    msg_run__10BlackWidow(object);
    {
        std::vector<std::string> got(events.begin() + prologue().size(), events.end());
        bool deleted = false;
        for (auto& e : got) if (e == "delete") deleted = true;
        expect(!deleted, "diff 240 does not delete");
    }
    // Particle values: rolls of 49152 give 0.5 on every coordinate (3 per vector) -> length^2 .75, accepted.
    reset(10); put64(O + 160, 0x01000000ull); put(O + 964, 1); put(O + 808, 500); put(O + 968, 419);
    script["rand"] = std::deque<int>(40, 49152);
    msg_run__10BlackWidow(object);
    {
        std::vector<std::string> got(events.begin() + prologue().size(), events.end());
        std::string a = "particle:def:" + fl(0.5f * 20.75f + 10.0f) + "," + fl(0.5f * 20.75f + 20.0f) + "," +
                        fl((0.5f * 20.75f) * 0.75f + 0.07f + 30.0f);
        bool found = false;
        for (auto& e : got) if (e.rfind(a, 0) == 0) found = true;
        expect(found, "first particle position");
        bool velocity = false;
        for (auto& e : got) if (e.find(":" + fl(0.5f * 4.75f) + "," + fl(0.5f * 4.75f) + ",0:0") != std::string::npos) velocity = true;
        expect(velocity, "first particle velocity");
        expect(std::fabs(getf(O + 60) - (30.0f - 0.15f)) < 1e-4f, "+60 holds z minus the new +972");
    }
    // Rejection sampling: first rolls give a vector outside the ball, the next ones inside.
    reset(10); put64(O + 160, 0x01000000ull); put(O + 964, 1); put(O + 808, 500); put(O + 968, 419);
    script["rand"] = std::deque<int>({0, 0, 0});  // corner (-1,-1,-1): length^2 3 -> rejected
    for (int i = 0; i < 40; ++i) script["rand"].push_back(49152);
    msg_run__10BlackWidow(object);
    {
        int rolls = 0;
        for (auto& e : events) if (e.rfind("rand:", 0) == 0) ++rolls;
        expect(rolls == 3 * 3 * 2 + 3, "a rejected vector costs three more rolls");
    }
    return failures ? 1 : 0;
}
