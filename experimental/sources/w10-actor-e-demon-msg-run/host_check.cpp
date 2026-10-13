// Host contract for msg_run__5Demon. Includes the source; not an EE input.
#define NORRATH_RTTI_C_HOST 1
#include "msg_run.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
using namespace rtti_c;
int frameNm;
unsigned char skills[252];
static std::string ev;
static std::deque<int> eiz_q, rolls, eiz2_q, crowd_q, skill104_q;
static int every_frame = 0, advance = 0, wander_result = 0, bexit_result = 0, yaw_result = 0, alloc_ok = 1, rank_result = 1, running_result = 0;
static void* type_result = nullptr;
static unsigned char object[1300], vtable[256], table[400], target[64], eobj[600], evt[64], blockmem[256], sk42[128], st42[256], sk57[128], st57[256], sk7[16];
static unsigned bits_of(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }
static std::string num(long v) { return std::to_string(v); }
static int pop_front(std::deque<int>& q) { if (q.empty()) return 0; const int v = q.front(); q.pop_front(); return v; }
static unsigned char soul_token[4], other_token[4];
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void*, void*) { ev += "N"; }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void*) { ev += "E"; return every_frame; }
extern "C" int objectActivationCheck__8Creaturei(void*, int d) { ev += "A" + num(d) + ";"; return 0; }
extern "C" int creatureAdvanceAnimation__8Creaturef(void*, float) { ev += "V;"; return advance; }
extern "C" void slideBack__8Creaturef(void*, float v) { ev += "B" + num(bits_of(v) == 0x3e4ccccc) + ";"; }
extern "C" int enemyInZone__8Creaturefs(void*, float d, short a) { ev += "Z" + num((long)d) + "/" + num(a) + ";"; return pop_front(eiz_q); }
extern "C" int enemyInZone2__8Creatureffssfi(void*, float a, float b, short c, short d, float e, int g) {
    ev += "Y" + num((long)a) + "," + num((long)b) + "," + num(c) + "," + num(d) + "," + num((long)e) + "," + num(g) + ";"; return pop_front(eiz2_q);
}
extern "C" int objectIsCrowded__8CreatureP10GameObjectffi(void*, void* other, float a, float b, int c) {
    ev += "C" + num((long)a) + "," + num((long)b) + "," + num(c) + (other == eobj ? "" : "!") + ";"; return pop_front(crowd_q);
}
extern "C" int smartWander__8Creature(void*) { ev += "W;"; return wander_result; }
extern "C" void turnToFaceEnemy__8Creature(void*) { ev += "T;"; }
extern "C" void steerAndRouteToEnemy__8Creature(void*) { ev += "S;"; }
extern "C" int bExitStatePersueIdle__8Creature(void*) { ev += "X;"; return bexit_result; }
extern "C" void move2__8CreatureG6Point3(void*, const void* p) { float v[3]; std::memcpy(v, p, 12); ev += "M" + num((long)(v[0] + v[1] + v[2])) + ";"; }
extern "C" void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float s, int a, int b, int c, int d, void* desc) {
    ev += "H" + num((long)s) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + (desc == object + 1032 ? "" : "!") + ";";
}
extern "C" int eRand__10GameObject(void*) { ev += "R;"; return pop_front(rolls); }
extern "C" float icos__Fi(int a) { ev += "c" + num(a) + ";"; return 2.0f; }
extern "C" float isin__Fi(int a) { ev += "i" + num(a) + ";"; return 3.0f; }
static float lead_before[3];
extern "C" void findEnemyLeadPosition__8CreaturefP6Point3(void*, float r, void* p) {
    ev += "D" + num((long)r) + ";"; std::memcpy(lead_before, p, 12); float* v = static_cast<float*>(p); v[0] = 500.0f; v[1] = 600.0f; v[2] = 700.0f;
}
extern "C" int creatureFindLeadYaw__8CreatureP10GameObjectf(void*, void* other, float r) { ev += "Y" + std::string(other == eobj ? "" : "!") + num((long)r) + ";"; return yaw_result; }
extern "C" void* blockAlloc__Fi(int n) { ev += "K" + num(n) + ";"; return alloc_ok ? blockmem : nullptr; }
static unsigned char soulmark[4], harmmark[4];
extern "C" void* __tf4Soul() { return soul_token; }
extern "C" void* __tf14SpellHarmTouch() { return harmmark; }
extern "C" int __eq__C9type_infoRC9type_info(void* l, void* r) { ev += "="; return l == r; }
extern "C" void __throw_bad_typeid() { ev += "!throw;"; }
extern "C" void __14SpellHarmTouchiP8CreatureP5Skill(void* self, int a, void* owner, void* skill) { ev += "h" + num(a) + (owner == object ? "" : "!") + (skill == sk42 ? "" : "?") + (self == blockmem ? "" : "*") + ";"; }
extern "C" void addSpell__8CreatureP5Spell(void*, void* spell) { ev += std::string("a") + (spell == blockmem ? "b" : spell == nullptr ? "n" : "?") + ";"; }
extern "C" void* checkIfSpellIsRunning__8CreatureRC9type_info(void*, void* type) { ev += std::string("k") + (type == harmmark ? "" : "!") + ";"; return running_result ? blockmem : nullptr; }
extern "C" void attack__14SpellHarmTouchP10GameObjectPiT2(void*, void* tgt, int* a, int* b) { ev += std::string("t") + (tgt == eobj ? "" : "!") + num(*a) + "," + num(*b) + ";"; *a += 100; *b += 200; }
extern "C" void creatureSlamEffect__8CreatureP8Creaturei(void*, void* other, int flag) { ev += std::string("m") + (other == eobj ? "" : "!") + num(flag) + ";"; }
static int sparks_ints[6]; static float sparks_position[3], sparks_velocity[3];
extern "C" void __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(void*, const void* position, const void* velocity, void* owner, void* ha, void* hb, int a, int b, int c, int d) {
    ev += std::string("Sparks") + (owner == object ? "" : "!") + (ha || hb ? "h" : "") + ";"; sparks_ints[0] = a; sparks_ints[1] = b; sparks_ints[2] = c; sparks_ints[3] = d;
    std::memcpy(sparks_position, position, 12); std::memcpy(sparks_velocity, velocity, 12);
}
static float fire_position[3]; static void* fire_skill;
extern "C" void __9FireStormiP10GameObjectG6Point3P5Skill(void*, int a, void* owner, const void* position, void* skill) {
    ev += "Fire" + num(a) + (owner == object ? "" : "!") + ";"; std::memcpy(fire_position, position, 12); fire_skill = skill;
}
static int weapon_flag; static void* weapon_model; static short weapon_angle; static float weapon_a, weapon_b;
extern "C" void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, const void* point, short angle, void* second_model, void* animation, float first, int flag, int zero, float second, void* matrix) {
    ev += std::string("w") + (description == object + 1032 ? "" : "!") + (state == object + 968 ? "" : "?") + (animation == object + 288 ? "" : "*") + (matrix ? "m" : "") + (zero ? "z" : "") + ";";
    weapon_flag = flag; weapon_model = model; weapon_angle = angle; weapon_a = first; weapon_b = second; (void)point; (void)second_model;
}
static float fire_a, fire_b, fire_c, fire_d; static int fire_angle;
extern "C" void fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(void*, void* animation, const void* point, short angle, void* st, float a, float b, float c, float d) {
    float p[3]; std::memcpy(p, point, 12);
    ev += std::string("f") + (animation == object + 288 ? "" : "!") + (st == object + 824 ? "" : "?") + (p[0] == 16.0f && p[1] == 20.0f && p[2] == 24.0f ? "" : "p") + ";";
    fire_angle = angle; fire_a = a; fire_b = b; fire_c = c; fire_d = d;
}
extern "C" void dropWeapon__5Demon(void*) { ev += "q;"; }
extern "C" void objectAddToDeleteList__FP10GameObject(void*) { ev += "x;"; }
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int flags) {
    ev += "o" + num(static_cast<unsigned char*>(h) - table) + "," + num(bits_of(a) == 0x3dcccccc && bits_of(b) == 0x3dcccccc) + "," + num(flags) + ";";
}
static void slot168(void* self) { ev += "u" + num(static_cast<unsigned char*>(self) - object) + ";"; }
static void slot176(void* self, int a, const void* p) { float v[3]; std::memcpy(v, p, 12); ev += "z" + num(static_cast<unsigned char*>(self) - object) + "," + num(a) + "," + num((long)v[0]) + ";"; }
static void slot184(void* self, int v) { ev += "s" + num(v) + "@" + num(static_cast<unsigned char*>(self) - object) + ";"; }
static void* type_fn() { ev += "y;"; return type_result; }
static float hurt_amount; static void* hurt_source; static int hurt_flag;
static void hurt_fn(void* self, float amount, void* source, int flag) { ev += std::string("hurt") + (self == eobj - 12 ? "" : "?") + ";"; hurt_amount = amount; hurt_source = source; hurt_flag = flag; }
static int rank_fn(void* self, int a, int b) { ev += "r" + num(a) + "," + num(b) + (self == sk42 - 8 ? "" : "!") + ";"; return rank_result; }
static int skill104_fn(void* self, int a, int b, void* c) { ev += "g" + num(a) + "," + num(b) + (c == object ? "" : "!") + (self == sk57 - 4 ? "" : "?") + ";"; return pop_front(skill104_q); }
static void reset(int state) {
    std::memset(object, 0, sizeof object); std::memset(vtable, 0, sizeof vtable); std::memset(table, 0, sizeof table); std::memset(target, 0, sizeof target); std::memset(eobj, 0, sizeof eobj);
    std::memset(evt, 0, sizeof evt); std::memset(sk42, 0, sizeof sk42); std::memset(st42, 0, sizeof st42); std::memset(sk57, 0, sizeof sk57); std::memset(st57, 0, sizeof st57);
    for (void* p : {(void*)table, (void*)target, (void*)eobj, (void*)evt, (void*)vtable, (void*)sk42, (void*)st42, (void*)sk57, (void*)st57, (void*)sk7, (void*)(skills + 0)}) host_note(p);
    host_note(reinterpret_cast<void*>(slot168)); host_note(reinterpret_cast<void*>(slot176)); host_note(reinterpret_cast<void*>(slot184));
    host_note(reinterpret_cast<void*>(type_fn)); host_note(reinterpret_cast<void*>(hurt_fn)); host_note(reinterpret_cast<void*>(rank_fn)); host_note(reinterpret_cast<void*>(skill104_fn));
    for (unsigned off : {12u, 132u}) { host_note(table + off); write_word(address(table) + off, address_bits(table + off)); }
    write_word(address(object) + 36u, address_bits(table));
    write_u16(address(vtable) + 168u, static_cast<unsigned>(-4)); write_word(address(vtable) + 172u, address_bits(reinterpret_cast<void*>(slot168)));
    write_u16(address(vtable) + 176u, static_cast<unsigned>(-16)); write_word(address(vtable) + 180u, address_bits(reinterpret_cast<void*>(slot176)));
    write_u16(address(vtable) + 184u, static_cast<unsigned>(-8)); write_word(address(vtable) + 188u, address_bits(reinterpret_cast<void*>(slot184)));
    write_word(address(object) + 72u, address_bits(vtable));
    write_word(address(target) + 12u, address_bits(eobj)); write_word(address(eobj) + 72u, address_bits(evt));
    write_word(address(evt) + 4u, address_bits(reinterpret_cast<void*>(type_fn)));
    write_u16(address(evt) + 16u, 12u); write_word(address(evt) + 20u, address_bits(reinterpret_cast<void*>(hurt_fn)));
    write_word(address(skills) + 4u * 42u, address_bits(sk42)); write_word(address(skills) + 4u * 57u, address_bits(sk57)); write_word(address(skills) + 4u * 7u, address_bits(sk7));
    write_word(address(sk42) + 96u, address_bits(st42)); write_u16(address(st42) + 96u, static_cast<unsigned>(-8)); write_word(address(st42) + 100u, address_bits(reinterpret_cast<void*>(rank_fn)));
    write_word(address(sk57) + 96u, address_bits(st57)); write_u16(address(st57) + 104u, static_cast<unsigned>(-4)); write_word(address(st57) + 108u, address_bits(reinterpret_cast<void*>(skill104_fn)));
    write_word(address(object) + 236u, 1u);
    write_u16(address(object) + 572u, static_cast<unsigned>(state));
    write_single(address(object) + 16u, 16.0f); write_single(address(object) + 20u, 20.0f); write_single(address(object) + 24u, 24.0f);
    write_single(address(object) + 28u, 2.0f); write_single(address(object) + 32u, 6.0f); write_single(address(object) + 224u, 7.0f); write_single(address(object) + 228u, 9.0f);
    write_u16(address(object) + 56u, 100u); write_word(address(object) + 808u, 5000u);
    ev.clear(); eiz_q.clear(); eiz2_q.clear(); crowd_q.clear(); rolls.clear(); skill104_q.clear(); every_frame = 0; advance = 0; wander_result = 0; bexit_result = 0;
    yaw_result = 0; alloc_ok = 1; rank_result = 1; running_result = 0; type_result = other_token; frameNm = 1000;
}
static void with_target() { write_word(address(object) + 576u, address_bits(target)); }
static int bad = 0;
static void expect(bool ok, const char* msg, long x = 0) { if (!ok) { std::printf("fail: %s (%ld) ev=%s\n", msg, x, ev.c_str()); ++bad; } }
static void run() { msg_run__5Demon(object); }
int main() {
    const std::string head = "NEA600;V;B1;u-4;";
    reset(2); write_word(address(object) + 960u, 1u); write_u16(address(object) + 572u, 6); run();
    expect(ev == "w;NEA600;V;B1;u-4;M0;s3@-8;", "weapon effect first, state 6 sequence");
    expect(weapon_flag == 1 && weapon_a == 0.5f && weapon_b == 1.0f && weapon_angle == 100, "weapon effect arguments", weapon_flag);
    reset(2); write_word(address(object) + 960u, 1u); run(); expect(ev == "w;NEA600;V;B1;u-4;" && weapon_flag == 0, "weapon effect, flag clear");
    reset(2); every_frame = 1; run(); expect(ev == "NE", "everyFrame stops");
    reset(2); write_word(address(object) + 236u, 0u); run(); expect(ev == "NEA600;", "zero +236 stops, activation result ignored");
    for (int st : {2, 4, 7, 11, 12, 18, 100, -1}) { reset(st); run(); expect(ev == head, "idle", st); }
    reset(0); eiz_q = {1}; run(); expect(ev == head + "Z400/32767;s3@-8;", "0 near");
    reset(0); write_word(address(object) + 256u, 4999u); run(); expect(ev == head + "Z400/32767;s1@-8;", "0 below");
    reset(0); write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "Z400/32767;", "0 equal");
    reset(1); eiz_q = {1}; run(); expect(ev == head + "M0;Z400/32767;s3@-8;", "1 near");
    reset(1); write_word(address(object) + 256u, 4999u); run(); expect(ev == head + "M0;Z400/32767;s0@-8;", "1 below");
    reset(1); write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "M0;Z400/32767;W;", "1 wander");
    // 3
    reset(3); run(); expect(ev == head + "s0@-8;", "3 without a target");
    reset(3); with_target(); type_result = soul_token; eiz_q = {1}; run(); expect(ev == head + "y;=Z42/5000;s13@-8;", "3 soul near");
    reset(3); with_target(); type_result = soul_token; eiz_q = {0, 1}; run(); expect(ev == head + "y;=Z42/5000;Z42/32767;T;", "3 soul face");
    reset(3); with_target(); type_result = soul_token; run(); expect(ev == head + "y;=Z42/5000;Z42/32767;S;", "3 soul steer");
    for (int r = 0; r < 12; ++r) { reset(3); with_target(); eiz_q = {1}; rolls = {r}; run(); expect(ev == head + "y;=Z42/5000;R;s" + num(r % 5 == 0 ? 14 : 6) + "@-8;", "3 attack pick", r); }
    reset(3); with_target(); eiz_q = {0, 1}; run(); expect(ev == head + "y;=Z42/5000;Z42/32767;T;", "3 other face");
    reset(3); with_target(); write_word(address(object) + 208u, 4u); eiz2_q = {1}; run(); expect(ev == head + "y;=Z42/5000;Z42/32767;Y0,400,100,5000,0,1;s15@-8;", "3 zone2");
    reset(3); with_target(); write_word(address(object) + 208u, 3u); eiz2_q = {1}; run(); expect(ev == head + "y;=Z42/5000;Z42/32767;C400,6,0;S;", "3 zone2 needs mode 4");
    reset(3); with_target(); crowd_q = {1}; rolls = {1000}; run();
    expect(ev == head + "y;=Z42/5000;Z42/32767;C400,6,0;R;s5@-8;" && read_word(address(object) + 256u) == 5000u + 1000u % 240u + 240u, "3 crowded");
    // 5
    reset(5); run(); expect(ev == head + "Z52/5000;X;", "5 idle");
    reset(5); write_word(address(object) + 208u, 4u); eiz2_q = {1}; run(); expect(ev == head + "Y0,400,100,5000,0,1;s15@-8;", "5 zone2");
    reset(5); eiz_q = {1}; rolls = {10}; run(); expect(ev == head + "Z52/5000;R;s14@-8;", "5 attack 14");
    reset(5); eiz_q = {1}; rolls = {11}; run(); expect(ev == head + "Z52/5000;R;s6@-8;", "5 attack 6");
    reset(5); bexit_result = 1; write_word(address(object) + 256u, 4999u); run(); expect(ev == head + "Z52/5000;X;s3@-8;", "5 exit, timer below");
    reset(5); bexit_result = 1; write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "Z52/5000;X;", "5 exit, no target");
    reset(5); bexit_result = 1; write_word(address(object) + 256u, 5000u); with_target(); crowd_q = {1}; run(); expect(ev == head + "Z52/5000;X;C400,6,1;", "5 exit, crowded");
    reset(5); bexit_result = 1; write_word(address(object) + 256u, 5000u); with_target(); crowd_q = {0}; run(); expect(ev == head + "Z52/5000;X;C400,6,1;s3@-8;", "5 exit, free");
    // 8, 9, 14
    reset(8); run(); expect(ev == head + "M0;s0@-8;", "8 without bit 4");
    reset(8); write_u16(address(object) + 294u, 0x10u); run(); expect(ev == head + "M0;", "8 with bit 4");
    reset(9); write_word(address(object) + 804u, 4999u); run(); expect(ev == head + "s3@-8;", "9 below");
    reset(9); write_word(address(object) + 804u, 5000u); run(); expect(ev == head, "9 equal");
    reset(14); write_word(address(object) + 256u, 4999u); run(); expect(ev == head + "s3@-8;", "14 below");
    reset(14); write_word(address(object) + 256u, 5000u); run(); expect(ev == head, "14 equal");
    // 10
    reset(10); write_u16(address(object) + 56u, 0xfff0u); object[40] = 200; for (unsigned i = 0; i < 12; ++i) if (i < 4) object[16 + i] = 0; write_single(address(object) + 16u, 16.0f); write_single(address(object) + 20u, 20.0f); write_single(address(object) + 24u, 24.0f);
    write_word(address(object) + 256u, 5000u); run();
    expect(ev == head + "M0;f;" && fire_angle == -16 && fire_a == 1.0f && fire_b == 7.0f && fire_c == 9.0f && fire_d == 200.0f, "10 fire and nothing else");
    reset(10); write_word(address(object) + 960u, 1u); write_word(address(object) + 256u, 4939u); run(); expect(ev == "w;" + head + "M0;f;q;" && read_word(address(object) + 12u) == 2u && (read_u64(address(object) + 160u) >> 41) == 1u, "10 drop weapon");
    reset(10); write_word(address(object) + 960u, 1u); write_word(address(object) + 256u, 4940u); run(); expect(ev == "w;" + head + "M0;f;" && read_word(address(object) + 12u) == 0u, "10 no drop at the boundary");
    reset(10); write_word(address(object) + 960u, 0u); write_word(address(object) + 256u, 4939u); run(); expect(ev == head + "M0;f;", "10 weapon word zero");
    reset(10); write_word(address(object) + 256u, 4924u); run(); expect(ev == head + "M0;f;x;", "10 delete");
    reset(10); write_word(address(object) + 256u, 4925u); run(); expect(ev == head + "M0;f;", "10 no delete at the boundary");
    // 13
    reset(13); run(); expect(ev == head + "M0;s3@-8;o132,1,32;", "13 without a target");
    reset(13); with_target(); run(); expect(ev == head + "M0;y;=s3@-8;o132,1,32;", "13 not a soul");
    reset(13); with_target(); type_result = soul_token; run(); expect(ev == head + "M0;y;=Z42/5000;s3@-8;o132,1,32;", "13 soul out of range");
    reset(13); with_target(); type_result = soul_token; eiz_q = {1}; write_single(address(eobj) + 560u, 5.0f); run(); expect(ev == head + "M0;y;=Z42/5000;T;", "13 soul in range, healthy");
    reset(13); with_target(); type_result = soul_token; eiz_q = {1}; write_single(address(eobj) + 560u, 0.0f); run(); expect(ev == head + "M0;y;=Z42/5000;T;s3@-8;", "13 soul at zero");
    reset(13); with_target(); type_result = soul_token; eiz_q = {1}; write_single(address(eobj) + 560u, -1.0f); run(); expect(ev == head + "M0;y;=Z42/5000;T;s3@-8;", "13 soul below zero");
    reset(13); with_target(); type_result = soul_token; eiz_q = {1}; write_word(address(eobj) + 560u, 0x7fc00000u); run(); expect(ev == head + "M0;y;=Z42/5000;T;", "13 NaN counts as above zero");
    reset(13); with_target(); type_result = soul_token; eiz_q = {1}; advance = 1; write_u16(address(object) + 202u, 3u); write_single(address(eobj) + 560u, 5.0f); run();
    expect(ev == head + "M0;y;=Z42/5000;yhurt;T;" || ev.find("hurt") != std::string::npos, "13 hurt call");
    expect(hurt_amount == 3.0f && hurt_source == object && hurt_flag == 0, "13 hurt arguments");
    // 6, 16, 17
    reset(6); run(); expect(ev == head + "M0;s3@-8;", "6 no target");
    reset(6); write_u16(address(object) + 294u, 8u); run(); expect(ev == head + "M0;", "6 no target, bit 3 set, no F bit 1");
    reset(6); with_target(); write_u16(address(object) + 294u, 8u); run(); expect(ev == head + "M0;T;", "6 target without F bit 0");
    reset(6); with_target(); write_u16(address(object) + 294u, 8u); advance = 1; write_u16(address(object) + 200u, 4); write_u16(address(object) + 202u, 5); write_u16(address(object) + 204u, 0xfffeu); run();
    expect(ev == head + "M0;H36,4,5,-2,8;T;", "6 melee");
    reset(17); with_target(); write_u16(address(object) + 294u, 8u); advance = 1; write_u16(address(object) + 200u, 4); write_u16(address(object) + 202u, 5); write_u16(address(object) + 204u, 0xfffeu); run();
    expect(ev == head + "M0;H36,4,5,-2," + num(0x80008) + ";T;", "17 adds a flag");
    reset(6); with_target(); write_word(address(object) + 208u, 3u); advance = 1; write_u16(address(object) + 294u, 8u); write_u16(address(object) + 200u, 4); write_u16(address(object) + 202u, 5); rolls = {40000}; run();
    expect(ev == head + "M0;R;H36,4,5,0,8;T;", "mode 3, roll too high");
    reset(6); with_target(); write_word(address(object) + 208u, 3u); advance = 1; write_u16(address(object) + 294u, 8u); write_u16(address(object) + 200u, 4); write_u16(address(object) + 202u, 5); rolls = {20000}; run();
    expect(ev == head + "M0;R;m1;H36,8,10,0,8;T;" , "mode 3, slam doubles");
    reset(6); with_target(); write_word(address(object) + 208u, 2u); advance = 1; write_u16(address(object) + 294u, 8u); write_word(address(object) + 964u, 999u); write_u16(address(object) + 200u, 4); write_u16(address(object) + 202u, 5);
    rank_result = 3; running_result = 1; run();
    expect(ev == head + "M0;r1,1;K28;h1;ab;k;t4,5;H36,104,205,0," + num(0x40008) + ";T;" && read_word(address(object) + 964u) == 1000u + 180u, "mode 2 harm touch");
    reset(6); with_target(); write_word(address(object) + 208u, 2u); advance = 1; write_u16(address(object) + 294u, 8u); write_word(address(object) + 964u, 1000u); run(); expect(ev.find("r1,1") == std::string::npos, "mode 2 waits for frameNm");
    reset(6); with_target(); write_word(address(object) + 208u, 2u); advance = 1; write_u16(address(object) + 294u, 8u); write_word(address(object) + 964u, 0xfffffff0u); alloc_ok = 0; run();
    expect(ev.find("K28;an;k;") != std::string::npos, "mode 2 failed allocation still calls addSpell");
    reset(6); with_target(); advance = 0; run(); expect(ev == head + "M0;T;s3@-8;", "6 target, no F, bit 3 clear");
    reset(6); with_target(); advance = 2; write_u16(address(object) + 294u, 8u); rolls = {2}; eiz_q = {1}; run(); expect(ev == head + "M0;T;R;Z42/5000;s16@-8;", "6 escalates to 16");
    reset(16); with_target(); advance = 2; write_u16(address(object) + 294u, 8u); rolls = {2}; eiz_q = {1}; run(); expect(ev == head + "M0;T;R;Z42/5000;s17@-8;", "16 escalates to 17");
    reset(17); with_target(); advance = 2; write_u16(address(object) + 294u, 8u); rolls = {2}; eiz_q = {1}; run(); expect(ev == head + "M0;T;R;Z42/5000;", "17 stays");
    reset(6); with_target(); advance = 2; write_u16(address(object) + 294u, 8u); rolls = {3}; run(); expect(ev == head + "M0;T;R;", "6 roll bit 0 set");
    reset(6); with_target(); advance = 2; write_u16(address(object) + 294u, 8u); rolls = {2}; run(); expect(ev == head + "M0;T;R;Z42/5000;", "6 not near");
    reset(6); with_target(); advance = 0; write_u16(address(object) + 294u, 8u); run(); expect(ev == head + "M0;T;", "6 needs F bit 1 to escalate");
    // 15
    reset(15); eiz_q = {1}; write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "Z42/5000;s6@-8;" , "15 without a target");
    reset(15); write_word(address(object) + 256u, 4999u); run(); expect(ev == head + "s3@-8;", "15 timer below, no target");
    reset(15); with_target(); write_word(address(object) + 256u, 5000u); run();
    expect(ev == head + "c100;i100;D400;z-16,0,500;Z42/5000;" && lead_before[0] == 16.0f + 2.0f * 300.0f && lead_before[1] == 20.0f + 3.0f * 300.0f && lead_before[2] == 24.0f, "15 lead and turn slot");
    reset(15); with_target(); advance = 1; rolls = {1, 0, 0}; write_word(address(object) + 256u, 5000u); run();
    expect(ev == head + "c100;i100;D400;R;K168;Fire1;z-16,0,500;Z42/5000;" && fire_position[0] == 500.0f && fire_position[1] == 600.0f && fire_position[2] == 706.0f && fire_skill == sk7, "15 fire storm");
    expect(read_u16(address(object) + 1048u) == 255u && read_u16(address(object) + 1050u) == 128u && read_u16(address(object) + 1052u) == 0u && read_u16(address(object) + 1054u) == 50u &&
           read_u16(address(object) + 1056u) == 255u && read_u16(address(object) + 1058u) == 128u && read_u16(address(object) + 1060u) == 0u && read_u16(address(object) + 1062u) == 100u, "15 fire storm halfwords");
    reset(15); with_target(); advance = 1; rolls = {1, 0, 0}; alloc_ok = 0; write_word(address(object) + 256u, 5000u); run(); expect(ev.find("K168;z") != std::string::npos && ev.find("Fire") == std::string::npos, "15 failed allocation");
    reset(15); with_target(); advance = 1; rolls = {0, 65536}; skill104_q = {10, 3, 4, 20}; yaw_result = 0x4000; write_word(address(object) + 256u, 5000u); run();
    expect(ev == head + "c100;i100;D400;R;g0,1;R;g1,1;g2,1;g3,1;Y20;c16384;i16384;K184;Sparks;z-16,0,500;Z42/5000;", "15 sparks sequence");
    expect(sparks_ints[0] == 10 && sparks_ints[1] == 10 + (int)(10.0f * single_from_bits(0x3e4ccccc) * 1.0f) && sparks_ints[2] == 3 && sparks_ints[3] == 4 * 3 * 4, "15 sparks numbers");
    expect(sparks_velocity[0] == 2.0f * 20.0f && sparks_velocity[1] == 3.0f * 20.0f && sparks_velocity[2] == 0.0f &&
           sparks_position[0] == 16.0f && sparks_position[1] == 20.0f && sparks_position[2] == 24.0f + 3.0f, "15 sparks vectors");
    reset(15); with_target(); advance = 0; write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "c100;i100;D400;z-16,0,500;Z42/5000;", "15 needs F bit 0 to spawn");
    reset(15); with_target(); eiz_q = {1}; write_word(address(object) + 256u, 5000u); run(); expect(ev == head + "c100;i100;D400;z-16,0,500;Z42/5000;s6@-8;", "15 goes to 6");
    reset(15); with_target(); write_word(address(object) + 256u, 4999u); run(); expect(ev == head + "c100;i100;D400;z-16,0,500;s3@-8;", "15 timer below");
    return bad ? 1 : 0;
}
