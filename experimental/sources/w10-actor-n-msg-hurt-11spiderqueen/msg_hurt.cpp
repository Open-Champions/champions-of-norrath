// Observed msg_hurt__11SpiderQueenfP10GameObjecti at 0x0020cd60, 464 bytes.
// When the signed byte at object+570 is non-zero the function returns at once. Otherwise:
//  1. It calls the Creature version, msg_hurt__8CreaturefP10GameObjecti, with the same arguments.
//  2. For i = 2 .. 7 (a threshold of 180 - 20*i): if the float at +560 is less than or equal to
//     that threshold and the word at +1028 + 4*i is zero, it stores the word at +808 plus 20 into
//     that slot, calls modelGetCharAttachmentPos(model R[12] where R is the block at +36, a copy
//     of the point at +16, the halfword at +56, the animation state at +288, i, two output
//     points, the float at +224), calls addEffect(3, first output point), and then ORs a
//     constant selected by i into the word at +244:
//       i=2 0x08021041, i=3 0x00042082, i=4 0x00184104, i=5 0x10208208, i=6 0x03810410,
//       i=7 0x01400820.
//  3. It stores modelGetActiveChunks(R[12], word at +244) as a 64-bit value at +552, copies the
//     7-byte text "Shelox" (with terminator) into bossMeterName, and stores the float at +560
//     converted to an integer into bossMeterCurrent.
// The Creature call's return value is not used; this function returns nothing.
// Unknown: what the slots at +1032.., the word at +808 and the masks mean. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* source, int kind);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int index, Point3* out1, Point3* out2, float scale);
void addEffect__FiG6Point3(int kind, Point3 position);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);

void msg_hurt__11SpiderQueenfP10GameObjecti(void* self, float amount, void* source, int kind) {
    using namespace w10n;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (static_cast<signed char>(o[570]) != 0) return;
    msg_hurt__8CreaturefP10GameObjecti(self, amount, source, kind);
    for (int i = 2; i < 8; ++i) {
        if (!(real(o, 560) <= static_cast<float>(180 - 20 * i))) continue;
        unsigned& slot = word(o, 1028 + 4 * i);
        if (slot != 0u) continue;
        slot = word(o, 808) + 20u;
        Point3 first, second;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            from_word(word(from_word(word(o, 36)), 12)), *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 288, i, &first, &second, real(o, 224));
        addEffect__FiG6Point3(3, first);
        static const unsigned masks[6] = {0x08021041u, 0x00042082u, 0x00184104u, 0x10208208u, 0x03810410u, 0x01400820u};
        word(o, 244) |= masks[i - 2];
    }
    dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(from_word(word(o, 36)), 12)), static_cast<int>(word(o, 244)));
    __builtin_memcpy(bossMeterName, "Shelox", 7);
    bossMeterCurrent = static_cast<int>(real(o, 560));
}
}
