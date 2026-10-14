#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_run__11MissileTrap at 0x1d9650, 616 bytes.
// Does something only when the signed halfword at object+78 divides frameNm
// exactly (signed remainder zero; a zero divisor is not handled by the image and
// is not defined here). Then the signed halfword at +76 selects:
//  0: a velocity {240*icos(a), 240*isin(a), 0} with a the signed halfword at +56;
//     a 184-byte block is allocated and, when non-null, Sparks is constructed in it
//     with the position at +16, that velocity, three nulls, and the ints 25, 12, 0
//     and 80.
//  1: a random angle offset r1 % 1000 (unsigned) is added to the +56 halfword
//     (call it b) and 500 is subtracted. The velocity is {400*icos(a'), 400*isin(a'),
//     (r2 % 60) - 30} with each of the first two products formed as (trig * 4) * 100.
//     A name "swsh_arrow%d.anm" is formatted with (r3 % 5) + 1, looked up in the
//     lump "fx.lmp" and played as a sequence at gain 1.0 on this object. A 452-byte
//     block is allocated; when non-null MissileWeapon is constructed in it with
//     (0, the words at +8 and +12 of item::globalItem, the position at +16, the
//     velocity, a' - 16384, null creature, two null sound headers, then ints
//     2, 8, 0, 0, 100, 4 and a null item description). The three random values
//     come from successive eRand calls on this object.
//  other: nothing.
// Unknown: what the +76 modes, the +78 period, the constants and the two global
// item words mean. norrath_block_alloc is blockAlloc (0x1441b8).
extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);
extern "C" unsigned norrath_actor_random(void* object);
extern "C" unsigned char* norrath_block_alloc(int size);
extern "C" void __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(
    void* self, const void* position, const void* velocity, void* creature, void* sound_a, void* sound_b,
    int a, int b, int c, int d);
extern "C" void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int zero, void* model, void* texture, const void* position, const void* velocity, int angle,
    void* creature, void* sound_a, void* sound_b, int a, int b, int c, int d, int e, int f, void* description);
extern "C" void* norrath_lump_find_resource(char* lump_name, char* resource_name);
extern "C" int norrath_sound_play_sequence(void* animation_header, void* object, float gain);
extern "C" int sprintf(char* out, const char* format, ...);
extern "C" int frameNm;
extern "C" unsigned char norrath_player_view_31f540[];

namespace {
// The image divides without the zero-divisor trap the compiler would add to a
// plain `%`, so on the target the divide is spelled out.
__attribute__((always_inline)) inline int signed_remainder(int value, int divisor) {
#if defined(__mips__)
    int remainder;
    __asm__("div $0, %1, %2\n\tmfhi %0" : "=r"(remainder) : "r"(value), "r"(divisor) : "hi", "lo");
    return remainder;
#else
    return value % divisor;
#endif
}
}

extern "C" void msg_run__11MissileTrap(void* self) {
    using namespace w8c;
    const Address base = address(self);
    if (signed_remainder(frameNm, read_i16(base + 78u)) != 0) return;
    const int mode = read_i16(base + 76u);
    if (mode == 0) {
        const int angle = read_i16(base + 56u);
        float velocity[3];
        velocity[0] = icos__Fi(angle) * single_from_bits(0x43700000u);
        velocity[1] = isin__Fi(angle) * single_from_bits(0x43700000u);
        velocity[2] = 0.0f;
        void* const block = norrath_block_alloc(184);
        if (block == nullptr) return;
        const float position[3] = {read_single(base + 16u), read_single(base + 20u), read_single(base + 24u)};
        __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(block, position, velocity, nullptr, nullptr, nullptr,
                                                           25, 12, 0, 80);
        return;
    }
    if (mode != 1) return;
    const int facing = read_i16(base + 56u);
    const int spread = static_cast<int>(norrath_actor_random(self) % 1000u);
    const int angle = facing + spread - 500;
    float velocity[3];
    const float forward = icos__Fi(angle) * single_from_bits(0x40800000u);
    const float side = isin__Fi(angle) * single_from_bits(0x40800000u);
    const unsigned lift = norrath_actor_random(self) % 60u;
    velocity[0] = forward * single_from_bits(0x42c80000u);
    velocity[1] = side * single_from_bits(0x42c80000u);
    velocity[2] = static_cast<float>(static_cast<int>(lift) - 30);
    const unsigned pick = norrath_actor_random(self) % 5u;
    char name[64];
    sprintf(name, "swsh_arrow%d.anm", static_cast<int>(pick) + 1);
    static char lump[] = {'f', 'x', '.', 'l', 'm', 'p', '\0'};
    norrath_sound_play_sequence(norrath_lump_find_resource(lump, name), self, 1.0f);
    void* const block = norrath_block_alloc(452);
    if (block == nullptr) return;
    const float position[3] = {read_single(base + 16u), read_single(base + 20u), read_single(base + 24u)};
    const Address item = address(norrath_player_view_31f540);
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        block, 0, pointer(read_word(item + 8u)), pointer(read_word(item + 12u)), position, velocity,
        angle - 16384, nullptr, nullptr, nullptr, 2, 8, 0, 0, 100, 4, nullptr);
}
