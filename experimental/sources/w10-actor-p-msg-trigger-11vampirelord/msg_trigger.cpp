// Observed msg_trigger__11VampireLordi at 0x00242b68, 548 bytes. The integer argument selects:
//  - 1: copies the 12-byte text "VampireLord" into bossMeterName, stores the floats at object+564 and
//    +560 converted to integers into bossMeterMax and bossMeterCurrent, calls soundResetStream__Fi(0)
//    and soundStartMusic__FPci("mx_boss_act2_vamplord_l.vag", 259).
//  - 0, when bit 3 of the byte at +570 is set: stores 22 in the halfword at +572, copies the word at
//    +808 to +1024, clears the word at +968, then six times (i = 0..5): reads the halfword at +56
//    (unsigned), draws r = eRand__10GameObject(this), takes
//    angle = (((yaw + 5461*i - 16383) & 0xffff) + r % 12000 - 6000) & 0xffff, and places a point at
//    (icos(angle)*240 + x, isin(angle)*240 + y, float(+32)*0.5 + z) where (x, y, z) is the point at +16.
//    It allocates 1088 bytes with blockAlloc__Fi and, if that succeeded, constructs a Critter
//    (__7CritterPcG6Point3sPPc) in it with the text "bat", that point, the short angle + 0x801f and
//    the pointer table batParams, then links it with listOnewayLink__FP8ListHeadT0Pv(object+1028,
//    critter, critter). After the six, animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi
//    runs on the animation state at +288 with the header at offset 148 of the block that the word at
//    +36 points to, rate 0.1 and flags 3.
//  - 0, bit 3 clear: sets the word at +1020 to 1 and the word at +1016 to the word at +808 plus 120.
//  - any other value: nothing.
// Inferred: six spawned bats (from the "bat" text and the loop); +560/+564 as current and maximum
// boss meter values. Unknown: what +568..572, +808, +968 and +1016..1024 hold; the 0.1 is read as a
// literal because the original holds it as an unnamed constant. Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
extern int bossMeterMax;
extern char* batParams[];
void soundResetStream__Fi(int stream);
void soundStartMusic__FPci(char* name, int id);
unsigned eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int size);
void critter_construct(void* self, char* name, Point3 position, short yaw, char** arguments) __asm__("__7CritterPcG6Point3sPPc");
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
}

extern "C" void msg_trigger__11VampireLordi(void* self, int message) {
    using namespace w10p;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (message == 1) {
        __builtin_memcpy(bossMeterName, "VampireLord", 12);
        bossMeterMax = static_cast<int>(real(o, 564));
        bossMeterCurrent = static_cast<int>(real(o, 560));
        soundResetStream__Fi(0);
        soundStartMusic__FPci(const_cast<char*>("mx_boss_act2_vamplord_l.vag"), 259);
        return;
    }
    if (message != 0) return;
    if ((o[570] & 8) == 0) {
        word(o, 1020) = 1u;
        word(o, 1016) = word(o, 808) + 120u;
        return;
    }
    half(o, 572) = 22;
    word(o, 1024) = word(o, 808);
    word(o, 968) = 0u;
    for (int i = 0; i < 6; ++i) {
        const unsigned yaw = static_cast<unsigned short>(half(o, 56));
        const unsigned roll = eRand__10GameObject(self);
        const unsigned angle = (((yaw + 5461u * static_cast<unsigned>(i) - 16383u) & 0xffffu) + roll % 12000u - 6000u) & 0xffffu;
        const float across = icos__Fi(static_cast<int>(angle)) * 240.0f;
        const float along = isin__Fi(static_cast<int>(angle)) * 240.0f;
        const Point3& here = *reinterpret_cast<const Point3*>(o + 16);
        const Point3 spot = {across + here.x, along + here.y, real(o, 32) * 0.5f + here.z};
        void* const critter = blockAlloc__Fi(1088);
        if (critter != nullptr) critter_construct(critter, const_cast<char*>("bat"), spot, static_cast<short>(angle + 0x801fu), batParams);
        if (critter != nullptr) listOnewayLink__FP8ListHeadT0Pv(o + 1028, critter, critter);
    }
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, from_word(word(from_word(word(o, 36)), 148)), 0.1f, 3);
}
