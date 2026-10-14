#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__8Skeletoni at 0x2453f0, 1352 bytes, emitted here as
// norrath_skeleton_set_state (the name the Skeleton constructor imports).
// The requested state goes to a stack word whose address is passed to
// setStateProcessing__8CreaturePi(object, &state); a nonzero result returns at
// once. Otherwise bit 0x200 of the 64-bit word at +160 is cleared and the
// state (unsigned, 16 and above only store) selects through the jump table at
// 0x4740f0. R is the record at skeletonResMap (0x42d8d0, 800 bytes) plus 104
// times the word at object+968; the animation state is at object+288. All
// transition and one-shot rates are 0.1 (0x3dcccccc) unless noted.
//   0: transition R+68, flags 3; +256 = +808 plus 180.
//   1: transition R+84, flags 2051; animGlobalSync.
//   3: transition R+108, flags 2051; animGlobalSync.
//   5: transition R+44, flags 3.
//   15: transition R+44, flags 3; +256 = +808 plus 120; bit 0x200 of the
//      64-bit word at +160 is set.
//   13: transition R+44, flags 3; one-shot R+28, flags 2056; then the tail.
//   6: transition R+44, flags 3; an eRand roll picks the one-shot header
//      R+12+8*(roll & 1), flags 2056; then the tail.
//      Tail: +256 = eRand modulo 20 (unsigned) plus 20.
//   14: transition R+60, flags 3; an eRand roll picks the one-shot header
//      R+12+8*(roll & 1), flags 2056; +256 = 1.
//   9: transition R+68, flags 3.
//   8: with the current halfword at +572 equal to 9 the state word becomes 9;
//      otherwise a one-shot with flags 16: R+36 when bit 0x200 of the 64-bit
//      word at +160 (read again) is set, else R+52.
//   10: word +12 |= 0x4002; bit 41 of the +160 word set; dropItem__8Skeletoni
//      (object, 8); when bit 0x1000000 of the +160 word is clear,
//      dropTreasure__8CreatureG6Point3iffffffPc(object, copy of the 12 bytes
//      at +16, the signed halfword at +56, 36.0, 2.0, 1.6 (0x3fcccccc),
//      0.6 (0x3f199999), 1.6, 0.25, "bonechip"); objectAddToDeleteList; then
//      the virtual function at byte offset 192 of the table at +72.
//   2, 4, 7, 11, 12 and 16 or more: only the store.
// Every path that does not return early stores the low 16 bits of the state
// word at +572.
// Unknown: the state names and the meaning of the record fields.
extern "C" {
extern unsigned char skeletonResMap[];
int setStateProcessing__8CreaturePi(void* self, int* state);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
int eRand__10GameObject(void* object);
void dropItem__8Skeletoni(void* self, int kind);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int a, float f12, float f13, float f14, float f15, float f16, float f17, char* name);
void objectAddToDeleteList__FP10GameObject(void* object);
}

namespace {
using namespace rtti_c;
__attribute__((always_inline)) inline void* header(Address object, unsigned offset) {
    return pointer(read_word(address(skeletonResMap) + 104u * read_word(object + 968u) + offset));
}
__attribute__((always_inline)) inline void transition(Address object, unsigned offset, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, offset), single_from_bits(0x3dccccccu), flags);
}
__attribute__((always_inline)) inline void one_shot(Address object, void* h, int flags) {
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), h, single_from_bits(0x3dccccccu), single_from_bits(0x3dccccccu), flags);
}
__attribute__((always_inline)) inline void tail(void* self, Address object) {
    write_word(object + 256u, static_cast<unsigned>(eRand__10GameObject(self)) % 20u + 20u);
}
}

extern "C" void norrath_skeleton_set_state(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    write_u64(object + 160u, read_u64(object + 160u) & ~0x200ull);
    switch (static_cast<unsigned>(state)) {
    case 0u:
        transition(object, 68u, 3);
        write_word(object + 256u, read_word(object + 808u) + 180u);
        break;
    case 1u:
        transition(object, 84u, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    case 3u:
        transition(object, 108u, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    case 5u:
        transition(object, 44u, 3);
        break;
    case 15u:
        transition(object, 44u, 3);
        write_word(object + 256u, read_word(object + 808u) + 120u);
        write_u64(object + 160u, read_u64(object + 160u) | 0x200ull);
        break;
    case 13u:
        transition(object, 44u, 3);
        one_shot(object, header(object, 28u), 2056);
        tail(self, object);
        break;
    case 6u: {
        transition(object, 44u, 3);
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self));
        one_shot(object, header(object, 12u + 8u * (roll & 1u)), 2056);
        tail(self, object);
        break;
    }
    case 14u: {
        transition(object, 60u, 3);
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self));
        one_shot(object, header(object, 12u + 8u * (roll & 1u)), 2056);
        write_word(object + 256u, 1u);
        break;
    }
    case 9u:
        transition(object, 68u, 3);
        break;
    case 8u:
        if (read_i16(object + 572u) == 9) {
            state = 9;
        } else if ((read_u64(object + 160u) & 0x200ull) != 0ull) {
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), header(object, 36u), single_from_bits(0x3dccccccu), single_from_bits(0x3dccccccu), 16);
        } else {
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), header(object, 52u), single_from_bits(0x3dccccccu), single_from_bits(0x3dccccccu), 16);
        }
        break;
    case 10u:
        write_word(object + 12u, read_word(object + 12u) | 0x4002u);
        write_u64(object + 160u, read_u64(object + 160u) | (1ull << 41));
        dropItem__8Skeletoni(self, 8);
        if ((read_u64(object + 160u) & 0x1000000ull) == 0ull) {
            unsigned char point[12];
            for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
            dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), single_from_bits(0x42100000u), single_from_bits(0x40000000u),
                                                     single_from_bits(0x3fcccccc), single_from_bits(0x3f199999u), single_from_bits(0x3fcccccc),
                                                     single_from_bits(0x3e800000u), const_cast<char*>("bonechip"));
        }
        objectAddToDeleteList__FP10GameObject(self);
        {
            typedef void (*SlotCall)(void*);
            void* adjusted;
            SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
            call(adjusted);
        }
        break;
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
