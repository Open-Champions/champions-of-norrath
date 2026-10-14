// Observed run__17SpellMinorHealingG6Point3s at 0x002a71c0, 252 bytes. O is the object, the first argument. The second
// argument (a point, copied to the callee's frame and never used) and the third are not read. With K the pool constant
// with bits 0x3c888888 (about 1/60, no symbol names it) and P the object that is the word at O+24, whose word at +960
// is an index into `player` (the array of 23984-byte records; R = player + 23984 * index):
//  1. the single at O+28 (call it t) is raised by K and stored;
//  2. when the single at R+172 plus 1.0 is not above the single at R+176, R+172 becomes R+172 plus the single at O+32;
//  3. when t is below the pool constant with bits 0x3effff78 (just under 0.5), halfwords of P are set: +140 = 104,
//     +142 = 107, +144 = 64, +146 = 63, +148 = 104, +150 = 107, +152 = 64;
//  4. when t is above the pool constant with bits 0x3fbfffde (just under 1.5) and O is not null, the entry at offset 8 of
//     the table whose address is the word at O+20 is called on O (adjusted by the entry's signed halfword) with the
//     argument 3.
// Inferred: a held minor-healing effect that tops up a running amount on the owning player record (the +176 value acts as
// a ceiling for it) and colours its owner while t is small, then ends. Unknown: what R+172, R+176 and the halfwords
// mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

static void call_end(unsigned char* o) {
    unsigned char* const entry = w12c::from_word(w12c::word(o, 20)) + 8;
    const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
    reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(function))(o + *reinterpret_cast<short*>(entry), 3);
}

extern "C" {
extern unsigned char player[];

void run__17SpellMinorHealingG6Point3s(void* self, Point3, short) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* const p = from_word(word(o, 24));
    const float t = real(o, 28) + bits(0x3c888888u);
    real(o, 28) = t;
    unsigned char* const r = player + static_cast<unsigned>(sword(p, 960)) * 23984u;
    const float amount = real(r, 172);
    if (amount + 1.0f <= real(r, 176)) real(r, 172) = amount + real(o, 32);
    const float after = real(o, 28);
    // The original reads through O before testing it for null; keep the compiler from dropping the test.
    __asm__("" : "+r"(o));
    if (after < bits(0x3effff78u)) {
        half(p, 146) = 63;
        half(p, 140) = 104;
        half(p, 142) = 107;
        half(p, 144) = 64;
        half(p, 148) = 104;
        half(p, 150) = 107;
        half(p, 152) = 64;
    }
    if (bits(0x3fbfffdeu) < after && o != nullptr) call_end(o);
}
}
