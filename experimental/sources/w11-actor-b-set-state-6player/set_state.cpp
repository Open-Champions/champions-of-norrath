// Observed setState__6Playeri at 0x001a4d50, 296 bytes. state is the second argument. K is the signed
// byte at offset 23582 of record number (word at object+960) in the static array `player` (record stride
// 23984); M = 13*K; the resource record is the word at object+36, and "header(n)" below is the word at
// (resource record + 8*M + n). The animation state is the structure at object+288.
//   state 0: sets the word at +1400 to 0 and calls animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi
//            (object+288, header, 0.3, 3) where header is the word at (resource record + 36) when bit 28
//            (0x10000000) of the global levelFlags is set, otherwise header(68), also with 0.3.
//   state 1: stores 80.0 in the float at +176 and calls resetRouting__8Creature(object). When the word at
//            +1400 is not 1 it calls animAddTransitionTo (object+288, header(76), 0.1, 3). It then calls
//            animGlobalSync__FP14AnimationState(object+288) and sets the word at +1400 to 1.
//   any other state: nothing.
// Inferred: +1400 is the current player state (msg_collision tests it for 9); the 0 and 1 handled here
// look like idle and a second stance. Unknown: what K selects (it is not shown to be a class number).
// Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern unsigned char player[];
extern unsigned levelFlags;
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* animation, void* header, float blend, int mode);
void animGlobalSync__FP14AnimationState(void* animation);
void resetRouting__8Creature(void* self);

void setState__6Playeri(void* self, int state) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    const int kind = static_cast<signed char>(player[word(o, 960) * 23984u + 23582]);
    const int m = kind * 13;
    unsigned char* resource = from_word(word(o, 36));
    unsigned char* animation = o + 288;
    if (state == 0) {
        void* header;
        if (levelFlags & 0x10000000u) header = from_word(word(resource, 36));
        else header = from_word(word(resource + m * 8, 68));
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header, 0.3f, 3);
        word(o, 1400) = 0;
    } else if (state == 1) {
        real(o, 176) = 80.0f;
        resetRouting__8Creature(self);
        if (word(o, 1400) != 1u) {
            void* header = from_word(word(resource + m * 8, 76));
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header, 0.1f, 3);
        }
        animGlobalSync__FP14AnimationState(animation);
        word(o, 1400) = 1;
    }
}
}
