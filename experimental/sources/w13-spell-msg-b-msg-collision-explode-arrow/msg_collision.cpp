// Observed msg_collision__22ExplodeArrowProjectileP10GameObject at 0x002a2158, 556 bytes. O is the projectile and X
// the second argument (only passed on to one call). Names: Q = the word at O+76 (an object, the owner), W = the word
// at Q+12; "flags" of an object = its word at +12; n = the signed word at O+104; S = the object at word O+112;
// E(k) = the entry at table byte k of the table whose address is the word at S+96, called on S (adjusted by the
// entry's signed halfword) with the arguments listed. Steps in order:
//  1. Nothing happens when Q is zero or when W's flags lack 0x20. The entry at table byte 160 of W's table is called
//     on W with X; a zero result returns without any further effect.
//  2. R = E(104)(2, n, W), read as an int; F = 0 for n below 6, 0x40000 for 6 to 11, 0x80000 for 12 and more.
//  3. The objects in the box O+16 -/+ float(R) on each axis are found with
//     objectFindInBox__FG6Point3T0PP10GameObjecti (capacity 512, a local list ended by a null word).
//  4. For each listed object T, in order, when the entry at table byte 160 of W's table (W looked up again through Q)
//     called on W with T gives a non-zero result: b = E(104)(0, n, W) as an int; the roll r = eRand__10GameObject(O);
//     the upper amount is (int)(float(b) + float(b) * 0.2 * (float(r) * 2^-16)), the 0.2 being the float 0x3e4ccccc;
//     and gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(T, W, float(b), float(upper), 100000, 0, 0,
//     F, null, false) runs.
//  5. terminate__22ExplodeArrowProjectilei(O, 2) is then called, also when the list was empty.
// Inferred: an explosive arrow hit damaging everything in a radius. Unknown: what S is, what the flag bit 0x20 and the
// result of the 160 entry mean, and what R measures. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
unsigned eRand__10GameObject(void* self);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
void terminate__22ExplodeArrowProjectilei(void* self, int hit);

void msg_collision__22ExplodeArrowProjectileP10GameObject(void* self, void* other) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 76) == 0u) return;
    unsigned char* const w = from_word(word(from_word(word(o, 76)), 12));
    if (!(word(w, 12) & 0x20u)) return;
    if (call_entry(w, 160, to_word(static_cast<unsigned char*>(other))) == 0u) return;
    int attack = 0;
    const int radius = static_cast<int>(call_entry_from(from_word(word(o, 112)), 96, 104, 2, word(o, 104), word(from_word(word(o, 76)), 12)));
    if (sword(o, 104) >= 6) {
        attack = 0x40000;
        if (sword(o, 104) >= 12) attack = 0x80000;
    }
    const float r = static_cast<float>(radius);
    Point3 low, high;
    low.x = real(o, 16) - r;
    low.y = real(o, 20) - r;
    low.z = real(o, 24) - r;
    high.x = real(o, 16) + r;
    high.y = real(o, 20) + r;
    high.z = real(o, 24) + r;
    void* found[512];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 512);
    if (found[0] != nullptr) {
        void** cursor = found;
        do {
            unsigned char* const target = static_cast<unsigned char*>(*cursor);
            ++cursor;
            unsigned char* const owner = from_word(word(from_word(word(o, 76)), 12));
            if (call_entry(owner, 160, to_word(target)) != 0u) {
                const int b = static_cast<int>(call_entry_from(from_word(word(o, 112)), 96, 104, 0, word(o, 104), word(from_word(word(o, 76)), 12)));
                float amount = static_cast<float>(b);
                const unsigned roll = eRand__10GameObject(o);
                const float low_amount = amount;
                amount = amount + amount * bits(0x3e4cccccu) * (static_cast<float>(static_cast<int>(roll)) * bits(0x37800000u));
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(target, from_word(word(from_word(word(o, 76)), 12)), low_amount, static_cast<float>(static_cast<int>(amount)),
                                                                                 100000, 0, 0, attack, nullptr, false);
            }
        } while (*cursor != nullptr);
    }
    terminate__22ExplodeArrowProjectilei(o, 2);
}
}
