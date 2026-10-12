#include "../level-anim-load-common/mem.hpp"
// Observed animSetToEnd__FP14AnimationState at 0x00163ba0, 184 bytes. A nonzero word at
// state+0 is a pointer: state+0 is cleared and the pointed-to word at +1552 is cleared.
// With a zero signed byte 4 the routine returns. Otherwise the last component, at
// state+8+32*(count-1), receives float +24 = (float)(int word at header+4). If its
// halfword +28 has bit 0x800, updateAnimation (0x00161528, a local function) runs on
// the component and the record linked through component+4 is adjusted: with
// d = component[+24] - (float)(signed half at link+28), link[+36] = link[+36] -
// (link[+36] + link[+48] * d) and link[+32] = link[+32] - (link[+32] + link[+44] * d).
// Inferred: header+4 is a frame count, so the component's time goes to the final frame.
// Unknown: what the link fields +28, +32..+48 mean beyond the arithmetic above, and
// the link pointer is not checked for null in the image; neither is it here.
extern "C" void updateAnimation__FP13animComponent(unsigned char* component);
extern "C" void animSetToEnd__FP14AnimationState(unsigned char* state) {
    using namespace norrath_lal;
    unsigned char* result = load_pointer(state + 0);
    if (result != nullptr) {
        store_word(state + 0, 0);
        store_word(result + 1552, 0);
    }
    const int count = load_signed_byte(state + 4);
    if (count == 0) return;
    unsigned char* component = state + 8 + (count - 1) * 32;
    unsigned char* header = load_pointer(component);
    const unsigned flags = load_half(component + 28);
    store_float(component + 24, static_cast<float>(static_cast<int>(load_word(header + 4))));
    if ((flags & 0x800u) == 0) return;
    updateAnimation__FP13animComponent(component);
    unsigned char* link = load_pointer(component + 4);
    const float time = load_float(component + 24);
    const float base = static_cast<float>(load_signed_half(link + 28));
    const float step_a = load_float(link + 44);
    const float value_a = load_float(link + 32);
    const float step_b = load_float(link + 48);
    const float value_b = load_float(link + 36);
    const float delta = time - base;
    const float moved_b = value_b + step_b * delta;
    const float moved_a = value_a + step_a * delta;
    store_float(link + 36, value_b - moved_b);
    store_float(link + 32, value_a - moved_a);
}
