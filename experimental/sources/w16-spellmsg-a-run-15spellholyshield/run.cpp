// Observed run__15SpellHolyShieldG6Point3s at 0x002ae590, 264 bytes. O is the object, the first argument. The second
// argument (a point, copied to the callee's frame and never used) and the third are not read. With K the pool constant
// with bits 0x3c888888 (about 1/60, no symbol names it): the single at O+24 is raised by K and the single at O+28 is
// lowered by K, both stored. Then:
//  - when the single at O+32 is below the new O+24 value: if O is not null, the entry at offset 8 of the table whose
//    address is the word at O+20 is called on O (adjusted by the entry's signed halfword) with the argument 3; nothing
//    else happens;
//  - otherwise, with P the object that is the word at O+12: a 3x4 matrix is made by
//    matMakeTransYaw__FP8Matrix34G6Point3s from the three singles at P+16 and the signed halfword at P+56, handed with
//    the word at O+36 to P_SetParticleSystemMatrix__FiP8Matrix34, and the halfwords of P are set: +140 = 21, +142 = 28,
//    +144 = 115, +146 = 55, +148 = 21, +150 = 28, +152 = 115, +154 = 55.
// Inferred: an elapsed-time counter against a limit in a held shield effect that follows its owner's position and yaw
// and tints it (the halfword pairs look like two colour triples with a fourth value). Unknown: what the halfwords and
// the table entry mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

static void call_end(unsigned char* o) {
    unsigned char* const entry = w12c::from_word(w12c::word(o, 20)) + 8;
    const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
    reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(function))(o + *reinterpret_cast<short*>(entry), 3);
}

extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const Point3* point, short yaw);
void P_SetParticleSystemMatrix__FiP8Matrix34(int system, void* matrix);

void run__15SpellHolyShieldG6Point3s(void* self, Point3, short) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    const float k = bits(0x3c888888u);
    const float elapsed = real(o, 24) + k;
    real(o, 28) = real(o, 28) - k;
    const float limit = real(o, 32);
    real(o, 24) = elapsed;
    // The original reads through O before testing it for null; keep the compiler from dropping the test.
    __asm__("" : "+r"(o));
    if (limit < elapsed) {
        if (o != nullptr) call_end(o);
        return;
    }
    unsigned char* const p = from_word(word(o, 12));
    alignas(16) unsigned char matrix[64];
    const Point3 position = {real(p, 16), real(p, 20), real(p, 24)};
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, &position, half(p, 56));
    P_SetParticleSystemMatrix__FiP8Matrix34(static_cast<int>(word(o, 36)), matrix);
    unsigned char* const q = from_word(word(o, 12));
    half(q, 154) = 55;
    half(q, 140) = 21;
    half(q, 142) = 28;
    half(q, 144) = 115;
    half(q, 148) = 21;
    half(q, 150) = 28;
    half(q, 152) = 115;
    half(q, 146) = 55;
}
}
