// Observed init__4Propi at 0x00147a48, 996 bytes. flags is the second argument. In order:
//   1. word +12 |= 0x1809, then objectUpdateInGrid__FP10GameObject(object); float +28 = 4.0;
//   2. flags & 0x100: halfword +320 |= 2 and word +12 |= 0x4000; flags & 0x200: halfword +320 |= 4;
//   3. random draws come from the per-object generator at word +68 (state = state * 1664525 + 1013904223;
//      a draw is (float)(state >> 16) * 2^-16). flags & 1: two draws, (d - 0.5) * 124.0 into floats
//      +252 then +256. flags & 8: the same with 224.0. flags & 0x10: two draws (d - 0.5) * 324.0 into
//      +252 then +256 and a third draw d * 480.0 into float +48. flags & 0x20: one draw d * 30.0 + 130.0 into
//      +48. flags & 0x40: one draw d * 60.0 + 260.0 into +48;
//   4. flags & 2: halfword +268 = 2000; then objectAddToRunList__FP10GameObject(object);
//   5. halfword +270 = 32767 when flags & 0x8000 else 180;
//   6. flags & 0x2000: halfword +320 |= 8, else flags & 0x4000: halfword +320 |= 0x10; flags & 0x10000:
//      halfword +320 |= 0x20;
//   7. flags & 0x80: halfword +270 = 0, halfword +320 |= 1, and when bit 1 of that halfword is clear,
//      objectRemoveFromRunList__FP10GameObject(object);
//   8. when bit 0x100 of levelFlags (read after step 7) is set: float +48 is clamped to at most 400.0 and
//      then multiplied by 0.25; floats +252 and +256 are halved.
// Inferred from names only: Prop initialisation, +252/+256 horizontal velocity, +48 an upward or lifetime
// value. Unknown: the meaning of the individual flag bits and of the +320 bits. Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern int levelFlags;
void objectUpdateInGrid__FP10GameObject(void* object);
void objectAddToRunList__FP10GameObject(void* object);
void objectRemoveFromRunList__FP10GameObject(void* object);
}

namespace {
// One draw of the per-object generator at word +68.
inline float draw(unsigned char* o) {
    unsigned state = w11b::word(o, 68) * 0x19660du + 0x3c6ef35fu;
    w11b::word(o, 68) = state;
    return static_cast<float>(state >> 16) * 1.52587890625e-05f;
}
}

extern "C" void init__4Propi(void* self, int flags) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    word(o, 12) |= 0x1809;
    objectUpdateInGrid__FP10GameObject(o);
    real(o, 28) = 4.0f;
    if (flags & 0x100) {
        half(o, 320) |= 2;
        word(o, 12) |= 0x4000;
    }
    if (flags & 0x200) half(o, 320) |= 4;
    if (flags & 1) {
        real(o, 252) = (draw(o) - 0.5f) * 124.0f;
        real(o, 256) = (draw(o) - 0.5f) * 124.0f;
    }
    if (flags & 8) {
        real(o, 252) = (draw(o) - 0.5f) * 224.0f;
        real(o, 256) = (draw(o) - 0.5f) * 224.0f;
    }
    if (flags & 0x10) {
        real(o, 252) = (draw(o) - 0.5f) * 324.0f;
        real(o, 256) = (draw(o) - 0.5f) * 324.0f;
        real(o, 48) = draw(o) * 480.0f;
    }
    if (flags & 0x20) real(o, 48) = draw(o) * 30.0f + 130.0f;
    if (flags & 0x40) real(o, 48) = draw(o) * 60.0f + 260.0f;
    if (flags & 2) half(o, 268) = 2000;
    objectAddToRunList__FP10GameObject(o);
    half(o, 270) = (flags & 0x8000) ? 32767 : 180;
    if (flags & 0x2000) half(o, 320) |= 8;
    else if (flags & 0x4000) half(o, 320) |= 0x10;
    if (flags & 0x10000) half(o, 320) |= 0x20;
    if (flags & 0x80) {
        half(o, 270) = 0;
        half(o, 320) |= 1;
        if (!(half(o, 320) & 2)) objectRemoveFromRunList__FP10GameObject(o);
    }
    if (levelFlags & 0x100) {
        float up = real(o, 48);
        if (up > 400.0f) up = 400.0f;
        real(o, 48) = up * 0.25f;
        real(o, 252) *= 0.5f;
        real(o, 256) *= 0.5f;
    }
}
