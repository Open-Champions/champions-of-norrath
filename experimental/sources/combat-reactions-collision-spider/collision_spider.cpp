#include "../combat-reactions-bytes/bytes.hpp"
// Observed msg_collision__11SmallSpiderP10GameObject at 0x20f918, 120 bytes.
// With a non-null other object, a signed halfword at object+572 that is not 10,
// and bit 0x40 set in the other object's word at +12, virtual slot byte 184 is
// called with this adjusted by that slot's signed halfword and with integer 10.
// The creature vtable names that slot setState__8Creaturei. In every case the
// Creature collision body then runs with the object and the other object.
// Unknown: what state 10 selects.
extern "C" void msg_collision__8CreatureP10GameObject(void* self, void* other);

extern "C" void msg_collision__11SmallSpiderP10GameObject(void* self, void* other) {
    using namespace combat_reactions;
    if (other != nullptr && read_i16(address(self) + 572u) != 10 &&
        (read_word(address(other) + 12u) & 0x40u) != 0u) {
        void* adjusted = nullptr;
        using StateCall = void (*)(void*, int);
        StateCall call = slot_function<StateCall>(self, 184u, &adjusted);
        call(adjusted, 10);
    }
    msg_collision__8CreatureP10GameObject(self, other);
}
