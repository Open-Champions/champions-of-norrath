// Host contract for newDamageHandler__8CreatureP12_modelHeader. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off host_check.cpp   (the source is included for its structures)
// Every callee is a stub that records its arguments in order. eRand returns scripted values (1, a miss, once the script runs out) so the
// one-in-three branches are chosen by the test. Expected numbers are computed here from the rules
// described in the source comment, not by calling the function under test.
#define NORRATH_HOST_CONTRACT 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>
#include "new_damage_handler.cpp"

int nmPlayers;
int g_BloodLevel;
signed char netNetworkMode;
int asyncRandState;
unsigned char trailParticleSpark[16];
PlayerView player[4];
void* skills[64];

static std::vector<std::string> log_;
static std::deque<int> rolls;
static int rolls_used;
static std::vector<int> box_answer(6);
static int tag_answer, alive_answer, rank_answer;
static float chance_answer;
static std::vector<bool> alloc_script;
static size_t alloc_next;
static std::vector<std::string> xp_log;
static std::vector<Point3> effect_points;
static std::vector<int> effect_kinds;
static std::vector<std::vector<int>> gut_args;      // a, b
static std::vector<Point3> gut_points;
static struct FloorCall { Point3 at; short a; float scale; int b, c; } floor_call;
static int floor_calls;
static std::vector<float> icos_args_seen;
static std::vector<int> angle_args;
static int entry184_arg_last;
static std::vector<int> entry184_args;
static float entry184_hit_speed;
static unsigned entry184_bits;
static unsigned char blocks[40][1024];
static int blocks_used;
static std::string fmt_seen, lump_seen_name;
static Point3 alert_point;
static float alert_radius;
static int alert_calls;
static int particles;
static Point3 first_particle_mid, last_particle_velocity;
static void* disint_model;
static int disint_flag, disint_calls;
static int ice_calls;
static std::vector<int> particle_marks;
static std::vector<Point3> chunk_velocities;

static void entry184(void*, int arg) { log_.push_back("entry184"); entry184_args.push_back(arg); }
extern "C" void* norrath_host_function(unsigned handle) { return handle == 184 ? reinterpret_cast<void*>(entry184) : nullptr; }

extern "C" {
void sendAlertMessage__FifG6Point3P10GameObject(int kind, float radius, const Point3* from, void*) {
    log_.push_back("alert"); ++alert_calls; alert_point = *from; alert_radius = radius; if (kind != 0) log_.push_back("BAD ALERT KIND");
}
void modelGetBoundingBox__FP12_modelHeaderlPiN52(void*, int index, int* a, int* b, int* c, int* d, int* e, int* f) {
    log_.push_back("bbox"); if (index != -1) log_.push_back("BAD BOX INDEX");
    *a = box_answer[0]; *b = box_answer[1]; *c = box_answer[2]; *d = box_answer[3]; *e = box_answer[4]; *f = box_answer[5];
}
float icos__Fi(int angle) { angle_args.push_back(angle); return 0.5f; }
float isin__Fi(int angle) { angle_args.push_back(angle); return 0.25f; }
void addEffect__FiG6Point3(int kind, const Point3* where) { log_.push_back("effect"); effect_kinds.push_back(kind); effect_points.push_back(*where); }
void* blockAlloc__Fi(int size) {
    log_.push_back("alloc");
    if (size != 616 && size != 184) log_.push_back("BAD ALLOC SIZE");
    bool ok = alloc_next < alloc_script.size() ? alloc_script[alloc_next] : true;
    ++alloc_next;
    if (!ok) return nullptr;
    return blocks[blocks_used++];
}
void __8GutChunkG6Point3iif(void* chunk, const Point3* where, int a, int b, float scale) {
    log_.push_back("gut"); gut_args.push_back({a, b}); gut_points.push_back(*where); if (scale != 1.0f) log_.push_back("BAD GUT SCALE");
    Chunk* c = static_cast<Chunk*>(chunk); c->velocity_x = 2.0f; c->velocity_y = 3.0f;
}
int eRand__10GameObject(void*) {
    log_.push_back("rand"); ++rolls_used;
    if (rolls.empty()) return 1;   // unscripted draws are a miss for every one-in-three test
    int v = rolls.front(); rolls.pop_front(); return v;
}
int sprintf(char* destination, const char* format, ...) { fmt_seen = format; destination[0] = 'x'; destination[1] = 0; return 0; }
void* lumpFindResource__FPcT0(char* lump, char* name) { lump_seen_name = lump; log_.push_back("lump"); return reinterpret_cast<void*>(0x77); }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float volume) { log_.push_back("sound"); if (volume != 1.0f) log_.push_back("BAD VOLUME"); }
static int attach_calls;
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void*, const Point3*, short, void*, int a, Point3* out, int, float) {
    log_.push_back("attach"); out->x = attach_calls ? 10.0f : 2.0f; out->y = attach_calls ? 20.0f : 4.0f; out->z = attach_calls ? 30.0f : 6.0f; ++attach_calls; (void)a;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void*, Point3* where, Point3* velocity, float) {
    log_.push_back("particle"); if (particles++ == 0) first_particle_mid = *where; last_particle_velocity = *velocity;
}
void floorAdd__FG6Point3sfii(const Point3* where, short a, float scale, int b, int c) { log_.push_back("floorAdd"); ++floor_calls; floor_call = {*where, a, scale, b, c}; }
void initStuff__6PhPropi(void* chunk, int kind) {
    log_.push_back("initStuff"); if (kind != 18) log_.push_back("BAD INIT KIND");
    const Chunk* c = static_cast<const Chunk*>(chunk); chunk_velocities.push_back({c->velocity_x, c->velocity_y, 0.0f});
}
void disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi(void*, void* model, void*, const Point3*, short, float, float, int flag) {
    log_.push_back("disint"); ++disint_calls; disint_model = model; disint_flag = flag;
}
void fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff(void*, void*, const Point3*, short, void*, float, float, float) { log_.push_back("ice"); ++ice_calls; }
int findTagInt__10GameObjectPci(void*, char* tag, int) { log_.push_back(std::string("tag:") + tag); return tag_answer; }
unsigned animSetToEndFullyBlended__FP14AnimationState(void*) { log_.push_back("animEnd"); return 0; }
float powf(float base, float exponent) { log_.push_back("powf"); return base * exponent; }
int playerGetSkillRank__Fii(int p, int skill) { log_.push_back("rank:" + std::to_string(p) + ":" + std::to_string(skill)); return rank_answer; }
float getChance__12SkillLifeTapi(void*, int rank) { log_.push_back("chance:" + std::to_string(rank)); return chance_answer; }
void __7OniBallG6Point3iP10GameObjectP10_vagHeader(void*, const Point3*, int, void*, void*) { log_.push_back("oni"); }
int gameCheckPlayerAlive__Fv() { return alive_answer; }
void givePlayerExperience__Fii(int p, int amount) { log_.push_back("xp"); xp_log.push_back(std::to_string(p) + ":" + std::to_string(amount)); }
}

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static int count(const char* name) { int n = 0; for (auto& s : log_) if (s == name) ++n; return n; }
static bool has(const char* name) { return count(name) != 0; }
static bool clean() { for (auto& s : log_) if (s.rfind("BAD", 0) == 0) return false; return true; }

alignas(16) static unsigned char vtable[256];
static DamageCreature creature;
static unsigned char model_memory[16];
static AttackerKey attacker_object;

static void reset() {
    std::memset(&creature, 0, sizeof creature);
    std::memset(vtable, 0, sizeof vtable);
    std::memset(blocks, 0, sizeof blocks);
    std::memset(player, 0, sizeof player);
    log_.clear(); rolls.clear(); rolls_used = 0; xp_log.clear(); effect_points.clear(); effect_kinds.clear(); gut_args.clear(); gut_points.clear();
    angle_args.clear(); chunk_velocities.clear(); entry184_args.clear(); alloc_script.clear(); alloc_next = 0; blocks_used = 0; floor_calls = 0; alert_calls = 0;
    particles = 0; disint_calls = 0; ice_calls = 0; attach_calls = 0; fmt_seen.clear(); lump_seen_name.clear();
    box_answer.assign(6, 0); tag_answer = 0; alive_answer = 1; rank_answer = 0; chance_answer = 0.0f;
    nmPlayers = 2; g_BloodLevel = 0; netNetworkMode = 0; asyncRandState = 0;
    VirtualEntry* e = reinterpret_cast<VirtualEntry*>(vtable + 184);
    e->delta = 0; e->function = 184;
    creature.vtable = reinterpret_cast<const VirtualEntry*>(vtable);
    creature.position = {10.0f, 20.0f, 30.0f};
    creature.radius = 10.0f;
    creature.pending = 5.0f;
    creature.state = 3;
    creature.hit_speed = 7.5f;
    creature.f560 = 100.0f;
    attacker_object.key = 0x1234;
}
static int runs;
static void run(bool with_model = true) {
    newDamageHandler__8CreatureP12_modelHeader(&creature, with_model ? model_memory : nullptr);
    ++runs;
    if (!clean()) { std::fprintf(stderr, "fail: a stub reported an error in run %d\n", runs); ++failures; }
}

int main() {
    // 1. Pending already zero: only the hit-speed table and the state-8 call run.
    reset(); creature.pending = 0.0f; creature.state = 10; creature.bits = 0x40000;
    run();
    expect(creature.bits == 0 && creature.hit_speed == 7.5f && log_.empty(), "state 10 with nothing pending: only the bits are cleared");
    reset(); creature.pending = 0.0f; creature.bits = 0x40000;
    run();
    expect(creature.hit_speed == 50.0f && creature.bits == 0 && log_.empty(), "pending zero: hit speed 50 for 0x40000, bits cleared (not 8192)");
    reset(); creature.pending = 0.0f; creature.bits = 0x80000; run(); expect(creature.hit_speed == 75.0f, "0x80000 gives 75");
    reset(); creature.pending = 0.0f; creature.bits = 0x100000; run(); expect(creature.hit_speed == 100.0f, "0x100000 gives 100");
    reset(); creature.pending = 0.0f; creature.bits = 0x200000; run(); expect(creature.hit_speed == 156.25f, "0x200000 gives 156.25");
    reset(); creature.pending = -2.0f; creature.bits = 0x40000 | 8192; run(); expect(creature.hit_speed == 50.0f && creature.bits == 0, "negative pending counts as handled; bits other than 8192 clear");
    reset(); creature.pending = 0.0f; creature.bits = 8192; rolls = {4};
    run();
    expect(entry184_args.empty() && creature.bits == 0 && rolls_used == 1, "bits 8192, roll not divisible by 3: no state call (bit 0x4000000 absent)");
    reset(); creature.pending = 0.0f; creature.bits = 8192; rolls = {4};
    creature.state = 8; run();
    expect(entry184_args.empty(), "state 8 never calls the entry");
    reset(); creature.pending = 0.0f; creature.bits = 8192; rolls = {6};
    run();
    expect(entry184_args.size() == 1 && entry184_args[0] == 8 && creature.bits == 0, "roll divisible by 3 and state not 8 calls the entry with 8");
    reset(); creature.pending = 0.0f; creature.bits = 8192; rolls = {6}; creature.state = 8; run();
    expect(entry184_args.empty() && creature.bits == 0, "roll divisible by 3 but state 8: no call");

    // 2. Pending: alert, clears pending, state 10 returns without clearing the bits.
    reset(); nmPlayers = 1; creature.state = 10; creature.bits = 0x5;
    run();
    expect(alert_calls == 1 && alert_point.x == 10.0f && alert_point.z == 30.0f && alert_radius == 600.0f, "single player: alert from the position with 600");
    expect(creature.pending == 0.0f && creature.bits == 0x5 && (creature.flags & 0x100) == 0, "state 10: pending cleared, nothing else touched");
    reset(); nmPlayers = 2; creature.bits = 0; creature.f560 = 5.0f; rolls = {};
    run();
    expect(alert_calls == 0 && (creature.flags & 0x100) != 0, "two players: no alert; flag 0x100 set");

    // 3. Gore effect point and the effect chain.
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.f860 = 1.0f; creature.radius = 10.0f;
    box_answer = {0, 0, 0, 0, 0, 50};
    run();
    expect(effect_kinds.size() == 1 && effect_kinds[0] == 10 && effect_points[0].x == 10.0f && effect_points[0].y == 20.0f && effect_points[0].z == 66.0f, "f860 above zero: effect 10 at position + 36 (box top 50 is not below 36)");
    expect(has("bbox"), "the bounding box is asked for when a model exists");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.f860 = 1.0f; box_answer = {0, 0, 4, 0, 0, 20};
    run(false);
    expect(!has("bbox") && effect_points[0].z == 66.0f, "no model: no bounding box and no height adjustment");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.f860 = 1.0f; box_answer = {0, 0, 4, 0, 0, 20};
    run();
    expect(effect_points[0].z == 30.0f + 8.0f + 12.0f, "box top under 36: z = position + (top - bottom)/2 + 12");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.f860 = 1.0f; box_answer = {0, 0, 40, 0, 0, 30};
    run();
    expect(effect_points[0].z == 66.0f, "a non-positive span keeps position + 36");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.f860 = 1.0f; box_answer = {0, 0, 0, 0, 0, 36};
    run();
    expect(effect_points[0].z == 66.0f, "box top of exactly 36 is not adjusted");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.f860 = 1.0f; creature.radius = 40.0f; creature.facing = 1000;
    run();
    expect(angle_args.size() == 2 && angle_args[0] == 1000 + 32767 && angle_args[1] == 1000 + 32767, "radius 25 or more: cos and sin of facing + 32767");
    expect(effect_points[0].x == 10.0f + 0.5f * (40.0f * 0.75f) && effect_points[0].y == 20.0f + 0.25f * (40.0f * 0.75f), "the point moves 0.75 radius along the facing");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.f860 = 1.0f; creature.radius = 25.0f; run();
    expect(angle_args.size() == 2, "a radius of exactly 25 counts");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.f860 = 1.0f; creature.radius = 24.9f; run();
    expect(angle_args.empty(), "below 25 no direction");
    struct { unsigned long long f64; int kind; } simple[] = {{0x4000, 10}, {0x8000, 11}, {0x10000, 2}};
    for (auto& s : simple) {
        reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = s.f64; run();
        expect(effect_kinds.size() == 1 && effect_kinds[0] == s.kind && gut_args.empty(), "single-effect flag selects its effect number");
    }

    // 4. Flags 0x2000 and 0x1000 effects with gut chunks.
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x2000; rolls = {2, 2};
    run();
    expect(effect_kinds == std::vector<int>({3}) && gut_args.size() == 1 && gut_args[0] == std::vector<int>({2, 5}), "0x2000: effect 3, one chunk (2,5); roll 2 is not a hit");
    expect(has("entry184") == false && creature.f560 == 5.0f, "still alive: handled by the alive path");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x2000; rolls = {3};
    run();
    expect(gut_args.size() == 2, "roll divisible by 3 adds a second chunk");
    reset(); creature.bits = 0x1; creature.f560 = 0.0f; creature.flags64 = 0x2000; rolls = {3, 3};
    run(); // dead: two more rolls in the chunk path, then death handling consumes more
    expect(gut_args.size() == 3 && gut_args[2] == std::vector<int>({2, 5}), "dead: one more chance of a chunk after the first two");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x1000; rolls = {3, 3};
    run();
    expect(effect_kinds == std::vector<int>({9}) && gut_args.size() == 2 && gut_args[0] == std::vector<int>({2, 3}), "0x1000: effect 9 and (2,3) chunks");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x2000; alloc_script = {false}; rolls = {3};
    run();
    expect(gut_args.size() == 1, "a failed allocation skips that chunk only");

    // 5. Blood-level path with flag 0x40000.
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x40000; g_BloodLevel = 0; netNetworkMode = 0;
    run();
    expect(effect_kinds.empty(), "no blood level and no network mode: no effect");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x40000; g_BloodLevel = 2; rolls = {3, 3};
    run();
    expect(effect_kinds == std::vector<int>({0}) && gut_args.size() == 3, "blood level: effect 0 and three chunks when both rolls hit");
    expect(gut_args[0] == std::vector<int>({2, 1}) && gut_args[1] == std::vector<int>({2, 1}) && gut_args[2] == std::vector<int>({2, 0}), "chunk numbers 1, 1, 0");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x40000; netNetworkMode = 1; rolls = {1};
    run();
    expect(effect_kinds == std::vector<int>({0}) && gut_args.size() == 1, "network mode alone also enables it");
    reset(); creature.bits = 0x1; creature.f560 = 0.0f; creature.flags64 = 0x40000; g_BloodLevel = 1; rolls = {1, 3, 4};
    run();
    expect(gut_args.size() == 2 && gut_args[1][0] == 4 % 3 + 1 && gut_args[1][1] == 0, "dead: a final chunk with number roll%3 + 1");

    // 6. Special sound and particle effect (0x20000).
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x20000; creature.angle = 0;
    for (int i = 0; i < 400; ++i) rolls.push_back(i % 2 ? 40000 : 10000);   // sums stay inside the unit sphere often enough
    run();
    expect(fmt_seen == "hit_mtlplate_bludg_med.anm" && lump_seen_name == "fx.lmp" && has("sound") && particles == 50, "0x20000: sound and fifty particles");
    expect(first_particle_mid.x == (2.0f + 10.0f) * 0.5f && first_particle_mid.y == (4.0f + 20.0f) * 0.5f && first_particle_mid.z == (6.0f + 30.0f) * 0.5f, "particles start at the midpoint of the two attachments");
    expect(count("attach") == 2, "two attachment points are asked for");

    // 7. Death: clamps, flags and the plain award path.
    reset(); creature.bits = 0x4; creature.f560 = 0.0f; creature.f856 = 3.0f; creature.f860 = 0.5f; creature.f864 = 1.0f; creature.f868 = 9.0f; creature.f872 = 0.25f;
    nmPlayers = 0; rolls = {5};
    run();
    expect(creature.f856 == 1.0f && creature.f860 == 0.5f && creature.f864 == 1.0f && creature.f868 == 1.0f && creature.f872 == 0.25f, "death clamps the five floats to 1.0");
    expect((creature.flags & 9) == 9, "death sets flags 9");
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; nmPlayers = 1;
    run();
    expect((creature.flags & 0x2000000) != 0 && count("disint") == 0 && entry184_args == std::vector<int>({10}) && (creature.flags & 0x2000) != 0, "bit 0x20 marks the creature removed and goes to the award");
    reset(); creature.bits = 0x10; creature.f560 = 0.0f; nmPlayers = 1;
    run();
    expect(disint_calls == 1 && disint_flag == 0 && disint_model == model_memory, "bit 0x10 with a model disintegrates (flag 0)");
    reset(); creature.bits = 0x2000000; creature.f560 = 0.0f; nmPlayers = 1; run();
    expect(disint_calls == 1 && disint_flag == 1, "bit 0x2000000 with a model disintegrates (flag 1)");
    reset(); creature.bits = 0x10; creature.f560 = 0.0f; nmPlayers = 1; run(false);
    expect(disint_calls == 0, "no model: no disintegration");
    reset(); creature.bits = 0x4; creature.f560 = 0.0f; creature.flags64 = 0x20000000; creature.f860 = 1.0f; nmPlayers = 1; rolls = {0}; run();
    expect(ice_calls == 0 && rolls_used == 1, "flag 0x20000000 skips the freeze (only the life-tap roll is drawn)");

    // 8. Freezing.
    reset(); creature.bits = 0x4; creature.f560 = 0.0f; creature.f860 = 1.0f; nmPlayers = 1; rolls = {8};
    run();
    expect(ice_calls == 1 && (creature.flags & 0x4002) == 0x4002 && (creature.flags64 & 0x20000000000ull) != 0 && entry184_args == std::vector<int>({10}), "roll with low three bits zero freezes, sets flags and goes to the award");
    reset(); creature.bits = 0x4; creature.f560 = 0.0f; creature.f860 = 1.0f; nmPlayers = 1; rolls = {4};
    run();
    expect(ice_calls == 0, "only the low three bits count: 4 does not freeze");
    reset(); creature.bits = 0x4; creature.f560 = 0.0f; creature.f860 = 1.0f; nmPlayers = 1; rolls = {9};
    run(); // not frozen, not the debris bits
    expect(ice_calls == 0 && (creature.flags64 & 0x20000000000ull) == 0, "other rolls do not freeze");

    // 9. Hit speeds on death (before the state call).
    struct { unsigned bits; float speed; } deadspeed[] = {{0x40000, 100.0f}, {0x80000, 150.0f}, {0x100000, 200.0f}, {0x200000, 312.5f}};
    for (auto& d : deadspeed) {
        reset(); creature.bits = d.bits | 0x20; creature.f560 = 0.0f; nmPlayers = 1; run();
        expect(creature.hit_speed == d.speed, "death hit speed table");
    }
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; tag_answer = 1; nmPlayers = 1; run();
    expect(count("animEnd") == 1 && creature.bits == 0 && !has("xp") && count("tag:Dead") == 1, "a Dead tag jumps the animation to its end and skips the rewards");

    // 10. Rewards.
    // Two players alive, attacker is player 1: weights 0.4 for others, 0.6 for the attacker.
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; nmPlayers = 3; alive_answer = 2; creature.f220 = 24.0f; creature.f564 = 90.0f; creature.attacker = &attacker_object;
    for (int i = 0; i < 3; ++i) { player[i].byte89 = 1; player[i].float172 = 1.0f; player[i].float196 = 10.0f; player[i].float200 = 12.0f; player[i].word80 = 0; player[i].key21872 = (i == 1) ? 0x1234 : 0x9999; }
    rank_answer = 4; chance_answer = 0.0f; rolls = {100};
    run();
    // share = 24 / 3 = 8; powf stub returns base * exponent = 8 * 1.4 = 11.2 -> 11; reward = int(11 * 0.75) = 8.
    expect(rolls_used == 1 && count("rank:1:59") == 1 && count("chance:4") == 1, "attacker 1 found by its key; skill rank asked for it");
    expect(xp_log == std::vector<std::string>({"0:3", "1:4", "2:3"}), "experience: others 0.4 * 8 = 3, the attacker 0.6 * 8 = 4");
    expect(player[1].float196 == 12.0f && player[0].float196 == 10.0f && player[2].float196 == 10.0f, "life tap is added for the attacker only and capped at float200");
    expect(!has("oni"), "chance zero spawns no ball");
    // Fewer than two alive: everyone with positive float172 gets the whole reward.
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; nmPlayers = 2; alive_answer = 1; creature.f220 = 4.0f; creature.f564 = 30.0f;
    player[0].byte89 = 1; player[0].float172 = 1.0f; player[0].float196 = 1.0f; player[0].float200 = 100.0f;
    player[1].byte89 = 1; player[1].float172 = 0.0f; player[1].float196 = 1.0f; player[1].float200 = 100.0f;
    rolls = {100};
    run();
    // share = 4 / 2 = 2; powf stub -> 2.8 -> 2; reward = int(2 * 0.75) = 1.
    expect(xp_log == std::vector<std::string>({"0:1"}), "players with float172 not above zero get nothing");
    expect(player[0].float196 == 1.0f + 30.0f * 0x1.111110p-5f && player[1].float196 == 1.0f, "life tap for the rewarded player: float564 * 1/30");
    expect(count("rank:-1:59") == 1, "no attacker object: rank is asked for player -1");
    // Attacker is player 0: it still gets the attacker weights.
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; nmPlayers = 2; alive_answer = 2; creature.f220 = 24.0f; creature.f564 = 90.0f; creature.attacker = &attacker_object;
    for (int i = 0; i < 2; ++i) { player[i].byte89 = 1; player[i].float172 = 1.0f; player[i].float196 = 10.0f; player[i].float200 = 12.0f; player[i].key21872 = (i == 0) ? 0x1234 : 0x9999; }
    rolls = {100};
    run();
    // share = 24 / 2 = 12; powf stub -> 16.8 -> 16; reward = 12; weights 0.6 and 0.4.
    expect(xp_log == std::vector<std::string>({"0:7", "1:4"}) && count("rank:0:59") == 1, "attacker index 0 is found and weighted");
    // The last matching player wins when two share the key.
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; nmPlayers = 3; alive_answer = 1; creature.attacker = &attacker_object;
    for (int i = 0; i < 3; ++i) { player[i].key21872 = (i == 0) ? 0x9999 : 0x1234; }
    rolls = {100};
    run();
    expect(count("rank:2:59") == 1, "when players 1 and 2 share the key, the later one is chosen");
    // Life tap ball.
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; nmPlayers = 1; chance_answer = 0.5f; rolls = {100};
    run();
    expect(has("oni"), "roll 100 < 0.5 * 65536 spawns the ball");
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; nmPlayers = 1; chance_answer = 0.5f; rolls = {40000};
    run();
    expect(!has("oni"), "roll 40000 >= 32768 spawns none");
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; nmPlayers = 1; chance_answer = 0.5f; rolls = {32768};
    run();
    expect(!has("oni"), "the comparison is strict");
    reset(); creature.bits = 0x20; creature.f560 = 0.0f; nmPlayers = 1; chance_answer = 0.5f; rolls = {32767}; alloc_script = {false};
    run();
    expect(!has("oni") && count("alloc") == 1, "failed allocation of the ball skips it");

    // 11. Debris spray (bits 0x40800, flags 0x1000): floorAdd and three effects.
    reset(); creature.bits = 0x40800; creature.f560 = 0.0f; creature.flags64 = 0x1000; g_BloodLevel = 1; nmPlayers = 1; creature.facing = 0; asyncRandState = 12345;
    for (int i = 0; i < 40; ++i) rolls.push_back(1000 + i);
    run();
    {
        const unsigned M = 0x19660d, C = 0x3c6ef35f;
        unsigned first = 12345u * M + C, second = first * M + C;
        expect(floor_calls == 1 && floor_call.a == static_cast<short>(static_cast<int>(first) >> 16) && floor_call.b == static_cast<int>((second >> 16) & 1) + 7 && floor_call.c == 0 && floor_call.scale == 44.0f, "floorAdd arguments from the generator state");
        expect(floor_call.at.x == 10.0f && floor_call.at.y == 20.0f && floor_call.at.z == 30.0f, "floorAdd at the position");
        // Independent simulation of the three sprays.
        unsigned state = second;
        std::vector<Point3> expected;
        for (int k = 0; k < 3; ++k) {
            Point3 v; unsigned t = state, third;
            for (;;) {
                unsigned r1 = t * M + C, r2 = r1 * M + C; third = r2 * M + C;
                float x = (float)(int)(r1 >> 16) * 0x1.0p-16f, y = (float)(int)(r2 >> 16) * 0x1.0p-16f, z = (float)(int)(third >> 16) * 0x1.0p-16f;
                v = {x + x - 1.0f, y + y - 1.0f, z + z - 1.0f};
                if (!(1.0f < v.x * v.x + v.y * v.y + v.z * v.z)) break;
                t = third;
            }
            state = third;
            expected.push_back({v.x * 10.0f + 10.0f, v.y * 10.0f + 20.0f, v.z * 10.0f + 66.0f});
        }
        size_t spray_first = effect_kinds.size() - 3;
        expect(effect_kinds.size() >= 3 && effect_kinds[spray_first] == 9 && effect_kinds[spray_first + 1] == 9 && effect_kinds[spray_first + 2] == 9, "three effects of kind 9 for flag 0x1000");
        bool same = effect_points.size() >= 3;
        for (int k = 0; same && k < 3; ++k) same = effect_points[spray_first + k].x == expected[k].x && effect_points[spray_first + k].y == expected[k].y && effect_points[spray_first + k].z == expected[k].z;
        expect(same && asyncRandState == static_cast<int>(state), "spray targets and the final generator state match the arithmetic");
    }
    expect(count("gut") == 6 && (creature.flags & 0x4002) == 0x4002 && (creature.flags64 & 0x20000000000ull) != 0, "2 + 4 thrown chunks and the debris flags");
    {
        size_t first_chunk = 0;
        for (size_t i = 0; i < gut_args.size(); ++i) if (gut_args[i][1] == 2) { first_chunk = i; break; }
        expect(gut_args.size() == 6 && gut_args[0][1] == 2 && gut_args[5][1] == 2, "chunk kind 2 for flag 0x1000 in both loops");
        (void)first_chunk;
    }
    expect(count("initStuff") == 6, "every thrown chunk is started");
    {
        // First thrown chunk (loop 2): rolls are consumed as [remainder, magnitude, angle] after the earlier draws.
        // Recompute from the logged angle argument and the stub trig values (cos 0.5, sin 0.25).
        expect(chunk_velocities.size() == 6, "six velocity records");
        const float c = 0x1.3333320p-2f;
        bool same = true;
        for (size_t i = 0; i < chunk_velocities.size(); ++i) {
            // Each stub trig call appended its angle; the magnitude is unknown here, so test the ratio: vx - 2c = 0.5 m, vy - 3c = 0.25 m.
            const float mx = (chunk_velocities[i].x - 2.0f * c) / 0.5f, my = (chunk_velocities[i].y - 3.0f * c) / 0.25f;
            same = same && std::fabs(mx - my) < 1e-3f * std::fabs(mx) + 1e-4f;
        }
        expect(same, "x and y gain the same magnitude along cos and sin; both old velocities are scaled by 0.3");
    }
    // Thrown chunks with scripted draws: six chunks, each drawing [remainder, magnitude, angle].
    reset(); creature.bits = 0x40800; creature.f560 = 0.0f; creature.flags64 = 0x1000; g_BloodLevel = 0; nmPlayers = 1; creature.facing = 100;
    {
        const int script[6][3] = {{4, 1000, 5}, {5, 60000, 15999}, {7, 32768, 0}, {8, 100, 39999}, {9, 20000, 20000}, {10, 5, 12345}};
        for (auto& row : script) for (int v : row) rolls.push_back(v);
        run();
        expect(chunk_velocities.size() == 6 && angle_args.size() == 12, "six chunks, two trig calls each");
        bool ok = chunk_velocities.size() == 6 && angle_args.size() == 12;
        for (int i = 0; ok && i < 6; ++i) {
            const bool first_loop = i < 2;
            const float scale = first_loop ? 80.0f : 50.0f, base = first_loop ? 20.0f : 10.0f;
            const unsigned range = first_loop ? 16000u : 40000u, start = first_loop ? 0xe0c0u : 0xb1e0u;
            const float magnitude = (float)script[i][1] * 0x1.0p-16f * scale + base;
            const short offset = static_cast<short>(start + static_cast<unsigned>(script[i][2]) % range);
            const float vx = 2.0f * 0x1.3333320p-2f + 0.5f * magnitude, vy = 3.0f * 0x1.3333320p-2f + 0.25f * magnitude;
            ok = ok && std::fabs(chunk_velocities[i].x - vx) < 1e-3f && std::fabs(chunk_velocities[i].y - vy) < 1e-3f
                    && angle_args[2 * i] == 100 + offset && angle_args[2 * i + 1] == 100 + offset;
        }
        expect(ok, "each thrown chunk: magnitude and angle follow the second and third draws of its three");
        expect(gut_args.size() == 6 && gut_args[0][0] == 4 % 3 && gut_args[1][0] == 5 % 3 && gut_args[2][0] == 7 % 3, "chunk number is the first draw modulo three");
    }
    reset(); creature.bits = 0x40800; creature.f560 = 0.0f; creature.flags64 = 0x2000; g_BloodLevel = 1; nmPlayers = 1; asyncRandState = 99;
    for (int i = 0; i < 40; ++i) rolls.push_back(7);
    run();
    expect(floor_calls == 1 && floor_call.b - static_cast<int>(((((99u * 0x19660du + 0x3c6ef35fu) * 0x19660du + 0x3c6ef35fu) >> 16) & 1)) == 4, "flag 0x2000: floorAdd base 4");
    expect(effect_kinds.size() >= 3 && effect_kinds[effect_kinds.size() - 1] == 3, "flag 0x2000: spray effect kind 3");
    reset(); creature.bits = 0x40800; creature.f560 = 0.0f; creature.flags64 = 0x40000; g_BloodLevel = 1; nmPlayers = 1; asyncRandState = 99;
    for (int i = 0; i < 40; ++i) rolls.push_back(7);
    run();
    expect(floor_calls == 1 && floor_call.b - static_cast<int>(((((99u * 0x19660du + 0x3c6ef35fu) * 0x19660du + 0x3c6ef35fu) >> 16) & 1)) == 1, "other flags: floorAdd base 1");
    expect(effect_kinds.size() >= 3 && effect_kinds[effect_kinds.size() - 1] == 1, "other flags: spray effect kind 1");
    reset(); creature.bits = 0x40800; creature.f560 = 0.0f; creature.flags64 = 0x2000; g_BloodLevel = 0; nmPlayers = 1;
    for (int i = 0; i < 40; ++i) rolls.push_back(7);
    run();
    expect(floor_calls == 0 && gut_args.size() == 6 && gut_args[0][1] == 5, "no blood level: no floor spray, kind 5 for flag 0x2000");
    reset(); creature.bits = 0x40800; creature.f560 = 0.0f; creature.flags64 = 0; g_BloodLevel = 0; nmPlayers = 1;
    for (int i = 0; i < 40; ++i) rolls.push_back(7);
    run();
    expect(gut_args.empty() && !has("alloc"), "no 0x3000 flag, no blood level and no network mode: no debris");
    reset(); creature.bits = 0x40800; creature.f560 = 0.0f; creature.flags64 = 0x40000; g_BloodLevel = 1; nmPlayers = 1;
    for (int i = 0; i < 40; ++i) rolls.push_back(7);
    run();
    expect(gut_args.size() == 6 && gut_args[0][1] == 0 && gut_args[2][1] == 1, "flag 0x40000: loop 2 uses kind 0, loop 3 uses kind 1");
    reset(); creature.bits = 0x800; creature.f560 = 0.0f; creature.flags64 = 0x1000; g_BloodLevel = 1; nmPlayers = 1;
    run();
    expect(floor_calls == 0 && gut_args.empty(), "both 0x40000 and 0x800 are needed for debris");

    // 12. Alive reactions.
    reset(); creature.bits = 0x40000; creature.f560 = 5.0f; creature.flags64 = 0; rolls = {1, 1};
    run();
    expect(creature.hit_speed == 100.0f && creature.bits == 0 && entry184_args.empty(), "alive: hit speed 100 for 0x40000, no state call for rolls not divisible by 3");
    reset(); creature.bits = 0x100000; creature.f560 = 5.0f; creature.flags64 = 0; rolls = {1}; run();
    expect(creature.hit_speed == 225.0f, "alive speed for 0x100000 is 225");
    reset(); creature.bits = 0x200000; creature.f560 = 5.0f; creature.flags64 = 0; rolls = {1}; run();
    expect(creature.hit_speed == 375.0f, "alive speed for 0x200000 is 375");
    reset(); creature.bits = 0x80000; creature.f560 = 5.0f; creature.flags64 = 0; rolls = {1}; run();
    expect(creature.hit_speed == 150.0f, "alive speed for 0x80000 is 150");
    reset(); creature.bits = 0x400000 | 0x40000; creature.f560 = 5.0f; creature.flags64 = 0; run();
    expect(creature.bits == 0 && rolls_used == 0 && creature.hit_speed == 100.0f, "0x400000 returns before any roll");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0; rolls = {3};
    run();
    expect(entry184_args == std::vector<int>({8}) && creature.bits == 0, "alive: a roll divisible by 3 calls the entry with 8");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x400; rolls = {1, 1};
    run();
    expect(entry184_args == std::vector<int>({8}), "flag 0x400 with a non-multiple roll and state not 8 calls at once");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.flags64 = 0x400; rolls = {1}; creature.state = 8;
    run();
    expect(rolls_used == 2 || rolls_used == 1, "flag 0x400 with state 8 falls to the second roll");
    reset(); creature.bits = 0x1 | 0x4000000; creature.f560 = 5.0f; creature.flags64 = 0; rolls = {1};
    run();
    expect(entry184_args == std::vector<int>({8}) && creature.bits == 0, "bit 0x4000000 allows the second call when the roll misses");
    reset(); creature.bits = 0x1 | 0x4000000; creature.f560 = 5.0f; creature.flags64 = 0; rolls = {1}; creature.state = 8; run();
    expect(entry184_args.empty(), "state 8 blocks the second call");
    reset(); creature.bits = 0x1; creature.f560 = 5.0f; creature.state = 3; creature.pending = 5.0f; creature.flags64 = 0; rolls = {1}; run();
    expect(creature.pending == 0.0f, "the pending damage is consumed");

    if (failures) return 1;
    std::printf("newDamageHandler contract passed\n");
    return 0;
}
