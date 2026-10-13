// Host contract for msg_run__7FireFly. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_run.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static std::string order;
static int every_result, steer_result;
static unsigned rolls[4], roll_index;
static std::vector<int> trig_args;
static float steer_point[3];
static void* steer_self;
static unsigned model_seen[2];
static void* dest_self; static int dest_range;
static int act_arg; static float adv_arg, slide_arg;
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { order += 'n'; model_seen[0] = vslots_b::address_bits(m); }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void* m) { order += 'e'; model_seen[1] = vslots_b::address_bits(m); return every_result; }
extern "C" int objectActivationCheck__8Creaturei(void*, int d) { order += 'o'; act_arg = d; return 9; }
extern "C" void creatureAdvanceAnimation__8Creaturef(void*, float a) { order += 'a'; adv_arg = a; }
extern "C" void slideBack__8Creaturef(void*, float a) { order += 'b'; slide_arg = a; }
extern "C" unsigned eRand__10GameObject(void*) { order += 'r'; return rolls[roll_index++ % 4]; }
extern "C" float icos__Fi(int a) { order += 'c'; trig_args.push_back(a); return 0.5f; }
extern "C" float isin__Fi(int a) { order += 's'; trig_args.push_back(a); return -0.25f; }
extern "C" int steerAndRouteToPosition__8CreatureG6Point3(void* self, const void* p) { order += 'T'; steer_self = self; std::memcpy(steer_point, p, 12); return steer_result; }
extern "C" void resetRouting__8Creature(void*) { order += 'X'; }
extern "C" void getSmartRandomDestination__8Creaturei(void* out, void* self, int range) {
    order += 'G'; dest_self = self; dest_range = range;
    const float v[3] = {1.5f, 2.5f, 3.5f}; std::memcpy(out, v, 12);
}
static unsigned char object[1200];
static unsigned char group[32];
static unsigned char model[4];
static unsigned w(unsigned off) { return vslots_b::read_word(vslots_b::address(object) + off); }
static void setw(unsigned off, unsigned v) { vslots_b::write_word(vslots_b::address(object) + off, v); }
static float fl(unsigned off) { float f; std::memcpy(&f, object + off, 4); return f; }
static void setup(int a, int b, int c, int d) {
    std::memset(object, 0x41, sizeof object);
    setw(36, vslots_b::address_bits(group));
    setw(968, a); setw(972, b); setw(808, c); setw(256, d);
    const float pos[3] = {10.0f, 20.0f, 30.0f}; std::memcpy(object + 16, pos, 12);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 0x1000u);
    const float tgt[3] = {-1.0f, -2.0f, -3.0f}; std::memcpy(object + 976, tgt, 12);
    const float rt[3] = {-4.0f, -5.0f, -6.0f}; std::memcpy(object + 272, rt, 12);
    order.clear(); trig_args.clear(); roll_index = 0;
}
int main() {
    using namespace vslots_b;
    host_note(group); host_note(model); host_note(object);
    write_word(address(group) + 12u, address_bits(model));
    rolls[0] = 1; rolls[1] = 0; rolls[2] = 7; rolls[3] = 8;
    // everyFrame non-zero: only the first two calls
    every_result = 1; setup(0, 0, 100, 0);
    msg_run__7FireFly(object);
    expect(order == "ne" && model_seen[0] == address_bits(model) && model_seen[1] == address_bits(model), "early return");
    every_result = 0;
    // timer not due (B >= C), steer not arrived, D >= C: no reroute; toggle zero -> steer uses R
    steer_result = 0; setup(0, 100, 100, 100);
    msg_run__7FireFly(object);
    expect(order == "neoabT" && act_arg == 600 && adv_arg == 1.0f && slide_arg == 1.0f, "call order, constants");
    expect(steer_self == object && steer_point[0] == -4.0f && steer_point[1] == -5.0f && steer_point[2] == -6.0f, "steer toward R");
    expect(w(972) == 100 && w(968) == 0, "no timer change");
    expect(fl(1112) == 10.0f && fl(1116) == 20.0f && fl(1120) == 30.0f, "position copy");
    // toggle one, not due: steer uses W
    setup(1, 100, 100, 100);
    msg_run__7FireFly(object);
    expect(steer_point[0] == -1.0f && steer_point[2] == -3.0f, "steer toward W");
    // due with toggle zero, roll low bit set (heading - 8192)
    setup(0, 50, 100, 1000); rolls[0] = 1;
    msg_run__7FireFly(object);
    expect(order == "neoabrcsT", "wander build order");
    expect(trig_args.size() == 2 && trig_args[0] == 0x1000 - 8192 && trig_args[1] == 0x1000 - 8192, "heading minus");
    expect(fl(976) == 10.0f + 0.5f * 100.0f && fl(980) == 20.0f + -0.25f * 100.0f && fl(984) == 30.0f, "W built");
    expect(w(972) == 120 && w(968) == 1, "due C+20, toggle flipped");
    expect(steer_point[0] == 60.0f, "steer toward the new W (toggle now one)");
    // low bit clear: heading + 8192, wrapping to 16 bits
    setup(0, 50, 100, 1000); rolls[0] = 2; write_u16(address(object) + 56u, 0x7fffu);
    msg_run__7FireFly(object);
    expect(trig_args.size() == 2 && trig_args[0] == static_cast<short>(0x7fff + 8192), "heading plus wraps");
    // due with toggle one: C+30, toggle cleared, steer toward R
    setup(1, 50, 100, 1000);
    msg_run__7FireFly(object);
    expect(order == "neoabT" && w(972) == 130 && w(968) == 0 && steer_point[0] == -4.0f, "due with toggle one");
    // arrived: reroute
    steer_result = 1; setup(0, 100, 100, 1000); rolls[0] = 8;
    msg_run__7FireFly(object);
    expect(order == "neoabTXGr" && dest_self == object && dest_range == 300, "reroute order");
    expect(fl(272) == 1.5f && fl(276) == 2.5f && fl(280) == 3.5f, "R updated");
    expect(w(256) == 100 + 8 % 3 + 300, "D updated");
    // not arrived but D < C: reroute too
    steer_result = 0; setup(0, 100, 100, 99); rolls[0] = 0xfffffffeu;
    msg_run__7FireFly(object);
    expect(order == "neoabTXGr" && w(256) == 100 + (0xfffffffeu % 3u) + 300, "D < C reroute, unsigned remainder");
    // D == C: no reroute
    setup(0, 100, 100, 100);
    msg_run__7FireFly(object);
    expect(order == "neoabT", "D == C no reroute");
    return failures ? 1 : 0;
}
