#include "../level-anim-load-common/mem.hpp"
// Observed animGetAnimComponent__FP14AnimationStateP15AnimationHeader at 0x00163d58, 116
// bytes. A null state or a null header returns null. Otherwise the signed byte at state+4
// is a count and the 32-byte components starting at state+8 are searched in order for the
// first whose word at offset 0 equals the header; its address is returned. A count of zero
// or below, or no match, returns null. Nothing is written.
// Inferred: the routine looks up the component playing a given animation header.
extern "C" unsigned char* animGetAnimComponent__FP14AnimationStateP15AnimationHeader(
    unsigned char* state, const unsigned char* header) {
    using namespace norrath_lal;
    if (state == nullptr || header == nullptr) return nullptr;
    const int count = load_signed_byte(state + 4);
    for (int i = 0; i < count; ++i) {
        unsigned char* component = state + 8 + i * 32;
        if (load_pointer(component) == header) return component;
    }
    return nullptr;
}
