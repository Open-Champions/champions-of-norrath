#include "../w11-rend-a-common/rend.hpp"
// Observed animRemove__FP14AnimationStateP15AnimationHeaderf at 0x001602d8, 436 bytes. Arguments:
// the animation state, an animation header pointer, and a float t. A null header returns. The
// signed byte at state+4 is the component count; components are 32-byte records from state+8.
// Every component whose word at +0 equals the header (the count is reread each pass) is handled:
//  * t == 0.0f: animResetFramestates__FP14AnimationState(state) is called, the later components
//    move down one place (32 bytes each, moving while index < count - 1, count read from memory
//    each pass), the count byte falls by one, and the same index is examined again.
//  * otherwise the component stays: its float at +8 is clamped to at most 1.0f; with
//    rate = 1.0f when t < 0x1.11111p-6 (a 4-byte constant in the image, 1/60 truncated), else
//    1.0f / (t * 60.0f), the float at +12 is set to -rate; its float at +16 is clamped to at
//    most 1.0f; the word at +20 is cleared; and bits 0x1000 and 0x0004 of the unsigned halfword at
//    +28 are cleared.
// A NaN t is not equal to zero and not below the constant, so it takes the division branch.
// Nothing is returned.
// Inferred: the component is faded out over t seconds (+12 a negative per-frame step) or
// removed at once. Unknown: what the floats at +8/+16 and the flag bits mean. Nothing here was
// executed on the EE.
extern "C" {
void animResetFramestates__FP14AnimationState(unsigned char* state);
void animRemove__FP14AnimationStateP15AnimationHeaderf(unsigned char* state, unsigned char* header, float t) {
    using namespace rend;
    if (header == nullptr) return;
    if (at<i8>(state, 4) <= 0) return;
    for (int i = 0; i < at<i8>(state, 4); ++i) {
        unsigned char* component = state + 8 + i * 32;
        if (ptr(component, 0) != header) continue;
        if (t == 0.0f) {
            animResetFramestates__FP14AnimationState(state);
            for (int j = i; j < at<i8>(state, 4) - 1; ++j) {
                unsigned char* to = state + 8 + j * 32;
                for (int w = 0; w != 32; w += 4) at<u32>(to, w) = at<u32>(to, w + 32);
            }
            at<i8>(state, 4) = static_cast<signed char>(at<i8>(state, 4) - 1);
            --i;
        } else {
            if (1.0f < at<f32>(component, 8)) at<f32>(component, 8) = 1.0f;
            float rate;
            if (t < 0x1.11111p-6f) rate = 1.0f;
            else rate = 1.0f / (t * 60.0f);
            at<f32>(component, 12) = -rate;
            if (1.0f < at<f32>(component, 16)) at<f32>(component, 16) = 1.0f;
            at<u32>(component, 20) = 0;
            at<u16>(component, 28) = static_cast<unsigned short>(at<u16>(component, 28) & 0xeffb);
        }
    }
}
}
