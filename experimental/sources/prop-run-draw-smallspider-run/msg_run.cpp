#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__11SmallSpider at 0x2098f8, 280 bytes.
// newDamageHandler and then everyFrame each get the object and the word at +12
// of the group at object+36 (re-read before each call). A non-zero everyFrame
// result returns. Otherwise objectActivationCheck(object, 600) returning zero
// returns. creatureAdvanceAnimation and slideBack then run with 1.0. The
// signed halfword at object+572 selects: 0 calls virtual slot byte 184 with
// the adjusted this and 1 when the signed word at object+256 is below the
// signed word at object+808; 1 calls smartWander; 10 runs
// objectRemoveFromRunList when bit 0x80 of the halfword at object+294 is set.
// Every other state returns. Slot byte 184 is setState__8Creaturei per the
// vslots-b record for stun. Unknown: what states 0, 1 and 10 stand for and
// what object+256 and +808 count.
extern "C" void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
extern "C" int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
extern "C" int objectActivationCheck__8Creaturei(void* self, int distance);
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount);
extern "C" void slideBack__8Creaturef(void* self, float amount);
extern "C" void smartWander__8Creature(void* self);
extern "C" void objectRemoveFromRunList__FP10GameObject(void* self);

extern "C" void msg_run__11SmallSpider(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    newDamageHandler__8CreatureP12_modelHeader(self, pointer(read_word(address(pointer(read_word(base + 36u))) + 12u)));
    if (everyFrame__8CreatureP12_modelHeader(self, pointer(read_word(address(pointer(read_word(base + 36u))) + 12u))) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 1.0f);
    const short state = read_i16(base + 572u);
    if (state == 1) {
        smartWander__8Creature(self);
    } else if (state == 0) {
        if (signed_word(read_word(base + 256u)) < signed_word(read_word(base + 808u))) {
            void* adjusted = nullptr;
            using SlotCall = void (*)(void*, int);
            SlotCall call = slot_function<SlotCall>(self, 184u, &adjusted);
            call(adjusted, 1);
        }
    } else if (state == 10) {
        if ((read_u16(base + 294u) & 0x80u) != 0u) objectRemoveFromRunList__FP10GameObject(self);
    }
}
