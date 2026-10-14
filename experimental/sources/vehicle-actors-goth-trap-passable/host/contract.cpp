// Host contract for norrath_goth_trap_set_passable_state. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char norrath_actors_b_view_4844a0[4];
}
extern "C" unsigned norrath_goth_trap_set_passable_state(void* object, int enabled);

static int failures = 0;
static unsigned char object[408];
static int step;
enum Kind { Collision, Move, Height };
static Kind kinds[8];
static void* worlds[8];
static int indexes[8];
static int flags[8];
static float points[8][3];
static short extras[8];
static unsigned height_bits[8];
static int promote_zero_to_two = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned float_bits(float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static float bits_to_float(unsigned bits) {
    float value;
    unsigned char* p = reinterpret_cast<unsigned char*>(&value);
    p[0] = static_cast<unsigned char>(bits);
    p[1] = static_cast<unsigned char>(bits >> 8);
    p[2] = static_cast<unsigned char>(bits >> 16);
    p[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}
extern "C" int worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int flag) {
    kinds[step] = Collision;
    worlds[step] = world;
    indexes[step] = index;
    flags[step] = flag;
    write_word(norrath_actors_b_view_4844a0, 0x20u);
    write_word(object + 396, 9u);
    const int result = 0x11;
    ++step;
    return result;
}
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, const void* point, short extra) {
    kinds[step] = Move;
    worlds[step] = world;
    indexes[step] = index;
    extras[step] = extra;
    std::memcpy(points[step], point, sizeof points[step]);
    ++step;
    return 0x22;
}
extern "C" int worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height) {
    kinds[step] = Height;
    worlds[step] = world;
    indexes[step] = index;
    height_bits[step] = float_bits(height);
    if (promote_zero_to_two) write_word(object + 376, 2u);
    ++step;
    return 0x33;
}

static void prepare(unsigned state, unsigned height, unsigned world_bits) {
    std::memset(object, 0x5a, sizeof object);
    write_word(object + 376, state);
    write_word(object + 396, 5u);
    write_word(object + 400, height);
    write_word(norrath_actors_b_view_4844a0, world_bits);
    step = 0;
    promote_zero_to_two = 0;
}

int main() {
    const unsigned height = float_bits(1000.0f);
    prepare(3u, height, 0x10u);
    unsigned result = norrath_goth_trap_set_passable_state(object, 0);
    expect(result == 0x11u && step == 1, "other state returns collision");
    expect(kinds[0] == Collision && flags[0] == 1 && indexes[0] == 5, "collision ignores the bool");
    expect(worlds[0] == reinterpret_cast<void*>(0x10ul), "collision reads world");

    prepare(0u, height, 0x10u);
    result = norrath_goth_trap_set_passable_state(object, 1);
    expect(result == 0x33u && step == 3, "state zero calls move then height");
    expect(kinds[1] == Move && extras[1] == 0 && indexes[1] == 9, "move sees the updated index");
    expect(worlds[1] == reinterpret_cast<void*>(0x20ul), "move reloads world");
    expect(points[1][0] == 0.0f && points[1][1] == 0.0f && points[1][2] == 0.0f, "zero point");
    expect(kinds[2] == Height && height_bits[2] == height && indexes[2] == 9, "state zero keeps height");

    prepare(1u, height, 0x10u);
    result = norrath_goth_trap_set_passable_state(object, 0);
    expect(result == 0x33u && step == 3 && kinds[1] == Move && kinds[2] == Height, "state one");
    expect(points[1][0] == 0.0f && points[1][1] == 0.0f && points[1][2] == bits_to_float(0xc3af0000u), "state one point");
    expect(height_bits[2] == float_bits(1000.0f - bits_to_float(0x43af0000u)), "state one height");

    prepare(2u, height, 0x10u);
    result = norrath_goth_trap_set_passable_state(object, 7);
    expect(result == 0x33u && step == 3 && kinds[1] == Move, "state two still moves");
    expect(points[1][2] == bits_to_float(0xc3af0000u), "state two point");
    expect(height_bits[2] == float_bits(1000.0f + bits_to_float(0x43800000u)), "state two height");

    prepare(0u, height, 0x10u);
    promote_zero_to_two = 1;
    result = norrath_goth_trap_set_passable_state(object, 1);
    expect(result == 0x33u && step == 5, "reloaded state two continues");
    expect(kinds[1] == Move && kinds[2] == Height && kinds[3] == Move && kinds[4] == Height, "zero then two");
    expect(points[3][2] == bits_to_float(0xc3af0000u), "second move uses -350");
    expect(height_bits[4] == float_bits(1000.0f + bits_to_float(0x43800000u)), "second height adds 256");

    if (failures != 0) return 1;
    std::printf("goth trap passable state passed\n");
    return 0;
}
