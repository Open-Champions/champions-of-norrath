// Host contract for move2__8CreatureG6Point3. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off host_check.cpp   (the source is included for its structures)
// Every callee is a stub that records its arguments. Expected values are computed in the
// test from the described rules, not by calling the function under test.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#define NORRATH_HOST_CONTRACT 1
#include "move2.cpp"

void* world = reinterpret_cast<void*>(0x1000);
static void entry48(void* self, void* arg);
static void entry80(void*, void* arg);
static void entry88(void* self, void* arg);
extern "C" void* norrath_host_function(unsigned handle) {
    switch (handle) {
        case 48: return reinterpret_cast<void*>(entry48);
        case 80: return reinterpret_cast<void*>(entry80);
        case 88: return reinterpret_cast<void*>(entry88);
    }
    return nullptr;
}
int frameNm;

static std::vector<std::string> log_;
static int move_answer, perturb_answer, check_answer;
static float ground_answer;
static std::vector<MoveObject*> candidates;
static std::vector<Point3> move_vectors;
static std::vector<float> move_radii;
static std::vector<Point3> move_positions;
static Point3 perturb_centre;
static float perturb_a, perturb_b;
static int perturb_calls, check_calls, update_calls, rand_calls, sprintf_number, entry48_calls;
static std::string lump_asked, name_asked, sprintf_format;
static float sound_volume;
static void* sound_object;
static float stand_radius;
static std::vector<MoveObject*> entry88_list;
static void* entry80_arg;
static void* entry88_self;
static std::vector<void*> entry48_self, entry48_arg;
static float find_low[3], find_high[3];
static int find_capacity;

extern "C" {
int objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(Point3* position, const Point3* move, float radius, int mode, void* record) {
    log_.push_back("move");
    move_positions.push_back(*position); move_vectors.push_back(*move); move_radii.push_back(radius);
    if (mode != 3) log_.push_back("BAD MODE");
    if (record) std::memset(record, 0x33, 16);
    return move_answer;
}
void objectUpdateInGrid__FP10GameObject(void*) { log_.push_back("update"); ++update_calls; }
void objectFindInBox__FG6Point3T0PP10GameObjecti(const Point3* low, const Point3* high, MoveObject** out, int capacity) {
    log_.push_back("find");
    find_low[0] = low->x; find_low[1] = low->y; find_low[2] = low->z; find_high[0] = high->x; find_high[1] = high->y; find_high[2] = high->z;
    find_capacity = capacity;
    for (size_t i = 0; i < candidates.size(); ++i) out[i] = candidates[i];
    out[candidates.size()] = nullptr;
}
int worldPerturbWater__FP12_worldHeaderG6Point3ff(void*, const Point3* centre, float a, float b) {
    log_.push_back("perturb"); ++perturb_calls; perturb_centre = *centre; perturb_a = a; perturb_b = b; return perturb_answer;
}
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void*, const Point3*, float depth, float* out) {
    log_.push_back("check"); ++check_calls; if (depth != 12.0f || out != nullptr) log_.push_back("BAD CHECK ARGS"); return check_answer;
}
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void*, const Point3*, float radius) { log_.push_back("stand"); stand_radius = radius; return ground_answer; }
int eRand__10GameObject(void*) { log_.push_back("rand"); ++rand_calls; return 1; }
int sprintf(char* destination, const char* format, ...) {
    sprintf_format = format; sprintf_number = 1;
    std::strcpy(destination, "foly_wtrsplash_sml1.anm"); return 0;
}
void* lumpFindResource__FPcT0(char* lump, char* name) { lump_asked = lump; name_asked = name; log_.push_back("lump"); return reinterpret_cast<void*>(0x5151); }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume) {
    log_.push_back("sound"); sound_object = object; sound_volume = volume; if (header != reinterpret_cast<void*>(0x5151)) log_.push_back("BAD HEADER");
}
}
static void entry48(void* self, void* arg) { log_.push_back("touch"); ++entry48_calls; entry48_self.push_back(self); entry48_arg.push_back(arg); }
static void entry80(void*, void* arg) { log_.push_back("hit"); entry80_arg = arg; }
static void entry88(void* self, void* arg) {
    log_.push_back("touched"); entry88_self = self;
    entry88_list.clear();
    for (MoveObject** p = static_cast<MoveObject**>(arg); *p; ++p) entry88_list.push_back(*p);
}

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static bool has(const char* name) { for (auto& s : log_) if (s == name) return true; return false; }
static int count(const char* name) { int n = 0; for (auto& s : log_) if (s == name) ++n; return n; }

alignas(16) static unsigned char creature_table[256];
alignas(16) static unsigned char other_tables[8][256];
static MoveObject creature;
static MoveObject others[8];
static void reset() {
    std::memset(&creature, 0, sizeof creature);
    std::memset(others, 0, sizeof others);
    std::memset(creature_table, 0, sizeof creature_table);
    log_.clear(); candidates.clear(); move_vectors.clear(); move_radii.clear(); move_positions.clear(); entry48_self.clear(); entry48_arg.clear();
    move_answer = 0; perturb_answer = 1; check_answer = 0; ground_answer = -1000.0f;
    perturb_calls = check_calls = update_calls = rand_calls = entry48_calls = 0;
    lump_asked.clear(); name_asked.clear(); entry80_arg = nullptr; entry88_self = nullptr; entry88_list.clear();
    frameNm = 1;
    creature.vtable = reinterpret_cast<const VirtualEntry*>(creature_table);
    VirtualEntry* e80 = reinterpret_cast<VirtualEntry*>(creature_table + 80);
    VirtualEntry* e88 = reinterpret_cast<VirtualEntry*>(creature_table + 88);
    e80->delta = 0; e80->function = 80;
    e88->delta = 0; e88->function = 88;
    creature.radius = 2.0f; creature.height = 10.0f; creature.position = {1.0f, 2.0f, 3.0f};
    for (int i = 0; i < 8; ++i) {
        std::memset(other_tables[i], 0, 256);
        others[i].vtable = reinterpret_cast<const VirtualEntry*>(other_tables[i]);
        VirtualEntry* e = reinterpret_cast<VirtualEntry*>(other_tables[i] + 48);
        e->delta = static_cast<short>(4 * i); e->function = 48;
        others[i].radius = 2.0f; others[i].height = 4.0f; others[i].position = {1.0f, 2.0f, 3.0f};
    }
}
static unsigned run(Point3 d) { return move2__8CreatureG6Point3(&creature, &d); }

int main() {
    // 1. bit 3 of byte 570: nothing happens.
    reset();
    creature.flags570 = 8; creature.pending_x = 5.0f; creature.pending_angle = 7;
    expect(run({1, 1, 1}) == 0 && log_.empty() && creature.pending_x == 5.0f && creature.pending_angle == 7 && creature.angle == 0, "byte 570 bit 3 returns 0 untouched");

    // 2. pending displacement is added (times 60), angle gains, pending cleared, last_xy stored.
    reset();
    creature.pending_x = 0.5f; creature.pending_y = -0.25f; creature.pending_angle = 100; creature.angle = 65500;
    move_answer = 7;
    unsigned r = run({0.0f, 0.0f, 6.0f});
    expect(creature.last_x == 30.0f && creature.last_y == -15.0f, "last_x/last_y are the displacement plus pending*60");
    expect(creature.pending_x == 0.0f && creature.pending_y == 0.0f && creature.pending_angle == 0, "pending fields cleared");
    expect(creature.angle == static_cast<unsigned short>(65500 + 100), "angle wraps as a halfword");
    expect(move_vectors.size() >= 1 && std::fabs(move_vectors[0].x - 30.0f * 0x1.111110p-6f) < 1e-6f && std::fabs(move_vectors[0].y + 15.0f * 0x1.111110p-6f) < 1e-6f, "world move gets the displacement scaled by 1/60");
    expect(std::fabs(move_vectors[0].z - 6.0f * 0x1.111110p-6f) < 1e-6f, "z is scaled too");
    expect((r & 1) != 0 && has("hit") && entry80_arg != nullptr, "a nonzero world move sets bit 1 and calls the +80 entry");
    expect(update_calls >= 1 && log_[0] == "move" && log_[1] == "update", "move then grid update, in that order");
    expect(move_radii[0] == 2.0f, "world move uses the radius");

    // 3. frozen creature: displacement zeroed, no movement because the squared length is not above 0.001.
    reset();
    creature.frozen = 3; creature.pending_x = 1.0f;
    r = run({5, 5, 5});
    expect(creature.last_x == 0.0f && creature.last_y == 0.0f && !has("move") && (r & 1) == 0, "frozen: displacement zero, no world move");
    expect(creature.pending_x == 0.0f, "frozen still clears pending");

    // 4. A tiny displacement below the 0.001 threshold: no move.
    reset();
    run({0.03f, 0.0f, 0.0f});   // 0.03/60 squared = 2.5e-7
    expect(!has("move"), "tiny displacement skips the world move");

    // 5. Water: flag 0x100000 clears the byte; no water calls.
    reset();
    creature.in_water = 1; creature.flags64 = 0x100000; frameNm = 0;
    run({0, 0, 0});
    expect(creature.in_water == 0 && perturb_calls == 0 && check_calls == 0, "flag 0x100000 clears the water byte without calls");

    // 6. In water, shallow (+176 < 0.01): perturbs only on frames with low four bits zero.
    reset();
    creature.in_water = 1; creature.water_depth = 0.005f; frameNm = 3;
    run({0, 0, 0});
    expect(perturb_calls == 0 && creature.in_water == 1, "shallow, odd frame: no perturbation, stays in water");
    reset();
    creature.in_water = 1; creature.water_depth = 0.005f; frameNm = 32; perturb_answer = 0;
    run({0, 0, 0});
    expect(perturb_calls == 1 && perturb_a == 300.0f && perturb_b == 12.0f && perturb_centre.x == 1.0f && perturb_centre.z == 3.0f, "shallow, frame 32: perturbs at the position with 300 and 12");
    expect(creature.in_water == 0, "a zero perturbation answer leaves the water");
    reset();
    creature.in_water = 1; creature.water_depth = 0.005f; frameNm = 8;   // 8 & 7 == 0 but 8 & 15 != 0
    run({0, 0, 0});
    expect(perturb_calls == 0, "shallow, frame 8: the test is on the low four bits");
    // deep: every call.
    reset();
    creature.in_water = 1; creature.water_depth = 0.5f; frameNm = 3; perturb_answer = 1;
    run({0, 0, 0});
    expect(perturb_calls == 1 && creature.in_water == 1, "deep: perturbs every call and stays while the answer is nonzero");
    // exactly 0.01 counts as deep
    reset();
    creature.in_water = 1; creature.water_depth = 0x1.47ae14p-7f; frameNm = 3;
    run({0, 0, 0});
    expect(perturb_calls == 1, "a depth equal to the threshold is not shallow");

    // 7. Not in water: check only on matching phase.
    reset();
    creature.in_water = 0; creature.splash_phase = 3; frameNm = 4;   // (4 ^ 3) & 7 = 7
    run({0, 0, 0});
    expect(check_calls == 0, "phase mismatch: no water check");
    reset();
    creature.in_water = 0; creature.splash_phase = 5; frameNm = 13; check_answer = 0;  // (13 ^ 5) & 7 = 0
    run({0, 0, 0});
    expect(check_calls == 1 && rand_calls == 0 && creature.in_water == 0, "phase match, no water found");
    reset();
    creature.in_water = 0; creature.splash_phase = 5; frameNm = 13; check_answer = 1;
    run({0, 0, 0});
    expect(creature.in_water == 1 && rand_calls == 1 && lump_asked == "fx.lmp" && name_asked == "foly_wtrsplash_sml1.anm", "water found: state set, rand drawn, resource asked");
    expect(sprintf_format == "foly_wtrsplash_sml%d.anm" && sound_volume == 0x1.333332p-1f && sound_object == &creature, "splash sound at volume 0.6 on the creature");
    expect(perturb_calls == 1 && perturb_a == 1500.0f && perturb_b == 24.0f, "splash perturbs with 1500 and 24");
    {
        size_t c = 0, l = 0, s = 0, p = 0;
        for (size_t i = 0; i < log_.size(); ++i) { if (log_[i] == "check") c = i; if (log_[i] == "rand") l = i; if (log_[i] == "lump") s = i; if (log_[i] == "perturb") p = i; }
        expect(c < l && l < s && s < p, "check, rand, lump, perturb in order");
    }

    // 8. Box query and the object loop.
    reset();
    creature.position = {10.0f, 20.0f, 30.0f}; creature.radius = 3.0f; creature.height = 12.0f; creature.vertical_speed = 0.0f;
    ground_answer = -5000.0f;
    candidates = {&others[0], &others[1], &others[2], &others[3], &creature, &others[4]};
    others[0].flags = 1;                                  // skipped by flag bit 0
    others[0].position = {10.0f, 20.0f, 30.0f};
    others[1].position = {10.0f, 20.0f, 30.0f};           // overlaps: counted, pushed? distance 0 -> not pushed
    others[2].position = {100.0f, 20.0f, 30.0f};          // out of reach
    others[3].position = {11.0f, 20.0f, 30.0f};           // overlaps, distance 1: pushed
    others[4].position = {10.0f, 22.0f, 30.0f};           // overlaps, distance 2: pushed
    r = run({0, 0, 0});
    expect(find_low[0] == 7.0f && find_low[1] == 17.0f && find_low[2] == -970.0f && find_high[0] == 13.0f && find_high[1] == 23.0f && find_high[2] == 1030.0f && find_capacity == 16, "box is position -/+ radius, -/+ 1000 in z, capacity 16");
    expect(entry48_calls == 3 && entry48_self.size() == 3, "three objects are counted");
    expect(entry48_arg[0] == &creature && entry48_arg[1] == &creature, "the creature is the argument of the object callback");
    expect(entry48_self[0] == reinterpret_cast<char*>(&others[1]) + 4 && entry48_self[1] == reinterpret_cast<char*>(&others[3]) + 12 && entry48_self[2] == reinterpret_cast<char*>(&others[4]) + 16, "callbacks receive the object adjusted by the entry delta, in list order");
    expect((r & 2) != 0 && entry88_list.size() == 3 && entry88_list[0] == &others[1] && entry88_list[1] == &others[3] && entry88_list[2] == &others[4], "+88 entry sees the touched list in order, null-terminated");
    expect(count("move") == 2, "only the two objects with nonzero distance are pushed");
    {
        // The push for others[3]: radius sum 3 + 2 = 5, distance 1: factor 4; offset away from the object: (10 - 11, 20 - 20) * 4.
        expect(std::fabs(move_vectors[0].x + 4.0f) < 1e-5f && move_vectors[0].y == 0.0f && move_vectors[0].z == 0.0f, "push vector for the first overlapping object");
        // others[4]: distance 2, factor (5 - 2)/2 = 1.5; offset (10 - 10, 20 - 22) * 1.5 = (0, -3).
        expect(std::fabs(move_vectors[1].y + 3.0f) < 1e-5f && move_vectors[1].x == 0.0f, "push vector for the second overlapping object");
    }

    // 9. Pushing is skipped for flagged objects, but they are still counted.
    reset();
    creature.position = {0, 0, 0}; creature.radius = 3.0f;
    candidates = {&others[0], &others[1], &others[2]};
    others[0].position = {1, 0, 0}; others[0].flags = 0x200;
    others[1].position = {1, 0, 0}; creature.flags = 0x400;
    others[2].position = {0.0f, 0.0f, 0.0f};
    r = run({0, 0, 0});
    expect(entry48_calls == 3 && !has("move"), "flag 0x200 on the object or 0x400 on the creature skips the push");
    reset();
    creature.position = {0, 0, 0}; creature.radius = 3.0f; creature.flags = 1;
    candidates = {&others[0]};
    others[0].position = {1, 0, 0};
    run({0, 0, 0});
    expect(entry48_calls == 1 && !has("move"), "creature flag 0x1 skips the push");

    // 10. Flag 0x10 objects (platforms): skipped or remembered as ground when the creature is above.
    reset();
    creature.position = {0, 0, 10}; creature.height = 8.0f; creature.radius = 3.0f; creature.flags = 0x10;
    candidates = {&others[0], &others[1], &others[2], &others[3]};
    others[0].position = {0, 1, 30};                       // object bottom + 3 above the creature top (18): skipped
    others[1].position = {0, 1, 4}; others[1].height = 3;  // top 7 below creature z 10: skipped
    others[2].position = {0, 1, 5}; others[2].height = 8;  // top 13, 3 above z: standing surface, remembered
    others[3].position = {0, 1, 5}; others[3].height = 6;  // top 11, lower than the remembered 13: not counted
    ground_answer = 0.0f;
    r = run({0, 0, 0});
    expect(entry48_calls == 0 && (r & 2) == 0, "platform surfaces within 5 are not counted");
    expect(creature.position.z == 13.0f, "the highest remembered top is used when above the world ground");
    // The 3.0 margin: an object whose bottom plus 3 equals the creature top is not skipped (strict compare).
    reset();
    creature.position = {0, 0, 10}; creature.height = 8.0f; creature.radius = 3.0f; creature.flags = 0x10;   // top 18
    candidates = {&others[0]};
    others[0].position = {0, 1, 15}; others[0].height = 8;   // 15 + 3 == 18
    ground_answer = -100.0f;
    r = run({0, 0, 0});
    expect(entry48_calls == 1 && count("move") == 1, "bottom + 3 equal to the creature top is kept and pushed");
    reset();
    creature.position = {0, 0, 10}; creature.height = 8.0f; creature.radius = 3.0f; creature.flags = 0x10;
    candidates = {&others[0]};
    others[0].position = {0, 1, 15.5f}; others[0].height = 8;   // 15.5 + 3 > 18
    ground_answer = -100.0f;
    run({0, 0, 0});
    expect(entry48_calls == 0 && !has("move"), "bottom + 3 above the creature top is skipped");
    // The same platform with 0x200 is counted instead.
    reset();
    creature.position = {0, 0, 10}; creature.height = 8.0f; creature.radius = 3.0f; creature.flags = 0x10;
    candidates = {&others[0]};
    others[0].position = {0, 1, 5}; others[0].height = 8; others[0].flags = 0x200;
    ground_answer = -100.0f;
    r = run({0, 0, 0});
    expect(entry48_calls == 1 && (r & 2) != 0 && !has("move"), "a platform with flag 0x200 is counted and never pushed");
    // Depth of 5 or more (top at least 5 above z) falls through to the push test and is counted.
    reset();
    creature.position = {0, 0, 10}; creature.height = 8.0f; creature.radius = 3.0f; creature.flags = 0x10;
    candidates = {&others[0]};
    others[0].position = {0, 1, 5}; others[0].height = 10; ground_answer = -100.0f;   // top 15, depth exactly 5
    run({0, 0, 0});
    expect(entry48_calls == 1 && count("move") == 1, "depth of exactly 5 is counted and pushed");

    // 11. Ground: stand height, gravity and the landing bit.
    reset();
    creature.position = {0, 0, 50}; creature.vertical_speed = 3.0f; ground_answer = 49.0f;
    r = run({0, 0, 0});
    float speed = 3.0f - 0x1.1ffffep+3f;
    float z = 50.0f + speed * 0x1.111110p-6f;
    expect(creature.vertical_speed == speed && std::fabs(creature.position.z - z) < 1e-5f && (r & 4) == 0, "airborne: speed loses 9, z gains speed/60, no landing bit");
    expect(stand_radius == 2.0f, "stand height is asked with the radius");
    reset();
    creature.position = {0, 0, 50}; creature.vertical_speed = -1.0f; ground_answer = 50.0f;
    r = run({0, 0, 0});
    expect(creature.position.z == 50.0f && creature.vertical_speed == 0.0f && (r & 4) != 0, "landing: z becomes the ground, speed zeroed, bit 4 set");
    reset();
    creature.position = {0, 0, 50}; creature.vertical_speed = 20.0f; ground_answer = 60.0f;
    r = run({0, 0, 0});
    expect(creature.position.z == 60.0f && creature.vertical_speed != 0.0f && (r & 4) == 0, "below the ground but moving up: clamped without the landing bit");
    reset();
    creature.position = {0, 0, 50}; creature.vertical_speed = 0.0f; ground_answer = 50.0f - 8.9999990f * 0x1.111110p-6f + 0.001f;
    r = run({0, 0, 0});
    // z after = 50 - 9*(1/60) = 49.85; minus 0.01 = 49.84; ground just above that -> clamped.
    expect(creature.position.z == ground_answer, "z minus 0.01 equal to or below the ground is clamped");

    // 12. The order of the virtual calls: hit entry before the touched entry.
    reset();
    move_answer = 1; candidates = {&others[0]};
    others[0].position = {1.0f, 2.0f, 3.0f};
    run({6.0f, 0.0f, 0.0f});
    expect(has("hit") && has("touched") && !log_.empty() && log_[log_.size() - 2] == "hit" && log_.back() == "touched", "+80 entry then +88 entry at the end");
    expect(entry88_self == &creature, "creature entries receive the adjusted creature");
    if (failures) return 1;
    std::printf("move2 contract passed\n");
    return 0;
}
