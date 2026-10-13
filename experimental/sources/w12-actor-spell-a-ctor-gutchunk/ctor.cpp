// Observed __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif at 0x001e5190, 160 bytes. This is the
// overload with a model header and an animation state, not the one at 0x001e5048. Arguments: the object, a
// position P (by value), a short yaw, a model header, an animation state, three char pointers A, B, C, a long N, an
// int F, an int S (selector) and a float (passed on). Returns nothing. Steps, in order:
//  1. The PhProp constructor at 0x00148d08 (model and animation placement overload) runs with (object, P, yaw,
//     model, animation, A, B, C, N, F, a null description, the float).
//  2. S goes to the word at +584, _vt$8GutChunk to the word at +72, the 12 bytes at +16 are copied to +592,
//     the word at +604 becomes 2 and the byte at +608 becomes 0.
// Observed: the image leaves 2 in the result register; that is a leftover, not a result.
// Unknown: what S, N and F mean. Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_GutChunk[] __asm__("_vt$8GutChunk");
void __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf(void* self, Point3 position, short yaw, void* model,
                                                                                    void* animation, char* a, char* b, char* c, long n,
                                                                                    int flags, void* description, float scale);
void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 position, short yaw, void* model, void* animation,
                                                                   char* a, char* b, char* c, long n, int flags, int selector, float scale);
}

extern "C" void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 position, short yaw, void* model,
                                                                               void* animation, char* a, char* b, char* c, long n,
                                                                               int flags, int selector, float scale) {
    using namespace w11;
    __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf(self, position, yaw, model, animation, a, b, c, n, flags,
                                                                                    nullptr, scale);
    i32(self, 584) = selector;
    u32(self, 72) = word(vt_GutChunk);
    *reinterpret_cast<Point3*>(&u32(self, 592)) = *reinterpret_cast<Point3*>(&u32(self, 16));
    i32(self, 604) = 2;
    u8(self, 608) = 0;
}
