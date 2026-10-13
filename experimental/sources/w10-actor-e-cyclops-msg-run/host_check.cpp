// Host contract for msg_run__7Cyclops. Includes the source; not an EE input.
#define NORRATH_RTTI_C_HOST 1
#include "msg_run.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>
using namespace rtti_c;
static std::string ev;
static std::deque<int> eiz_q, rolls;
static int every_frame = 0, activation = 1, advance = 0, alt_result = 0, wander_result = 0, bexit_result = 0, heading_result = 0, alloc_ok = 1;
static unsigned char object[1300], vtable[256], table[400], target[64], inner[64], blockmem[256];
static std::vector<std::string> notes;
static unsigned bits_of(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }
static std::string num(long v) { return std::to_string(v); }
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void*, void* model) { ev += "N"; if (model != table + 12) ev += "!"; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void*) { ev += "E"; return every_frame; }
extern "C" int objectActivationCheck__8Creaturei(void*, int d) { ev += "A" + num(d) + ";"; return activation; }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float r) { ev += "V" + num(bits_of(r) == 0x3f800000u) + ";"; return advance; }
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void*, const void* point, short a, void* anim, int index, void* out_a, void* out_b, float scale) {
    ev += "M" + num(index) + ":" + num(a) + (anim == object + 288 ? "" : "!") + (out_b == nullptr ? "" : "?") + (scale == 1.0f ? "" : "s") + ";";
    float p[3]; std::memcpy(p, point, 12); if (p[0] != 10.0f + 6.0f || p[1] != 20.0f || p[2] != 24.0f) ev += "p!";
    static_cast<float*>(out_a)[0] = 7.0f + static_cast<float>(index); static_cast<float*>(out_a)[1] = 8.0f; static_cast<float*>(out_a)[2] = 9.0f;
}
extern "C" void generateDust__7CyclopsG6Point3(void*, const void* point) { float p[3]; std::memcpy(p, point, 12); ev += "D" + num((long)p[0]) + ";"; }
extern "C" void slideBack__8Creaturef(void*, float v) { ev += "B" + num(bits_of(v) == 0x3e4ccccc) + ";"; }
extern "C" int enemyInZone__8Creaturefs(void*, float d, short a) { ev += "Z" + num((long)d) + "/" + num(a) + ";"; const int r = eiz_q.empty() ? 0 : eiz_q.front(); if (!eiz_q.empty()) eiz_q.pop_front(); return r; }
extern "C" int alternateEnemyInZone__8Creatureffss(void*, float a, float b, short c, short d) { ev += "Y" + num((long)a) + "," + num((long)b) + "," + num(c) + "," + num(d) + ";"; return alt_result; }
extern "C" int smartWander__8Creature(void*) { ev += "W;"; return wander_result; }
extern "C" void turnToFaceEnemy__8Creature(void*) { ev += "T;"; }
extern "C" void steerAndRouteToEnemy__8Creature(void*) { ev += "S;"; }
extern "C" int bExitStatePersueIdle__8Creature(void*) { ev += "X;"; return bexit_result; }
extern "C" int eRand__10GameObject(void*) { ev += "R;"; const int r = rolls.empty() ? 0 : rolls.front(); if (!rolls.empty()) rolls.pop_front(); return r; }
extern "C" float icos__Fi(int a) { ev += "c" + num(a) + ";"; return 2.0f; }
extern "C" float isin__Fi(int a) { ev += "i" + num(a) + ";"; return 3.0f; }
extern "C" int iatan2__Fff(float a, float b) { ev += "a" + num((long)a) + "," + num((long)b) + ";"; return heading_result; }
extern "C" void* blockAlloc__Fi(int n) { ev += "K" + num(n) + ";"; return alloc_ok ? blockmem : nullptr; }
static float seen_point[3], seen_velocity[3]; static int seen_args[8]; static float seen_scale; static void *seen_owner, *seen_map, *seen_block;
extern "C" void __9dustCloudG6Point3iiii(void* self, const void* point, int a, int b, int c, int d) {
    ev += "Q" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ";"; std::memcpy(seen_point, point, 12); seen_block = self;
}
extern "C" void __14poisonGasSpellG6Point3iiiP10GameObjecti(void* self, const void* point, int a, int b, int c, void* owner, int d) {
    ev += "G" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ";"; std::memcpy(seen_point, point, 12); seen_owner = owner; seen_block = self;
}
extern "C" void __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature(
    void* self, void* map, int a, int b, int c, int d, const void* position, const void* velocity, int angle, float scale, void* owner) {
    ev += "P" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(angle) + ";";
    std::memcpy(seen_point, position, 12); std::memcpy(seen_velocity, velocity, 12); seen_scale = scale; seen_owner = owner; seen_map = map; seen_block = self;
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float r) { ev += "U" + num(static_cast<unsigned char*>(h) - table) + (o == object ? "" : "!") + (r == 1.0f ? "" : "r") + ";"; }
extern "C" void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float s, int a, int b, int c, int d, void* desc) {
    ev += "H" + num((long)s) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + (desc ? "!" : "") + ";";
}
extern "C" void objectRemoveFromRunList__FP10GameObject(void*) { ev += "J;"; }
static float floor_point[3]; static int floor_angle; static float floor_size; static int floor_a, floor_b;
extern "C" void floorAdd__FG6Point3sfii(const void* point, short angle, float size, int a, int b) {
    ev += "F;"; std::memcpy(floor_point, point, 12); floor_angle = angle; floor_size = size; floor_a = a; floor_b = b;
}
static void slot168(void* self) { ev += "u" + num(static_cast<unsigned char*>(self) - object) + ";"; }
static void slot184(void* self, int v) { ev += "s" + num(v) + "@" + num(static_cast<unsigned char*>(self) - object) + ";"; }
static void reset(int state) {
    std::memset(object, 0, sizeof object); std::memset(vtable, 0, sizeof vtable); std::memset(table, 0, sizeof table);
    std::memset(target, 0, sizeof target); std::memset(inner, 0, sizeof inner);
    host_note(table); host_note(target); host_note(inner);
    for (unsigned off : {12u, 180u, 196u, 204u}) { host_note(table + off); write_word(address(table) + off, address_bits(table + off)); }
    write_word(address(object) + 36u, address_bits(table));
    write_u16(address(vtable) + 168u, static_cast<unsigned>(-4)); write_word(address(vtable) + 172u, address_bits(reinterpret_cast<void*>(slot168)));
    write_u16(address(vtable) + 184u, static_cast<unsigned>(-8)); write_word(address(vtable) + 188u, address_bits(reinterpret_cast<void*>(slot184)));
    host_note(reinterpret_cast<void*>(slot168)); host_note(reinterpret_cast<void*>(slot184)); host_note(vtable);
    write_word(address(object) + 72u, address_bits(vtable));
    write_u16(address(object) + 572u, static_cast<unsigned>(state));
    write_single(address(object) + 16u, 16.0f); write_single(address(object) + 20u, 20.0f); write_single(address(object) + 24u, 24.0f);
    write_u16(address(object) + 56u, 100u); write_word(address(object) + 808u, 5000u);
    ev.clear(); eiz_q.clear(); rolls.clear(); every_frame = 0; activation = 1; advance = 0; alt_result = 0; wander_result = 0; bexit_result = 0; heading_result = 0; alloc_ok = 1;
}
static int bad = 0;
static void expect(bool ok, const char* msg, long x = 0) { if (!ok) { std::printf("fail: %s (%ld) ev=%s\n", msg, x, ev.c_str()); ++bad; } }
static void run() { msg_run__7Cyclops(object); }
int main() {
    const std::string head = "NEA600;V1;B1;u-4;";
    // prologue
    reset(4); every_frame = 1; run(); expect(ev == "NE", "everyFrame stops");
    reset(4); activation = 0; run(); expect(ev == "NEA600;", "activation check stops");
    reset(4); advance = 4; run(); expect(ev == "NEA600;V1;M3:100;D10;B1;u-4;", "bit 2: attachment 3 and dust");
    reset(4); advance = 8; run(); expect(ev == "NEA600;V1;M2:100;D9;B1;u-4;", "bit 3: attachment 2 and dust");
    reset(4); advance = 12; run(); expect(ev == "NEA600;V1;M3:100;D10;B1;u-4;", "bit 2 wins over bit 3");
    for (int st : {4, 7, 11, 12, 17, 18, -1}) { reset(st); run(); expect(ev == head, "idle states", st); }
    // state 0
    reset(0); eiz_q = {1}; run(); expect(ev == head + "Z400/32767;s3@-8;", "0 near", 0);
    reset(0); eiz_q = {0, 1}; run(); expect(ev == head + "Z400/32767;Z500/16000;s3@-8;", "0 mid");
    reset(0); write_word(address(object) + 256u, 4999u); run(); expect(ev == head + "Z400/32767;Z500/16000;s1@-8;", "0 timer below");
    reset(0); write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "Z400/32767;Z500/16000;", "0 timer equal");
    reset(0); write_word(address(object) + 256u, 0x80000000u); run(); expect(ev == head + "Z400/32767;Z500/16000;s1@-8;", "0 signed compare");
    // states 1 and 2
    reset(1); eiz_q = {1}; run(); expect(ev == head + "Z400/32767;s0@-8;", "1 near");
    reset(1); eiz_q = {0, 1}; run(); expect(ev == head + "Z400/32767;Z500/16000;s0@-8;", "1 mid");
    reset(1); run(); expect(ev == head + "Z400/32767;Z500/16000;W;", "1 wander");
    reset(2); eiz_q = {1}; run(); expect(ev == head + "Z400/32767;s0@-8;", "2 near");
    reset(2); wander_result = 1; run(); expect(ev == head + "Z400/32767;Z500/16000;W;", "2 wander result nonzero");
    reset(2); wander_result = 0; run(); expect(ev == head + "Z400/32767;Z500/16000;W;s0@-8;", "2 wander result zero");
    // state 3
    reset(3); run(); expect(ev == head + "s0@-8;", "3 without a target");
    write_word(address(target) + 12u, address_bits(inner));
    auto with_target = [&](int st) { reset(st); write_word(address(object) + 576u, address_bits(target)); write_word(address(target) + 12u, address_bits(inner)); };
    with_target(3); write_word(address(object) + 960u, 5000u); eiz_q = {0, 0, 1}; run(); expect(ev == head + "Z100/1000;Z100/3000;Z100/32767;T;", "3 face enemy");
    with_target(3); write_word(address(object) + 960u, 5000u); run(); expect(ev == head + "Z100/1000;Z100/3000;Z100/32767;S;", "3 steer");
    with_target(3); write_word(address(object) + 960u, 5000u); eiz_q = {0, 1}; run(); expect(ev == head + "Z100/1000;Z100/3000;s13@-8;", "3 mid range");
    for (int r = 0; r < 8; ++r) {
        with_target(3); write_word(address(object) + 960u, 5000u); eiz_q = {1}; rolls = {r}; run();
        const int want = (r % 6 == 0) ? 14 : (r % 6 == 1) ? 15 : 6;
        expect(ev == head + "Z100/1000;R;s" + num(want) + "@-8;", "3 attack pick", r);
    }
    with_target(3); write_word(address(object) + 960u, 4999u); alt_result = 1; run();
    expect(ev == head + "Y0,100," + num((short)(100 - 32767)) + ",5000;s16@-8;", "3 alternate zone");
    with_target(3); write_word(address(object) + 960u, 4999u); alt_result = 0; eiz_q = {1}; rolls = {1}; run();
    expect(ev == head + "Y0,100," + num((short)(100 - 32767)) + ",5000;Z100/1000;R;s15@-8;", "3 alternate zone miss");
    with_target(3); write_word(address(object) + 960u, 0xfffffff0u); alt_result = 1; run(); expect(ev == head + "Y0,100," + num((short)(100 - 32767)) + ",5000;s16@-8;", "3 signed compare");
    // state 5
    reset(5); eiz_q = {0}; run(); expect(ev == head + "Z100/1000;X;", "5 idle");
    reset(5); eiz_q = {0}; bexit_result = 1; run(); expect(ev == head + "Z100/1000;X;s3@-8;", "5 persue idle exit");
    reset(5); eiz_q = {1}; rolls = {0}; bexit_result = 1; run(); expect(ev == head + "Z100/1000;R;s14@-8;X;s3@-8;", "5 attack then exit");
    reset(5); eiz_q = {1}; rolls = {7}; run(); expect(ev == head + "Z100/1000;R;s15@-8;X;", "5 attack 15");
    reset(5); eiz_q = {1}; rolls = {8}; run(); expect(ev == head + "Z100/1000;R;s6@-8;X;", "5 attack 6");
    // 8, 9
    reset(8); run(); expect(ev == head + "s3@-8;", "8 without bit 4");
    reset(8); write_u16(address(object) + 294u, 0x10u); run(); expect(ev == head, "8 with bit 4");
    reset(9); write_word(address(object) + 804u, 4999u); run(); expect(ev == head + "s0@-8;", "9 below");
    reset(9); write_word(address(object) + 804u, 5000u); run(); expect(ev == head, "9 equal");
    reset(9); write_word(address(object) + 804u, 0x80000000u); run(); expect(ev == head + "s0@-8;", "9 signed");
    // 10
    reset(10); write_u16(address(object) + 294u, 0x80u); run(); expect(ev == head + "J;", "10 remove from the run list");
    reset(10); advance = 1; rolls = {70000, 23}; run();
    expect(ev == head + "c100;i100;R;R;F;", "10 floor decal", 0);
    expect(floor_point[0] == 16.0f + 2.0f * 100.0f && floor_point[1] == 20.0f + 3.0f * 100.0f && floor_point[2] == 24.0f && floor_angle == (short)70000 && floor_size == 43.0f && floor_a == 0 && floor_b == 1, "10 decal arguments");
    reset(10); advance = 1; write_word(address(object) + 12u, 2u); run(); expect(ev == head, "10 bit 1 of +12 suppresses the decal");
    reset(10); advance = 2; run(); expect(ev == head && read_word(address(object) + 12u) == 1u, "10 sets bit 0 of +12");
    reset(10); advance = 2; write_word(address(object) + 12u, 2u); run(); expect(read_word(address(object) + 12u) == 2u, "10 keeps +12 when bit 1 is set");
    reset(10); advance = 3; run(); expect(ev == head + "c100;i100;R;R;F;" && read_word(address(object) + 12u) == 1u, "10 both");
    // 6, 13, 14, 15, 16
    for (int st : {6, 13, 14, 15, 16}) { reset(st); advance = 1; run(); expect(ev == head + "s3@-8;", "no target: only the tail", st); }
    reset(14); write_word(address(object) + 576u, address_bits(target)); run(); expect(ev == head + "s3@-8;", "14 without flag 1: tail only");
    reset(14); write_word(address(object) + 576u, address_bits(target)); write_u16(address(object) + 294u, 8u); advance = 1; run();
    expect(ev == head + "c100;i100;K108;Q300,40,0,1;U204;", "14 dust cloud");
    expect(seen_point[0] == 16.0f + 2.0f * 80.0f && seen_point[1] == 20.0f + 3.0f * 80.0f && seen_point[2] == 24.0f + 36.0f, "14 position");
    reset(14); write_word(address(object) + 576u, address_bits(target)); advance = 1; alloc_ok = 0; run(); expect(ev == head + "c100;i100;K108;U204;s3@-8;", "14 failed allocation");
    reset(16); write_word(address(object) + 576u, address_bits(target)); advance = 1; run();
    expect(ev == head + "c100;i100;K112;G300,40,1,5;U196;s3@-8;" && seen_owner == object, "16 gas spell");
    expect(seen_point[0] == 16.0f - 2.0f * 80.0f && seen_point[1] == 20.0f - 3.0f * 80.0f && seen_point[2] == 60.0f, "16 position");
    for (unsigned flags : {0u, 0x10u}) {
        reset(6); write_word(address(object) + 576u, address_bits(target)); write_word(address(object) + 244u, flags); advance = 1;
        write_u16(address(object) + 200u, 5); write_u16(address(object) + 202u, 6); write_u16(address(object) + 204u, 0xfffeu); run();
        expect(ev == head + "M" + num(flags ? 4 : 5) + ":100;D" + num(flags ? 11 : 12) + ";H60,5,6,-2,1048577;s3@-8;", "6 melee", flags);
    }
    reset(13); write_word(address(object) + 576u, address_bits(target)); advance = 1; run(); expect(ev.find("H60") != std::string::npos, "13 shares the melee branch");
    // 15: aim at the point held at (target+12)+16
    reset(15); write_word(address(object) + 576u, address_bits(target)); write_word(address(target) + 12u, address_bits(inner));
    write_single(address(inner) + 16u, 36.0f); write_single(address(inner) + 20u, 44.0f); write_single(address(inner) + 24u, 0.0f);
    advance = 1; heading_result = 16384 - 100; rolls = {1500, 22, 65536};
    run();
    expect(ev == head + "c100;i100;a20,24;R;c" + num((short)(100 + 1500 % 2000 - 1000)) + ";i" + num((short)(100 + 1500 % 2000 - 1000)) + ";R;K168;R;P19,20,21,23," + num(100 - 16384) + ";U180;s3@-8;", "15 projectile sequence", 0);
    expect(seen_point[0] == 16.0f + 2.0f * 10.0f && seen_point[1] == 20.0f + 3.0f * 10.0f && seen_point[2] == 24.0f + 120.0f, "15 position");
    expect(seen_velocity[0] == 2.0f * 260.0f && seen_velocity[1] == 3.0f * 260.0f && seen_velocity[2] == static_cast<float>(22 % 15) - 150.0f, "15 velocity");
    expect(seen_scale == 65536.0f * (1.0f / 65536.0f) * 0.5f + 0.5f && seen_owner == object && seen_map == table, "15 scale, owner and map");
    // 15: the aim is clamped to within 4000 of the facing
    for (int want : {0, 1, 2}) {
        reset(15); write_word(address(object) + 576u, address_bits(target)); write_word(address(target) + 12u, address_bits(inner)); advance = 1;
        heading_result = want == 0 ? 16384 - 100 : want == 1 ? 16384 - (100 + 5000) : 16384 - (100 - 5000); rolls = {1000, 0, 0};
        run();
        const int center = want == 0 ? 100 : want == 1 ? 100 + 4000 : 100 - 4000;
        expect(ev.find("c" + num(center) + ";i" + num(center) + ";") != std::string::npos, "15 aim clamp", want);
    }
    reset(15); write_word(address(object) + 576u, address_bits(target)); write_word(address(target) + 12u, address_bits(inner)); advance = 1; alloc_ok = 0; rolls = {0, 0};
    run(); expect(ev.find("K168;U180;") != std::string::npos && ev.find("P19") == std::string::npos, "15 failed allocation");
    // the tail: bit 3 of +294
    reset(15); write_word(address(object) + 576u, address_bits(target)); write_u16(address(object) + 294u, 8u); run(); expect(ev == head, "tail suppressed by bit 3");
    return bad ? 1 : 0;
}
