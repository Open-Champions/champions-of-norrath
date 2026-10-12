#define NORRATH_W8C_HOST 1
#include "run.cpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
int frameNm = 0;
unsigned char norrath_player_view_31f540[32];
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static int cos_arg = 0, sin_arg = 0, rand_calls = 0, alloc_sizes[4], allocs = 0, alloc_null_at = -1;
static unsigned rand_values[3];
static int sparks_calls = 0, missile_calls = 0, sound_calls = 0;
static float sparks_pos[3], sparks_vel[3], missile_pos[3], missile_vel[3];
static int sparks_ints[4], missile_angle = 0, missile_ints[7];
static void* missile_model = nullptr; static void* missile_texture = nullptr;
static char lump_seen[16], resource_seen[32];
static unsigned char block_a[512], block_b[512];
extern "C" float icos__Fi(int angle) { cos_arg = angle; return 0.5f; }
extern "C" float isin__Fi(int angle) { sin_arg = angle; return -0.25f; }
extern "C" unsigned norrath_actor_random(void*) { return rand_values[rand_calls++]; }
extern "C" unsigned char* norrath_block_alloc(int size) {
    alloc_sizes[allocs] = size;
    unsigned char* r = (allocs == alloc_null_at) ? nullptr : (size == 184 ? block_a : block_b);
    ++allocs;
    return r;
}
extern "C" void __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(void* self, const void* pos, const void* vel, void* c, void* a, void* b,
                                                               int i0, int i1, int i2, int i3) {
    ++sparks_calls; expect(self == block_a && !c && !a && !b, "sparks block and nulls");
    std::memcpy(sparks_pos, pos, 12); std::memcpy(sparks_vel, vel, 12);
    sparks_ints[0] = i0; sparks_ints[1] = i1; sparks_ints[2] = i2; sparks_ints[3] = i3;
}
extern "C" void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int zero, void* model, void* texture, const void* pos, const void* vel, int angle, void* creature,
    void* sa, void* sb, int a, int b, int c, int d, int e, int f, void* desc) {
    ++missile_calls; expect(self == block_b && zero == 0 && !creature && !sa && !sb && !desc, "missile block and nulls");
    missile_model = model; missile_texture = texture;
    std::memcpy(missile_pos, pos, 12); std::memcpy(missile_vel, vel, 12);
    missile_angle = angle;
    missile_ints[0] = a; missile_ints[1] = b; missile_ints[2] = c; missile_ints[3] = d; missile_ints[4] = e; missile_ints[5] = f;
}
extern "C" void* norrath_lump_find_resource(char* lump, char* resource) {
    std::strcpy(lump_seen, lump); std::strcpy(resource_seen, resource); return lump_seen;
}
extern "C" int norrath_sound_play_sequence(void* header, void*, float gain) { ++sound_calls; expect(header == lump_seen && gain == 1.0f, "sound args"); return 0; }
extern "C" int sprintf(char* out, const char* format, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, format);
    const int n = __builtin_va_arg(args, int);
    __builtin_va_end(args);
    expect(std::strcmp(format, "swsh_arrow%d.anm") == 0, "arrow format");
    std::strcpy(out, "swsh_arrow");
    out[10] = static_cast<char>('0' + n); out[11] = 0;
    std::strcat(out, ".anm");
    return 0;
}
static void run(int frame, int period, int mode, int angle) {
    using namespace w8c;
    unsigned char object[300];
    std::memset(object, 0, sizeof object);
    write_u16(address(object) + 78u, static_cast<unsigned>(period));
    write_u16(address(object) + 76u, static_cast<unsigned>(mode));
    write_u16(address(object) + 56u, static_cast<unsigned>(angle));
    write_single(address(object) + 16u, 7.0f); write_single(address(object) + 20u, 8.0f); write_single(address(object) + 24u, 9.0f);
    frameNm = frame;
    sparks_calls = missile_calls = sound_calls = allocs = rand_calls = 0;
    msg_run__11MissileTrap(object);
}
int main() {
    using namespace w8c;
    write_word(address(norrath_player_view_31f540) + 8u, 0x1000u); write_word(address(norrath_player_view_31f540) + 12u, 0x2000u);
    host_note(reinterpret_cast<void*>(0x1000u)); host_note(reinterpret_cast<void*>(0x2000u));
    run(7, 3, 0, 100);
    expect(sparks_calls == 0 && missile_calls == 0 && allocs == 0, "off-period frame does nothing");
    run(-9, -3, 0, 100);
    expect(sparks_calls == 1 && allocs == 1 && alloc_sizes[0] == 184, "negative period divides evenly");
    expect(sparks_vel[0] == 120.0f && sparks_vel[1] == -60.0f && sparks_vel[2] == 0.0f && cos_arg == 100 && sin_arg == 100, "sparks velocity");
    expect(sparks_pos[0] == 7.0f && sparks_pos[2] == 9.0f && sparks_ints[0] == 25 && sparks_ints[1] == 12 && sparks_ints[2] == 0 && sparks_ints[3] == 80, "sparks args");
    alloc_null_at = 0;
    run(6, 3, 0, 0);
    expect(sparks_calls == 0 && allocs == 1, "null block ends");
    alloc_null_at = -1;
    run(6, 3, 2, 0);
    expect(sparks_calls == 0 && missile_calls == 0 && allocs == 0, "other mode does nothing");
    rand_values[0] = 1234; rand_values[1] = 75; rand_values[2] = 7;
    run(6, 3, 1, 0x1000);
    expect(missile_calls == 1 && sound_calls == 1 && allocs == 1 && alloc_sizes[0] == 452, "missile path");
    expect(cos_arg == 0x1000 + 234 - 500 && sin_arg == cos_arg && missile_angle == cos_arg - 16384, "angle offsets");
    expect(missile_vel[0] == 0.5f * 4.0f * 100.0f && missile_vel[1] == -0.25f * 4.0f * 100.0f && missile_vel[2] == -15.0f, "missile velocity");
    expect(std::strcmp(lump_seen, "fx.lmp") == 0 && std::strcmp(resource_seen, "swsh_arrow3.anm") == 0, "sound name from r3 % 5 + 1");
    expect(missile_model == reinterpret_cast<void*>(0x1000u) && missile_texture == reinterpret_cast<void*>(0x2000u), "global item words");
    expect(missile_ints[0] == 2 && missile_ints[1] == 8 && missile_ints[2] == 0 && missile_ints[3] == 0 && missile_ints[4] == 100 && missile_ints[5] == 4, "missile ints");
    alloc_null_at = 0;
    run(6, 3, 1, 0);
    expect(missile_calls == 0 && sound_calls == 1, "null block still plays the sound");
    return failures ? 1 : 0;
}
