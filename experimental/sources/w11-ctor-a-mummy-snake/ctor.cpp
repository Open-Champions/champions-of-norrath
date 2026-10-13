// Observed __10MummySnakeG6Point3sP10GameObjectT3P15AnimationHeader at 0x23c238, 216 bytes.
// Arguments: the object, a three-float position (by value), a short (not read), two GameObject
// pointers (the second may be null) and an animation header pointer. Steps, in order:
//  1. The GameObject constructor runs on the object with the position, a null resource map and
//     a null fourth argument.
//  2. The word at +76 is cleared, the word at +72 gets _vt$10MummySnake, the header pointer is
//     stored at +96, and bits 0x9 are set in the word at +12.
//  3. The word at +80 is cleared (before the next call starts), then
//     listOnewayLink__FP8ListHeadT0Pv(object+76, first, first) runs unconditionally; when the second pointer is non-null the same call runs with (object+80,
//     second, second).
//  4. The word at +88 is set to frameNm + 360 and objectAddToRunList__FP10GameObject runs.
// Unknown: what the two linked objects are, what +88 means, and the short argument's purpose.
// Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_MummySnake[] __asm__("_vt$10MummySnake");
void __10MummySnakeG6Point3sP10GameObjectT3P15AnimationHeader(void* self, Point3 position, short heading,
                                                              void* first, void* second, void* header);
}

extern "C" void __10MummySnakeG6Point3sP10GameObjectT3P15AnimationHeader(void* self, Point3 position, short heading,
                                                                          void* first, void* second, void* header) {
    using namespace w11;
    (void)heading;
    game_object_construct(self, position, nullptr, nullptr);
    u32(self, 76) = 0;
    u32(self, 72) = word(vt_MummySnake);
    u32(self, 96) = word(header);
    u32(self, 12) |= 0x9u;
    u32(self, 80) = 0;
    listOnewayLink__FP8ListHeadT0Pv(&u32(self, 76), first, first);
    if (second) listOnewayLink__FP8ListHeadT0Pv(&u32(self, 80), second, second);
    i32(self, 88) = frameNm + 360;
    objectAddToRunList__FP10GameObject(self);
}
