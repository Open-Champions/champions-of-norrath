// Observed run__14SpellHarmTouchG6Point3s at 0x0029b170, 168 bytes. O is the object, the first argument. The
// second argument (a point, copied to the callee's frame and never used) and the third (a short, overwritten) are
// not read. The single at O+24 is lowered by 1/60 (the pool constant with bits 0x3c888888, no symbol names it) and
// stored; call the result t. Then:
//  - t < 0: the entry at offset 8 of the table whose address is the word at O+20 is called on O (adjusted by the
//    entry's signed halfword) with the argument 3;
//  - otherwise: with P the object that is the word at O+12, halfword +146 of P becomes 113 and halfwords +142 and
//    +150 of P become (int)(min(1.0, t) * 128.0), truncated to 16 bits.
// Inferred: a countdown in seconds of a held touch spell whose remaining time drives a fade value on another object.
// Unknown: what the word at O+20 table entry and the three halfwords of P mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" void run__14SpellHarmTouchG6Point3s(void* self, Point3, short) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const float t = real(o, 24) - bits(0x3c888888u);
    real(o, 24) = t;
    if (t < 0.0f) {
        unsigned char* const entry = from_word(word(o, 20)) + 8;
        const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
#if defined(W12_SPELL_C_HOST)
        w12_spell_c_host_call(function, o + *reinterpret_cast<short*>(entry), 1, 3, 0);
#else
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(function))(o + *reinterpret_cast<short*>(entry), 3);
#endif
    } else {
        const float fade = (t < 1.0f ? t : 1.0f) * 128.0f;
        unsigned char* const p = from_word(word(o, 12));
        half(p, 146) = 113;
        half(p, 142) = static_cast<short>(static_cast<int>(fade));
        half(p, 150) = static_cast<short>(static_cast<int>(fade));
    }
}
