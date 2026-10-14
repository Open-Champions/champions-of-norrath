#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setWeapon__5Mummy at 0x23bcf0, 104 bytes, emitted here as
// norrath_mummy_set_weapon (the name its constructor imports). Reads the word at
// object+960 as an unsigned value and stores a word at object+964 from a
// six-entry jump table at 0x4730c0: 0 -> 44, 1 -> 48, 2 -> 44, 3 -> 48,
// 4 -> 46, 5 -> 50. Values of 6 and above store nothing.
// Inferred: +960 is a variant and +964 a weapon number. Unknown: what the
// numbers name.
extern "C" void norrath_mummy_set_weapon(void* self) {
    using namespace rtti_c;
    const Address object = address(self);
    switch (read_word(object + 960u)) {
    case 0u:
    case 2u:
        write_word(object + 964u, 44u);
        break;
    case 1u:
    case 3u:
        write_word(object + 964u, 48u);
        break;
    case 4u:
        write_word(object + 964u, 46u);
        break;
    case 5u:
        write_word(object + 964u, 50u);
        break;
    default:
        break;
    }
}
