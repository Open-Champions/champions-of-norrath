// Observed msg_collision__12ShockOfFrostP10GameObject at 0x002bd800, 588 bytes. Arguments: the bolt o and the object T it
// touched (T may be null from step 3 on; step 2 reads its flags without a test). "flags" of an object is its word at +12.
// Q is the word at o+76 (an object; the owner's own object is the word at Q+12).
//  1. The word at o+204 becomes 0.
//  2. When T's flags lack 0x20 the word at o+196 becomes 1. Otherwise, when Q is non-zero, the entry at table offset 160
//     is called on the object that is the word at Q+12; a zero result returns.
//  3. When T is non-null: if its flags have 0x40, eRand__10GameObject(o) is called and a negative result returns;
//     otherwise, if its flags have 0x20 and the 64-bit value at T+160 has bit 0x200, return.
//  4. X is the object that is the word at o+200. Its table is the word at X+96 (not +72); the entry at offset 112 is
//     called on X with (0, signed halfword at o+148, the address o+76) and gives the amount n.
//  5. An item description is built on the stack (see spell.hpp), then f = (float)n * 0.2 (the pool constant with bits
//     0x3e4ccccc, no symbol names it) is multiplied by (float)eRand__10GameObject(o) * 2^-16; the first entry whose
//     first halfword is -1 or 22 gets 22, n, n + (int)f and 0 as its four halfwords.
//  6. gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(T, null, 0.0, 0.0, 100000, 0, 0, 0, description,
//     false) runs, then the word at o+204 becomes T and the word at o+196 becomes 1.
// Inferred: a one-shot frost hit; 22 is a description kind. Unknown: what the words at o+76, +196, +200, +204, the
// halfword at +148 and the entry at 112 mean. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
int eRand__10GameObject(void* object);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);

void msg_collision__12ShockOfFrostP10GameObject(void* self, void* other) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = static_cast<unsigned char*>(other);
    word(o, 204) = 0;
    const unsigned q = word(o, 76);
    if (!(word(t, 12) & 0x20u)) {
        word(o, 196) = 1;
    } else if (q != 0) {
        if (call_entry(from_word(word(from_word(q), 12)), 160) == 0) return;
    }
    if (t != nullptr) {
        const unsigned flags = word(t, 12);
        if (flags & 0x40u) {
            if (eRand__10GameObject(o) < 0) return;
        } else if (flags & 0x20u) {
            if (dword(t, 160) & 0x200ull) return;
        }
    }
    const int n = w13a::call_entry_112(from_word(word(o, 200)), half(o, 148), o + 76);
    unsigned char description[80];
    w13a::init_description(description);
    float f = static_cast<float>(n) * bits(0x3e4ccccc);
    f = f * (static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u));
    w13a::fill_description(description, 22, static_cast<short>(n), static_cast<short>(n + static_cast<int>(f)), 0);
    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(t, nullptr, 0.0f, 0.0f, 100000, 0, 0, 0, description, false);
    word(o, 204) = to_word(t);
    word(o, 196) = 1;
}
}
