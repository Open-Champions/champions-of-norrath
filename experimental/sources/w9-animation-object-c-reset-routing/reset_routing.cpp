#include "../w9-animation-object-c-common/mem.hpp"
// Observed resetRouting__8Creature at 0x00113460, 40 bytes. Stores, with no reads of the
// creature: the float -1.0 at +260 and +264, the word -1 at +580 and +584, and the word
// zero at +708 and +716.
// Inferred: this clears a routing/path state (the name says so); the -1 words look like
// "no index" markers and the floats like "no value" markers. No field names are claimed.
// Unknown: what each of the six fields is used for.
extern "C" void resetRouting__8Creature(unsigned char* self) {
    using namespace norrath_aoc;
    store_word(self + 716, 0);
    store_word(self + 580, 0xffffffffu);
    store_float(self + 260, -1.0f);
    store_word(self + 708, 0);
    store_word(self + 584, 0xffffffffu);
    store_float(self + 264, -1.0f);
}
