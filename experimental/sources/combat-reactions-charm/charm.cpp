#include "../combat-reactions-bytes/bytes.hpp"
// Observed charm__8Creaturei at 0x112ba0, 64 bytes.
// When bit 0x20 of the word at object+12 is clear nothing is written. When it
// is set, the low 16 bits of the argument are stored at object+792, the list
// head at object+576 is destroyed, and the word at object+576 is cleared.
// Inferred: listDestroy is the already defined norrath_list_destroy.
// Unknown: what the stored halfword and the 0x20 flag mean.
extern "C" void norrath_list_destroy(void* head);

extern "C" void charm__8Creaturei(void* self, int value) {
    using namespace combat_reactions;
    const Address base = address(self);
    if ((read_word(base + 12u) & 0x20u) == 0u) return;
    write_u16(base + 792u, static_cast<unsigned>(value));
    norrath_list_destroy(static_cast<unsigned char*>(self) + 576u);
    write_word(base + 576u, 0u);
}
