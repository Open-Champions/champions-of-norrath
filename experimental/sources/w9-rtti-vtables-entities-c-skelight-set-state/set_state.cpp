#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__8Skelighti at 0x246cf0, 544 bytes, emitted here as
// norrath_skelight_set_state (the name the Skelight constructor imports). The
// requested state is copied to a stack word whose address goes to
// setStateProcessing__8CreaturePi(object, &state); a nonzero result returns at
// once, without storing the halfword. Otherwise a switch on the possibly
// changed word (unsigned; 14 and above only store it) runs, and the low 16
// bits of the state word are stored at object+572. The animation state is at
// object+288, the word at object+36 points to a header table and all
// transition rates are 0.1 (0x3dcccccc). Observed jump table at 0x474250.
//   0: transition to the header at table+52, flags 3; +256 = +808 plus 60.
//   1: resetRouting; getSmartRandomDestination(range 300) copied to +272;
//      transition to table+68, flags 2051; animGlobalSync; +256 = +808 + 300.
//   3: transition to table+68, flags 2051; animGlobalSync.
//   5: transition to table+52, flags 3.
//   6: transition to table+52, flags 3; one eRand roll picks the header at
//      table+28+8*(roll & 1) for animAddOneShot with both rates 0.1, flags
//      2056; +256 = 1.
//   8: the state word becomes the signed halfword at +572 (no change).
//   10: bit 2 of the word at +12 and bit 41 of the 64-bit word at +160 are
//      set; dropItem__8Skelighti(object, 8); objectAddToDeleteList.
//   2, 4, 7, 9, 11, 12, 13: only the store.
// Unknown: the state names and what the fields at +256 and +272 mean.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
int eRand__10GameObject(void* object);
void dropItem__8Skelighti(void* self, int kind);
void objectAddToDeleteList__FP10GameObject(void* object);
}

namespace {
using namespace rtti_c;
__attribute__((always_inline)) inline void* header_at(Address object, unsigned offset) {
    return pointer(read_word(address(pointer(read_word(object + 36u))) + offset));
}
}

extern "C" void norrath_skelight_set_state(void* self, int requested) {
    const Address object = address(self);
    void* const animation = bytes(object + 288u);
    const float rate = single_from_bits(0x3dccccccu);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    switch (static_cast<unsigned>(state)) {
    case 0u:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 52u), rate, 3);
        write_word(object + 256u, read_word(object + 808u) + 60u);
        break;
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 300);
        for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 68u), rate, 2051);
        animGlobalSync__FP14AnimationState(animation);
        write_word(object + 256u, read_word(object + 808u) + 300u);
        break;
    }
    case 3u:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 68u), rate, 2051);
        animGlobalSync__FP14AnimationState(animation);
        break;
    case 5u:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 52u), rate, 3);
        break;
    case 6u: {
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header_at(object, 52u), rate, 3);
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self));
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header_at(object, 28u + 8u * (roll & 1u)), rate, rate, 2056);
        write_word(object + 256u, 1u);
        break;
    }
    case 8u:
        state = read_i16(object + 572u);
        break;
    case 10u:
        write_word(object + 12u, read_word(object + 12u) | 2u);
        write_u64(object + 160u, read_u64(object + 160u) | (1ull << 41));
        dropItem__8Skelighti(self, 8);
        objectAddToDeleteList__FP10GameObject(self);
        break;
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
