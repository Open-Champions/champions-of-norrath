#include "../vslots-b-bytes/bytes.hpp"
// Observed setState__7Nataslai at 0x233800, 116 bytes.
// States 0, 2, 4, and 7, and every unsigned state at or above 11, store the
// low 16 bits at object+572. State 0 also calls animAddTransitionTo with
// object+288, the word 28 bytes after the pointer at object+36, the single
// 0x3e999999, and mode 3. The store follows that call. The other states below
// 11 leave object+572 unchanged. animAddTransitionTo is not defined here.
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int mode);
namespace {
__attribute__((always_inline)) inline float from_bits(unsigned bits) {
    float value;
    unsigned char* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(bits);
    out[1] = static_cast<unsigned char>(bits >> 8);
    out[2] = static_cast<unsigned char>(bits >> 16);
    out[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}
}
extern "C" void setState__7Nataslai(void* self, int state) {
    using namespace vslots_b;
    const unsigned index = static_cast<unsigned>(state);
    const bool stores = index >= 11u || state == 0 || state == 2 || state == 4 || state == 7;
    if (state == 0) {
        const Address group = address(pointer(read_word(address(self) + 36u)));
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
            static_cast<unsigned char*>(self) + 288u, pointer(read_word(group + 28u)), from_bits(0x3e999999u), 3);
    }
    if (stores) write_u16(address(self) + 572u, static_cast<unsigned>(state));
}
