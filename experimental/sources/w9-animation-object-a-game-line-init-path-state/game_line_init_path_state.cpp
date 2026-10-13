#include "../w9-animation-object-a-common/mem.hpp"
// Observed gameLineInitPathState__FP17gameLinePathStatePcP6Point3 at 0x00279b58, 88
// bytes. Clears the first 56 bytes of the path state with memset, then stores the third
// argument (the Point3 pointer) at +0, the second (the text pointer) at +20, and -1 at
// +28 and +32. Nothing else is touched.
// Inferred: +0 is the position being followed, +20 the line name that
// gameLineUpdatePosition looks up case-insensitively, +28 the index of that line in the
// table of lines (-1 meaning not yet looked up) and +32 a point index (-1 before the
// first point is chosen). These readings come from gameLineUpdatePosition's use of the
// same words.
// Unknown: whether 56 is the whole structure (it is the size the original clears).
extern "C" void* memset(void* destination, int value, __SIZE_TYPE__ count);

extern "C" void gameLineInitPathState__FP17gameLinePathStatePcP6Point3(
    unsigned char* state, unsigned char* tag, unsigned char* point) {
    using namespace norrath_ao;
    memset(state, 0, 56);
    store_pointer(state + 0, point);
    store_pointer(state + 20, tag);
    store_word(state + 28, 0xffffffffu);
    store_word(state + 32, 0xffffffffu);
}
