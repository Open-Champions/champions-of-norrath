// Build with -DNORRATH_W10AC_HOST (32-bit address words become offsets from
// norrath_w10ac_host_base; vtable function words map through norrath_w10ac_host_function).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void setState__6Badgeri(void*, int);
unsigned char* norrath_w10ac_host_base;
static unsigned char memory[0x4000];
static unsigned char* object;
static int failures = 0;
static std::vector<std::string> events;
static int processing_result = 0;
static int processing_replace = -1;
static int roll = 0;
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
extern "C" void animGlobalSync__FP14AnimationState(void* s) { events.push_back("sync:" + off(s)); }
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* position, int yaw, float a, float b,
                                                          float c, float d, float e, float f, char* text) {
    float p[3];
    std::memcpy(p, position, 12);
    events.push_back("drop:" + off(self) + ":" + fl(p[0]) + "," + fl(p[1]) + "," + fl(p[2]) + ":" + std::to_string(yaw) + ":" +
                     fl(a) + "," + fl(b) + "," + fl(c) + "," + fl(d) + "," + fl(e) + "," + fl(f) + ":" + (text ? "?" : "null"));
}
extern "C" int eRand__10GameObject(void*) { events.push_back("rand:" + std::to_string(roll)); return roll; }
static void fake_slot(void* self) { events.push_back("slot192:" + off(self)); }
extern "C" void* norrath_w10ac_host_function(unsigned) { return reinterpret_cast<void*>(&fake_slot); }

static void put(unsigned o, unsigned v) { std::memcpy(memory + o, &v, 4); }
static void put16(unsigned o, unsigned v) { std::memcpy(memory + o, &v, 2); }
static void putf(unsigned o, float v) { std::memcpy(memory + o, &v, 4); }
static unsigned get(unsigned o) { unsigned v; std::memcpy(&v, memory + o, 4); return v; }
static unsigned get16(unsigned o) { unsigned v = 0; std::memcpy(&v, memory + o, 2); return v; }
static void reset(int current = 0) {
    std::memset(memory, 0, sizeof memory);
    events.clear();
    processing_result = 0; processing_replace = -1; roll = 0;
    object = memory + O;
    put(O + 36, T);
    for (unsigned i = 0; i < 40; ++i) put(T + 4 * i, 0x1000 + 0x10 * i);  // header words: 0x1000 + 16 * index
    put(O + 72, 0x2000);       // vtable
    put16(0x2000 + 192, 0x20); // adjustment
    put(0x2000 + 196, 1);      // function word
    put16(O + 572, static_cast<unsigned>(current));
    put(O + 808, 1000);
}
static std::string hdr(unsigned byte_offset) { return std::to_string(0x1000 + 0x10 * (byte_offset / 4)); }
static std::string tr(unsigned off_, const char* t, int f) {
    return "trans:" + std::to_string(ANIM) + ":" + hdr(off_) + ":" + t + ":" + std::to_string(f);
}
static std::string shot(unsigned off_, int f) {
    return "shot:" + std::to_string(ANIM) + ":" + hdr(off_) + ":0.1:0.1:" + std::to_string(f);
}
static void state_is(int expected, const char* message) { expect(static_cast<int>(get16(O + 572)) == (expected & 0xffff), message); }
int main() {
    norrath_w10ac_host_base = memory;
    std::string proc = "proc:" + std::to_string(O) + ":";
    // Processing returns nonzero: nothing else happens, the state halfword is untouched.
    reset(77); processing_result = 1; setState__6Badgeri(object, 5);
    expect(events.size() == 1 && get16(O + 572) == 77, "nonzero processing result returns at once");
    // State 0.
    reset(); setState__6Badgeri(object, 0);
    expect(events == std::vector<std::string>({proc + "0", tr(132, "0.1", 3)}), "state 0 calls");
    expect(get(O + 256) == 1180, "state 0 +256"); state_is(0, "state 0 stored");
    // State 1.
    reset(); setState__6Badgeri(object, 1);
    expect(events == std::vector<std::string>({proc + "1", "reset:" + std::to_string(O), "dest:" + std::to_string(O) + ":200",
                                               tr(148, "0.1", 2051), "sync:" + std::to_string(ANIM)}), "state 1 calls");
    expect(memory[O + 272] == 0x40 && memory[O + 283] == 0x4b, "destination copied to +272"); state_is(1, "state 1 stored");
    // State 3 for several current states.
    for (int cur : {0, 5, 7, 9, 2}) {
        reset(cur); setState__6Badgeri(object, 3);
        expect(events == std::vector<std::string>({proc + "3", "reset:" + std::to_string(O), tr(76, "0.2", 2051),
                                                   "sync:" + std::to_string(ANIM)}), "state 3 transitions");
    }
    for (int cur : {3, 6, 13, 14, 15, 16}) {
        reset(cur); setState__6Badgeri(object, 3);
        expect(events == std::vector<std::string>({proc + "3", "reset:" + std::to_string(O), "sync:" + std::to_string(ANIM)}),
               "state 3 with a transition-free current state");
        state_is(3, "state 3 stored");
    }
    // States 5 and 9.
    for (int s : {5, 9}) {
        reset(); setState__6Badgeri(object, s);
        expect(events == std::vector<std::string>({proc + std::to_string(s), tr(132, "0.1", 3)}), "state 5/9 calls");
        state_is(s, "state 5/9 stored");
    }
    // One-shot states.
    struct Shot { int s; unsigned hdr; int flags; } shots[] = {{6, 36, 2056}, {14, 44, 18440}, {15, 60, 18440}, {16, 52, 2056}, {13, 84, 2056}};
    for (auto& sh : shots) {
        reset(); put(O + 960, 0xdeadbeef); setState__6Badgeri(object, sh.s);
        expect(events == std::vector<std::string>({proc + std::to_string(sh.s), shot(sh.hdr, sh.flags)}), "one-shot calls");
        expect(get(O + 960) == 0, "one-shot clears +960"); state_is(sh.s, "one-shot state stored");
    }
    // State 7.
    reset(); setState__6Badgeri(object, 7);
    expect(events == std::vector<std::string>({proc + "7", "reset:" + std::to_string(O), tr(68, "0.1", 2051), "sync:" + std::to_string(ANIM)}),
           "state 7 calls");
    expect(get(O + 256) == 1060, "state 7 +256"); state_is(7, "state 7 stored");
    // State 8.
    reset(0); setState__6Badgeri(object, 8);
    expect(events == std::vector<std::string>({proc + "8", shot(108, 16)}), "state 8 one-shot");
    state_is(8, "state 8 stored");
    reset(6); put16(O + 294, 8); setState__6Badgeri(object, 8);
    expect(events.size() == 1, "state 8 from 6 with bit 3 does nothing"); state_is(6, "state 8 keeps state 6");
    reset(6); put16(O + 294, 7); setState__6Badgeri(object, 8);
    expect(events.size() == 2 && events[1] == shot(108, 16), "state 8 from 6 without bit 3 plays the one-shot");
    reset(9); setState__6Badgeri(object, 8);
    expect(events.size() == 1, "state 8 from 9 does nothing"); state_is(9, "state 8 keeps state 9");
    reset(0xfffe); setState__6Badgeri(object, 8);
    expect(events.size() == 2, "state 8 from other"); state_is(8, "state 8 stored");
    // State 10.
    for (int r : {0, 1, 2, 3}) {
        reset(); roll = r; put(O + 12, 0xffffffffu); putf(O + 16, 1.5f); putf(O + 20, 2.5f); putf(O + 24, 3.5f); put16(O + 56, 0xfff0);
        setState__6Badgeri(object, 10);
        std::vector<std::string> want = {proc + "10", "drop:" + std::to_string(O) + ":1.5,2.5,3.5:-16:36,2,2.8,0,3,0.25:null",
            "rand:" + std::to_string(r), "trans:" + std::to_string(ANIM) + ":" + hdr(92 + 8 * (r & 1)) + ":0.1:6",
            "slot192:" + std::to_string(O + 0x20)};
        expect(events == want, "state 10 calls");
        expect(get(O + 12) == 0xffffffffu, "bit 1 set keeps the flags word"); state_is(10, "state 10 stored");
    }
    reset(); put(O + 12, 0xfffffffdu); setState__6Badgeri(object, 10);
    expect(get(O + 12) == 0xfffffffcu, "bit 1 clear drops bit 0");
    // Do-nothing states and unknown values; the low 16 bits are stored.
    for (int s : {2, 4, 11, 12, 17, 100, -1, 0x12345}) {
        reset(); setState__6Badgeri(object, s);
        expect(events.size() == 1, "do-nothing state"); state_is(s, "do-nothing state stored");
    }
    // The processing call may change the state.
    reset(); processing_replace = 5; setState__6Badgeri(object, 12);
    expect(events.size() == 2 && events[1] == tr(132, "0.1", 3), "the replaced state selects the case");
    state_is(5, "the replaced state is stored");
    return failures ? 1 : 0;
}
