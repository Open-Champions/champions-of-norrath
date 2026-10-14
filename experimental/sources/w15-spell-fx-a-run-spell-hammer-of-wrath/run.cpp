// Observed run__18SpellHammerOfWrathG6Point3s at 0x002b5730, 92 bytes. The object is the first argument. The second (a point, copied to
// the callee's frame and never read) and the third are not used. When the word at +24 of the object is zero, the entry at
// offset 8 of the table whose address is the word at +20 is called on the object (adjusted by the entry's signed halfword)
// with the argument 3. The original also tests the object for null after reading +24, which a null object could not reach.
// Inferred: a deletion-style call from a table slot, since the same call shape is used for an object-ending step. Unknown:
// what the word at +24 is and what the table entry does. Nothing here ran on the EE.
#include "../w15-spell-fx-a-common/access.hpp"

extern "C" void run__18SpellHammerOfWrathG6Point3s(void* self, Point3, short) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 24) == 0u) {
        unsigned char* const entry = from_word(word(o, 20)) + 8;
        const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
#if defined(W12_SPELL_C_HOST)
        w12_spell_c_host_call(function, o + *reinterpret_cast<short*>(entry), 1, 3, 0);
#else
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(function))(o + *reinterpret_cast<short*>(entry), 3);
#endif
    }
}
