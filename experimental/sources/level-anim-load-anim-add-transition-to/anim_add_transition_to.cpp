#include "../level-anim-load-common/mem.hpp"
// Observed animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi at 0x00160128,
// 208 bytes. A null header returns. The target slot is the next free one, or, when
// signed byte 4 is not below signed byte 5, the last one; in that case a nonzero link
// word at component+4 of that last slot has its word +20 cleared and is cleared. If
// the slot index is zero the rate is replaced by 0.0. Byte 4 becomes index+1, the state
// halfword +6 gains the flag bits, and the component is filled as animAdd does, except
// that float +8 is 0.0 and float +16 is 1.0, float +20 is 0, and float +12 is a
// step: 1.0 when rate is below the 435.0 literal at 0x0047fe50, otherwise 1.0 / (rate * 60.0).
// Inferred: float +12 is a blend step per frame; the 435.0 threshold looks odd next to a
// 60.0 scale and its intent is not claimed.
// Unknown: callers pass rates well below 435 (the NPC native passes 0.3), so the
// division path has not been seen taken.
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
    unsigned char* state, unsigned char* header, float rate, int flags) {
    using namespace norrath_lal;
    if (header == nullptr) return;
    int index = load_signed_byte(state + 4);
    if (!(index < load_signed_byte(state + 5))) {
        index -= 1;
        unsigned char* last = load_pointer(state + 12 + index * 32);
        if (last != nullptr) {
            store_word(last + 20, 0);
            store_word(state + 12 + index * 32, 0);
        }
    }
    if (index == 0) rate = 0.0f;
    unsigned char* component = state + 8 + index * 32;
    state[4] = static_cast<unsigned char>(index + 1);
    store_half(state + 6, load_half(state + 6) | static_cast<unsigned>(flags));
    unsigned char* link = load_pointer(component + 4);
    store_pointer(component + 0, header);
    store_word(component + 24, 0);
    store_half(component + 30, 0);
    if (link != nullptr) {
        store_word(link + 20, 0);
        store_word(component + 4, 0);
    }
    store_half(component + 28, static_cast<unsigned>(flags));
    store_float(component + 8, 0.0f);
    float step = 1.0f;
    if (!(rate < 435.0f)) step = 1.0f / (rate * 60.0f);
    store_float(component + 12, step);
    store_float(component + 16, 1.0f);
    store_word(component + 20, 0);
}
