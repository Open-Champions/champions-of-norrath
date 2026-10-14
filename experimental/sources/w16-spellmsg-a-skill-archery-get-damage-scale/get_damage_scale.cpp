// Observed getDamageScale__12SkillArcheryi at 0x002a2c58, 108 bytes. S is the object (first argument) and R the int
// second argument (a rank, from the call in the player damage recomputation). The table whose address is the word at S+96
// is used twice, each time with the entry's signed halfword added to S:
//  1. the entry at offset 120 is called with (S, R, 20) and returns an int V;
//  2. the entry at offset 88 is called with (S, 0, V) and returns an int W.
// The result is the single (float)W * 0.01 (the pool constant with bits 0x3c23d70a, no symbol names it).
// Inferred: a percentage from the skill's rank that scales archery damage; the two table calls look like a lookup of a
// per-rank value (the 20 may be a value index) followed by its conversion. Unknown: what the two table entries are.
// Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" float getDamageScale__12SkillArcheryi(void* self, int rank) {
    using namespace w12c;
    unsigned char* const s = static_cast<unsigned char*>(self);
    unsigned char* entry = from_word(word(s, 96)) + 120;
    const int value = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
        s + *reinterpret_cast<short*>(entry), rank, 20);
    entry = from_word(word(s, 96)) + 88;
    const int scaled = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
        s + *reinterpret_cast<short*>(entry), 0, value);
    return static_cast<float>(scaled) * bits(0x3c23d70au);
}
