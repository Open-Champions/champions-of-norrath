// Observed msg_collision__19ColdArrowProjectileP10GameObject at 0x0029ef98, 976 bytes. O is the projectile and X
// the object it touched (non-null). Names: Q = the word at O+76 (an object, the owner); nothing runs when it is zero.
// W = the word at Q+12; "flags" of an object = its word at +12; n = the signed word at O+104; S = the object at word
// O+172; E(k) = the entry at table byte k of the table whose address is the word at S+96, called on S (adjusted by the
// entry's signed halfword) with the arguments listed. D is a local item description of the shape the FireArrow handler
// builds (halfwords at +16..+22 and +24..+30 zeroed, bytes at +32 and +33 = -1, four 8-byte slots at +36 starting -1).
//  A. When W's flags have 0x40 (a player): return when X's flags have 0x40 or 0x01000000, or have 0x20 and the 64-bit
//     word at X+160 has bit 0x04000000. addEffect__FiG6Point3(10, O+16) runs. F = 0x40 for n below 11, 0x40040 for 11
//     to 15, 0x80040 for 16 and more. a = E(112)(2, n, O+76) and b = E(112)(2, n, O+76) are called in this order; the
//     first free slot of D (or one holding 32) becomes {32, 100, a, b} (halfwords +0, +2, +4, +6). With p the word at
//     W+960 and R the player record at player + 23984p, gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
//     X, W, float(word R+212), float(word R+216), 100000, 0, 0, F, D, false) runs.
//  B. Otherwise the entry at table byte 160 of W's table is called on W with X; a zero result returns. Then
//     addEffect__FiG6Point3(10, O+16); s = E(112)(0, n, O+76); a roll r = eRand__10GameObject(O); the upper amount is
//     s + (int)(float(s) * 0.2 * (float(r) * 2^-16)); and gameAttackResolver(X, W, float(s), float(upper), 100000, 0,
//     0, 64, null, false) runs.
//  C. When X's flags (read again) have 0x20: u = E(88)(3, n, S) (the fourth argument is S itself, not adjusted) and
//     the float at X+860 gets float(u / 2) added (u / 2 truncated toward zero). Then, for every path that got here,
//     terminate__19ColdArrowProjectilei(O, 2) is called.
// Inferred: a cold arrow hit that damages the target and, on a flagged target, adds to a float at +860 (a chill
// amount, by the name only). Unknown: what S is, the flag bits, the slot values and the float at X+860.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char player[];
unsigned eRand__10GameObject(void* self);
void addEffect__FiG6Point3(int effect, Point3 position);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
void terminate__19ColdArrowProjectilei(void* self, int hit);

void msg_collision__19ColdArrowProjectileP10GameObject(void* self, void* other) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const x = static_cast<unsigned char*>(other);
    if (word(o, 76) == 0u) return;
    unsigned char description[80];
    for (int k = 0; k < 4; ++k) half(description, 60 - 8 * k) = -1;
    description[33] = 0xff;
    description[32] = 0xff;
    for (int k = 0; k < 4; ++k) {
        half(description, 16 + 2 * k) = 0;
        half(description, 24 + 2 * k) = 0;
    }
    unsigned char* const w = from_word(word(from_word(word(o, 76)), 12));
    if (word(w, 12) & 0x40u) {
        const unsigned flags = word(x, 12);
        if (flags & 0x01000040u) return;
        if ((flags & 0x20u) && (dword(x, 160) & 0x04000000ull)) return;
        addEffect__FiG6Point3(10, *reinterpret_cast<Point3*>(o + 16));
        const int n = sword(o, 104);
        int attack = 0x40;
        if (n >= 16) attack = 0x80040;
        else if (n >= 11) attack = 0x40040;
        const unsigned a = call_entry_from(from_word(word(o, 172)), 96, 112, 2, word(o, 104), to_word(o + 76));
        const unsigned b = call_entry_from(from_word(word(o, 172)), 96, 112, 2, word(o, 104), to_word(o + 76));
        for (int k = 0; k < 4; ++k) {
            unsigned char* const slot = description + 36 + 8 * k;
            if (half(slot, 0) == -1 || half(slot, 0) == 32) {
                half(slot, 0) = 32;
                half(slot, 6) = static_cast<short>(b);
                half(slot, 2) = 100;
                half(slot, 4) = static_cast<short>(a);
                break;
            }
        }
        unsigned char* const record = player + 208 + sword(w, 960) * 23984;
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, from_word(word(from_word(word(o, 76)), 12)), static_cast<float>(sword(record, 4)),
                                                                         static_cast<float>(sword(record, 8)), 100000, 0, 0, attack, description, false);
    } else {
        if (call_entry(w, 160, to_word(x)) == 0u) return;
        addEffect__FiG6Point3(10, *reinterpret_cast<Point3*>(o + 16));
        const int s = static_cast<int>(call_entry_from(from_word(word(o, 172)), 96, 112, 0, word(o, 104), to_word(o + 76)));
        const unsigned roll = eRand__10GameObject(o);
        const int upper = s + static_cast<int>(static_cast<float>(s) * bits(0x3e4cccccu) * (static_cast<float>(static_cast<int>(roll)) * bits(0x37800000u)));
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, from_word(word(from_word(word(o, 76)), 12)), static_cast<float>(s), static_cast<float>(upper),
                                                                         100000, 0, 0, 64, nullptr, false);
    }
    if (word(x, 12) & 0x20u) {
        const int u = static_cast<int>(call_entry_from(from_word(word(o, 172)), 96, 88, 3, word(o, 104), word(o, 172)));
        real(x, 860) = real(x, 860) + static_cast<float>(u / 2);
    }
    terminate__19ColdArrowProjectilei(o, 2);
}
}
