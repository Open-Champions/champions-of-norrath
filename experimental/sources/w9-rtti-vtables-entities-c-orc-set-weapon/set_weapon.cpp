#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setWeapon__3Orc at 0x226fb8, 256 bytes, emitted here as
// norrath_orc_set_weapon (the name the Orc constructor imports). Calls
// eRand__10GameObject(object) once first, then switches on the word at
// object+1116 (variant) and stores a word at object+972:
//   0: bit 8 of the roll set -> 1; else a roll bit in 0x16 set -> 4; else 3;
//   4: 2, and the word at object+1124 becomes 1 (the roll is not used);
//   1: a roll bit in 0x16 set -> 5; else a bit in 0x32 set -> 7; else 14;
//   2 and 3: eRand is called a second time and the result modulo 3 (unsigned)
//      plus 14 is stored;
//   anything else: nothing is stored (the first roll is still taken).
// Unknown: what the variants and weapon numbers mean.
extern "C" unsigned eRand__10GameObject(void* object);

extern "C" void norrath_orc_set_weapon(void* self) {
    using namespace rtti_c;
    const Address object = address(self);
    const unsigned roll = eRand__10GameObject(self);
    const unsigned variant = read_word(object + 1116u);
    if (variant == 4u) {
        write_word(object + 1124u, 1u);
        write_word(object + 972u, 2u);
    } else if (variant == 0u) {
        if ((roll & 0x8u) != 0u) write_word(object + 972u, 1u);
        else if ((roll & 0x16u) != 0u) write_word(object + 972u, 4u);
        else write_word(object + 972u, 3u);
    } else if (variant == 1u) {
        if ((roll & 0x16u) != 0u) write_word(object + 972u, 5u);
        else if ((roll & 0x32u) != 0u) write_word(object + 972u, 7u);
        else write_word(object + 972u, 14u);
    } else if (variant - 2u < 2u) {
        write_word(object + 972u, eRand__10GameObject(self) % 3u + 14u);
    }
}
