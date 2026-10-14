#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_run__8FireBomb at 0x289a38, 808 bytes.
// The word at +84 counts calls: it is incremented every call and nothing else
// happens until its old value equals the word at +80, when it restarts from 0.
// Then, if findTagString(object, "script", 0) is non-null, scriptStart is run on a
// second findTagString lookup of the same tag and a zero result ends the call.
// Next gamePositionVisible(position at +16, 960.0) must be nonzero.
// A heading is drawn from the shared generator syncRandState (x = x * 1664525 +
// 1013904223, value = x >> 16): a = (halfword at +56) + (value % 8123) - 4061,
// truncated to 16 bits. The unit direction d = (icos(a), isin(a)) / |.|, with the
// reciprocal square root taken by rsqrt.s, is used as follows. P is the position.
//  Signed byte +78 nonzero: A = (P.x + d.x*K1, P.y + d.y*K1, P.z); the second
//    Point3 handed to FireBomblet is B = (A.x, A.y, P.z + K5) and the velocity-like
//    third is C = ((P.x - A.x)*K2*K3, (P.y - A.y)*K2*K3, K4).
//  Signed byte +78 zero: a second draw gives m = (value % 386) + 332 as a float and
//    B = (P.x + d.x*m, P.y + d.y*m, P.z + K6) with no third point (null).
// K1..K6 are the six consecutive floats at 0x482570 (401.0, about 1.18, 1/60 as
// the single 0x3c888888, about -7.03, 357.0 and -1037.0); they are written here by
// bit pattern.
// A 184-byte block is allocated; when non-null FireBomblet is constructed in it with
// (a copy of P, the signed halfword at +56, the address object+77, B, the third
// point or null, the signed byte at +79).
// Unknown: what the bomb timing words, direction, constants and bytes 77-79 mean.
// Inferred: norrath_actor_find_tag_string is findTagString, norrath_block_alloc is
// blockAlloc.
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" int scriptStart__FPc(char* name);
extern "C" int gamePositionVisible__FP6Point3f(const void* position, float range);
extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);
extern "C" unsigned char* norrath_block_alloc(int size);
extern "C" void __11FireBombletG6Point3sPcT1P6Point3i(
    void* self, const void* position, short heading, char* marker, const void* anchor, const void* velocity, int value);
extern "C" unsigned syncRandState;

namespace {
__attribute__((always_inline)) inline float reciprocal_root(float length_squared) {
    const float one = 1.0f;
#if defined(__mips__)
    float result;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return one / __builtin_sqrtf(length_squared);
#endif
}

__attribute__((always_inline)) inline unsigned draw() {
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    return syncRandState >> 16;
}
}

extern "C" void msg_run__8FireBomb(void* self) {
    using namespace w8c;
    const Address base = address(self);
    const unsigned old = read_word(base + 84u);
    write_word(base + 84u, old + 1u);
    if (old != read_word(base + 80u)) return;
    write_word(base + 84u, 0u);
    static char tag[] = {'s', 'c', 'r', 'i', 'p', 't', '\0'};
    if (norrath_actor_find_tag_string(self, tag, 0) != nullptr) {
        if (scriptStart__FPc(norrath_actor_find_tag_string(self, tag, 0)) == 0) return;
    }
    unsigned char* const position = static_cast<unsigned char*>(self) + 16;
    if (gamePositionVisible__FP6Point3f(position, single_from_bits(0x44700000u)) == 0) return;
    const int heading = static_cast<short>(read_u16(base + 56u) + draw() % 8123u - 4061u);
    const float cosine = icos__Fi(heading);
    const float sine = isin__Fi(heading);
    const float inverse = reciprocal_root(cosine * cosine + sine * sine);
    const float dir_y = sine * inverse;
    const float dir_x = cosine * inverse;
    float anchor[3];
    float velocity[3];
    const void* velocity_argument;
    if (static_cast<signed char>(read_u8(base + 78u)) != 0) {
        const float shift_x = dir_x * single_from_bits(0x43c88000u);
        const float shift_y = dir_y * single_from_bits(0x43c88000u);
        anchor[0] = read_single(base + 16u) + shift_x;
        anchor[1] = read_single(base + 20u) + shift_y;
        anchor[2] = read_single(base + 24u);
        velocity[2] = single_from_bits(0xc0e1163bu);
        velocity[0] = (read_single(base + 16u) - anchor[0]) * single_from_bits(0x3f9709bfu);
        velocity[1] = (read_single(base + 20u) - anchor[1]) * single_from_bits(0x3f9709bfu);
        anchor[2] = anchor[2] + single_from_bits(0x43b28000u);
        velocity[0] = velocity[0] * single_from_bits(0x3c888888u);
        velocity[1] = velocity[1] * single_from_bits(0x3c888888u);
        velocity_argument = velocity;
    } else {
        const float reach = static_cast<float>(static_cast<int>(draw() % 386u) + 332);
        const float shift_y = dir_y * reach;
        const float shift_x = dir_x * reach;
        anchor[0] = read_single(base + 16u) + shift_x;
        anchor[1] = read_single(base + 20u) + shift_y;
        anchor[2] = read_single(base + 24u);
        anchor[2] = anchor[2] + single_from_bits(0xc481a000u);
        velocity_argument = nullptr;
    }
    void* const block = norrath_block_alloc(184);
    if (block == nullptr) return;
    const float copy[3] = {read_single(base + 16u), read_single(base + 20u), read_single(base + 24u)};
    __11FireBombletG6Point3sPcT1P6Point3i(block, copy, static_cast<short>(read_i16(base + 56u)),
                                         static_cast<char*>(self) + 77, anchor, velocity_argument,
                                         static_cast<signed char>(read_u8(base + 79u)));
}
