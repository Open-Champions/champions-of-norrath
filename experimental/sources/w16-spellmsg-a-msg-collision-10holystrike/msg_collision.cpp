// Observed msg_collision__10HolyStrikeP10GameObject at 0x002b2520, 680 bytes. Arguments: the strike O and the object T it touched
// (T is read without a null test). "flags" of an object is its word at +12. Q is the word at O+84 (an object, or zero). K is the
// pool constant with bits 0x3e4ccccc (about 0.2, no symbol names it); a roll r is (float)eRand__10GameObject(O) * 2^-16.
//  1. Nothing happens when the word at O+120 is nonzero.
//  2. When Q is nonzero, R is the word at Q+12 and the entry at table offset 160 (table = the word at R+72) is called on R; a zero
//     result returns.
//  3. If T's flags have 0x40, eRand__10GameObject(O) is called and a negative result returns; otherwise, if its flags have 0x20 and the
//     64-bit value at T+160 has bit 0x200, return.
//  4. X is the object that is the word at O+104. Its table is the word at X+96; the entry at offset 88 is called on X with
//     (1 if the signed byte at T+172 equals 16 else 0, the signed halfword at O+92) and gives the amount n.
//  5. An item description is built on the stack (see spell.hpp). With f = (float)n, the amount a is computed with a macro-like
//     pattern that evaluates its expression (n + (n * r) * K, a new roll each time) twice: when ((f + (f * r1) * K) + 0.5) is not below
//     zero, a = (int)((f + (f * r2) * K) + 1.0) with a second roll r2; otherwise a = (int)(((f + (f * r2) * K) + 0.5) - 0.5).
//  6. gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(T, (the word at Q+12, or 0 when Q is zero), f, (float)a, 100000,
//     0, 0, 0x80000, description, false) runs. Then the word at O+120 becomes 1 and the halfwords of T are set: +146 = 64, +140 = 255,
//     +142 = 200, +144 = 135, +148 = 255, +150 = 200, +152 = 135, +154 = 64.
// Inferred: a one-shot holy hit that remembers it has struck and tints the target; the roll-twice pattern looks like a rounding macro
// whose argument has a side effect. Unknown: what the words at O+84, +104, +120, the halfword at +92, the byte at T+172 and the entry
// at 88 mean. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
int eRand__10GameObject(void* object);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);

void msg_collision__10HolyStrikeP10GameObject(void* self, void* other) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = static_cast<unsigned char*>(other);
    if (word(o, 120) != 0u) return;
    const unsigned q = word(o, 84);
    if (q != 0u) {
        if (call_entry(from_word(word(from_word(q), 12)), 160) == 0u) return;
    }
    const unsigned flags = word(t, 12);
    if (flags & 0x40u) {
        if (eRand__10GameObject(o) < 0) return;
    } else if (flags & 0x20u) {
        if (dword(t, 160) & 0x200ull) return;
    }
    unsigned char* const x = from_word(word(o, 104));
    unsigned char* const entry = from_word(word(x, 96)) + 88;
    const int n = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
        x + *reinterpret_cast<short*>(entry), static_cast<signed char>(t[172]) == 16 ? 1 : 0, half(o, 92));
    unsigned char description[80];
    w13a::init_description(description);
    const float f = static_cast<float>(n);
    const float K = bits(0x3e4ccccc);
    int amount;
    const float first = (f + (f * (static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u))) * K) + 0.5f;
    if (0.0f <= first) {
        amount = static_cast<int>((f + (f * (static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u))) * K) + 1.0f);
    } else {
        amount = static_cast<int>(((f + (f * (static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u))) * K) + 0.5f) - 0.5f);
    }
    const unsigned q2 = word(o, 84);
    unsigned char* const owner = q2 != 0u ? from_word(word(from_word(q2), 12)) : nullptr;
    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(t, owner, f, static_cast<float>(amount), 100000, 0, 0, 0x80000, description, false);
    word(o, 120) = 1;
    half(t, 146) = 64;
    half(t, 140) = 255;
    half(t, 142) = 200;
    half(t, 144) = 135;
    half(t, 148) = 255;
    half(t, 150) = 200;
    half(t, 152) = 135;
    half(t, 154) = 64;
}
}
