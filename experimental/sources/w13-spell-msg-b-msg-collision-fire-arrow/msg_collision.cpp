// Observed msg_collision__19FireArrowProjectileP10GameObject at 0x0029dea8, 1056 bytes. O is the projectile and X
// the object it touched (non-null). Names: Q = the word at O+76 (an object, the owner); nothing runs when it is zero.
// W = the word at Q+12 (the object it refers to); "flags" of an object = its word at +12; n = the signed word at O+108.
// D is a local item description of the same shape the FireBomblet handler builds: halfwords at +16..+22 and +24..+30
// zeroed, bytes at +32 and +33 set to -1 and four 8-byte slots at +36 whose first halfword is -1. Steps in order:
//  1. S = the object at word O+172. The entry at offset 112 of the table whose address is the word at S+96 is called
//     on S (adjusted by its signed halfword) with (0, n, O+76); the result is s. A roll r = eRand__10GameObject(O).
//     The upper amount is s + (int)(float(s) * 0.2 * (float(r) * 2^-16)).
//  2. When W's flags have 0x40 (a player), then: return when X's flags have 0x40 or 0x01000000, or have 0x20 and the
//     64-bit word at X+160 has bit 0x04000000. Else the first slot of D that is free (-1) or already 31 becomes
//     {31, 100, s, s} (halfwords at +0, +2, +4, +6). With p the word at W+960 and R the player record at
//     player + 23984p, gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(X, W, float(word R+212),
//     float(word R+216), 100000, 0, 0, F, D, false) runs, F = 0x60 for n below 6, 0x40060 for 6 to 10, 0x80060 for 11
//     to 15 and 0x200060 for 16 and more.
//  3. Otherwise the entry at offset 160 of W's table is called on W with X; a zero result returns. The first free
//     slot of D (or one equal to 21) becomes {21, 0, 0, 0}; gameAttackResolver(X, W, float(s), float(upper), 100000,
//     0, 0, 0x60, D, false) runs.
//  4. A StaticFire (1192 bytes from blockAlloc__Fi, constructed with a position, 300.0, an int and 0.5) is made,
//     when the allocation succeeded: for X's flags with 0x20 at X+16 with the int 60n + 120; else for flags with
//     0x200 at O+16 and the int 1020; else at X+16 and 1020. Then terminate__19FireArrowProjectilei(O, 2) is called.
// Inferred: a fire arrow hit; the skill call gives the damage. Unknown: what S is, the flag bits and what the slots hold.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char player[];
unsigned eRand__10GameObject(void* self);
void* blockAlloc__Fi(int size);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
void StaticFire_construct(void* self, Point3 where, float a, int b, float c) __asm__("__10StaticFireG6Point3fif");
void terminate__19FireArrowProjectilei(void* self, int hit);

void msg_collision__19FireArrowProjectileP10GameObject(void* self, void* other) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const x = static_cast<unsigned char*>(other);
    unsigned char description[80];
    for (int k = 0; k < 4; ++k) half(description, 60 - 8 * k) = -1;
    description[33] = 0xff;
    description[32] = 0xff;
    for (int k = 0; k < 4; ++k) {
        half(description, 16 + 2 * k) = 0;
        half(description, 24 + 2 * k) = 0;
    }
    if (word(o, 76) == 0u) return;
    unsigned char* const w = from_word(word(from_word(word(o, 76)), 12));
    const int s = static_cast<int>(call_entry_from(from_word(word(o, 172)), 96, 112, 0, word(o, 108), to_word(o + 76)));
    const unsigned roll = eRand__10GameObject(o);
    const int upper = s + static_cast<int>(static_cast<float>(s) * bits(0x3e4cccccu) * (static_cast<float>(static_cast<int>(roll)) * bits(0x37800000u)));
    if (word(w, 12) & 0x40u) {
        const unsigned flags = word(x, 12);
        if (flags & 0x01000040u) return;
        if ((flags & 0x20u) && (dword(x, 160) & 0x04000000ull)) return;
        const int n = sword(o, 108);
        int attack = 0x60;
        if (n >= 16) attack = 0x200060;
        else {
            if (n >= 6) attack = 0x40060;
            if (n >= 11) attack = 0x80060;
        }
        for (int k = 0; k < 4; ++k) {
            unsigned char* const slot = description + 36 + 8 * k;
            if (half(slot, 0) == -1 || half(slot, 0) == 31) {
                half(slot, 0) = 31;
                half(slot, 6) = static_cast<short>(s);
                half(slot, 2) = 100;
                half(slot, 4) = static_cast<short>(s);
                break;
            }
        }
        unsigned char* const record = player + 208 + sword(w, 960) * 23984;
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, w, static_cast<float>(sword(record, 4)), static_cast<float>(sword(record, 8)), 100000, 0, 0,
                                                                         attack, description, false);
    } else {
        if (call_entry(w, 160, to_word(x)) == 0u) return;
        for (int k = 0; k < 4; ++k) {
            unsigned char* const slot = description + 36 + 8 * k;
            if (half(slot, 0) == -1 || half(slot, 0) == 21) {
                half(slot, 0) = 21;
                half(slot, 6) = 0;
                half(slot, 2) = 0;
                half(slot, 4) = 0;
                break;
            }
        }
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, w, static_cast<float>(s), static_cast<float>(upper), 100000, 0, 0, 0x60, description, false);
    }
    const unsigned flags = word(x, 12);
    unsigned char* block = static_cast<unsigned char*>(blockAlloc__Fi(1192));
    if (flags & 0x20u) {
        if (block != nullptr) StaticFire_construct(block, *reinterpret_cast<Point3*>(x + 16), 300.0f, sword(o, 108) * 60 + 120, 0.5f);
    } else if (flags & 0x200u) {
        if (block != nullptr) StaticFire_construct(block, *reinterpret_cast<Point3*>(o + 16), 300.0f, 1020, 0.5f);
    } else {
        if (block != nullptr) StaticFire_construct(block, *reinterpret_cast<Point3*>(x + 16), 300.0f, 1020, 0.5f);
    }
    terminate__19FireArrowProjectilei(o, 2);
}
}
