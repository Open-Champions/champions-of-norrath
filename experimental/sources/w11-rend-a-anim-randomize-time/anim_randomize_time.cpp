#include "../w11-rend-a-common/rend.hpp"
// Observed animRandomizeTime__FP14AnimationState at 0x001601f8, 224 bytes. The signed byte at
// state+4 is a component count n; with n == 0 the routine returns. Otherwise the component
// is the 32-byte record at state + 8 + (n-1)*32 (the last one in use). The generator word
// eRandState is advanced once (state = state * 1664525 + 1013904223, 32-bit) and the new
// state's top 16 bits r (0..65535) give f = r * (1/65536) * (float)(int word at
// (word at component+0) + 4), stored as the float at component+24. When bit 0x800 of the
// halfword at component+28 is set, updateAnimation__FP13animComponent(component) is called
// and then, with p = the word at component+4 and d = component[24] - (float)(signed halfword
// at p+28):
//   p[36] = p[36] - (p[36] + p[48] * d)   and   p[32] = p[32] - (p[32] + p[44] * d)
// (floats, byte offsets), with both products and sums rounded in that order. Without bit
// 0x800 nothing further happens. Nothing is returned.
// Inferred: this sets a random start time inside the component's length (the word at +4 of
// its header) and re-derives a position offset for it. Unknown: the layout beyond the
// offsets used, and what the offsets p+32/p+36/p+44/p+48 hold. Nothing here was executed on the EE.
extern "C" {
extern int eRandState;
void updateAnimation__FP13animComponent(unsigned char* component);
void animRandomizeTime__FP14AnimationState(unsigned char* state) {
    using namespace rend;
    const int count = at<i8>(state, 4);
    if (count == 0) return;
    unsigned char* component = state + 8 + (count - 1) * 32;
    unsigned char* header = ptr(component, 0);
    const int length = at<i32>(header, 4);
    const unsigned next = static_cast<unsigned>(eRandState) * 1664525u + 1013904223u;
    eRandState = static_cast<int>(next);
    const float fraction = static_cast<float>(static_cast<int>(next >> 16)) * (1.0f / 65536.0f);
    at<f32>(component, 24) = fraction * static_cast<float>(length);
    if ((at<u16>(component, 28) & 0x800u) == 0) return;
    updateAnimation__FP13animComponent(component);
    unsigned char* p = ptr(component, 4);
    const float d = at<f32>(component, 24) - static_cast<float>(at<i16>(p, 28));
    const float a = at<f32>(p, 36);
    const float b = at<f32>(p, 32);
    const float ya = at<f32>(p, 48) * d;
    const float yb = at<f32>(p, 44) * d;
    at<f32>(p, 36) = a - (a + ya);
    at<f32>(p, 32) = b - (b + yb);
}
}
