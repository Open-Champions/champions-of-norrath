#define NORRATH_W8C_HOST 1
#include "run.cpp"
#include <cstdio>
#include <cstring>
unsigned char player[95936];
unsigned char nmPlayers[4];
int frameNm = 0;
unsigned levelFlags = 0;
unsigned char norrath_actors_b_view_4844a0[4];
unsigned char norrath_player_view_31f540[32];
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static int run_adds = 0, medium_adds = 0, find_calls = 0, hit_calls = 0, sparks_calls = 0, missile_calls = 0, sound_calls = 0;
static float boxes[4][2][3], aims[4][3], hit_starts[16][3];
static unsigned headings[4];
static int sparks_count = 0, missile_ints[2];
static float hit_range = 0.0f;
static void* listed[3];
static float hit_answer = -1.0f;
static unsigned char blocks[2][600];
static unsigned char object[300], target[100], second_target[100], players[2][64];
extern "C" float icos__Fi(int) { return 1.0f; }
extern "C" float isin__Fi(int) { return 0.5f; }
extern "C" unsigned norrath_actor_random(void*) { return 7; }
extern "C" unsigned char* norrath_block_alloc(int size) { return blocks[size == 184 ? 0 : 1]; }
extern "C" unsigned norrath_actor_add_run_list(void*) { ++run_adds; return 0; }
extern "C" unsigned norrath_actor_add_medium_run_list(void*) { ++medium_adds; return 0; }
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** out, int capacity) {
    expect(capacity == 256, "capacity 256");
    std::memcpy(boxes[find_calls][0], low, 12); std::memcpy(boxes[find_calls][1], high, 12);
    headings[find_calls] = w8c::read_u16(w8c::address(object) + 56u);
    out[0] = listed[0]; out[1] = listed[1]; out[2] = nullptr;
    ++find_calls;
}
extern "C" float worldHitscan__FP12_worldHeaderG6Point3T1fi(void*, const void* start, const void* direction, float range, int flags) {
    std::memcpy(hit_starts[hit_calls], start, 12);
    if (hit_calls < 4) std::memcpy(aims[hit_calls], direction, 12);
    hit_range = range; expect(flags == 0, "hitscan flags 0");
    ++hit_calls; return hit_answer;
}
extern "C" void __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(void* self, const void*, const void*, void*, void*, void*, int a, int b, int c, int d) {
    ++sparks_calls; expect(self == blocks[0] && b == 12 && c == 0 && d == 80, "sparks args"); sparks_count = a;
}
extern "C" void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int, void*, void*, const void*, const void*, int angle, void*, void*, void*, int a, int b, int c, int d, int e, int f, void*) {
    ++missile_calls; expect(self == blocks[1] && c == 0 && d == 0 && e == 10 && f == 4, "missile ints"); missile_ints[0] = a; missile_ints[1] = b; (void)angle;
}
extern "C" void* norrath_lump_find_resource(char*, char*) { return nullptr; }
extern "C" int norrath_sound_play_sequence(void*, void*, float) { ++sound_calls; return 0; }
extern "C" int sprintf(char* out, const char*, ...) { out[0] = 0; return 0; }

static void setup(unsigned char flag76, int mode, int period, int frame) {
    using namespace w8c;
    std::memset(object, 0, sizeof object); std::memset(target, 0, sizeof target); std::memset(second_target, 0, sizeof second_target);
    std::memset(player, 0, sizeof player); std::memset(players, 0, sizeof players);
    object[76] = flag76;
    write_u16(address(object) + 82u, static_cast<unsigned>(mode));
    write_u16(address(object) + 84u, static_cast<unsigned>(period));
    write_single(address(object) + 16u, 100.0f); write_single(address(object) + 20u, 200.0f); write_single(address(object) + 24u, 300.0f);
    write_word(address(target) + 12u, 0x40u); write_word(address(second_target) + 12u, 0u);
    listed[0] = target; listed[1] = second_target;
    host_note(players[0]); host_note(players[1]);
    write_word(address(player) + 21872u, address_bits(players[0]));
    write_single(address(players[0]) + 16u, 100.0f); write_single(address(players[0]) + 20u, 200.0f); write_single(address(players[0]) + 24u, 300.0f);
    write_word(address(nmPlayers), 1u);
    frameNm = frame; levelFlags = 0; hit_answer = -1.0f;
    run_adds = medium_adds = find_calls = hit_calls = sparks_calls = missile_calls = sound_calls = 0;
}
int main() {
    using namespace w8c;
    // far player: medium list, byte 76 cleared, off-period with empty counters ends
    setup(1, 0, 5, 3);
    write_single(address(players[0]) + 16u, 100.0f + 720.0f);
    msg_run__7Shooter(object);
    expect(medium_adds == 1 && run_adds == 0 && object[76] == 0 && find_calls == 0, "far player, off period, counters empty");
    // exactly 720 away is not near (strict)
    setup(0, 0, 5, 3);
    write_single(address(players[0]) + 16u, 100.0f + 720.0f);
    msg_run__7Shooter(object);
    expect(medium_adds == 0 && run_adds == 0, "no change when the byte already matches");
    // near player joins the run list once
    setup(0, 0, 5, 3);
    msg_run__7Shooter(object);
    expect(run_adds == 1 && object[76] == 1, "near player runs");
    // on-period: four directions, sparks per listed flagged object per direction
    setup(1, 0, 5, 10);
    msg_run__7Shooter(object);
    expect(find_calls == 4 && sparks_calls == 4 && hit_calls == 4, "four boxes, four sparks (one flagged object each)");
    expect(boxes[0][1][0] == 820.0f && boxes[0][0][1] == 104.0f && boxes[0][1][1] == 296.0f, "direction 0 box");
    expect(boxes[0][0][2] == 240.0f && boxes[0][1][2] == 360.0f, "box height");
    expect(boxes[1][0][1] == 200.0f - 720.0f && boxes[1][0][0] == 4.0f && boxes[1][1][0] == 196.0f, "direction 1 box");
    expect(boxes[2][0][0] == 100.0f - 720.0f && boxes[3][1][1] == 920.0f, "direction 2 and 3 boxes");
    expect(headings[0] == 0u && headings[1] == 0xc000u && headings[2] == 0x8000u && headings[3] == 0x4000u, "headings per direction");
    expect(read_u16(address(object) + 56u) == 0x4000u, "heading left at 0x4000");
    expect(aims[0][0] == 1.0f && aims[1][1] == -1.0f && aims[2][0] == -1.0f && aims[3][1] == 1.0f && hit_range == 48.0f, "aims and range");
    expect(hit_starts[0][0] == 136.0f && hit_starts[0][1] == 218.0f && hit_starts[0][2] == 300.0f, "hitscan start 36 ahead");
    expect(sparks_count == 25, "default spark count");
    setup(1, 0, 5, 10); levelFlags = 0x8000u;
    msg_run__7Shooter(object);
    expect(sparks_count == 60, "flagged spark count");
    setup(1, 0, 5, 10); hit_answer = 0.0f;
    msg_run__7Shooter(object);
    expect(sparks_calls == 0 && hit_calls == 4, "only a -1 hitscan fires");
    // off-period with a nonzero counter still shoots; arrows: counter with low bits clear fires and decrements
    setup(1, 1, 5, 3);
    object[77] = 8; object[78] = 3;
    msg_run__7Shooter(object);
    expect(find_calls == 4 && missile_calls == 1 && object[77] == 7 && object[78] == 2 && sound_calls == 1, "arrow: fire on multiples of eight, always decrement");
    expect(missile_ints[0] == 1 && missile_ints[1] == 6, "missile ints");
    setup(1, 1, 5, 3); levelFlags = 0x8000u; object[77] = 16;
    msg_run__7Shooter(object);
    expect(missile_ints[0] == 10 && missile_ints[1] == 34, "flagged missile ints");
    // on-period with mode 1 fills zero counters with 40
    setup(1, 1, 5, 10);
    object[78] = 0xff;
    msg_run__7Shooter(object);
    expect(object[77] == 39 && object[78] == 0xfe && object[79] == 39 && object[80] == 39, "zero counters become 40, then each fired counter drops by one");
    expect(missile_calls == 3, "counters 40, 40 and 40 fire; 0xff does not");
    return failures ? 1 : 0;
}
