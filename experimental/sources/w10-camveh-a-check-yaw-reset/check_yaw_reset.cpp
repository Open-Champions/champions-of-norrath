// Observed checkYawReset__6Camera at 0x0026ed68, 296 bytes. Nothing is
// returned. Nothing here was executed on the EE.
//
// Returns at once when levelFlags (0x004844c0) has bit 0x2000000 set, when the
// byte at camera+352 is nonzero, or when the word at camera+100 or the word at
// camera+56 is nonzero. Let i be the word at camera+64. The 88-byte pad entry
// i (table at 0x0034beb8, flags word at +12) must have bit 0x200 set, and its
// floats at +20 and +24 must satisfy x*x + y*y < 0.032 (the float with bits
// 0x3d03126e), otherwise nothing happens.
// Then the halfword at camera+138 is chosen: if the 23984-byte player entry i
// has a pointer at +21872, it is 16384 minus the unsigned halfword at +56 of
// that pointer's target. Otherwise the signed byte at camera+353 is read: a
// negative value gives -8192; a nonnegative value j looks at the pointer at
// +21872 of player entry j and gives 16384 minus that halfword, or -8192 when
// that pointer is null. Finally the word at camera+56 receives
// fastestRotDirection__FssPi(halfword at +104, halfword at +138, null).
// What these fields mean is not established here.
extern "C" {
extern unsigned levelFlags;
extern unsigned char pad[];
extern unsigned char player[];
int fastestRotDirection__FssPi(short from, short to, int* amount);
}

typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;
typedef float __attribute__((may_alias)) AliasedFloat;
typedef unsigned char* __attribute__((may_alias)) AliasedPtr;

extern "C" void checkYawReset__6Camera(unsigned char* camera) {
    if (levelFlags & 0x2000000) return;
    if (*reinterpret_cast<signed char*>(camera + 352) != 0) return;
    if (*reinterpret_cast<AliasedInt*>(camera + 100) != 0) return;
    if (*reinterpret_cast<AliasedInt*>(camera + 56) != 0) return;
    const int i = *reinterpret_cast<AliasedInt*>(camera + 64);
    const unsigned char* entry = pad + 88 * i;
    if ((*reinterpret_cast<const AliasedInt*>(entry + 12) & 0x200) == 0) return;
    const float x = *reinterpret_cast<const AliasedFloat*>(entry + 20);
    const float y = *reinterpret_cast<const AliasedFloat*>(entry + 24);
    if (!(x * x + y * y < 0.03199999779462814f)) return;

    int yaw;
    unsigned char* target = *reinterpret_cast<AliasedPtr*>(player + 23984 * i + 21872);
    if (target != nullptr) {
        yaw = 16384 - *reinterpret_cast<AliasedUShort*>(target + 56);
    } else {
        const int j = *reinterpret_cast<signed char*>(camera + 353);
        if (j < 0) {
            yaw = -8192;
        } else {
            unsigned char* other = *reinterpret_cast<AliasedPtr*>(player + 23984 * j + 21872);
            yaw = other != nullptr ? 16384 - *reinterpret_cast<AliasedUShort*>(other + 56) : -8192;
        }
    }
    *reinterpret_cast<AliasedShort*>(camera + 138) = static_cast<short>(yaw);
    *reinterpret_cast<AliasedInt*>(camera + 56) =
        fastestRotDirection__FssPi(*reinterpret_cast<AliasedShort*>(camera + 104),
                                   *reinterpret_cast<AliasedShort*>(camera + 138), nullptr);
}
