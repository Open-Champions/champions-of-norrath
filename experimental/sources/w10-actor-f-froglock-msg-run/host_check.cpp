// Host contract for msg_run__8Froglock. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_run.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

int frameNm;
void* world;
unsigned char skills[256];
unsigned char globalItem[64];
float worldDifficulty;

static std::string log_text;
static std::deque<int> zone_results, cross_results;
static std::deque<unsigned> rolls;
static int every_result, activation_result, advance_result, water_result, exit_result, aim_result;
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s (log %s)\n", m, log_text.c_str()); ++failures; } }
static void add(const std::string& t) { log_text += t; log_text += ';'; }
static std::string f2s(float f) { char b[32]; std::snprintf(b, sizeof b, "%g", f); return b; }
static unsigned char object[1300], group[64], model[4], table[256], node[32], target[32], block[600], desc[8];
static void* adjusted_for[4];
static float last_point[3], last_velocity[3];
static int last_power, last_missile_heading;
static void* last_skill;
static short last_short_heading;
static int last_missile_ints[9];

extern "C" void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { add("ndh"); expect(m == model, "model"); }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void*) { add("ef"); return every_result; }
extern "C" int objectActivationCheck__8Creaturei(void*, int d) { add("oac" + std::to_string(d)); return activation_result; }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float a) { add("adv" + f2s(a)); return advance_result; }
extern "C" void slideBack__8Creaturef(void*, float a) { add("slide" + f2s(a)); }
extern "C" void move2__8CreatureG6Point3(void*, const void* p) { float v[3]; std::memcpy(v, p, 12); add("move2" + f2s(v[0]) + f2s(v[1]) + f2s(v[2])); }
extern "C" float icos__Fi(int a) { add("cos" + std::to_string(a)); return 0.5f; }
extern "C" float isin__Fi(int a) { add("sin" + std::to_string(a)); return 0.25f; }
extern "C" int iatan2__Fff(float a, float b) { add("atan" + f2s(a) + "," + f2s(b)); return aim_result; }
extern "C" int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void*, const void* p, float d, float* out) {
    float v[3]; std::memcpy(v, p, 12); add("water" + f2s(v[0]) + "," + f2s(v[1]) + "," + f2s(v[2]) + "/" + f2s(d) + (out ? "/out" : "/null")); return water_result;
}
extern "C" int enemyInZone__8Creaturefs(void*, float d, short n) {
    const int r = zone_results.empty() ? 0 : zone_results.front(); if (!zone_results.empty()) zone_results.pop_front();
    add("zone" + f2s(d) + "/" + std::to_string(n) + (r ? "+" : "-")); return r;
}
extern "C" void smartWander__8Creature(void*) { add("wander"); }
extern "C" int bCrossingWater__8Froglock(void*) {
    const int r = cross_results.empty() ? 0 : cross_results.front(); if (!cross_results.empty()) cross_results.pop_front();
    add(std::string("cross") + (r ? "+" : "-")); return r;
}
extern "C" void steerAndRouteToEnemy__8Creature(void*) { add("steer"); }
extern "C" void turnToFaceEnemy__8Creature(void*) { add("face"); }
extern "C" int bExitStatePersueIdle__8Creature(void*) { add(std::string("exit") + (exit_result ? "+" : "-")); return exit_result; }
extern "C" void flee2__8CreatureP10GameObject(void*, void* t) { add(std::string("flee2") + (t == target ? "/target" : "/bad")); }
extern "C" void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float s, int a, int b, int c, int d, void* desc_arg) {
    add("melee" + f2s(s) + "/" + std::to_string(a) + "/" + std::to_string(b) + "/" + std::to_string(c) + "/" + std::to_string(d) + (desc_arg == object + 1032 ? "/desc" : "/bad"));
}
extern "C" void findEnemyLeadPosition__8CreaturefP6Point3(void*, float d, void* p) {
    float v[3]; std::memcpy(v, p, 12); add("lead" + f2s(d) + "/" + f2s(v[0]) + "," + f2s(v[1]) + "," + f2s(v[2]));
    const float moved[3] = {v[0] + 1.0f, v[1] + 2.0f, v[2] + 3.0f}; std::memcpy(p, moved, 12);
}
extern "C" unsigned eRand__10GameObject(void*) { add("rand"); const unsigned r = rolls.front(); rolls.pop_front(); rolls.push_back(r); return r; }
extern "C" int sprintf(char* buffer, const char* format, ...);
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) { add(std::string("lump/") + lump + "/" + name); return model; }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) { add(std::string("sound") + (h == model && o == object ? "/ok/" : "/bad/") + f2s(v)); }
static bool alloc_ok;
extern "C" void* blockAlloc__Fi(int size) { add("alloc" + std::to_string(size)); return alloc_ok ? block : nullptr; }
static void record_projectile(const char* name, void* self, const void* p, short heading, void* shooter, int power, void* skill) {
    std::memcpy(last_point, p, 12); last_short_heading = heading; last_power = power; last_skill = skill;
    add(std::string(name) + (self == block && shooter == object ? "/ok" : "/bad"));
}
extern "C" void __21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill(void* s, const void* p, short h, void* o, int pw, void* k) { record_projectile("poison", s, p, h, o, pw, k); }
extern "C" void __19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill(void* s, const void* p, short h, void* o, int pw, void* k) { record_projectile("cold", s, p, h, o, pw, k); }
extern "C" void __22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill(void* s, const void* p, short h, void* o, int pw, void* k) { record_projectile("explode", s, p, h, o, pw, k); }
extern "C" void __19FireArrowProjectileG6Point3sP10GameObjectiP5Skill(void* s, const void* p, short h, void* o, int pw, void* k) { record_projectile("fire", s, p, h, o, pw, k); }
static void* missile_model; static void* missile_texture;
extern "C" void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int first, void* m, void* t, const void* start, const void* velocity, int heading, void* creature,
    void* va, void* vb, int a, int b, int c, int d, int e, int f, void* description) {
    std::memcpy(last_point, start, 12); std::memcpy(last_velocity, velocity, 12); last_missile_heading = heading;
    last_missile_ints[0] = first; last_missile_ints[1] = a; last_missile_ints[2] = b; last_missile_ints[3] = c; last_missile_ints[4] = d;
    last_missile_ints[5] = e; last_missile_ints[6] = f; last_missile_ints[7] = (va || vb || description) ? 1 : 0; last_missile_ints[8] = 0;
    missile_model = m; missile_texture = t;
    add(std::string("missile") + (self == block && creature == object ? "/ok" : "/bad"));
}
extern "C" void objectRemoveFromRunList__FP10GameObject(void*) { add("rmrun"); }
extern "C" void floorAdd__FG6Point3sfii(const void* p, short angle, float size, int a, int b) {
    float v[3]; std::memcpy(v, p, 12); add("floor" + f2s(v[0]) + "/" + std::to_string(angle) + "/" + f2s(size) + "/" + std::to_string(a) + "/" + std::to_string(b));
}
static void slot_168(void* adj, int) { add("upd"); }
static void slot_184(void* adj, int state) { add("st" + std::to_string(state)); adjusted_for[0] = adj; }
static void slot_176(void* adj, int zero, void* p) { float v[3]; std::memcpy(v, p, 12); add("turn" + std::to_string(zero) + "/" + f2s(v[0]) + "," + f2s(v[1]) + "," + f2s(v[2])); adjusted_for[1] = adj; }

static unsigned getw(unsigned off) { return vslots_b::read_word(vslots_b::address(object) + off); }
static void setw(unsigned off, unsigned v) { vslots_b::write_word(vslots_b::address(object) + off, v); }
static void setup(int state) {
    std::memset(object, 0x41, sizeof object);
    setw(36, vslots_b::address_bits(group));
    vslots_b::write_word(vslots_b::address(group) + 12u, vslots_b::address_bits(model));
    setw(72, vslots_b::address_bits(table));
    std::memset(table, 0, sizeof table);
    const struct { unsigned off; void* fn; } slots[] = {{168, reinterpret_cast<void*>(slot_168)}, {176, reinterpret_cast<void*>(slot_176)}, {184, reinterpret_cast<void*>(slot_184)}};
    for (auto& s : slots) { vslots_b::write_u16(vslots_b::address(table) + s.off, 8u); vslots_b::write_word(vslots_b::address(table) + s.off + 4u, vslots_b::address_bits(s.fn)); }
    vslots_b::write_u16(vslots_b::address(object) + 572u, static_cast<unsigned>(state) & 0xffffu);
    const float pos[3] = {10.0f, 20.0f, 30.0f}; std::memcpy(object + 16, pos, 12);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 0x0100u);
    object[568] = 1; object[1105] = 0; object[569] = 0;
    setw(576, 0); setw(960, 0); setw(256, 5); setw(804, 0); setw(808, 10); setw(12, 0);
    vslots_b::write_u16(vslots_b::address(object) + 294u, 0u);
    log_text.clear(); zone_results.clear(); cross_results.clear(); rolls = {7, 8, 9, 10};
    every_result = 0; activation_result = 1; advance_result = 0; water_result = 0; exit_result = 0; alloc_ok = true; aim_result = 0;
    frameNm = 1;   // (frameNm xor byte 569) & 7 != 0: no water check
}
static std::string head() { return "ndh;ef;oac600;adv1;slide0.5;upd;move2000;"; }
static std::string tail_text(const std::string& body) { return head() + body; }
int main() {
    using namespace vslots_b;
    host_note(group); host_note(model); host_note(table); host_note(object); host_note(node); host_note(target); host_note(block);
    host_note(reinterpret_cast<void*>(slot_168)); host_note(reinterpret_cast<void*>(slot_176)); host_note(reinterpret_cast<void*>(slot_184));
    host_note(desc);
    write_word(address(globalItem) + 8u, address_bits(model)); write_word(address(globalItem) + 12u, address_bits(desc));
    for (unsigned i = 0; i < 64; ++i) write_word(address(skills) + 4u * i, 0x1000u + i);
    for (unsigned i = 0; i < 64; ++i) host_note(reinterpret_cast<void*>(0x1000u + i));
    worldDifficulty = 3.7f;
    // early returns
    setup(0); every_result = 1; msg_run__8Froglock(object); expect(log_text == "ndh;ef;", "everyFrame stops");
    setup(0); activation_result = 0; msg_run__8Froglock(object); expect(log_text == "ndh;ef;oac600;", "activation stops");
    // no work states: tail bytes only
    for (int s : {2, 4, 11, 12, 16, 100, -1}) {
        setup(s); object[568] = 0x42; object[1105] = 0x37; object[1104] = 0; object[1106] = 0;
        msg_run__8Froglock(object);
        expect(log_text == head() && object[1104] == 0x42 && object[1106] == 0x37, "state without work still copies the bytes");
    }
    // water check: frame gate passes
    setup(2); frameNm = 5; object[569] = 5; water_result = 1; msg_run__8Froglock(object);
    expect(log_text == head() + "cos256;sin256;water" + f2s(10.0f + 0.5f * 130.0f) + "," + f2s(20.0f + 0.25f * 130.0f) + ",30/12/null;" && object[1105] == 1, "water check sets Q");
    setup(2); frameNm = 0xff; object[569] = 0xff; water_result = 0; object[1105] = 1; msg_run__8Froglock(object);
    expect(object[1105] == 0 && object[1106] == 0, "sign-extended tag matches -1; Q cleared; old Q copied to 1106 after the check");
    // state 0
    setup(0); zone_results = {1}; msg_run__8Froglock(object);
    expect(log_text == tail_text("zone400/32767+;st3;"), "state 0 close enemy");
    setup(0); zone_results = {0, 1}; msg_run__8Froglock(object);
    expect(log_text == tail_text("zone400/32767-;zone500/16000+;st3;"), "state 0 far enemy");
    setup(0); zone_results = {0, 0}; setw(256, 5); setw(808, 10); msg_run__8Froglock(object);
    expect(log_text == tail_text("zone400/32767-;zone500/16000-;st1;"), "state 0 timer due");
    setup(0); zone_results = {0, 0}; setw(256, 10); setw(808, 10); msg_run__8Froglock(object);
    expect(log_text == tail_text("zone400/32767-;zone500/16000-;"), "state 0 timer not due");
    // state 1
    setup(1); zone_results = {1}; msg_run__8Froglock(object); expect(log_text == tail_text("zone400/32767+;st0;"), "state 1 close");
    setup(1); zone_results = {0, 0}; cross_results = {0}; msg_run__8Froglock(object);
    expect(log_text == tail_text("zone400/32767-;zone500/16000-;wander;cross-;"), "state 1 wander");
    setup(1); zone_results = {0, 0}; cross_results = {1}; msg_run__8Froglock(object);
    expect(log_text == tail_text("zone400/32767-;zone500/16000-;wander;cross+;st1;"), "state 1 crossing water");
    // state 7
    setup(7); cross_results = {1}; msg_run__8Froglock(object); expect(log_text == tail_text("cross+;st3;"), "state 7 crossing");
    setup(7); cross_results = {0}; setw(808, 10); setw(256, 10); msg_run__8Froglock(object); expect(log_text == tail_text("cross-;st3;"), "state 7 timer");
    setup(7); cross_results = {0}; setw(808, 5); setw(256, 10); msg_run__8Froglock(object); expect(log_text == tail_text("cross-;st3;"), "state 7 no target");
    setup(7); cross_results = {0}; setw(808, 5); setw(256, 10); setw(576, address_bits(node)); setw(0, 0);
    write_word(address(node) + 12u, address_bits(target));
    msg_run__8Froglock(object); expect(log_text == tail_text("cross-;flee2/target;"), "state 7 flee");
    // state 9
    setup(9); setw(804, 5); setw(808, 10); msg_run__8Froglock(object); expect(log_text == tail_text("st0;"), "state 9 due");
    setup(9); setw(804, 10); setw(808, 10); msg_run__8Froglock(object); expect(log_text == tail_text(""), "state 9 not due");
    // states 8 and 15
    for (int s : {8, 15}) {
        setup(s); msg_run__8Froglock(object); expect(log_text == tail_text("st3;"), "state 8/15 without bit 4");
        setup(s); write_u16(address(object) + 294u, 0x10u); msg_run__8Froglock(object); expect(log_text == tail_text(""), "state 8/15 with bit 4");
    }
    // state 10
    setup(10); advance_result = 0x10; write_u16(address(object) + 294u, 0x80u); msg_run__8Froglock(object);
    expect(log_text == tail_text("rmrun;rand;rand;floor10/7/" + f2s(8 % 10 + 25.0f) + "/0/1;"), "state 10 floor effect");
    setup(10); advance_result = 0; msg_run__8Froglock(object); expect(log_text == tail_text(""), "state 10 without flags bit 4");
    setup(10); advance_result = 0x10; setw(12, 2); msg_run__8Froglock(object); expect(log_text == tail_text(""), "state 10 with bit 1 of the word at +12");
    // state 6
    setup(6); msg_run__8Froglock(object); expect(log_text == tail_text("st3;"), "state 6 no list, no bit 3");
    setup(6); write_u16(address(object) + 294u, 8u); msg_run__8Froglock(object); expect(log_text == tail_text(""), "state 6 no list, bit 3");
    setup(6); setw(576, address_bits(node)); advance_result = 1; write_u16(address(object) + 200u, 11); write_u16(address(object) + 202u, 0xfff5u); write_u16(address(object) + 204u, 13);
    msg_run__8Froglock(object); expect(log_text == tail_text("melee36/11/-11/13/8/desc;st3;"), "state 6 melee, no bit 3");
    setup(6); setw(576, address_bits(node)); advance_result = 0; write_u16(address(object) + 294u, 8u); msg_run__8Froglock(object);
    expect(log_text == tail_text("face;"), "state 6 turns to face while bit 3 stays set");
    // state 5
    setup(5); setw(960, 16); zone_results = {1}; msg_run__8Froglock(object); expect(log_text == tail_text("zone400/6000+;st13;"), "state 5 melee range");
    setup(5); setw(960, 16); zone_results = {0}; exit_result = 1; msg_run__8Froglock(object);
    expect(log_text == tail_text("zone400/6000-;exit+;st3;"), "state 5 with w960 16 skips the second zone");
    setup(5); setw(960, 0); { float r = 2.0f; std::memcpy(object + 28, &r, 4); } zone_results = {1}; msg_run__8Froglock(object);
    expect(log_text == tail_text("zone22/6000+;st6;"), "state 5 second zone");
    setup(5); setw(960, 0); zone_results = {0}; exit_result = 0; msg_run__8Froglock(object);
    expect(log_text.find("exit-;") != std::string::npos && log_text.find("st") == std::string::npos, "state 5 exit not taken");
    // state 3
    setup(3); msg_run__8Froglock(object); expect(log_text == tail_text("st0;"), "state 3 without a target");
    setup(3); setw(576, address_bits(node)); setw(960, 16); zone_results = {1}; msg_run__8Froglock(object); expect(log_text == tail_text("zone100/32767+;st7;"), "state 3 close");
    setup(3); setw(576, address_bits(node)); setw(960, 16); zone_results = {0, 1}; object[568] = 5; msg_run__8Froglock(object); expect(log_text == tail_text("zone100/32767-;zone400/6000+;st14;"), "state 3 shoot (P set)");
    setup(3); setw(576, address_bits(node)); setw(960, 16); zone_results = {0, 1}; object[568] = 0; msg_run__8Froglock(object); expect(log_text == tail_text("zone100/32767-;zone400/6000+;st13;"), "state 3 shoot (P clear)");
    setup(3); setw(576, address_bits(node)); setw(960, 16); zone_results = {0, 0}; cross_results = {1}; msg_run__8Froglock(object); expect(log_text == tail_text("zone100/32767-;zone400/6000-;steer;cross+;st3;"), "state 3 steer");
    setup(3); setw(576, address_bits(node)); setw(960, 0); { float r = 2.0f; std::memcpy(object + 28, &r, 4); } zone_results = {1}; msg_run__8Froglock(object);
    expect(log_text == tail_text("zone42/6000+;st6;"), "state 3 other w960, near");
    setup(3); setw(576, address_bits(node)); setw(960, 0); { float r = 2.0f; std::memcpy(object + 28, &r, 4); } zone_results = {0, 0, 1}; msg_run__8Froglock(object);
    expect(log_text == tail_text("zone42/6000-;zone300/32767-;zone450/1000+;st15;"), "state 3 other w960, mid");
    setup(3); setw(576, address_bits(node)); setw(960, 0); { float r = 2.0f; std::memcpy(object + 28, &r, 4); } zone_results = {0, 1, 1}; msg_run__8Froglock(object);
    expect(log_text == tail_text("zone42/6000-;zone300/32767+;zone42/32767+;face;"), "state 3 other w960, face");
    // states 13 and 14: no list -> st0 then the common end
    for (int s : {13, 14}) {
        setup(s); setw(256, 3); msg_run__8Froglock(object);
        expect(log_text == tail_text("st0;") && getw(256) == 2, "13/14 without target decrements the timer and keeps going");
        setup(s); setw(256, 1); msg_run__8Froglock(object);
        expect(log_text == tail_text("st0;st3;") && getw(256) == 0, "13/14 timer expired");
        setup(s); setw(960, 16); setw(256, 3); zone_results = {1}; msg_run__8Froglock(object);
        expect(log_text == tail_text("st0;zone100/32767+;st7;"), "13/14 close enemy while waiting");
        setup(s); write_u16(address(object) + 294u, 8u); setw(256, 3); msg_run__8Froglock(object);
        expect(log_text == tail_text("st0;") && getw(256) == 3, "13/14 bit 3 set: no decrement, timer positive");
        setup(s); write_u16(address(object) + 294u, 8u); setw(256, 0); msg_run__8Froglock(object);
        expect(log_text == tail_text("st0;st3;") && getw(256) == 0, "13/14 bit 3 set: timer not positive");
    }
    // with a target and no attack flag: lead position only
    setup(13); setw(576, address_bits(node)); setw(256, 4); advance_result = 0; msg_run__8Froglock(object);
    expect(log_text == tail_text("cos256;sin256;lead400/160,95,30;"), "13 lead only");
    setup(14); setw(576, address_bits(node)); setw(256, 4); advance_result = 0; write_u16(address(object) + 294u, 8u); msg_run__8Froglock(object);
    expect(log_text == tail_text("cos256;sin256;lead400/160,95,30;turn0/161,97,33;"), "14 turn to the lead position");
    expect(adjusted_for[1] == object + 8, "turn call adjusted");
    // with the attack flag: each projectile kind
    struct Kind { unsigned w208; const char* name; int size; unsigned skill; };
    const Kind kinds[] = {{1, "poison", 176, 45}, {2, "cold", 176, 44}, {3, "explode", 116, 47}, {4, "fire", 176, 43}};
    for (const Kind& k : kinds) {
        setup(13); setw(576, address_bits(node)); setw(256, 4); advance_result = 1; setw(208, k.w208);
        write_u16(address(object) + 56u, 0x0100u);
        float h = 6.0f; std::memcpy(object + 32, &h, 4);
        aim_result = 12000;
        rolls = {1000, 31, 2, 9};
        msg_run__8Froglock(object);
        // heading: 16384 - 12000 = 4384, clamped to 256 + 1000 = 1256, plus 1000 % 1800 - 900 = 100 gives 1356
        expect(log_text.find("atan151,77;rand;cos1356;sin1356;rand;rand;lump/fx.lmp/swsh_arrow") != std::string::npos, "aim, jitter, velocity and sound order");
        expect(log_text.find(std::string("alloc") + std::to_string(k.size) + ";" + k.name + "/ok;") != std::string::npos, k.name);
        expect(last_short_heading == 1356 && last_power == 3 && last_skill == reinterpret_cast<void*>(0x1000u + k.skill), "projectile arguments");
        expect(last_point[0] == 10.0f + 0.0f && last_point[1] == 20.0f && last_point[2] == 30.0f + (6.0f * 0.5f + 12.0f), "projectile point");
        expect(log_text.find("lump/fx.lmp/swsh_arrow") != std::string::npos && log_text.find("sound/ok/1;") != std::string::npos, "sound");
    }
    // missile weapon default
    setup(14); setw(576, address_bits(node)); setw(256, 4); advance_result = 1; setw(208, 9);
    write_u16(address(object) + 200u, 21); write_u16(address(object) + 202u, 0xffe1u); write_u16(address(object) + 204u, 33);
    aim_result = 0; rolls = {0, 59, 3, 9};
    msg_run__8Froglock(object);
    expect(log_text.find("alloc452;missile/ok;") != std::string::npos, "missile weapon");
    expect(missile_model == model && missile_texture == desc, "missile model and texture words");
    expect(last_point[0] == 10.0f && last_point[1] == 20.0f && last_point[2] == 78.0f, "missile start point");
    expect(last_missile_ints[1] == 21 && last_missile_ints[2] == -15 && last_missile_ints[5] == 33 && last_missile_ints[6] == 4 && last_missile_ints[3] == 0 && last_missile_ints[4] == 0 && last_missile_ints[0] == 0, "missile integers (halfword/2 truncates toward zero)");
    expect(last_velocity[2] == static_cast<float>(59 - 30), "missile velocity z");
    // failed allocation skips the construction but still reaches the end
    setup(14); setw(576, address_bits(node)); setw(256, 4); advance_result = 1; setw(208, 1); alloc_ok = false;
    msg_run__8Froglock(object);
    expect(log_text.find("alloc176;") != std::string::npos && log_text.find("poison") == std::string::npos, "failed allocation");
    return failures ? 1 : 0;
}
