#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__5Timer at 0x21ccc8, 64 bytes.
// The word at object+76 is reduced by one with 32-bit wrap. A signed result
// above zero returns. Otherwise objectDoMyActions receives the word at
// object+80, then objectRemoveFromRunList receives the object.
// Those two callees are not defined here.
extern "C" void objectDoMyActions__FP10GameObjecti(void* self, int value);
extern "C" void objectRemoveFromRunList__FP10GameObject(void* self);
extern "C" void msg_run__5Timer(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    const unsigned next = read_word(base + 76u) - 1u;
    write_word(base + 76u, next);
    if (signed_word(next) > 0) return;
    objectDoMyActions__FP10GameObjecti(self, signed_word(read_word(base + 80u)));
    objectRemoveFromRunList__FP10GameObject(self);
}
