#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setWeapons__6Goblin at 0x22c180, 204 bytes. Switches on the word at
// object+972 and stores a word at object+988:
//   0, 1, 2 and 3: eRand__10GameObject(object) modulo 3 (unsigned) plus 5;
//   4: 3;
//   5: 2;
//   anything else: nothing is stored and eRand is not called.
// Inferred: +972 is a variant selector and +988 a weapon index. Unknown: what
// the numbers name.
extern "C" unsigned eRand__10GameObject(void* object);

extern "C" void setWeapons__6Goblin(void* self) {
    using namespace rtti_c;
    const Address object = address(self);
    const unsigned variant = read_word(object + 972u);
    if (variant <= 3u) {
        write_word(object + 988u, eRand__10GameObject(self) % 3u + 5u);
    } else if (variant == 4u) {
        write_word(object + 988u, 3u);
    } else if (variant == 5u) {
        write_word(object + 988u, 2u);
    }
}
