#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__14GnomeNavigator at 0x201fd0, 476 bytes.
// newDamageHandler and everyFrame each get the object and the word at +12 of
// the group at object+36 (re-read before each call). A non-zero everyFrame
// result returns, as does a zero objectActivationCheck(object, 600). Then
// creatureAdvanceAnimation(1.0), slideBack(1.0) and virtual slot byte 168
// (adjusted this, no other argument set) run. The signed halfword at
// object+572 is compared as unsigned against 11 and indexes the 11-word jump
// table at 0x46f440. Setting a state is virtual slot byte 184 with the
// adjusted this and the state number (setState__8Creature per vslots-b).
//   state 0: returns unless (8 bytes at +160) & 0xc00000 is non-zero; then
//     setState(1) when the signed word +256 is below the signed word +808 or
//     bFarFromHome is non-zero.
//   state 1: steerAndRouteToPosition gets a copy of the point at +272; a
//     non-zero result, or +256 below +808, calls setState(0).
//   state 3: setState(0) when the word at +576 is zero.
//   state 5: setState(3) when bExitStatePersueIdle is non-zero.
//   state 9: setState(3) when the signed word +804 is below +808.
//   state 10: objectRemoveFromRunList when bit 0x80 of the halfword at +294.
// States 2, 4, 6, 7, 8 and anything out of range return. Unknown: the names
// of the states and fields.
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
extern "C" int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
extern "C" int objectActivationCheck__8Creaturei(void* self, int distance);
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount);
extern "C" void slideBack__8Creaturef(void* self, float amount);
extern "C" int bFarFromHome__8Creature(void* self);
extern "C" int steerAndRouteToPosition__8CreatureG6Point3(void* self, const void* point);
extern "C" int bExitStatePersueIdle__8Creature(void* self);
extern "C" void objectRemoveFromRunList__FP10GameObject(void* self);

namespace {
void set_state(void* self, int state) {
    void* adjusted = nullptr;
    using SlotCall = void (*)(void*, int);
    SlotCall call = vslots_b::slot_function<SlotCall>(self, 184u, &adjusted);
    call(adjusted, state);
}
}

extern "C" void msg_run__14GnomeNavigator(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    newDamageHandler__8CreatureP12_modelHeader(self, pointer(read_word(address(pointer(read_word(base + 36u))) + 12u)));
    if (everyFrame__8CreatureP12_modelHeader(self, pointer(read_word(address(pointer(read_word(base + 36u))) + 12u))) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 1.0f);
    {
        void* adjusted = nullptr;
        using SlotCall = void (*)(void*);
        SlotCall call = slot_function<SlotCall>(self, 168u, &adjusted);
        call(adjusted);
    }
    const unsigned state = static_cast<unsigned>(static_cast<int>(read_i16(base + 572u)));
    switch (state) {
    case 0:
        if ((read_u64(base + 160u) & 0xc00000ull) == 0ull) return;
        if (signed_word(read_word(base + 256u)) < signed_word(read_word(base + 808u))) {
            set_state(self, 1);
        } else if (bFarFromHome__8Creature(self) != 0) {
            set_state(self, 1);
        }
        break;
    case 1: {
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(base + 272u + i));
        if (steerAndRouteToPosition__8CreatureG6Point3(self, point) != 0) {
            set_state(self, 0);
        } else if (signed_word(read_word(base + 256u)) < signed_word(read_word(base + 808u))) {
            set_state(self, 0);
        }
        break;
    }
    case 3:
        if (read_word(base + 576u) == 0u) set_state(self, 0);
        break;
    case 5:
        if (bExitStatePersueIdle__8Creature(self) != 0) set_state(self, 3);
        break;
    case 9:
        if (signed_word(read_word(base + 804u)) < signed_word(read_word(base + 808u))) set_state(self, 3);
        break;
    case 10:
        if ((read_u16(base + 294u) & 0x80u) != 0u) objectRemoveFromRunList__FP10GameObject(self);
        break;
    default:
        break;
    }
}
