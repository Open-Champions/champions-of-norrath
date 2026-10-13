// Observed msg_collision__21PoisonArrowProjectileP10GameObject at 0x002a0638, 592 bytes. O is the projectile and X
// the object it touched (non-null). Names: Q = the word at O+76 (an object, the owner); nothing runs when it is zero.
// W = the word at Q+12; "flags" of an object = its word at +12; n = the signed word at O+104; S = the object at word
// O+172; E(k) = the entry at table byte k of the table whose address is the word at S+96, called on S (adjusted by the
// entry's signed halfword) with the arguments listed. Steps in order:
//  1. s = E(112)(0, n, O+76); a roll r = eRand__10GameObject(O); the upper amount u = s + (int)(float(s) * 0.2 *
//     (float(r) * 2^-16)), the 0.2 being the float 0x3e4ccccc read through the global pointer.
//  2. When W's flags have 0x40 (a player): return when X's flags have 0x40 or 0x01000000, or have 0x20 and the 64-bit
//     word at X+160 has bit 0x04000000. With p the word at W+960 and R the player record at player + 23984p,
//     gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(X, W, float(word R+212 + s), float(word R+216 + s),
//     100000, 0, 0, F, null, false) runs, F = 0x10040 for n below 11, 0x50040 for 11 to 15, 0x90040 for 16 and more.
//  3. Otherwise the entry at table byte 160 of W's table is called on W with X; a zero result returns. Then
//     gameAttackResolver(X, W, float(s), float(u), 100000, 0, 0, 0x10040, null, false) runs.
//  4. When X's flags (read again) have 0x20: v = E(88)(3, n, S) (the fourth argument is S itself, not adjusted) and the
//     float at X+868 becomes float(v). Then, for every path that got here, terminate__21PoisonArrowProjectilei(O, 2).
// Inferred: a poison arrow hit; the float at X+868 is likely a poison amount (by the class name only). Unknown: what S
// is, the flag bits and the roles of the offsets. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char player[];
unsigned eRand__10GameObject(void* self);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
void terminate__21PoisonArrowProjectilei(void* self, int hit);

void msg_collision__21PoisonArrowProjectileP10GameObject(void* self, void* other) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const x = static_cast<unsigned char*>(other);
    if (word(o, 76) == 0u) return;
    const int s = static_cast<int>(call_entry_from(from_word(word(o, 172)), 96, 112, 0, word(o, 104), to_word(o + 76)));
    const unsigned roll = eRand__10GameObject(o);
    const int upper = s + static_cast<int>(static_cast<float>(s) * bits(0x3e4cccccu) * (static_cast<float>(static_cast<int>(roll)) * bits(0x37800000u)));
    unsigned char* const w = from_word(word(from_word(word(o, 76)), 12));
    if (word(w, 12) & 0x40u) {
        const unsigned flags = word(x, 12);
        if (flags & 0x01000040u) return;
        if ((flags & 0x20u) && (dword(x, 160) & 0x04000000ull)) return;
        const int n = sword(o, 104);
        int attack = 0x10040;
        if (n >= 11) attack = 0x50040;
        if (n >= 16) attack = 0x90040;
        unsigned char* const record = player + 208 + sword(w, 960) * 23984;
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, from_word(word(from_word(word(o, 76)), 12)), static_cast<float>(sword(record, 4) + s),
                                                                         static_cast<float>(sword(record, 8) + s), 100000, 0, 0, attack, nullptr, false);
    } else {
        if (call_entry(w, 160, to_word(x)) == 0u) return;
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, from_word(word(from_word(word(o, 76)), 12)), static_cast<float>(s), static_cast<float>(upper),
                                                                         100000, 0, 0, 0x10040, nullptr, false);
    }
    if (word(x, 12) & 0x20u) {
        const int v = static_cast<int>(call_entry_from(from_word(word(o, 172)), 96, 88, 3, word(o, 104), word(o, 172)));
        real(x, 868) = static_cast<float>(v);
    }
    terminate__21PoisonArrowProjectilei(o, 2);
}
}
