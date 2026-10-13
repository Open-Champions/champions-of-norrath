#include "../w9-animation-object-a-common/mem.hpp"
// Observed animAddOneShot__FP14AnimationStateP15AnimationHeaderffi at 0x00160010, 280
// bytes. A null header returns. The target slot is the next free one, or, when signed
// byte 4 is not below signed byte 5, the last one; in that case a nonzero link word at
// component+4 of that last slot has its word +20 cleared and is cleared. Byte 4 becomes
// slot+1, the state halfword +6 gains the flag bits, and the component is filled: word
// +0 is the header, word +24 and halfword +30 are cleared, halfword +28 holds the flags,
// a nonzero word at +4 has that record's word +20 cleared and is cleared, and word +8 is
// zero. A rate is turned into a step: 1.0 when the rate is below 1/60 (0x3c888888),
// otherwise 1.0 / (rate * 60.0). The first float argument gives the step stored at +12.
// The second gives a step s; -s is stored at +20 and the float at +16 is
// ((float)(integer word at header+4) * (1/60) * 60.0) * s.
// Inferred: the pair of rates are the in and out blend speeds of a play-once animation,
// and header+4 is its length in frames (the same word animAddToEnd reads).
// Unknown: the unit of each rate beyond the 60 factor; the three 1/60 literals are
// separate constants in the original and are all the same value.
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(
    unsigned char* state, unsigned char* header, float first, float second, int flags) {
    using namespace norrath_ao;
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
    store_word(component + 8, 0);
    float step = 1.0f;
    if (!(first < 1.0f / 60.0f)) step = 1.0f / (first * 60.0f);
    store_float(component + 12, step);
    float other = 1.0f;
    if (!(second < 1.0f / 60.0f)) other = 1.0f / (second * 60.0f);
    const float length = static_cast<float>(static_cast<int>(load_word(header + 4)));
    store_float(component + 20, -other);
    store_float(component + 16, length * (1.0f / 60.0f) * 60.0f * other);
}
