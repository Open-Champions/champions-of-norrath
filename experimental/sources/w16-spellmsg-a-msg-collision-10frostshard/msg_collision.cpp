// Observed msg_collision__10FrostShardP10GameObject at 0x002bc528, 604 bytes. Arguments: the shard O and the object T it touched
// (T may be null from step 3 on; step 2 reads its flags without a test). "flags" of an object is its word at +12. Q is the word
// at O+76 and R = 0 at first.
//  1. Nothing happens when the word at O+112 is nonzero or when T equals the word at O+120.
//  2. When Q is nonzero and T's flags have 0x20, R becomes the word at Q+12 and the entry at table offset 160 (table = the word at
//     R+72) is called on R; a zero result returns. When T's flags lack 0x20 (whatever Q is) the word at O+112 becomes 1.
//  3. When T is non-null: if its flags have 0x40, eRand__10GameObject(O) is called and a negative result returns; otherwise, if
//     its flags have 0x20 and the 64-bit value at T+160 has bit 0x200, return.
//  4. X is the object that is the word at O+116. Its table is the word at X+96; the entry at offset 112 is called on X with
//     (2, signed halfword at O+88, the address O+76) and gives the amount n.
//  5. An item description is built on the stack (see spell.hpp), then f = (float)n * 0.2 (the pool constant with bits 0x3e4ccccc,
//     no symbol names it) is multiplied by (float)eRand__10GameObject(O) * 2^-16; the first entry whose first halfword is -1 or 22
//     gets 22, n, n + (int)f and 0 as its four halfwords.
//  6. gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(T, R, 0.0, 0.0, 100000, 0, 0, 0, description, false) runs,
//     then the word at O+112 becomes 1.
// Inferred: a one-shot frost hit that remembers it has hit; the same shape as the shock-of-frost collision with other offsets.
// Unknown: what the words at O+76, +112, +116, +120, the halfword at +88 and the entry at 112 mean. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
int eRand__10GameObject(void* object);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);

void msg_collision__10FrostShardP10GameObject(void* self, void* other) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* t = static_cast<unsigned char*>(other);
    unsigned char* r = nullptr;
    if (word(o, 112) != 0u) return;
    if (t == from_word(word(o, 120))) return;
    const unsigned q = word(o, 76);
    const unsigned flags0 = word(t, 12);
    __asm__("" : "+r"(t));
    if (q != 0u) {
        if ((flags0 & 0x20u) != 0u) {
            r = from_word(word(from_word(q), 12));
            if (call_entry(r, 160) == 0u) return;
        } else {
            word(o, 112) = 1;
        }
    } else if ((flags0 & 0x20u) == 0u) {
        word(o, 112) = 1;
    }
    if (t != nullptr) {
        const unsigned flags = word(t, 12);
        if (flags & 0x40u) {
            if (eRand__10GameObject(o) < 0) return;
        } else if (flags & 0x20u) {
            if (dword(t, 160) & 0x200ull) return;
        }
    }
    unsigned char* const x = from_word(word(o, 116));
    unsigned char* const entry = from_word(word(x, 96)) + 112;
    const int n = reinterpret_cast<int (*)(void*, int, int, void*)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
        x + *reinterpret_cast<short*>(entry), 2, half(o, 88), o + 76);
    unsigned char description[80];
    w13a::init_description(description);
    float f = static_cast<float>(n) * bits(0x3e4ccccc);
    f = f * (static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u));
    w13a::fill_description(description, 22, static_cast<short>(n), static_cast<short>(n + static_cast<int>(f)), 0);
    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(t, r, 0.0f, 0.0f, 100000, 0, 0, 0, description, false);
    word(o, 112) = 1;
}
}
