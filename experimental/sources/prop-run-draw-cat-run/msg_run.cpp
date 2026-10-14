#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__3Cat at 0x1ee528, 880 bytes.
// newDamageHandler and everyFrame each get the object and the word at +12 of
// the group at object+36 (re-read before each call). A non-zero everyFrame
// result returns. objectActivationCheck(object, 600) runs with its result
// unused, then creatureAdvanceAnimation(1.0) and slideBack(1.0). For the
// signed halfword state at object+572 equal to 1, 3 or 7, addTurnAnimation
// runs; otherwise subTurnAnimation does. The state is re-read and compared as
// unsigned against 14 (jump table of 14 words at 0x46d9f0); states other than
// 0, 1, 3 and 13 return. Setting a state is virtual slot byte 184 with the
// adjusted this and the state number. Words: A is +968, B is +808, C is +972,
// D is +964, all signed compares. eRand is eRand__10GameObject of the object,
// its low bit is "bit" below.
//   state 0: if A < B and readyToSwitch: bit 0 -> switchToWalk, setState(1);
//     bit 1 -> switchToRun, setState(3); return. Otherwise, if C < B and
//     bit 0x20 of halfword +294 is clear: r = eRand; C = B + r % 300 + 120
//     (unsigned remainder); animAddOneShot(&object+288, header at group+124,
//     K0, K0, 32).
//   state 13: if A < B, D is zero, readyToSwitch and bit 0x8 of +294 is clear:
//     the same walk/run choice, then return. Otherwise, if C < B and bit 0x20
//     of +294 is clear: r = eRand; C = B + r % 480 + 240; a second eRand
//     chooses animAddOneShot(&object+288, header at group+140, K1, K3, 32)
//     for bit 0 clear or group+148 with K2, K3, 32 for bit 0 set.
//   state 1 and state 3 (identical): if A < B, a new eRand chooses
//     setState(13) for bit 0 clear or setState(0) for bit 0 set; otherwise
//     smartWander.
// Observed: K0 0x3c23d70a, K1 and K2 0x3e4ccccc, K3 0x3e999999 in .lit4 at
// 0x480c94..0x480ca0. Unknown: what the states, A, B, C, D mean.
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
extern "C" int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
extern "C" int objectActivationCheck__8Creaturei(void* self, int distance);
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount);
extern "C" void slideBack__8Creaturef(void* self, float amount);
extern "C" void addTurnAnimation__3Cat(void* self);
extern "C" void subTurnAnimation__3Cat(void* self);
extern "C" int readyToSwitch__3Cat(void* self);
extern "C" unsigned eRand__10GameObject(void* self);
extern "C" void switchToWalk__3Cat(void* self);
extern "C" void switchToRun__3Cat(void* self);
extern "C" void smartWander__8Creature(void* self);
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float first, float second, int flags);

namespace {
void set_state(void* self, int state) {
    void* adjusted = nullptr;
    using SlotCall = void (*)(void*, int);
    SlotCall call = vslots_b::slot_function<SlotCall>(self, 184u, &adjusted);
    call(adjusted, state);
}
void walk_or_run(void* self) {
    if ((eRand__10GameObject(self) & 1u) == 0u) {
        switchToWalk__3Cat(self);
        set_state(self, 1);
    } else {
        switchToRun__3Cat(self);
        set_state(self, 3);
    }
}
}

extern "C" void msg_run__3Cat(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    const float k0 = 0x1.47ae14p-7f, k1 = 0x1.999998p-3f, k2 = 0x1.999998p-3f, k3 = 0x1.333332p-2f;
    newDamageHandler__8CreatureP12_modelHeader(self, pointer(read_word(address(pointer(read_word(base + 36u))) + 12u)));
    if (everyFrame__8CreatureP12_modelHeader(self, pointer(read_word(address(pointer(read_word(base + 36u))) + 12u))) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 1.0f);
    const short first_state = read_i16(base + 572u);
    if (first_state == 3 || first_state == 1 || first_state == 7) addTurnAnimation__3Cat(self);
    else subTurnAnimation__3Cat(self);
    const unsigned state = static_cast<unsigned>(static_cast<int>(read_i16(base + 572u)));
    const int b = signed_word(read_word(base + 808u));
    switch (state) {
    case 0:
        if (signed_word(read_word(base + 968u)) < b && readyToSwitch__3Cat(self) != 0) {
            walk_or_run(self);
            return;
        }
        if (signed_word(read_word(base + 972u)) < signed_word(read_word(base + 808u)) && (read_u16(base + 294u) & 0x20u) == 0u) {
            const unsigned r = eRand__10GameObject(self);
            write_word(base + 972u, read_word(base + 808u) + r % 300u + 120u);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(
                pointer(base + 288u), pointer(read_word(address(pointer(read_word(base + 36u))) + 124u)), k0, k0, 32);
        }
        break;
    case 13:
        if (signed_word(read_word(base + 968u)) < b && read_word(base + 964u) == 0u && readyToSwitch__3Cat(self) != 0 &&
            (read_u16(base + 294u) & 0x8u) == 0u) {
            walk_or_run(self);
            return;
        }
        if (signed_word(read_word(base + 972u)) < signed_word(read_word(base + 808u)) && (read_u16(base + 294u) & 0x20u) == 0u) {
            const unsigned r = eRand__10GameObject(self);
            write_word(base + 972u, read_word(base + 808u) + r % 480u + 240u);
            const Address group = address(pointer(read_word(base + 36u)));
            if ((eRand__10GameObject(self) & 1u) == 0u) {
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(pointer(base + 288u), pointer(read_word(group + 140u)), k1, k3, 32);
            } else {
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(pointer(base + 288u), pointer(read_word(group + 148u)), k2, k3, 32);
            }
        }
        break;
    case 1:
    case 3:
        if (signed_word(read_word(base + 968u)) < b) {
            set_state(self, (eRand__10GameObject(self) & 1u) == 0u ? 13 : 0);
        } else {
            smartWander__8Creature(self);
        }
        break;
    default:
        break;
    }
}
