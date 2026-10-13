// Host contract for msg_run__13FemaleDarkElf. Includes the source; not an EE input.
#define NORRATH_RTTI_C_HOST 1
#include "msg_run.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
using namespace rtti_c;
char levelName[200];
unsigned char skills[252];
static std::string ev;
static std::deque<int> eiz_q, rolls, eiz2_q, crowd_q;
static int every_frame = 0, activation = 1, advance = 0, wander_result = 0, bexit_result = 0, strti_result = 1, yaw_result = 0, alloc_ok = 1;
static void* dead_result = nullptr;
static unsigned char object[1300], vtable[256], table[400], target[64], inner[64], blockmem[256], deadobj[128], deadtable[256];
static unsigned bits_of(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }
static std::string num(long v) { return std::to_string(v); }
static int pop_front(std::deque<int>& q) { if (q.empty()) return 0; const int v = q.front(); q.pop_front(); return v; }
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void*, void*) { ev += "N"; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void*) { ev += "E"; return every_frame; }
extern "C" int objectActivationCheck__8Creaturei(void*, int d) { ev += "A" + num(d) + ";"; return activation; }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float) { ev += "V;"; return advance; }
extern "C" void slideBack__8Creaturef(void*, float v) { ev += "B" + num(bits_of(v)) + ";"; }
extern "C" int enemyInZone__8Creaturefs(void*, float d, short a) { ev += "Z" + num((long)d) + "/" + num(a) + ";"; return pop_front(eiz_q); }
extern "C" int enemyInZone2__8Creatureffssfi(void*, float a, float b, short c, short d, float e, int g) {
    ev += "Y" + num((long)a) + "," + num((long)b) + "," + num(c) + "," + num(d) + "," + num((long)e) + "," + num(g) + ";"; return pop_front(eiz2_q);
}
extern "C" int objectIsCrowded__8CreatureP10GameObjectffi(void*, void* other, float a, float b, int c) {
    ev += "C" + num((long)a) + "," + num((long)b) + "," + num(c) + (other == inner ? "" : "!") + ";"; return pop_front(crowd_q);
}
extern "C" int smartWander__8Creature(void*) { ev += "W;"; return wander_result; }
extern "C" void turnToFaceEnemy__8Creature(void*) { ev += "T;"; }
extern "C" void steerAndRouteToEnemy__8Creature(void*) { ev += "S;"; }
extern "C" int bExitStatePersueIdle__8Creature(void*) { ev += "X;"; return bexit_result; }
extern "C" void flee2__8CreatureP10GameObject(void*, void* other) { ev += std::string("L") + (other == inner ? "" : "!") + ";"; }
extern "C" void move2__8CreatureG6Point3(void*, const void* p) { float v[3]; std::memcpy(v, p, 12); ev += "M" + num((long)(v[0] + v[1] + v[2])) + ";"; }
static int melee_args[5];
extern "C" void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float s, int a, int b, int c, int d, void* desc) {
    ev += "H" + num((long)s) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + (desc == object + 1032 ? "" : "!") + ";";
}
extern "C" void objectRemoveFromRunList__FP10GameObject(void*) { ev += "J;"; }
static float floor_point[3]; static int floor_angle; static float floor_size; static int floor_a, floor_b;
extern "C" void floorAdd__FG6Point3sfii(const void* point, short angle, float size, int a, int b) {
    ev += "F;"; std::memcpy(floor_point, point, 12); floor_angle = angle; floor_size = size; floor_a = a; floor_b = b;
}
extern "C" int eRand__10GameObject(void*) { ev += "R;"; return pop_front(rolls); }
extern "C" float icos__Fi(int a) { ev += "c" + num(a) + ";"; return 2.0f; }
extern "C" float isin__Fi(int a) { ev += "i" + num(a) + ";"; return 3.0f; }
static float lead_before[3];
extern "C" void findEnemyLeadPosition__8CreaturefP6Point3(void*, float r, void* p) {
    ev += "D" + num((long)r) + ";"; std::memcpy(lead_before, p, 12); float* v = static_cast<float*>(p); v[0] = 500.0f; v[1] = 600.0f; v[2] = 700.0f;
}
extern "C" int strticmp__FPCcT0(const char* a, const char* b) { ev += std::string("Q") + (a == levelName + 81 ? "" : "!") + b + ";"; return strti_result; }
extern "C" void* ISeeDeadPeople__8Creaturef(void*, float r) { ev += "I" + num((long)r) + ";"; return dead_result; }
extern "C" int creatureFindLeadYaw__8CreatureP10GameObjectf(void*, void* other, float r) { ev += "Y" + std::string(other == inner ? "" : "!") + num((long)r) + ";"; return yaw_result; }
extern "C" void* blockAlloc__Fi(int n) { ev += "K" + num(n) + ";"; return alloc_ok ? blockmem : nullptr; }
static float bolt_position[3], rain_position[3]; static int bolt_yaw = 0, bolt_b = 0; static void *bolt_owner, *bolt_skill, *rain_owner, *rain_skill, *rain_header, *block_seen;
extern "C" void __11DiseaseBoltsiG6Point3P10GameObjectP5Skill(void* self, int a, int b, const void* position, void* owner, void* skill) {
    ev += "O;"; block_seen = self; bolt_yaw = a; bolt_b = b; std::memcpy(bolt_position, position, 12); bolt_owner = owner; bolt_skill = skill;
}
extern "C" void __10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader(void* self, int a, void* owner, const void* position, void* skill, void* header) {
    ev += "P" + num(a) + ";"; block_seen = self; std::memcpy(rain_position, position, 12); rain_owner = owner; rain_skill = skill; rain_header = header;
}
static std::string lump_name, lump_item; static void* sound_header; static void* sound_object;
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { ev += "G;"; lump_name = l; lump_item = n; return reinterpret_cast<void*>(0x7777); }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float r) { ev += "U;"; sound_header = h; sound_object = o; (void)r; }
static void slot168(void* self) { ev += "u" + num(static_cast<unsigned char*>(self) - object) + ";"; }
static void slot184(void* self, int v) {
    if (self >= deadobj - 16 && self < deadobj + 112) ev += "d" + num(v) + "@" + num(static_cast<unsigned char*>(self) - deadobj) + ";";
    else ev += "s" + num(v) + "@" + num(static_cast<unsigned char*>(self) - object) + ";";
}
static void reset(int state) {
    std::memset(object, 0, sizeof object); std::memset(vtable, 0, sizeof vtable); std::memset(table, 0, sizeof table);
    std::memset(target, 0, sizeof target); std::memset(inner, 0, sizeof inner); std::memset(deadobj, 0, sizeof deadobj); std::memset(deadtable, 0, sizeof deadtable);
    host_note(table); host_note(target); host_note(inner); host_note(vtable); host_note(deadtable); host_note(deadobj); host_note(skills); host_note(skills + 16); host_note(skills + 96);
    host_note(reinterpret_cast<void*>(slot168)); host_note(reinterpret_cast<void*>(slot184));
    for (unsigned off : {12u, 316u}) { host_note(table + off); write_word(address(table) + off, address_bits(table + off)); }
    write_word(address(object) + 36u, address_bits(table));
    write_u16(address(vtable) + 168u, static_cast<unsigned>(-4)); write_word(address(vtable) + 172u, address_bits(reinterpret_cast<void*>(slot168)));
    write_u16(address(vtable) + 184u, static_cast<unsigned>(-8)); write_word(address(vtable) + 188u, address_bits(reinterpret_cast<void*>(slot184)));
    write_word(address(object) + 72u, address_bits(vtable));
    write_u16(address(deadtable) + 184u, 4u); write_word(address(deadtable) + 188u, address_bits(reinterpret_cast<void*>(slot184)));
    write_word(address(deadobj) + 72u, address_bits(deadtable));
    write_word(address(skills) + 16u, address_bits(skills + 16)); write_word(address(skills) + 96u, address_bits(skills + 96));
    write_u16(address(object) + 572u, static_cast<unsigned>(state));
    write_single(address(object) + 16u, 16.0f); write_single(address(object) + 20u, 20.0f); write_single(address(object) + 24u, 24.0f);
    write_single(address(object) + 28u, 2.0f); write_single(address(object) + 32u, 6.0f);
    write_u16(address(object) + 56u, 100u); write_word(address(object) + 808u, 5000u);
    ev.clear(); eiz_q.clear(); eiz2_q.clear(); crowd_q.clear(); rolls.clear(); every_frame = 0; activation = 1; advance = 0; wander_result = 0; bexit_result = 0;
    strti_result = 1; yaw_result = 0; alloc_ok = 1; dead_result = nullptr;
}
static void with_target() { write_word(address(object) + 576u, address_bits(target)); write_word(address(target) + 12u, address_bits(inner)); }
static int bad = 0;
static void expect(bool ok, const char* msg, long x = 0) { if (!ok) { std::printf("fail: %s (%ld) ev=%s\n", msg, x, ev.c_str()); ++bad; } }
static void run() { msg_run__13FemaleDarkElf(object); }
int main() {
    const std::string head = "NEA600;V;B" + num(bits_of(0.5f)) + ";u-4;";
    const std::string head1 = "NEA600;V;B1050253721;u-4;";
    reset(2); every_frame = 1; run(); expect(ev == "NE", "everyFrame stops");
    reset(2); activation = 0; run(); expect(ev == "NEA600;", "activation stops");
    reset(2); object[960] = 1; run(); expect(ev == "NEA600;V;B1050253721;u-4;", "kind 1 slides by 0.3");
    reset(2); object[960] = 0xff; run(); expect(ev == head, "other kinds slide by 0.5");
    for (int st : {2, 4, 11, 12, 15, 16, 100, -1}) { reset(st); run(); expect(ev == head, "idle", st); }
    // state 0 and 1
    reset(0); eiz_q = {1}; run(); expect(ev == head + "Z300/32767;s3@-8;", "0 near");
    reset(0); eiz_q = {0, 1}; run(); expect(ev == head + "Z300/32767;Z400/16000;s3@-8;", "0 mid");
    reset(0); write_word(address(object) + 256u, 4999u); run(); expect(ev == head + "Z300/32767;Z400/16000;s1@-8;", "0 below");
    reset(0); write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "Z300/32767;Z400/16000;", "0 equal");
    reset(0); write_word(address(object) + 256u, 0x80000000u); run(); expect(ev == head + "Z300/32767;Z400/16000;s1@-8;", "0 signed");
    reset(1); eiz_q = {1}; run(); expect(ev == head + "Z300/32767;s0@-8;", "1 near");
    reset(1); eiz_q = {0, 1}; run(); expect(ev == head + "Z300/32767;Z400/16000;s0@-8;", "1 mid");
    reset(1); run(); expect(ev == head + "Z300/32767;Z400/16000;W;", "1 wander");
    // state 3
    reset(3); run(); expect(ev == head + "s0@-8;", "3 without a target");
    reset(3); with_target(); eiz_q = {1}; run(); expect(ev == head + "Z22/6000;s6@-8;", "3 kind 0 near");
    reset(3); with_target(); eiz_q = {0, 1}; run(); expect(ev == head + "Z22/6000;Z22/32767;T;", "3 kind 0 face");
    reset(3); with_target(); crowd_q = {0}; run(); expect(ev == head + "Z22/6000;Z22/32767;C400,6,0;S;", "3 kind 0 steer");
    reset(3); with_target(); crowd_q = {1}; rolls = {1000}; run();
    expect(ev == head + "Z22/6000;Z22/32767;C400,6,0;R;s5@-8;" && read_word(address(object) + 256u) == 5000u + 1000u % 240u + 240u, "3 kind 0 crowded");
    reset(3); with_target(); object[960] = 2; eiz_q = {1}; run(); expect(ev == head + "Z100/32767;s7@-8;", "3 kind 2 close");
    reset(3); with_target(); object[960] = 2; eiz2_q = {1}; run(); expect(ev == head + "Z100/32767;Y0,400,100,2000,0,1;s13@-8;", "3 kind 2 zone2");
    reset(3); with_target(); object[960] = 2; run(); expect(ev == head + "Z100/32767;Y0,400,100,2000,0,1;S;", "3 kind 2 steer");
    // state 5
    reset(5); run(); expect(ev == head + "X;", "5 idle");
    reset(5); with_target(); eiz_q = {1}; run(); expect(ev == head + "Z22/6000;s6@-8;", "5 kind 0 near");
    reset(5); object[960] = 1; eiz2_q = {1}; run(); expect(ev == head1 + "Y0,400,100,2000,0,1;s13@-8;", "5 kind 1 zone2");
    reset(5); object[960] = 1; bexit_result = 1; write_word(address(object) + 256u, 4999u); run(); expect(ev == head1 + "Y0,400,100,2000,0,1;X;s3@-8;", "5 exit, timer below");
    reset(5); bexit_result = 1; write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "X;", "5 exit, no target");
    reset(5); bexit_result = 1; with_target(); write_word(address(object) + 256u, 5000u); crowd_q = {1}; run(); expect(ev == head + "Z22/6000;X;C400,6,1;", "5 exit, crowded");
    reset(5); bexit_result = 1; with_target(); write_word(address(object) + 256u, 5000u); eiz_q = {0}; crowd_q = {0}; run(); expect(ev == head + "Z22/6000;X;C400,6,1;s3@-8;", "5 exit, free");
    // 7, 8, 9, 14
    reset(7); write_word(address(object) + 256u, 5001u); with_target(); run(); expect(ev == head + "L;", "7 flee");
    reset(7); write_word(address(object) + 256u, 5001u); run(); expect(ev == head + "s3@-8;", "7 no target");
    reset(7); write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "s3@-8;", "7 timer not above");
    reset(14); write_word(address(object) + 256u, 4999u); run(); expect(ev == head + "s3@-8;", "14 below");
    reset(14); write_word(address(object) + 256u, 5000u); run(); expect(ev == head, "14 equal");
    reset(9); write_word(address(object) + 804u, 4999u); run(); expect(ev == head + "s3@-8;", "9 below");
    reset(9); write_word(address(object) + 804u, 5000u); run(); expect(ev == head, "9 equal");
    const unsigned long long a_bits[] = {0x0030000000000000ull, 0x0020000000000000ull, 0, 0x00b0000000000000ull, 0x00a0000000000000ull, 0x0090000000000000ull, 0x00a8000000000000ull, 0xffffffffffffffffull};
    const bool a_set[] = {false, false, true, false, false, true, false, true};
    for (int i = 0; i < 8; ++i) {
        reset(8); write_u64(address(object) + 288u, a_bits[i]); run();
        const unsigned long long s = a_bits[i];
        const bool want = (s & 0x0030000000000000ull) == 0ull || (s & 0x00b0000000000000ull) == 0x00a0000000000000ull;
        expect(ev == head + (want ? "s3@-8;" : ""), "8", i);
        (void)a_set;
    }
    // state 10
    reset(10); write_u16(address(object) + 294u, 0x80u); run(); expect(ev == head + "J;", "10 remove from run list");
    reset(10); advance = 0x10; rolls = {70000, 23}; run();
    expect(ev == head + "R;R;F;" && floor_point[0] == 16.0f && floor_point[1] == 20.0f && floor_point[2] == 24.0f && floor_angle == (short)70000 && floor_size == 23.0f && floor_a == 0 && floor_b == 1, "10 decal");
    reset(10); advance = 0x10; write_word(address(object) + 12u, 2u); run(); expect(ev == head, "10 bit 1 of +12");
    reset(10); advance = 1; run(); expect(ev == head, "10 needs bit 4 of F");
    // state 6
    reset(6); run(); expect(ev == head + "M0;s3@-8;" || ev == head + "M0;", "6 no target", 0);
    reset(6); write_word(address(object) + 256u, 3u); run(); expect(ev == head + "M0;" && read_word(address(object) + 256u) == 2u, "6 counts down");
    reset(6); write_word(address(object) + 256u, 1u); run(); expect(ev == head + "M0;s3@-8;" && read_word(address(object) + 256u) == 0u, "6 reaches zero");
    reset(6); write_word(address(object) + 256u, 0x80000000u); run(); expect(ev == head + "M0;" && read_word(address(object) + 256u) == 0x7fffffffu, "6 wraps to a positive counter");
    reset(6); write_word(address(object) + 256u, 3u); write_u16(address(object) + 294u, 8u); run(); expect(ev == head + "M0;" && read_word(address(object) + 256u) == 3u, "6 holds with bit 3");
    reset(6); with_target(); write_word(address(object) + 256u, 3u); advance = 1; write_u16(address(object) + 200u, 4); write_u16(address(object) + 202u, 5); write_u16(address(object) + 204u, 0xfffeu); run();
    expect(ev == head + "M0;H36,4,5,-2,8;" && (read_u64(address(object) + 160u) & 0x200ull) != 0ull, "6 melee and flag");
    reset(6); with_target(); write_word(address(object) + 256u, 3u); write_u16(address(object) + 294u, 8u); run(); expect(ev == head + "M0;T;" && (read_u64(address(object) + 160u) & 0x200ull) == 0ull, "6 turns");
    // state 13
    reset(13); rolls = {1, 2, 3}; run(); expect(ev == head + "R;R;R;s3@-8;", "13 without a target: three rolls");
    reset(13); with_target(); rolls = {1, 2, 3}; write_word(address(object) + 256u, 3u); run();
    expect(ev == head + "R;R;R;c100;i100;D400;T;Z100/32767;s7@-8;" || ev == head + "R;R;R;c100;i100;D400;T;Z100/32767;", "13 no flag", 0);
    expect(lead_before[0] == 16.0f + 2.0f * 300.0f && lead_before[1] == 20.0f + 3.0f * 300.0f && lead_before[2] == 24.0f, "13 point");
    // pick 0: DiseaseBolt
    reset(13); with_target(); advance = 1; rolls = {1, 2, 3, 4}; yaw_result = 0x12345; write_word(address(object) + 256u, 3u); eiz_q = {0}; strti_result = 0; run();
    expect(ev == head + "R;R;R;c100;i100;D400;T;R;Q" + "islcaves;" + "Y" + "350;K124;O;Z100/32767;", "13 pick 0 sequence", 0);
    expect(bolt_yaw == 0x12345 && bolt_b == 1 && bolt_owner == object && bolt_skill == skills + 16 && bolt_position[0] == 16.0f && bolt_position[1] == 20.0f && bolt_position[2] == 24.0f + 3.0f, "13 bolt arguments");
    expect(read_u16(address(object) + 150u) == 255u && read_u16(address(object) + 154u) == 100u && read_u16(address(object) + 140u) == 255u && read_u16(address(object) + 142u) == 255u &&
           read_u16(address(object) + 148u) == 255u && read_u16(address(object) + 146u) == 50u && read_u16(address(object) + 144u) == 0u && read_u16(address(object) + 152u) == 0u, "13 pick 0 halfwords");
    // pick 1: PoisonRain
    reset(13); with_target(); advance = 1; rolls = {1, 2, 3, 5}; write_word(address(object) + 256u, 3u); eiz_q = {0}; strti_result = 0; run();
    expect(ev == head + "R;R;R;c100;i100;D400;T;R;Qislcaves;K108;P1;Z100/32767;", "13 pick 1 sequence");
    expect(rain_owner == object && rain_skill == skills + 96 && rain_header == table + 316 && rain_position[0] == 500.0f && rain_position[1] == 600.0f && rain_position[2] == 700.0f, "13 rain arguments");
    expect(read_u16(address(object) + 146u) == 50u && read_u16(address(object) + 150u) == 255u && read_u16(address(object) + 154u) == 100u && read_u16(address(object) + 140u) == 0u &&
           read_u16(address(object) + 142u) == 255u && read_u16(address(object) + 144u) == 0u && read_u16(address(object) + 148u) == 0u && read_u16(address(object) + 152u) == 0u, "13 pick 1 halfwords");
    reset(13); with_target(); advance = 1; rolls = {1, 2, 3, 5}; write_word(address(object) + 256u, 3u); alloc_ok = 0; strti_result = 0; run(); expect(ev.find("K108;Z") != std::string::npos && ev.find("P1") == std::string::npos, "13 failed allocation");
    // pick 3: the dead creature
    reset(13); with_target(); advance = 1; rolls = {1, 2, 3, 4}; write_word(address(object) + 256u, 3u); strti_result = 5; dead_result = deadobj; run();
    expect(ev == head + "R;R;R;c100;i100;D400;T;R;Q" + "islcaves;I300;d15@4;G;U;Z100/32767;", "13 pick 3 sequence");
    expect(lump_name == "fx.lmp" && lump_item == "spel_he_heal.anm" && sound_object == deadobj && sound_header == reinterpret_cast<void*>(0x7777), "13 sound");
    expect(read_u16(address(object) + 146u) == 50u && read_u16(address(object) + 150u) == 128u && read_u16(address(object) + 152u) == 255u && read_u16(address(object) + 142u) == 128u &&
           read_u16(address(object) + 144u) == 255u && read_u16(address(object) + 154u) == 150u && read_u16(address(object) + 140u) == 0u && read_u16(address(object) + 148u) == 0u, "13 pick 3 halfwords");
    reset(13); with_target(); advance = 1; rolls = {1, 2, 3, 5}; write_word(address(object) + 256u, 3u); strti_result = 5; dead_result = nullptr; run();
    expect(ev.find("I300;") != std::string::npos && ev.find("K108;P1;") != std::string::npos, "13 no dead creature falls back to the roll");
    reset(13); with_target(); advance = 1; rolls = {1, 2, 3, 4}; write_word(address(object) + 256u, 3u); strti_result = 0; dead_result = deadobj; run(); expect(ev.find("I300") == std::string::npos, "13 islcaves skips the dead search");
    // the tail
    reset(13); with_target(); write_word(address(object) + 256u, 3u); write_u16(address(object) + 294u, 8u); run(); expect(ev.find("Z100") == std::string::npos && read_word(address(object) + 256u) == 3u, "13 bit 3 holds the counter");
    reset(13); with_target(); write_word(address(object) + 256u, 1u); run(); expect(read_word(address(object) + 256u) == 0u, "13 counts down");
    return bad ? 1 : 0;
}
