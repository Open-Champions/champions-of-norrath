// Observed msg_collisionWorld__6GoblinG6Point3 at 0x0022f1d8, 100 bytes.
// The position argument is copied but never used. When the signed halfword at +572 is 8 and bit 5
// (0x20) of the halfword at +294 is clear, a one-shot animation is added to the member at +288:
// header = the word at (table word +36) + 172, both float arguments the single-precision constant
// with bits 0x3dcccccc (just under 0.1), flags 32.
// Inferred: the table at +36 holds animation header pointers. Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
}

extern "C" void msg_collisionWorld__6GoblinG6Point3(void* self, Point3) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<short*>(object + 572) != 8) return;
    if ((*reinterpret_cast<unsigned short*>(object + 294) & 0x20u) != 0) return;
    unsigned char* table = w10g::from_word(w10g::word_at(self, 36));
    void* header = w10g::from_word(*reinterpret_cast<unsigned*>(table + 172));
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(object + 288, header, 0x1.999998p-4f, 0x1.999998p-4f, 32);
}
