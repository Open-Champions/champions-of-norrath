// Host contract for msg_run__10FireBeetle. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_run.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

int frameNm;
static std::string log_text;
static std::deque<int> zone_results;
static std::deque<unsigned> rolls;
static int every_result, activation_result, advance_result, exit_result, aim_result;
static bool alloc_ok;
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s (log %s)\n", m, log_text.c_str()); ++failures; } }
static void add(const std::string& t) { log_text += t; log_text += ';'; }
static std::string f2s(float f) { char b[32]; std::snprintf(b, sizeof b, "%g", f); return b; }
static unsigned char object[1300], group[160], model[4], table[256], block[600], header_a[4], header_b[4];
static float lead_in[3], proj_start[3], proj_velocity[3], light_point[3];
static float light_floats[12];
static int proj_ints[6];
static void* light_object; static int light_flag;
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { add("ndh"); expect(m == model, "model"); }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void*) { add("ef"); return every_result; }
extern "C" int objectActivationCheck__8Creaturei(void*, int d) { add("oac" + std::to_string(d)); return activation_result; }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float a) { add("adv" + f2s(a)); return advance_result; }
extern "C" void slideBack__8Creaturef(void*, float a) { add("slide" + f2s(a)); }
extern "C" float icos__Fi(int a) { add("cos" + std::to_string(a)); return 0.5f; }
extern "C" float isin__Fi(int a) { add("sin" + std::to_string(a)); return 0.25f; }
extern "C" int iatan2__Fff(float a, float b) { add("atan" + f2s(a) + "," + f2s(b)); return aim_result; }
extern "C" int enemyInZone__8Creaturefs(void*, float d, short n) {
    const int r = zone_results.empty() ? 0 : zone_results.front(); if (!zone_results.empty()) zone_results.pop_front();
    add("zone" + f2s(d) + "/" + std::to_string(n) + (r ? "+" : "-")); return r;
}
extern "C" void smartWander__8Creature(void*) { add("wander"); }
extern "C" void steerAndRouteToEnemy__8Creature(void*) { add("steerE"); }
extern "C" int steerAndRouteToPosition__8CreatureG6Point3(void*, const void* p) { float v[3]; std::memcpy(v, p, 12); add("steerP" + f2s(v[0]) + "," + f2s(v[1]) + "," + f2s(v[2])); return 0; }
extern "C" void turnToFaceEnemy__8Creature(void*) { add("face"); }
extern "C" int bExitStatePersueIdle__8Creature(void*) { add(std::string("exit") + (exit_result ? "+" : "-")); return exit_result; }
extern "C" void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float s, int a, int b, int c, int d, void* desc_arg) {
    add("melee" + f2s(s) + "/" + std::to_string(a) + "/" + std::to_string(b) + "/" + std::to_string(c) + "/" + std::to_string(d) + (desc_arg == object + 992 ? "/desc" : "/bad"));
}
extern "C" void findEnemyLeadPosition__8CreaturefP6Point3(void*, float d, void* p) {
    float v[3]; std::memcpy(v, p, 12); add("lead" + f2s(d) + "/" + f2s(v[0]) + "," + f2s(v[1]) + "," + f2s(v[2]));
    const float moved[3] = {v[0] + 1.0f, v[1] + 2.0f, v[2] + 4.0f}; std::memcpy(p, moved, 12); std::memcpy(lead_in, moved, 12);
}
extern "C" unsigned eRand__10GameObject(void*) { add("rand"); const unsigned r = rolls.front(); rolls.pop_front(); rolls.push_back(r); return r; }
extern "C" void* blockAlloc__Fi(int size) { add("alloc" + std::to_string(size)); return alloc_ok ? block : nullptr; }
extern "C" void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void* b, const float* p, float p3, float p4, float p5, float p6, float p7, float p8,
                                                                    float p9, float p10, float p11, float p12, float p13, float p14, void* o, int flag) {
    std::memcpy(light_point, p, 12);
    const float all[12] = {p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14}; std::memcpy(light_floats, all, sizeof all);
    light_object = o; light_flag = flag; add(std::string("light") + (b == block ? "/ok" : "/bad"));
}
extern "C" void __16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader(void* b, const void* start, const void* velocity, int heading,
                                                                                  void* creature, int a, int bb, int c, int d, int e, void* header) {
    std::memcpy(proj_start, start, 12); std::memcpy(proj_velocity, velocity, 12);
    proj_ints[0] = heading; proj_ints[1] = a; proj_ints[2] = bb; proj_ints[3] = c; proj_ints[4] = d; proj_ints[5] = e;
    add(std::string("proj") + (b == block && creature == object ? "/ok" : "/bad") + (header == header_a ? "/ha" : "/hx"));
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    add(std::string("sound") + (h == header_b ? "/hb" : "/hx") + (o == object ? "/self/" : "/bad/") + f2s(v));
}
extern "C" void objectAddToSlowRunList__FP10GameObject(void*) { add("slowrun"); }
extern "C" void floorAdd__FG6Point3sfii(const void* p, short angle, float size, int a, int b) {
    float v[3]; std::memcpy(v, p, 12); add("floor" + f2s(v[0]) + "/" + std::to_string(angle) + "/" + f2s(size) + "/" + std::to_string(a) + "/" + std::to_string(b));
}
static void slot_168(void*, int) { add("upd"); }
static void slot_184(void*, int state) { add("st" + std::to_string(state)); }

static unsigned getw(unsigned off) { return vslots_b::read_word(vslots_b::address(object) + off); }
static void setw(unsigned off, unsigned v) { vslots_b::write_word(vslots_b::address(object) + off, v); }
static void setup(int state) {
    std::memset(object, 0x41, sizeof object);
    std::memset(group, 0, sizeof group);
    setw(36, vslots_b::address_bits(group));
    vslots_b::write_word(vslots_b::address(group) + 12u, vslots_b::address_bits(model));
    vslots_b::write_word(vslots_b::address(group) + 132u, vslots_b::address_bits(header_b));
    vslots_b::write_word(vslots_b::address(group) + 140u, vslots_b::address_bits(header_a));
    setw(72, vslots_b::address_bits(table));
    std::memset(table, 0, sizeof table);
    const struct { unsigned off; void* fn; } slots[] = {{168, reinterpret_cast<void*>(slot_168)}, {184, reinterpret_cast<void*>(slot_184)}};
    for (auto& s : slots) { vslots_b::write_u16(vslots_b::address(table) + s.off, 8u); vslots_b::write_word(vslots_b::address(table) + s.off + 4u, vslots_b::address_bits(s.fn)); }
    vslots_b::write_u16(vslots_b::address(object) + 572u, static_cast<unsigned>(state) & 0xffffu);
    const float pos[3] = {10.0f, 20.0f, 30.0f}; std::memcpy(object + 16, pos, 12);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 0x0100u);
    setw(576, 0); setw(256, 5); setw(804, 0); setw(808, 10); setw(12, 0); setw(964, 100); setw(960, 0);
    { const float size = 0.0f; std::memcpy(object + 28, &size, 4); }
    setw(968, 1); setw(972, 100);   // timer not due, toggle set
    vslots_b::write_u16(vslots_b::address(object) + 294u, 0u);
    log_text.clear(); zone_results.clear(); rolls = {7, 8, 9, 10};
    every_result = 0; activation_result = 1; advance_result = 0; exit_result = 0; alloc_ok = true; aim_result = 0;
}
static std::string head() { return "ndh;ef;oac600;adv1;slide0.4;upd;"; }
int main() {
    using namespace vslots_b;
    host_note(group); host_note(model); host_note(table); host_note(object); host_note(block); host_note(header_a); host_note(header_b);
    host_note(reinterpret_cast<void*>(slot_168)); host_note(reinterpret_cast<void*>(slot_184));
    setup(0); every_result = 1; msg_run__10FireBeetle(object); expect(log_text == "ndh;ef;", "everyFrame stops");
    setup(0); activation_result = 0; msg_run__10FireBeetle(object); expect(log_text == "ndh;ef;oac600;", "activation stops");
    for (int s : {2, 4, 7, 11, 12, 15, 100, -1}) { setup(s); msg_run__10FireBeetle(object); expect(log_text == head(), "no-work state"); }
    // timing block
    setup(2); setw(972, 5); setw(808, 10); setw(968, 1); msg_run__10FireBeetle(object);
    expect(log_text == head() && getw(972) == 25 && getw(968) == 0, "timer due, toggle one: +15 and flipped");
    setup(2); setw(972, 5); setw(808, 10); setw(968, 0); rolls = {1, 0, 0, 0}; msg_run__10FireBeetle(object);
    expect(log_text == head() + "rand;cos" + std::to_string(256 - 8192) + ";sin" + std::to_string(256 - 8192) + ";" && getw(972) == 20 && getw(968) == 1, "timer due, toggle zero: wander target, +10");
    {
        float w[3]; std::memcpy(w, object + 976, 12);
        expect(w[0] == 10.0f + 0.5f * 100.0f && w[1] == 20.0f + 0.25f * 100.0f && w[2] == 30.0f + 0.0f * 100.0f, "wander target");
    }
    setup(2); setw(972, 5); setw(808, 10); setw(968, 0); rolls = {2, 0, 0, 0}; msg_run__10FireBeetle(object);
    expect(log_text == head() + "rand;cos" + std::to_string(256 + 8192) + ";sin" + std::to_string(256 + 8192) + ";", "heading plus 8192 for an even roll");
    // state 0 and 1
    setup(0); zone_results = {1}; msg_run__10FireBeetle(object); expect(log_text == head() + "zone400/32767+;st3;", "0: near");
    setup(0); zone_results = {0, 1}; msg_run__10FireBeetle(object); expect(log_text == head() + "zone400/32767-;zone400/18000+;st3;", "0: far");
    setup(0); zone_results = {0, 0}; setw(256, 5); msg_run__10FireBeetle(object); expect(log_text == head() + "zone400/32767-;zone400/18000-;st1;", "0: due");
    setup(0); zone_results = {0, 0}; setw(256, 10); msg_run__10FireBeetle(object); expect(log_text == head() + "zone400/32767-;zone400/18000-;", "0: not due");
    setup(1); zone_results = {1}; msg_run__10FireBeetle(object); expect(log_text == head() + "zone400/32767+;st3;", "1: near");
    setup(1); zone_results = {0, 0}; msg_run__10FireBeetle(object); expect(log_text == head() + "zone400/32767-;zone400/18000-;wander;", "1: wander");
    // state 8, 9, 13
    setup(8); msg_run__10FireBeetle(object); expect(log_text == head() + "st3;", "8 without bit 3");
    setup(8); write_u16(address(object) + 294u, 8u); msg_run__10FireBeetle(object); expect(log_text == head(), "8 with bit 3");
    setup(9); setw(804, 5); msg_run__10FireBeetle(object); expect(log_text == head() + "st0;", "9 due");
    setup(9); setw(804, 10); msg_run__10FireBeetle(object); expect(log_text == head(), "9 not due");
    setup(13); msg_run__10FireBeetle(object); expect(log_text == head() + "st1;", "13 without target");
    setup(13); setw(576, 1); setw(960, 10); msg_run__10FireBeetle(object); expect(log_text == head() + "st3;", "13 clock not below w960");
    setup(13); setw(576, 1); setw(960, 11); msg_run__10FireBeetle(object); expect(log_text == head() + "steerE;", "13 steers");
    // state 10
    setup(10); write_u16(address(object) + 294u, 0x80u); advance_result = 2; msg_run__10FireBeetle(object);
    expect(log_text == head() + "slowrun;rand;rand;floor10/7/" + f2s(8 % 20 + 35.0f) + "/6/1;" && (getw(12) & 1) == 1, "10: slow run list and floor effect");
    setup(10); advance_result = 0; msg_run__10FireBeetle(object); expect(log_text == head(), "10: flags bit 1 clear");
    setup(10); advance_result = 2; setw(12, 2); msg_run__10FireBeetle(object); expect(log_text == head(), "10: bit 1 of +12 set");
    // states 3 and 5
    setup(3); msg_run__10FireBeetle(object); expect(log_text == head() + "st0;", "3 without target");
    setup(3); setw(576, 1); zone_results = {1}; msg_run__10FireBeetle(object); expect(log_text == head() + "zone20/4000+;st6;", "3: close");
    setup(3); setw(576, 1); setw(964, 5); setw(808, 10); zone_results = {0, 0, 1}; frameNm = 777; msg_run__10FireBeetle(object);
    expect(log_text == head() + "zone20/4000-;zone100/32767-;zone400/3000+;st14;alloc188;light/ok;" && getw(248) == 777, "3: light effect");
    expect(light_floats[0] == 255.0f && light_floats[1] == 155.0f && light_floats[2] == 0.0f && light_floats[3] == -96.0f && light_floats[8] == 180.0f &&
           light_floats[9] == 144.0f && light_floats[10] == 0.0f && light_floats[11] == 1.5f && light_object == object && light_flag == 0, "light numbers");
    expect(light_point[0] == 10.0f && light_point[1] == 20.0f && light_point[2] == 66.0f, "light point");
    setup(3); setw(576, 1); setw(964, 5); setw(808, 10); zone_results = {0, 0, 1}; alloc_ok = false; msg_run__10FireBeetle(object);
    expect(log_text == head() + "zone20/4000-;zone100/32767-;zone400/3000+;st14;alloc188;", "3: failed allocation");
    setup(3); setw(576, 1); setw(964, 5); setw(808, 10); zone_results = {0, 1}; msg_run__10FireBeetle(object);
    expect(log_text.find("zone100/32767+;zone200/32767") != std::string::npos && log_text.find("st14") == std::string::npos && log_text.find("alloc") == std::string::npos, "3: close enemy skips light and goes to the 200 test");
    setup(3); setw(576, 1); zone_results = {0, 0, 1}; msg_run__10FireBeetle(object);   // C not below clock
    expect(log_text == head() + "zone20/4000-;zone200/32767-;zone400/8000+;st13;", "3: far zone");
    setup(3); setw(576, 1); { float r = 2.0f; std::memcpy(object + 28, &r, 4); } zone_results = {0, 1, 1}; msg_run__10FireBeetle(object);
    expect(log_text == head() + "zone22/4000-;zone200/32767+;zone22/32767+;face;", "3: face enemy");
    setup(3); setw(576, 1); zone_results = {0, 1, 0}; setw(968, 0); msg_run__10FireBeetle(object);
    expect(log_text == head() + "zone20/4000-;zone200/32767+;zone20/32767-;steerE;", "3: steer to enemy (toggle zero)");
    setup(3); setw(576, 1); zone_results = {0, 1, 0}; setw(968, 1); msg_run__10FireBeetle(object);
    expect(log_text == head() + "zone20/4000-;zone200/32767+;zone20/32767-;steerP" + f2s(vslots_b::read_single(address(object) + 976u)) + "," + f2s(vslots_b::read_single(address(object) + 980u)) + "," +
                           f2s(vslots_b::read_single(address(object) + 984u)) + ";", "3: steer to the wander target (toggle one)");
    setup(5); zone_results = {1}; msg_run__10FireBeetle(object); expect(log_text == head() + "zone20/4000+;st6;", "5: close");
    setup(5); exit_result = 1; msg_run__10FireBeetle(object); expect(log_text == head() + "zone20/4000-;exit+;st3;", "5: exit");
    setup(5); exit_result = 0; msg_run__10FireBeetle(object); expect(log_text == head() + "zone20/4000-;exit-;", "5: no exit");
    setup(5); setw(964, 5); setw(808, 10); zone_results = {0, 0, 1}; msg_run__10FireBeetle(object);
    expect(log_text == head() + "zone20/4000-;zone100/32767-;zone400/3000+;st14;alloc188;light/ok;", "5: light effect");
    // state 6
    setup(6); setw(256, 1); msg_run__10FireBeetle(object); expect(log_text == head() + "st3;" && getw(256) == 0, "6: no target, timer expired");
    setup(6); setw(256, 3); msg_run__10FireBeetle(object); expect(log_text == head() && getw(256) == 2, "6: no target, timer ticking");
    setup(6); setw(256, 3); write_u16(address(object) + 294u, 8u); msg_run__10FireBeetle(object); expect(log_text == head() && getw(256) == 3, "6: bit 3 set, timer untouched");
    setup(6); setw(576, 1); advance_result = 1; write_u16(address(object) + 200u, 11); write_u16(address(object) + 202u, 0xfff5u); write_u16(address(object) + 204u, 13); setw(256, 3);
    msg_run__10FireBeetle(object); expect(log_text == head() + "melee50/11/-11/13/8/desc;" && getw(256) == 2, "6: melee");
    setup(6); setw(576, 1); write_u16(address(object) + 294u, 8u); setw(256, 3); msg_run__10FireBeetle(object);
    expect(log_text == head() + "face;" && getw(256) == 3, "6: turn while bit 3 stays set");
    // state 14
    setup(14); setw(256, 3); msg_run__10FireBeetle(object); expect(log_text == head() && getw(256) == 2, "14: no target");
    setup(14); setw(576, 1); setw(256, 3); advance_result = 0; msg_run__10FireBeetle(object); expect(log_text == head() && getw(256) == 2, "14: no attack flag");
    setup(14); setw(576, 1); setw(256, 3); advance_result = 1; aim_result = 12000; write_u16(address(object) + 200u, 21); write_u16(address(object) + 202u, 0xffe1u); write_u16(address(object) + 204u, 33);
    rolls = {1200, 7, 9, 10};
    msg_run__10FireBeetle(object);
    // lead = (10 + 150 + 1, 20 + 75 + 2, 30 + 4); aim from (10, 20) -> (151, 77); heading 16384 - 12000 = 4384 clamped to 256 + 4000 = 4256, jitter 200 - 500 => 3956
    expect(log_text == head() + "cos256;sin256;lead300/160,95,30;cos256;sin256;atan151,77;rand;cos3956;sin3956;rand;alloc460;proj/ok/ha;sound/hb/self/1;" && getw(256) == 2, "14: attack call order");
    expect(proj_start[0] == 10.0f + 0.5f * 10.0f && proj_start[1] == 20.0f + 0.25f * 10.0f && proj_start[2] == 35.0f, "projectile start");
    expect(proj_velocity[0] == 0.5f * 300.0f && proj_velocity[1] == 0.25f * 300.0f, "projectile velocity x, y");
    {
        const float dx = 10.0f - 161.0f, dy = 20.0f - 97.0f, dz = 30.0f - 34.0f;
        const float dist = std::sqrt(dx * dx + dy * dy + dz * dz) * __builtin_bit_cast(float, 0x3b5a740du);
        expect(proj_velocity[2] == (7.0f + 200.0f) * dist, "projectile velocity z");
    }
    expect(proj_ints[0] == 256 - 16384 && proj_ints[1] == 21 && proj_ints[2] == -31 && proj_ints[3] == 0 && proj_ints[4] == 0 && proj_ints[5] == 33, "projectile integers");
    setup(14); setw(576, 1); setw(256, 3); advance_result = 1; alloc_ok = false; msg_run__10FireBeetle(object);
    expect(log_text.find("alloc460;sound/hb/self/1;") != std::string::npos && log_text.find("proj") == std::string::npos, "failed allocation still plays the sound");
    // clamp the other way
    setup(14); setw(576, 1); setw(256, 3); advance_result = 1; aim_result = -30000; rolls = {500, 0, 0, 0}; msg_run__10FireBeetle(object);
    expect(log_text.find("cos" + std::to_string(static_cast<short>(256 - 4000)) + ";") != std::string::npos, "heading clamped below");
    return failures ? 1 : 0;
}
