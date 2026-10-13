// Build with -DNORRATH_W10AC_HOST (32-bit address words become offsets from
// norrath_w10ac_host_base; vtable function words map through norrath_w10ac_host_function).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void setState__10BlackWidowi(void*, int);
unsigned char* norrath_w10ac_host_base;
static unsigned char memory[0x4000];
static unsigned char* object;
static int failures = 0;
static std::vector<std::string> events;
static int processing_result = 0;
static int processing_replace = -1;
static std::vector<unsigned> rolls;
static size_t roll_index = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - memory); }
static std::string fl(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static const unsigned O = 0x10, T = 0x800, ANIM = O + 288;
extern "C" int setStateProcessing__8CreaturePi(void* self, int* state) {
    events.push_back("proc:" + off(self) + ":" + std::to_string(*state));
    if (processing_replace >= 0) *state = processing_replace;
    return processing_result;
}
extern "C" void resetRouting__8Creature(void* self) { events.push_back("reset:" + off(self)); }
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void* self, int distance) {
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(result)[i] = static_cast<unsigned char>(0x40 + i);
    events.push_back("dest:" + off(self) + ":" + std::to_string(distance));
}
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float t, int f) {
    events.push_back("trans:" + off(s) + ":" + off(h) + ":" + fl(t) + ":" + std::to_string(f));
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    events.push_back("shot:" + off(s) + ":" + off(h) + ":" + fl(a) + ":" + fl(b) + ":" + std::to_string(f));
}
extern "C" void animSetToEnd__FP14AnimationState(void* s) { events.push_back("toend:" + off(s)); }
extern "C" void reset__3Web(void* w) { events.push_back("web:" + off(w)); }
extern "C" void loseBodyPart__10BlackWidowi(void* self, int part) { events.push_back("lose:" + off(self) + ":" + std::to_string(part)); }
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* position, int yaw, float a, float b,
                                                          float c, float d, float e, float f, const char* text) {
    float p[3];
    std::memcpy(p, position, 12);
    events.push_back("drop:" + off(self) + ":" + fl(p[0]) + "," + fl(p[1]) + "," + fl(p[2]) + ":" + std::to_string(yaw) + ":" +
                     fl(a) + "," + fl(b) + "," + fl(c) + "," + fl(d) + "," + fl(e) + "," + fl(f) + ":" + (text ? text : "null"));
}
extern "C" int eRand__10GameObject(void*) {
    unsigned v = roll_index < rolls.size() ? rolls[roll_index] : 0;
    ++roll_index;
    events.push_back("rand:" + std::to_string(v));
    return static_cast<int>(v);
}
static void fake_slot(void* self) { events.push_back("slot192:" + off(self)); }
extern "C" void* norrath_w10ac_host_function(unsigned) { return reinterpret_cast<void*>(&fake_slot); }

static void put(unsigned o, unsigned v) { std::memcpy(memory + o, &v, 4); }
static void put16(unsigned o, unsigned v) { std::memcpy(memory + o, &v, 2); }
static void put64(unsigned o, unsigned long long v) { std::memcpy(memory + o, &v, 8); }
static unsigned get(unsigned o) { unsigned v; std::memcpy(&v, memory + o, 4); return v; }
static unsigned long long get64(unsigned o) { unsigned long long v; std::memcpy(&v, memory + o, 8); return v; }
static unsigned get16(unsigned o) { unsigned v = 0; std::memcpy(&v, memory + o, 2); return v; }
static float getf(unsigned o) { float v; std::memcpy(&v, memory + o, 4); return v; }
static void reset(int current = 0) {
    std::memset(memory, 0, sizeof memory);
    events.clear();
    processing_result = 0; processing_replace = -1; rolls.clear(); roll_index = 0;
    object = memory + O;
    put(O + 36, T);
    for (unsigned i = 0; i < 40; ++i) put(T + 4 * i, 0x1000 + 0x10 * i);
    put(O + 72, 0x2000);
    put16(0x2000 + 192, 0x20);
    put(0x2000 + 196, 1);
    put16(O + 572, static_cast<unsigned>(current));
    put(O + 808, 1000);
    put64(O + 160, 0xffffffffffffffffull);
    put(O + 12, 0xffffffffu);
}
static std::string hdr(unsigned byte_offset) { return std::to_string(0x1000 + 0x10 * (byte_offset / 4)); }
static std::string tr(unsigned off_, const char* t, int f) {
    return "trans:" + std::to_string(ANIM) + ":" + hdr(off_) + ":" + t + ":" + std::to_string(f);
}
static std::string shot(unsigned off_, const char* a, const char* b, int f) {
    return "shot:" + std::to_string(ANIM) + ":" + hdr(off_) + ":" + a + ":" + b + ":" + std::to_string(f);
}
static void state_is(int expected, const char* message) { expect(static_cast<int>(get16(O + 572)) == (expected & 0xffff), message); }
int main() {
    norrath_w10ac_host_base = memory;
    std::string proc = "proc:" + std::to_string(O) + ":";
    std::string self = std::to_string(O);
    // Nonzero processing result: nothing changes.
    reset(77); processing_result = 1; setState__10BlackWidowi(object, 5);
    expect(events.size() == 1 && get16(O + 572) == 77 && getf(O + 176) == 0.0f && get(O + 12) == 0xffffffffu, "processing result stops everything");
    // Any state but 3 sets the float at +176 to 140; the tail clears bit 11 of +160 and bit 0 of +12.
    reset(); setState__10BlackWidowi(object, 2);
    expect(getf(O + 176) == 140.0f && get64(O + 160) == 0xfffffffffffff7ffull && get(O + 12) == 0xfffffffeu, "common state tail");
    state_is(2, "state 2 stored");
    // 16 skips the tail clears; 10 skips only the +12 clear.
    reset(); setState__10BlackWidowi(object, 16);
    expect(get64(O + 160) == 0xffffffffffffffffull && get(O + 12) == 0xffffffffu, "state 16 keeps both words");
    reset(); setState__10BlackWidowi(object, 10);
    expect(get64(O + 160) == 0xfffffffffffff7ffull && get(O + 12) == 0xffffffffu, "state 10 clears only +160 bit 11");
    // The tail test uses the low 16 bits, signed.
    reset(); setState__10BlackWidowi(object, 0x10010);
    expect(get64(O + 160) == 0xffffffffffffffffull && get(O + 12) == 0xffffffffu, "0x10010 acts as 16");
    state_is(0x10010, "low half stored");
    // State 0.
    reset(); rolls = {125}; setState__10BlackWidowi(object, 0);
    expect(events == std::vector<std::string>({proc + "0", "rand:125", tr(116, "0.3", 3)}), "state 0 calls");
    expect(get(O + 256) == 1000 + 5 + 120, "state 0 +256 uses roll % 60");
    // State 1.
    reset(); setState__10BlackWidowi(object, 1);
    expect(events == std::vector<std::string>({proc + "1", "reset:" + self, "dest:" + self + ":200", tr(140, "0.3", 3)}), "state 1 calls");
    expect(memory[O + 272] == 0x40 && memory[O + 283] == 0x4b, "destination copied");
    // State 3 does not set +176 before the switch, sets it itself.
    reset(); rolls = {400}; setState__10BlackWidowi(object, 3);
    expect(events == std::vector<std::string>({proc + "3", "rand:400", "reset:" + self, tr(140, "0.3", 3)}), "state 3 calls");
    expect(getf(O + 176) == 140.0f && get(O + 960) == 1000 + 40 + 180, "state 3 sets +176 and +960 (roll % 180)");
    // State 5 and 9: skipped when the current state is 16.
    reset(0); setState__10BlackWidowi(object, 5);
    expect(events == std::vector<std::string>({proc + "5", tr(124, "0.1", 3)}), "state 5");
    reset(16); setState__10BlackWidowi(object, 5);
    expect(events.size() == 1, "state 5 skipped when current is 16");
    reset(0); setState__10BlackWidowi(object, 9);
    expect(events == std::vector<std::string>({proc + "9", tr(116, "0.3", 3)}), "state 9");
    reset(16); setState__10BlackWidowi(object, 9);
    expect(events.size() == 1, "state 9 from 16 does nothing"); state_is(16, "state 9 from 16 stores 16");
    // State 6.
    reset(); rolls = {31}; setState__10BlackWidowi(object, 6);
    expect(events == std::vector<std::string>({proc + "6", tr(124, "0.1", 3), shot(52, "0.01", "0.01", 8), "rand:31"}), "state 6 calls");
    expect(get(O + 256) == 1 + 15, "state 6 +256 = roll % 15 + 15");
    // State 15.
    reset(); rolls = {44}; put(O + 964, 0); setState__10BlackWidowi(object, 15);
    expect(events == std::vector<std::string>({proc + "15", tr(124, "0.1", 3), shot(36, "0.01", "0.1", 8), "rand:44"}), "state 15 calls");
    expect(get(O + 964) == 1 && get(O + 256) == 14 + 15, "state 15 words");
    // State 13.
    reset(); put(O + 256, 99); setState__10BlackWidowi(object, 13);
    expect(events == std::vector<std::string>({proc + "13", "web:" + std::to_string(O + 976), tr(124, "0.1", 3), shot(132, "0.1", "0.1", 8)}),
           "state 13 calls");
    expect(get(O + 256) == 0, "state 13 clears +256");
    // State 14.
    reset(); setState__10BlackWidowi(object, 14);
    expect(events == std::vector<std::string>({proc + "14", tr(60, "0.1", 3)}) && get(O + 960) == 1300, "state 14");
    // State 8.
    for (int cur : {9, 16}) {
        reset(cur); setState__10BlackWidowi(object, 8);
        expect(events.size() == 1, "state 8 from 9/16 does nothing"); state_is(cur, "state 8 keeps the current state");
    }
    reset(6); put16(O + 294, 8); setState__10BlackWidowi(object, 8);
    expect(events.size() == 1, "state 8 from 6 with bit 3"); state_is(6, "state 8 keeps 6");
    reset(6); put16(O + 294, 7); setState__10BlackWidowi(object, 8);
    expect(events.size() == 2 && events[1] == shot(108, "0.1", "0.1", 16), "state 8 from 6 without bit 3 plays the one-shot");
    state_is(8, "state 8 stored");
    reset(1); setState__10BlackWidowi(object, 8);
    expect(events.size() == 2 && events[1] == shot(108, "0.1", "0.1", 16), "state 8 from 1");
    // State 10.
    for (unsigned flags : {0u, 2u}) {
        reset(); put(O + 12, flags); put(O + 964, 77);
        float p[3] = {1.5f, 2.5f, 3.5f}; std::memcpy(memory + O + 16, p, 12); put16(O + 56, 0xfff0);
        rolls = {0x0f, 9, 7};  // lose part (15 & 7 = 7); 9 % 3 == 0 so part 8; pick = 7 & 3 = 3
        setState__10BlackWidowi(object, 10);
        std::vector<std::string> want = {proc + "10", "rand:15", "lose:" + self + ":7", "rand:9", "lose:" + self + ":8",
            "drop:" + self + ":1.5,2.5,3.5:-16:36,2,1.6,0.6,1.4,0.25:venomsac", "rand:7", tr(68 + 8 * 3, "0.3", 6)};
        if (flags & 2) want.push_back("toend:" + std::to_string(ANIM));
        want.push_back("slot192:" + std::to_string(O + 0x20));
        expect(events == want, "state 10 calls");
        if (events != want) for (auto& e : events) std::fprintf(stderr, "  got %s\n", e.c_str());
        expect(get(O + 964) == 0, "state 10 clears +964");
    }
    reset(); rolls = {8, 10, 4}; setState__10BlackWidowi(object, 10);
    expect(events[1] == "rand:8" && events[2] == "lose:" + self + ":0" && events[3] == "rand:10" && events[4].rfind("drop:", 0) == 0,
           "no second part unless the second roll is divisible by 3");
    // Do-nothing states and the replaced-state path.
    for (int s : {4, 7, 11, 12, 17, 100, -1}) {
        reset(); setState__10BlackWidowi(object, s);
        expect(events.size() == 1, "do-nothing state"); state_is(s, "do-nothing state stored");
    }
    reset(); processing_replace = 14; setState__10BlackWidowi(object, 12);
    expect(events.size() == 2 && events[1] == tr(60, "0.1", 3), "the replaced state selects the case");
    return failures ? 1 : 0;
}
