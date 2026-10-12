#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_run__7Shooter at 0x1da5d0, 1808 bytes.
// near = 1 when any player object (pointer at +21872 of each 23984-byte record of
// `player`, null skipped, nmPlayers records) lies strictly inside squared distance
// 518400.0 (720 squared) of the position at +16, else 0. The signed byte at +76
// holds the previous value; when it differs the object joins the run list (near)
// or the medium run list (not near). The byte becomes near.
// When the signed halfword at +82 is 1 and the signed remainder of frameNm by the
// signed halfword at +84 is zero, each of the four signed bytes at +77..+80 that is
// zero becomes 40.
// The shooting pass runs when that remainder is zero, or otherwise when the four
// bytes at +77..+80 do not sum to zero. For direction d = 0..3 a box is built
// around the position P (z from P.z-60 to P.z+60) and the heading halfword at +56
// is set: d0 x to P.x+720, y within 96, heading 0, direction (1,0,0); d1 y down
// to P.y-720, x within 96, heading 0xc000, direction (0,-1,0); d2 x down to
// P.x-720, y within 96, heading 0x8000, direction (-1,0,0); d3 y up to P.y+720,
// x within 96, heading 0x4000, direction (0,1,0). objectFindInBox lists up to 256
// objects in the box; for each whose +12 flags have bit 0x40, a point 36 units ahead
// of P along the heading is tested with worldHitscan(world, point, direction, 48.0,
// 0); only a result of exactly -1.0 (the line is clear) acts:
//   halfword +82 == 0: Sparks is built (velocity 240*(icos, isin, 0) of the heading,
//     position 34 ahead of P, count 25 or 60 when bit 0x8000 of levelFlags is set,
//     12, 0, 80; the allocation is 184 bytes).
//   halfword +82 == 1: when the byte at +77+d is nonzero and its low three bits are
//     zero a missile is launched exactly like MissileTrap mode 1 (random spread
//     r%1000 - 500, speed 4*100, lift r%60-30, "swsh_arrow%d.anm" from fx.lmp,
//     position 22 ahead, 452-byte block, ints 1/6 or 10/34 when bit 0x8000 of
//     levelFlags is set, 0, 0, 10, 4). A nonzero +77+d byte is then decremented.
// The heading halfword is left at 0x4000 afterwards.
// Unknown: what the ranges, speeds, modes and the byte counters mean.
// Inferred: callee names as in the other sources of this package; the zero-check
// free division is spelled as in MissileTrap.
extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);
extern "C" unsigned norrath_actor_random(void* object);
extern "C" unsigned char* norrath_block_alloc(int size);
extern "C" unsigned norrath_actor_add_run_list(void* object);
extern "C" unsigned norrath_actor_add_medium_run_list(void* object);
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** out, int capacity);
extern "C" float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* world, const void* start, const void* direction,
                                                           float range, int flags);
extern "C" void __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(
    void* self, const void* position, const void* velocity, void* creature, void* sound_a, void* sound_b,
    int a, int b, int c, int d);
extern "C" void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int zero, void* model, void* texture, const void* position, const void* velocity, int angle,
    void* creature, void* sound_a, void* sound_b, int a, int b, int c, int d, int e, int f, void* description);
extern "C" void* norrath_lump_find_resource(char* lump_name, char* resource_name);
extern "C" int norrath_sound_play_sequence(void* animation_header, void* object, float gain);
extern "C" int sprintf(char* out, const char* format, ...);
extern "C" unsigned char player[];
extern "C" unsigned char nmPlayers[4];
extern "C" int frameNm;
extern "C" unsigned levelFlags;
extern "C" unsigned char norrath_actors_b_view_4844a0[];
extern "C" unsigned char norrath_player_view_31f540[];

namespace {
__attribute__((always_inline)) inline int signed_remainder(int value, int divisor) {
#if defined(__mips__)
    int remainder;
    __asm__("div $0, %1, %2\n\tmfhi %0" : "=r"(remainder) : "r"(value), "r"(divisor) : "hi", "lo");
    return remainder;
#else
    return value % divisor;
#endif
}

using w8c::single_from_bits;

void fire_arrow(void* self, w8c::Address base) {
    using namespace w8c;
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
    float position[3];
    position[0] = read_single(base + 16u) + icos__Fi(read_i16(base + 56u)) * single_from_bits(0x41b00000u);
    position[1] = read_single(base + 20u) + isin__Fi(read_i16(base + 56u)) * single_from_bits(0x41b00000u);
    position[2] = read_single(base + 24u);
    const unsigned pick = norrath_actor_random(self) % 5u;
    char name[64];
    sprintf(name, "swsh_arrow%d.anm", static_cast<int>(pick) + 1);
    static char lump[] = {'f', 'x', '.', 'l', 'm', 'p', '\0'};
    norrath_sound_play_sequence(norrath_lump_find_resource(lump, name), self, 1.0f);
    int first = 1;
    int second = 6;
    if ((levelFlags & 0x8000u) != 0u) {
        first = 10;
        second = 34;
    }
    void* const block = norrath_block_alloc(452);
    if (block == nullptr) return;
    const Address item = address(norrath_player_view_31f540);
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        block, 0, pointer(read_word(item + 8u)), pointer(read_word(item + 12u)), position, velocity,
        angle - 16384, nullptr, nullptr, nullptr, first, second, 0, 0, 10, 4, nullptr);
}

void fire_sparks(void* self, w8c::Address base) {
    using namespace w8c;
    float velocity[3];
    velocity[0] = icos__Fi(read_i16(base + 56u)) * single_from_bits(0x43700000u);
    velocity[1] = isin__Fi(read_i16(base + 56u)) * single_from_bits(0x43700000u);
    velocity[2] = 0.0f;
    float position[3];
    position[0] = read_single(base + 16u) + icos__Fi(read_i16(base + 56u)) * single_from_bits(0x42080000u);
    position[1] = read_single(base + 20u) + isin__Fi(read_i16(base + 56u)) * single_from_bits(0x42080000u);
    position[2] = read_single(base + 24u);
    int count = 25;
    if ((levelFlags & 0x8000u) != 0u) count = 60;
    void* const block = norrath_block_alloc(184);
    if (block == nullptr) return;
    __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(block, position, velocity, nullptr, nullptr, nullptr, count,
                                                       12, 0, 80);
    (void)self;
}
}

extern "C" void msg_run__7Shooter(void* self) {
    using namespace w8c;
    const Address base = address(self);
    int near_player = 0;
    const int players = signed_word(read_word(address(nmPlayers)));
    if (players > 0) {
        for (int i = 0; i < players; ++i) {
            void* const other = pointer(read_word(address(player) + 21872u + 23984u * static_cast<unsigned>(i)));
            if (other == nullptr) continue;
            const Address other_base = address(other);
            const float dx = read_single(base + 16u) - read_single(other_base + 16u);
            const float dy = read_single(base + 20u) - read_single(other_base + 20u);
            const float dz = read_single(base + 24u) - read_single(other_base + 24u);
            if (dx * dx + dy * dy + dz * dz < single_from_bits(0x48fd2000u)) near_player = 1;
        }
    }
    if (near_player != static_cast<signed char>(read_u8(base + 76u))) {
        if (near_player != 0) norrath_actor_add_run_list(self);
        else norrath_actor_add_medium_run_list(self);
    }
    write_u8(base + 76u, static_cast<unsigned>(near_player));
    const int mode = read_i16(base + 82u);
    if (mode == 1 && signed_remainder(frameNm, read_i16(base + 84u)) == 0) {
        for (unsigned k = 0u; k < 4u; ++k)
            if (static_cast<signed char>(read_u8(base + 77u + k)) == 0) write_u8(base + 77u + k, 40u);
    }
    if (signed_remainder(frameNm, read_i16(base + 84u)) != 0) {
        const int sum = static_cast<signed char>(read_u8(base + 77u)) + static_cast<signed char>(read_u8(base + 80u)) +
                        static_cast<signed char>(read_u8(base + 79u)) + static_cast<signed char>(read_u8(base + 78u));
        if (sum == 0) return;
    }
    for (int direction = 0; direction < 4; ++direction) {
        const float x = read_single(base + 16u);
        const float y = read_single(base + 20u);
        const float z = read_single(base + 24u);
        float low[3] = {x, y, z - single_from_bits(0x42700000u)};
        float high[3] = {x, y, z + single_from_bits(0x42700000u)};
        float aim[3] = {0.0f, 0.0f, 0.0f};
        const float near_reach = single_from_bits(0x42c00000u);
        const float far_reach = single_from_bits(0x44340000u);
        if (direction == 0) {
            high[1] = y + near_reach;
            low[1] = y - near_reach;
            write_u16(base + 56u, 0u);
            high[0] = x + far_reach;
            aim[0] = 1.0f;
        } else if (direction == 1) {
            low[0] = low[0] - near_reach;
            low[1] = low[1] - far_reach;
            write_u16(base + 56u, 0xc000u);
            high[0] = high[0] + near_reach;
            aim[1] = -1.0f;
        } else if (direction == 2) {
            high[1] = y + near_reach;
            low[0] = low[0] - far_reach;
            write_u16(base + 56u, 0x8000u);
            low[1] = low[1] - near_reach;
            aim[0] = -1.0f;
        } else {
            high[1] = y + far_reach;
            low[0] = low[0] - near_reach;
            write_u16(base + 56u, 0x4000u);
            high[0] = high[0] + near_reach;
            aim[1] = 1.0f;
        }
        void* list[256];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 256);
        for (int i = 0; list[i] != nullptr; ++i) {
            void* const other = list[i];
            if ((read_word(address(other) + 12u) & 0x40u) == 0u) continue;
            float start[3];
            start[0] = read_single(base + 16u) + icos__Fi(read_i16(base + 56u)) * single_from_bits(0x42100000u);
            start[1] = read_single(base + 20u) + isin__Fi(read_i16(base + 56u)) * single_from_bits(0x42100000u);
            start[2] = read_single(base + 24u);
            const float result = worldHitscan__FP12_worldHeaderG6Point3T1fi(
                pointer(read_word(address(norrath_actors_b_view_4844a0))), start, aim, single_from_bits(0x42400000u), 0);
            if (!(result == -1.0f)) continue;
            const int kind = read_i16(base + 82u);
            if (kind == 0) {
                fire_sparks(self, base);
                continue;
            }
            if (kind != 1) continue;
            const Address counter = base + 77u + static_cast<unsigned>(direction);
            if (static_cast<signed char>(read_u8(counter)) == 0) continue;
            if ((read_u8(counter) & 7u) == 0u) fire_arrow(self, base);
            if (static_cast<signed char>(read_u8(counter)) != 0) write_u8(counter, read_u8(counter) - 1u);
        }
    }
}
