// Provenance: the body is the reviewed w13-spell-msg-a unit w13-spell-msg-a-msg-collision-11diseasebolt, re-verified in this package against the original with a fresh paired run and given a host
// contract (the earlier unit has none and was left out of the plan). Select one of the two units, not both.
// Observed msg_collision__11DiseaseBoltP10GameObject at 0x002930f0, 548 bytes. Arguments: the bolt o and the object T it
// touched (never null here: its word at +12 is read at once). Nothing is returned. "flags" of an object is its word
// at +12. W is the word at o+84 (an object, the owner) and the owner's own object is the word at W+12.
//  1. Nothing happens when the word at o+120 is non-zero.
//  2. When W is non-zero, the entry at table offset 160 is called on the object that is the word at W+12; a zero result
//     returns. When W is zero and T's flags lack 0x20, the word at o+120 becomes 1.
//  3. When T's flags have 0x40: eRand__10GameObject(o) is called and a negative result returns. Otherwise, when T's flags
//     have 0x20 and the 64-bit value at T+160 has bit 0x200, return.
//  4. X is the object that is the word at o+92. Its table is the word at X+96 (not +72); the entry at offset 112 is
//     called on X with (0, signed halfword at o+96, the address o+76) and gives the amount n.
//  5. An item description is built on the stack (see spell.hpp): the first entry whose first halfword is -1 or 38 gets
//     38, n, n and 0 as its four halfwords.
//  6. d = (float)n + (float)n * ((float)eRand__10GameObject(o) * 2^-16) * 0.2 (0.2 is the pool constant with bits
//     0x3e4ccccc, no symbol names it).
//  7. gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(T, the owner's object (the word at W+12, or null
//     when W is zero; W is read again here), (float)n, d, 100000, 0, 0, 0, description, false) runs and the word at
//     o+120 becomes 1.
// Inferred: a one-shot hit that applies an attack whose size comes from a call on another object and a random spread;
// 38 is a description kind. Unknown: what the words at o+84, +92, +96 and +120, the entry at 112 and the kind mean.
// Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
int eRand__10GameObject(void* object);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);

void msg_collision__11DiseaseBoltP10GameObject(void* self, void* other) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = static_cast<unsigned char*>(other);
    if (word(o, 120) != 0) return;
    unsigned flags;
    if (word(o, 84) != 0) {
        if (call_entry(from_word(word(from_word(word(o, 84)), 12)), 160) == 0) return;
        flags = word(t, 12);
    } else {
        flags = word(t, 12);
        if (!(flags & 0x20u)) word(o, 120) = 1;
    }
    if (flags & 0x40u) {
        if (eRand__10GameObject(o) < 0) return;
    } else if (flags & 0x20u) {
        if (dword(t, 160) & 0x200ull) return;
    }
    const int n = call_entry_112(from_word(word(o, 92)), half(o, 96), o + 76);
    unsigned char description[80];
    init_description(description);
    fill_description(description, 38, static_cast<short>(n), static_cast<short>(n), 0);
    const float amount = static_cast<float>(n);
    const float spread = amount * (static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u));
    unsigned char* owner = nullptr;
    if (word(o, 84) != 0) owner = from_word(word(from_word(word(o, 84)), 12));
    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(t, owner, amount, amount + spread * bits(0x3e4ccccc), 100000, 0, 0, 0, description, false);
    word(o, 120) = 1;
}
}
