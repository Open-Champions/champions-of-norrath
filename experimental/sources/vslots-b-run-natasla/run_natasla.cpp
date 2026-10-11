#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__7Natasla at 0x2337b0, 80 bytes.
// everyFrame receives the word at (object+36)+12. A nonzero return stops.
// Otherwise objectActivationCheck is called with 600, then
// creatureAdvanceAnimation with the single 1. Those three callees are not
// defined here.
extern "C" unsigned everyFrame__8CreatureP12_modelHeader(void* self, void* model);
extern "C" void objectActivationCheck__8Creaturei(void* self, int distance);
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount);
extern "C" void msg_run__7Natasla(void* self) {
    using namespace vslots_b;
    const Address group = address(pointer(read_word(address(self) + 36u)));
    if (everyFrame__8CreatureP12_modelHeader(self, pointer(read_word(group + 12u))) != 0u) return;
    objectActivationCheck__8Creaturei(self, 600);
    creatureAdvanceAnimation__8Creaturef(self, 1.0f);
}
