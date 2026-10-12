#include "../combat-reactions-bytes/bytes.hpp"
// Observed msg_collision__3CatP10GameObject at 0x1eedf0, 120 bytes.
// The other object is not read. A word equal to 1 at object+964 returns. A
// signed halfword at object+572 that is neither 0 nor 13 returns. Otherwise
// readyToSwitch runs with the object; a zero result returns. A nonzero result
// runs switchToRun with the object, then calls virtual slot byte 184 with this
// adjusted by that slot's signed halfword and with integer 3. The creature
// vtable names that slot setState__8Creaturei.
// Unknown: what states 0, 13 and 3 select and what the word at +964 holds.
extern "C" int readyToSwitch__3Cat(void* self);
extern "C" void switchToRun__3Cat(void* self);

extern "C" void msg_collision__3CatP10GameObject(void* self, void* other) {
    (void)other;
    using namespace combat_reactions;
    const Address base = address(self);
    if (read_word(base + 964u) == 1u) return;
    const int state = read_i16(base + 572u);
    if (state != 0 && state != 13) return;
    if (readyToSwitch__3Cat(self) == 0) return;
    switchToRun__3Cat(self);
    void* adjusted = nullptr;
    using StateCall = void (*)(void*, int);
    StateCall call = slot_function<StateCall>(self, 184u, &adjusted);
    call(adjusted, 3);
}
