// Build with -DNORRATH_W10AC_HOST (32-bit pointer words become offsets from
// norrath_w10ac_host_base).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void msg_run__8Catapult(void*);
unsigned char* norrath_w10ac_host_base;
static unsigned char memory[0x4000];
static unsigned char* object;
static int failures = 0;
static std::vector<std::string> events;
static int active_result = 1;
static int script_result = 1;
static bool alloc_fail = false;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - memory); }
static std::string fl(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" int objectActiveProcessing__FP10GameObjectii(void* o, int a, int b) {
    events.push_back("active:" + off(o) + ":" + std::to_string(a) + ":" + std::to_string(b));
    return active_result;
}
extern "C" int scriptStart__FPc(const char* name) {
    events.push_back(std::string("script:") + name);
    return script_result;
}
extern "C" float icos__Fi(int a) { events.push_back("icos:" + std::to_string(a)); return 3.0f; }
extern "C" float isin__Fi(int a) { events.push_back("isin:" + std::to_string(a)); return 5.0f; }
extern "C" unsigned char* norrath_block_alloc(int n) {
    events.push_back("alloc:" + std::to_string(n));
    return alloc_fail ? nullptr : memory + 0x3000;
}
extern "C" void __11FireBombletG6Point3sPcT1P6Point3i(void* self, const void* position, short yaw, char* text,
                                                      const void* second, void* velocity, int flag) {
    float p[3], q[3], v[3];
    std::memcpy(p, position, 12);
    std::memcpy(q, second, 12);
    std::memcpy(v, velocity, 12);
    events.push_back("fire:" + off(self) + ":" + fl(p[0]) + "," + fl(p[1]) + "," + fl(p[2]) + ":" + std::to_string(yaw) +
                     ":" + (text == nullptr ? "null" : "text") + ":" + fl(q[0]) + "," + fl(q[1]) + "," + fl(q[2]) + ":" +
                     fl(v[0]) + "," + fl(v[1]) + "," + fl(v[2]) + ":" + std::to_string(flag));
}
extern "C" void setHold__11FireBombleti(void* b, int hold) { events.push_back("hold:" + off(b) + ":" + std::to_string(hold)); }
extern "C" void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* s, void* p, void* a, short* b) {
    events.push_back("anim:" + off(s) + ":" + off(p) + ":" + (a == nullptr ? "n" : "?") + (b == nullptr ? "n" : "?"));
}
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* position, short yaw, void* state, int a, void* out_a, void* out_b, float scale) {
    float p[3];
    std::memcpy(p, position, 12);
    events.push_back("attach:" + off(model) + ":" + fl(p[0]) + ":" + std::to_string(yaw) + ":" + off(state) + ":" +
                     std::to_string(a) + ":" + off(out_a) + ":" + off(out_b) + ":" + fl(scale));
}
static void put(unsigned o, unsigned v) { std::memcpy(memory + o, &v, 4); }
static void putf(unsigned o, float v) { std::memcpy(memory + o, &v, 4); }
static unsigned get(unsigned o) { unsigned v; std::memcpy(&v, memory + o, 4); return v; }
static const unsigned O = 0x10;
static void reset() {
    std::memset(memory, 0, sizeof memory);
    events.clear();
    active_result = 1; script_result = 1; alloc_fail = false;
    object = memory + O;
    putf(O + 16, 10.0f); putf(O + 20, 20.0f); putf(O + 24, 30.0f);
    put(O + 36, 0x400); put(0x400 + 12, 0x500);
    putf(O + 240, 4.0f);
    int16_t yaw = 100; std::memcpy(memory + O + 56, &yaw, 2);
}
int main() {
    norrath_w10ac_host_base = memory;
    // Byte +87 returns immediately.
    reset(); memory[O + 87] = 1;
    msg_run__8Catapult(object);
    expect(events.empty(), "byte 87 stops before any call");
    // Activity result stored in byte 76; zero low byte stops.
    reset(); memory[O + 76] = 0x85; active_result = 0x100;
    msg_run__8Catapult(object);
    expect(events.size() == 1 && events[0] == "active:" + std::to_string(O) + ":-123:1600" && memory[O + 76] == 0,
           "activity call with sign-extended byte; result stored; zero stops");
    // Countdown.
    reset(); put(O + 244, 5);
    msg_run__8Catapult(object);
    expect(events.size() == 1 && get(O + 244) == 4, "countdown decremented and stops");
    // Launch: distance below 2.1, no script.
    reset(); putf(O + 124, 1.0f);
    msg_run__8Catapult(object);
    std::vector<std::string> want = {"active:" + std::to_string(O) + ":0:1600", "icos:100", "isin:100", "alloc:184",
        "fire:12288:10,20,30:-32668:null:10,20,130:12,20,2:1", "hold:12288:1",
        "attach:1280:10:100:108:0:12304:244:1"};
    expect(events == want, "launch sequence");
    if (events != want) for (auto& e : events) std::fprintf(stderr, "  got %s\n", e.c_str());
    expect(memory[O + 84] == 1 && memory[O + 78] == 1 && memory[O + 85] == 1 && get(O + 88) == 0x3000, "launch state");
    // Launch with a script that fails: nothing launched, +85 cleared, +84 set.
    reset(); putf(O + 124, 1.0f); put(O + 80, 0x700); std::memcpy(memory + 0x700, "scr", 4); script_result = 0; memory[O + 85] = 1;
    msg_run__8Catapult(object);
    expect(events.size() == 2 && events[1] == "script:scr" && memory[O + 84] == 1 && memory[O + 85] == 0 && memory[O + 78] == 0 && get(O + 88) == 0,
           "failed script");
    // Script succeeds: launch proceeds.
    reset(); putf(O + 124, 1.0f); put(O + 80, 0x700); std::memcpy(memory + 0x700, "scr", 4); script_result = 1;
    msg_run__8Catapult(object);
    expect(events.size() == 8 && events[1] == "script:scr", "script then launch");
    // Allocation failure: held word becomes 0, flags still set, no hold call.
    reset(); putf(O + 124, 1.0f); alloc_fail = true; put(O + 88, 0x1111);
    msg_run__8Catapult(object);
    expect(get(O + 88) == 0 && memory[O + 78] == 1 && memory[O + 85] == 1 && events.size() == 4, "allocation failure");
    // Distance exactly at the launch constant: no launch.
    reset(); putf(O + 124, 2.10017991065979f);
    msg_run__8Catapult(object);
    expect(events.size() == 1 && memory[O + 84] == 0, "launch constant is strict");
    // Release: flag 78 set, distance beyond 158.58.
    reset(); memory[O + 78] = 1; memory[O + 84] = 1; put(O + 88, 0x1000); putf(O + 124, 200.0f);
    msg_run__8Catapult(object);
    expect(events.size() == 2 && events[1] == "hold:4096:0" && get(O + 88) == 0 && memory[O + 78] == 0 && memory[O + 84] == 0,
           "release clears the hold");
    // Release with only +84 set: only +84 cleared.
    reset(); memory[O + 84] = 1; putf(O + 124, 200.0f);
    msg_run__8Catapult(object);
    expect(events.size() == 1 && memory[O + 84] == 0, "only +84 cleared");
    // Within range: nothing changes.
    reset(); memory[O + 84] = 1; memory[O + 78] = 1; putf(O + 124, 100.0f);
    msg_run__8Catapult(object);
    expect(events.size() == 1 && memory[O + 84] == 1 && memory[O + 78] == 1, "in range keeps state");
    // Animation and attachment.
    reset(); memory[O + 96] = 0xff; put(O + 88, 0x1000); putf(O + 124, 100.0f); memory[O + 84] = 1;
    msg_run__8Catapult(object);
    expect(events.size() == 3 && events[1] == "anim:" + std::to_string(O + 92) + ":" + std::to_string(O + 16) + ":nn" &&
               events[2] == "attach:1280:10:100:" + std::to_string(O + 92) + ":0:" + std::to_string(0x1000 + 16) + ":" +
                                std::to_string(O + 228) + ":1",
           "animation advance then attachment position");
    return failures ? 1 : 0;
}
