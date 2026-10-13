// Observed msg_trigger__9MummyKingi at 0x0023ad30, 348 bytes. Let R be the block at the word object+36 and
// P the point at +16. The argument selects:
//  1: bit 26 of the 64-bit word at +160 and bit 24 of the word at +12 are cleared; the ten bytes
//     "MummyKing" plus terminator go to bossMeterName (0x0034b988); the float at +560 is truncated into
//     bossMeterCurrent (0x00483f48); soundResetStream(0); soundStartMusic("mx_boss_act3_mumking_l.vag", 259).
//  0: with the word at +960 non-zero, fxWeaponToDust(R[12], the word at R+116, animation state at +288, copy of P,
//     halfword +56, float +224, 0.0, 0) runs and +960 becomes 0. With +960 zero it becomes 14 and
//     fxWeaponToSparkles takes the same arguments.
//  any other value: nothing.
// Meaning of the values is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void soundResetStream__Fi(int stream);
void soundStartMusic__FPci(char* name, int value);
void fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(void* model, void* model2, void* animation, Point3 position, short a, float f, float g, int h);
void fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(void* model, void* model2, void* animation, Point3 position, short a, float f, float g, int h);

void msg_trigger__9MummyKingi(void* self, int value) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (value == 1) {
        dword(o, 160) &= ~0x04000000ull;
        word(o, 12) &= ~0x01000000u;
        __builtin_memcpy(bossMeterName, "MummyKing", 10);
        bossMeterCurrent = static_cast<int>(real(o, 560));
        soundResetStream__Fi(0);
        soundStartMusic__FPci(const_cast<char*>("mx_boss_act3_mumking_l.vag"), 259);
    } else if (value == 0) {
        unsigned char* const block = from_word(word(o, 36));
        if (word(o, 960) != 0u) {
            fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(
                from_word(word(block, 12)), from_word(word(block, 116)), o + 288, *reinterpret_cast<Point3*>(o + 16), half(o, 56), real(o, 224), 0.0f, 0);
            word(o, 960) = 0;
        } else {
            word(o, 960) = 14;
            fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(
                from_word(word(block, 12)), from_word(word(block, 116)), o + 288, *reinterpret_cast<Point3*>(o + 16), half(o, 56), real(o, 224), 0.0f, 0);
        }
    }
}
}
