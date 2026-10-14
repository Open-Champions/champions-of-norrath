#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_poke__13UserParamPropi at 0x154c78, 72 bytes.
// objectDoMyActions runs on the object with 1; the poke argument is unused.
// A non-null object then calls table slot byte 8 with the integer 3. In the
// Vehicle table that slot holds the destructor (observed in the published
// view), so this reads as a delete request. Inferred only: other tables are
// not checked here, and the meaning of the argument 3 is not established.
extern "C" void objectDoMyActions__FP10GameObjecti(void* object, int value);
extern "C" void msg_poke__13UserParamPropi(void* self, int value) {
    using namespace vslots_b;
    (void)value;
    objectDoMyActions__FP10GameObjecti(self, 1);
    if (self == nullptr) return;
    void* adjusted;
    using SlotCall = void (*)(void*, int);
    SlotCall call = slot_function<SlotCall>(self, 8u, &adjusted);
    call(adjusted, 3);
}
