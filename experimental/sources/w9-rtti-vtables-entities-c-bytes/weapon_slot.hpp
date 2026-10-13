#pragma once
// Shared by the initType bodies that fill one of four 8-byte weapon-effect
// slots (Skeleton, Orc, Mummy, MaleDarkElf). Observed in each: a global pointer
// word of the 252-byte table skills (at 0x34b440) names an object whose table
// pointer is the word at +96; the virtual function at byte offset 96 of that
// table (adjustment halfword, function word) is called with (object adjusted,
// 0, 1) and returns an amount s. The amount is scaled by a single factor and by
// one eRand__10GameObject roll as a signed single times 2^-16, truncated, and
// added to s. The first slot whose type halfword is -1 or already equals the
// given type receives the type, s at +2, the total at +4 and 0 at +6.
// Unknown: what skills[] holds and what the call computes.
#include "bytes.hpp"
extern "C" {
extern unsigned char skills[];
int eRand__10GameObject(void* object);
}
namespace rtti_c {
__attribute__((always_inline)) inline void add_weapon_slot(void* self, unsigned slots_offset, unsigned skills_word,
                                                          unsigned type, unsigned factor_bits) {
    const Address object = address(self);
    void* const owner = pointer(read_word(address(skills) + 4u * skills_word));
    const Address table = address(pointer(read_word(address(owner) + 96u))) + 96u;
    typedef int (*Call)(void*, int, int);
    Call call = reinterpret_cast<Call>(pointer(read_word(table + 4u)));
    const int amount = call(static_cast<unsigned char*>(owner) + read_i16(table), 0, 1);
    float scaled = static_cast<float>(amount) * single_from_bits(factor_bits);
    const int roll = eRand__10GameObject(self);
    scaled = scaled * (static_cast<float>(roll) * single_from_bits(0x37800000u));
    const int total = amount + static_cast<int>(scaled);
    for (unsigned slot = 0; slot < 4u; ++slot) {
        const Address entry = object + slots_offset + 8u * slot;
        const unsigned current = read_u16(entry);
        if (current == 0xffffu || current == type) {
            write_u16(entry, type);
            write_u16(entry + 6u, 0u);
            write_u16(entry + 2u, static_cast<unsigned>(amount));
            write_u16(entry + 4u, static_cast<unsigned>(total));
            break;
        }
    }
}
}
