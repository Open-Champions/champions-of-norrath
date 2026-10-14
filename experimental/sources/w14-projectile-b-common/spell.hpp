// Shared helpers for the w14-projectile-b sources: the w13-spell-msg-a helpers with the three three-argument table calls routed
// through w14b_host_call3 when a host contract defines W12_SPELL_C_HOST (the EE path is unchanged). Observed behaviour only;
// nothing here is an EE input by itself.
#ifndef W14_PROJECTILE_B_SPELL_HPP
#define W14_PROJECTILE_B_SPELL_HPP
#include "../w12-actor-spell-c-common/access.hpp"

#if defined(W12_SPELL_C_HOST)
// Logging hook for the host contract: function word, adjusted object, then the three arguments.
extern "C++" int w14b_host_call3(unsigned function, void* adjusted, int first, int second, void* third);
#endif

namespace w13a {

// The 80-byte item description a caller builds on its stack before gameAttackResolver (the layout is the one the
// w11-actor-c sparks collision records): bytes 32 and 33 are -1, the eight halfwords at 16..31 are 0 and the first
// halfword of each of the four 8-byte entries at 36, 44, 52 and 60 is -1. Bytes outside those ranges are not written.
inline void init_description(unsigned char* d) {
    for (int entry = 3; entry >= 0; --entry) *reinterpret_cast<short*>(d + 36 + 8 * entry) = -1;
    d[33] = 0xff;
    d[32] = 0xff;
    for (int i = 0; i < 8; ++i) *reinterpret_cast<short*>(d + 16 + 2 * i) = 0;
}

// Finds the first of the four entries whose first halfword is -1 or `kind` and fills it in: first halfword `kind`,
// then `a` at +2, `b` at +4 and `c` at +6. Nothing is written when no entry matches.
inline void fill_description(unsigned char* d, short kind, short a, short b, short c) {
    for (int entry = 0; entry < 4; ++entry) {
        unsigned char* e = d + 36 + 8 * entry;
        const short first = *reinterpret_cast<short*>(e);
        if (first == -1 || first == kind) {
            *reinterpret_cast<short*>(e) = kind;
            *reinterpret_cast<short*>(e + 2) = a;
            *reinterpret_cast<short*>(e + 4) = b;
            *reinterpret_cast<short*>(e + 6) = c;
            return;
        }
    }
}

// The call both hit handlers make on the object whose address is the word at o+tableObjectOffset: its table is the
// word at +96 of that object (not +72), the entry at offset 112 is called on the object (adjusted by the entry's signed
// halfword) with (0, signed halfword, address).
inline int call_entry_112(unsigned char* object, int halfword, void* address) {
    unsigned char* const entry = w12c::from_word(w12c::word(object, 96)) + 112;
    const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
#if defined(W12_SPELL_C_HOST)
    return w14b_host_call3(function, object + *reinterpret_cast<short*>(entry), 0, halfword, address);
#else
    return reinterpret_cast<int (*)(void*, int, int, void*)>(static_cast<__UINTPTR_TYPE__>(function))(
        object + *reinterpret_cast<short*>(entry), 0, halfword, address);
#endif
}

// Same call with a mode word in the second position: the entry at offset 112 of the table that is the word at +96 of the object
// is called on the object (adjusted by the entry's signed halfword) with (mode, signed halfword, address).
inline int call_entry_112_mode(unsigned char* object, int mode, int halfword, void* address) {
    unsigned char* const entry = w12c::from_word(w12c::word(object, 96)) + 112;
    const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
#if defined(W12_SPELL_C_HOST)
    return w14b_host_call3(function, object + *reinterpret_cast<short*>(entry), mode, halfword, address);
#else
    return reinterpret_cast<int (*)(void*, int, int, void*)>(static_cast<__UINTPTR_TYPE__>(function))(
        object + *reinterpret_cast<short*>(entry), mode, halfword, address);
#endif
}

// The call the cone of fire makes on its caster: the entry at offset 104 of the table that is the word at +96 of the object is called on the
// object (adjusted by the entry's signed halfword) with (mode, signed byte, address-word object).
inline int call_entry_104_mode(unsigned char* object, int mode, int byte, void* argument) {
    unsigned char* const entry = w12c::from_word(w12c::word(object, 96)) + 104;
    const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
#if defined(W12_SPELL_C_HOST)
    return w14b_host_call3(function, object + *reinterpret_cast<short*>(entry), mode, byte, argument);
#else
    return reinterpret_cast<int (*)(void*, int, int, void*)>(static_cast<__UINTPTR_TYPE__>(function))(
        object + *reinterpret_cast<short*>(entry), mode, byte, argument);
#endif
}

}  // namespace w13a

#endif
