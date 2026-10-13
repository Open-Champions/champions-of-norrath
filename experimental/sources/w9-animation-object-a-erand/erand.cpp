#include "../w9-animation-object-a-common/mem.hpp"
// Observed eRand__10GameObject at 0x00153fa8, 40 bytes. The word at object+68 is
// replaced by word*1664525 + 1013904223 (32-bit wrap) and the new word shifted
// right by 16 is returned, so the result is 0..65535.
// Inferred: this is a per-object linear congruential step; the multiplier and
// increment are the well-known 0x0019660d and 0x3c6ef35f pair. No field name is claimed.
// Unknown: what seeds the word at +68 and who else reads it.
extern "C" unsigned eRand__10GameObject(unsigned char* self) {
    using namespace norrath_ao;
    const unsigned next = load_word(self + 68) * 0x0019660du + 0x3c6ef35fu;
    store_word(self + 68, next);
    return next >> 16;
}
