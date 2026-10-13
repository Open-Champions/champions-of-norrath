// Observed msg_collision__19MultiFireProjectileP10GameObject at 0x002a16f8, 792 bytes. O is the projectile and X
// the object it touched (non-null). Names: Q = the word at O+76 (an object, the owner); nothing runs when it is zero.
// W = the word at Q+12; "flags" of an object = its word at +12; n = the signed word at O+104; S = the object at word
// O+112; E(k) = the entry at table byte k of the table whose address is the word at S+96, called on S (adjusted by the
// entry's signed halfword) with the arguments listed. round(v) = (int)(v + 0.5) for v >= 0.0, else (int)(v - 0.5).
//  A. When W's flags have 0x40 (a player): return when X's flags have 0x40 or 0x01000000, or have 0x20 and the 64-bit
//     word at X+160 has bit 0x04000000. s = E(112)(0, n, O+76); v = E(88)(2, n, S) (the fourth argument is S itself,
//     not adjusted); k = float(s) * 0.01 / float(v). With p the word at W+960 and R the player record at
//     player + 23984p: a = round(float(word R+212) * k) and b = round(float(word R+216) * k) (the record is
//     looked up again through Q for the second one). Then
//     gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(X, W, float(a), float(b), 100000, 0, 0,
//     0x40000, null, false) runs.
//  B. Otherwise the entry at table byte 160 of W's table is called on W with X; a zero result returns. s = E(112)(0,
//     n, O+76); k = float(s) * 0.2 (the float 0x3e4ccccc); a first roll r1 = eRand__10GameObject(O) and a second
//     r2 = eRand__10GameObject(O) are drawn; the bonus is (int)(k * (float(r2) * 2^-16) + 0.5) when
//     k * (float(r1) * 2^-16) >= 0.0, else (int)(k * (float(r2) * 2^-16) - 0.5); and
//     gameAttackResolver(X, W, float(s), float(s + bonus), 100000, 0, 0, 0x40000, null, false) runs.
//  C. In both cases terminate__19MultiFireProjectilei(O, 2) is then called.
// Inferred: a multi-shot fire arrow hit. Unknown: what S is, the flag bits and the roles of the record words.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char player[];
unsigned eRand__10GameObject(void* self);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
void terminate__19MultiFireProjectilei(void* self, int hit);

namespace {
int round_half_away(float v) {
    if (0.0f <= v) return static_cast<int>(v + 0.5f);
    return static_cast<int>(v - 0.5f);
}
}  // namespace

void msg_collision__19MultiFireProjectileP10GameObject(void* self, void* other) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const x = static_cast<unsigned char*>(other);
    if (word(o, 76) == 0u) return;
    unsigned char* const w = from_word(word(from_word(word(o, 76)), 12));
    if (word(w, 12) & 0x40u) {
        const unsigned flags = word(x, 12);
        if (flags & 0x01000040u) return;
        if ((flags & 0x20u) && (dword(x, 160) & 0x04000000ull)) return;
        const int s = static_cast<int>(call_entry_from(from_word(word(o, 112)), 96, 112, 0, word(o, 104), to_word(o + 76)));
        float k = static_cast<float>(s) * bits(0x3c23d70au);
        const int v = static_cast<int>(call_entry_from(from_word(word(o, 112)), 96, 88, 2, word(o, 104), word(o, 112)));
        k = k / static_cast<float>(v);
        unsigned char* const record = player + 208 + sword(from_word(word(from_word(word(o, 76)), 12)), 960) * 23984;
        const int a = round_half_away(static_cast<float>(sword(record, 4)) * k);
        unsigned char* const record2 = player + 208 + sword(from_word(word(from_word(word(o, 76)), 12)), 960) * 23984;
        const int b = round_half_away(static_cast<float>(sword(record2, 8)) * k);
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, from_word(word(from_word(word(o, 76)), 12)), static_cast<float>(a), static_cast<float>(b), 100000,
                                                                         0, 0, 0x40000, nullptr, false);
    } else {
        if (call_entry(w, 160, to_word(x)) == 0u) return;
        const int s = static_cast<int>(call_entry_from(from_word(word(o, 112)), 96, 112, 0, word(o, 104), to_word(o + 76)));
        const float k = static_cast<float>(s) * bits(0x3e4cccccu);
        const unsigned r1 = eRand__10GameObject(o);
        int bonus;
        if (0.0f <= k * (static_cast<float>(static_cast<int>(r1)) * bits(0x37800000u))) {
            const unsigned r2 = eRand__10GameObject(o);
            bonus = static_cast<int>(k * (static_cast<float>(static_cast<int>(r2)) * bits(0x37800000u)) + 0.5f);
        } else {
            const unsigned r2 = eRand__10GameObject(o);
            bonus = static_cast<int>(k * (static_cast<float>(static_cast<int>(r2)) * bits(0x37800000u)) - 0.5f);
        }
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, from_word(word(from_word(word(o, 76)), 12)), static_cast<float>(s), static_cast<float>(s + bonus),
                                                                         100000, 0, 0, 0x40000, nullptr, false);
    }
    terminate__19MultiFireProjectilei(o, 2);
}
}
