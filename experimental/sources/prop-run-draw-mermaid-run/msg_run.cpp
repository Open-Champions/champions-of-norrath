#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__7Mermaid at 0x1f7d80, 392 bytes.
// newDamageHandler and everyFrame each get the object and the word at +12 of
// the group at object+36 (re-read before each call). A non-zero everyFrame
// result returns. objectActivationCheck(object, 600) runs with its result
// unused, then creatureAdvanceAnimation(1.0), slideBack(K0), virtual slot byte
// 168 with the adjusted this and no other argument set, and move2 with a
// zero Point3. The signed halfword at object+572 is then compared as unsigned
// against 11 and indexes a jump table of 11 words; states 2, 4, 6, 7 and 9
// (and anything out of range) return. Setting a state is virtual slot byte 184
// with the adjusted this and the state number (setState__8Creaturei per the
// vslots-b record for stun):
//   state 0: setState(1) when signed word +256 is below signed word +808.
//   state 1: setState(0) when +256 is below +808, otherwise smartWander.
//   state 3: when the word at +576 is zero, setState(0).
//   state 5: bExitStatePersueIdle non-zero -> setState(3).
//   state 8: when bit 0x10 of the halfword at +294 is clear, setState(0).
//   state 10: when bit 0x80 of the halfword at +294 is set,
//             objectRemoveFromRunList.
// Observed: K0 is the .lit4 word 0x3e4ccccc at 0x480ef4; the jump table is the
// 11 words at 0x46e990. Unknown: names of states and of object+256/+576/+808.
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
extern "C" int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
extern "C" int objectActivationCheck__8Creaturei(void* self, int distance);
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount);
extern "C" void slideBack__8Creaturef(void* self, float amount);
extern "C" void move2__8CreatureG6Point3(void* self, const void* point);
extern "C" int bExitStatePersueIdle__8Creature(void* self);
extern "C" void smartWander__8Creature(void* self);
extern "C" void objectRemoveFromRunList__FP10GameObject(void* self);

extern "C" void msg_run__7Mermaid(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    newDamageHandler__8CreatureP12_modelHeader(self, pointer(read_word(address(pointer(read_word(base + 36u))) + 12u)));
    if (everyFrame__8CreatureP12_modelHeader(self, pointer(read_word(address(pointer(read_word(base + 36u))) + 12u))) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0x1.999998p-3f);
    {
        void* adjusted = nullptr;
        using SlotCall = void (*)(void*);
        SlotCall call = slot_function<SlotCall>(self, 168u, &adjusted);
        call(adjusted);
    }
    const float zero_point[3] = {0.0f, 0.0f, 0.0f};
    move2__8CreatureG6Point3(self, zero_point);
    const unsigned state = static_cast<unsigned>(static_cast<int>(read_i16(base + 572u)));
    int next = -1;
    switch (state) {
    case 0:
        if (signed_word(read_word(base + 256u)) < signed_word(read_word(base + 808u))) next = 1;
        break;
    case 1:
        if (signed_word(read_word(base + 256u)) < signed_word(read_word(base + 808u))) next = 0;
        else smartWander__8Creature(self);
        break;
    case 3:
        if (read_word(base + 576u) == 0u) next = 0;
        break;
    case 5:
        if (bExitStatePersueIdle__8Creature(self) != 0) next = 3;
        break;
    case 8:
        if ((read_u16(base + 294u) & 0x10u) == 0u) next = 0;
        break;
    case 10:
        if ((read_u16(base + 294u) & 0x80u) != 0u) objectRemoveFromRunList__FP10GameObject(self);
        break;
    default:
        break;
    }
    if (next >= 0) {
        void* adjusted = nullptr;
        using SlotCall = void (*)(void*, int);
        SlotCall call = slot_function<SlotCall>(self, 184u, &adjusted);
        call(adjusted, next);
    }
}
