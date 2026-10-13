#include "../level-anim-load-common/mem.hpp"
// Observed animRemoveNonpermanentAnimations__FP14AnimationState at 0x0015fcb0, 232 bytes.
// The signed byte at state+4 is a count of 32-byte components starting at state+8.
// Components whose halfword at +28 has bit 0x1000 set are kept and packed toward the
// front, in order, by copying all 32 bytes to the next free slot. Other components are
// dropped: when the word at their +4 (a link record) is nonzero, the word at +20 of that
// record is cleared and the component's +4 word is cleared. For a kept component that
// moves to a different slot with a nonzero +4 word, the source slot's +4 word is cleared
// and the link record's +20 word is set to the address of the new slot. Finally the
// byte at state+4 receives the number of kept components.
// Inferred: bit 0x1000 marks a permanent animation and +20 of the link record points back
// to the owning component. Unknown: the link record's other fields and the fate of the
// stale component bytes past the new count.
extern "C" void animRemoveNonpermanentAnimations__FP14AnimationState(unsigned char* state) {
    using namespace norrath_lal;
    const int count = load_signed_byte(state + 4);
    int kept = 0;
    for (int i = 0; i < count; ++i) {
        unsigned char* source = state + 8 + i * 32;
        unsigned char* target = state + 8 + kept * 32;
        if ((load_half(source + 28) & 0x1000u) == 0) {
            unsigned char* link = load_pointer(source + 4);
            if (link != nullptr) {
                store_word(link + 20, 0);
                store_word(source + 4, 0);
            }
        } else {
            for (unsigned j = 0; j != 32; ++j) target[j] = source[j];
            if (load_word(source + 4) != 0 && kept != i) {
                unsigned char* link = load_pointer(target + 4);
                store_word(source + 4, 0);
                store_pointer(link + 20, target);
            }
            ++kept;
        }
    }
    state[4] = static_cast<unsigned char>(kept);
}
