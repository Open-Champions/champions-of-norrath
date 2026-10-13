// Build with -DNORRATH_W10AC_HOST (32-bit pointer words become offsets from
// norrath_w10ac_host_base; the reciprocal square root uses the host square root).
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void msg_hurt__8CatapultfP10GameObjecti(void*, float, void*, int);
unsigned char* norrath_w10ac_host_base;
extern "C" signed char cutMode = 0;
static unsigned char memory[0x4000];
static unsigned char* object;
static int failures = 0;
static std::vector<std::string> events;
static bool tag_reduce = false, tag_death = false;
static int tag_int = 0;
static int record_count = 0;
static std::vector<int> rolls;
static size_t roll_index = 0;
static std::vector<bool> allocs;
static size_t alloc_index = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - memory); }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { events.push_back("runlist:" + off(o)); }
static char reduce_text[] = "reduceCarnage";
static char death_text[] = "deathScript";
extern "C" char* findTagString__10GameObjectPci(void*, const char* tag, int index) {
    events.push_back(std::string("tagstr:") + tag + ":" + std::to_string(index));
    if (std::string(tag) == "reduceCarnage") return tag_reduce ? reduce_text : nullptr;
    return tag_death ? death_text : nullptr;
}
extern "C" int findTagInt__10GameObjectPci(void*, const char* tag, int index) {
    events.push_back(std::string("tagint:") + tag + ":" + std::to_string(index));
    return tag_int;
}
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    events.push_back(std::string("lump:") + lump + ":" + name);
    memory[0x3800 + 18] = static_cast<unsigned char>(record_count);
    return memory + 0x3800;
}
extern "C" int eRand__10GameObject(void*) {
    int v = roll_index < rolls.size() ? rolls[roll_index] : 0;
    ++roll_index;
    events.push_back("rand:" + std::to_string(v));
    return v;
}
extern "C" unsigned char* norrath_block_alloc(int n) {
    bool ok = alloc_index < allocs.size() ? allocs[alloc_index] : true;
    ++alloc_index;
    events.push_back("alloc:" + std::to_string(n) + (ok ? ":ok" : ":null"));
    return ok ? memory + 0x3000 : nullptr;
}
extern "C" void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const void* position, const char* lump,
                                                                 const char* model, const char* texture, long mask,
                                                                 int kind, void* description, float scale) {
    float p[3];
    std::memcpy(p, position, 12);
    events.push_back("prop:" + off(self) + ":" + std::to_string(static_cast<int>(p[0])) + ":" + lump + ":" + model + ":" +
                     texture + ":" + std::to_string(mask) + ":" + std::to_string(kind) + ":" +
                     (description == nullptr ? "null" : "?") + ":" + std::to_string(scale));
}
extern "C" void setOnFire__6PhPropi(void* p, int on) { events.push_back("fire:" + off(p) + ":" + std::to_string(on)); }
extern "C" void setHold__11FireBombleti(void* b, int h) { events.push_back("hold:" + off(b) + ":" + std::to_string(h)); }
extern "C" void detach__11FireBomblet(void* b) { events.push_back("detach:" + off(b)); }
extern "C" int scriptStart__FPc(const char* name) { events.push_back(std::string("script:") + name); return 0; }
extern "C" void objectAddToDeleteList__FP10GameObject(void* o) { events.push_back("delete:" + off(o)); }

static void put(unsigned o, unsigned v) { std::memcpy(memory + o, &v, 4); }
static void putf(unsigned o, float v) { std::memcpy(memory + o, &v, 4); }
static float getf(unsigned o) { float v; std::memcpy(&v, memory + o, 4); return v; }
static unsigned get(unsigned o) { unsigned v; std::memcpy(&v, memory + o, 4); return v; }
static const unsigned O = 0x10, OTHER = 0x800, HELD = 0x1000;
static void reset() {
    std::memset(memory, 0, sizeof memory);
    events.clear();
    tag_reduce = tag_death = false; tag_int = 0; record_count = 0;
    rolls.clear(); roll_index = 0; allocs.clear(); alloc_index = 0; cutMode = 0;
    object = memory + O;
    putf(O + 16, 10.0f); putf(O + 20, 20.0f); putf(O + 24, 30.0f);
    putf(OTHER + 16, 10.0f); putf(OTHER + 20, 20.0f); putf(OTHER + 24, 30.0f);
}
static void run() { msg_hurt__8CatapultfP10GameObjecti(object, 0.0f, memory + OTHER, 7); }
int main() {
    norrath_w10ac_host_base = memory;
    // Already hurt: nothing.
    reset(); memory[O + 87] = 1; run();
    expect(events.empty(), "byte 87 set returns at once");
    // Minimal: no pieces, byte 76 zero adds to the run list; death at the end.
    reset(); run();
    std::vector<std::string> want = {"runlist:" + std::to_string(O), "tagstr:reduceCarnage:0",
        "lump:catapult.lmp:catapult_broken.vif", "tagstr:deathScript:0", "delete:" + std::to_string(O)};
    expect(events == want, "minimal sequence");
    expect(memory[O + 87] == 1, "byte 87 set to 1");
    reset(); memory[O + 76] = 3; tag_death = true; run();
    expect(events.size() == 5 && events[0] == "tagstr:reduceCarnage:0" && events[3] == "script:deathScript" &&
               events[4] == "delete:" + std::to_string(O),
           "no run list when +76 is nonzero; death script text is started");
    // Pieces: three, no carnage tag, rolls decide fire (low three bits zero).
    reset(); record_count = 3; memory[O + 76] = 1; rolls = {8, 1, 16}; run();
    want = {"tagstr:reduceCarnage:0", "lump:catapult.lmp:catapult_broken.vif"};
    for (int i = 0; i < 3; ++i) {
        want.push_back("alloc:584:ok");
        want.push_back("prop:" + std::to_string(0x3000) + ":10:catapult.lmp:catapult_broken.vif:catapult_broken.tex:" +
                       std::to_string(1 << i) + ":534:null:1.000000");
        want.push_back("rand:" + std::to_string(rolls[i]));
        if ((rolls[i] & 7) == 0) want.push_back("fire:" + std::to_string(0x3000) + ":1");
    }
    want.push_back("tagstr:deathScript:0");
    want.push_back("delete:" + std::to_string(O));
    expect(events == want, "three pieces");
    if (events != want) for (auto& e : events) std::fprintf(stderr, "  got %s\n", e.c_str());
    // Carnage reduction with a nonzero tag value: an even roll skips the piece (and uses no alloc).
    reset(); record_count = 3; memory[O + 76] = 1; tag_reduce = true; tag_int = 9; rolls = {2, 3, 0, 4, 5, 6, 7, 9}; run();
    int allocs_seen = 0;
    for (auto& e : events) if (e.rfind("alloc:", 0) == 0) ++allocs_seen;
    expect(allocs_seen == 1, "carnage reduction skips pieces whose first roll is even");
    expect(events.size() > 3 && events[1] == "tagint:reduceCarnage:0", "tag value read");
    // Zero tag value means no reduction even though the tag exists.
    reset(); record_count = 2; memory[O + 76] = 1; tag_reduce = true; tag_int = 0; rolls = {1, 1, 1, 1}; run();
    allocs_seen = 0;
    for (auto& e : events) if (e.rfind("alloc:", 0) == 0) ++allocs_seen;
    expect(allocs_seen == 2, "zero tag value does not reduce");
    // Failed allocation: no constructor and no fire roll.
    reset(); record_count = 1; memory[O + 76] = 1; allocs = {false}; run();
    expect(events.size() == 5 && events[2] == "alloc:584:null", "failed allocation skips constructor and roll");
    // Held bomblet released: velocity is the unit vector from the other object, times 4.
    reset(); memory[O + 76] = 1; memory[O + 78] = 1; put(O + 88, HELD);
    putf(O + 16, 13.0f); putf(O + 20, 24.0f); putf(O + 24, 30.0f);
    putf(OTHER + 16, 10.0f); putf(OTHER + 20, 20.0f); putf(OTHER + 24, 30.0f);
    run();
    expect(events.size() >= 4 && events[2] == "hold:" + std::to_string(HELD) + ":0" && events[3] == "detach:" + std::to_string(HELD),
           "hold released and detached");
    expect(get(O + 88) == 0 && memory[O + 78] == 0, "held word and flag cleared");
    expect(std::fabs(getf(HELD + 76) - 2.4f) < 1e-5f && std::fabs(getf(HELD + 80) - 3.2f) < 1e-5f && getf(HELD + 84) == 0.0f,
           "velocity direction times four (3-4-5 triangle)");
    // cutMode nonzero blocks the release.
    reset(); memory[O + 76] = 1; memory[O + 78] = 1; put(O + 88, HELD); cutMode = 1; run();
    expect(events.size() == 4 && get(O + 88) == HELD && memory[O + 78] == 1, "cutMode blocks the release");
    // No held object: flag stays.
    reset(); memory[O + 76] = 1; memory[O + 78] = 1; run();
    expect(memory[O + 78] == 1, "no held object leaves the flag");
    return failures ? 1 : 0;
}
