#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed getModel__11LavaMonster at 0x1ee280, 32 bytes, emitted here as
// norrath_lava_monster_get_model (the name the LavaMonster constructor
// imports). The word at object+36 points to a table. When the word at
// object+1048 is zero it returns the word at table+12, otherwise the word at
// table+28.
// Inferred: +1048 selects between two model slots of the resource table.
// Unknown: what each slot names.
extern "C" void* norrath_lava_monster_get_model(void* self) {
    using namespace rtti_c;
    const Address object = address(self);
    const Address table = address(pointer(read_word(object + 36u)));
    if (read_word(object + 1048u) == 0u) return pointer(read_word(table + 12u));
    return pointer(read_word(table + 28u));
}
