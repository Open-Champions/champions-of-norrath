// Observed msg_run__12GlowingShaft at 0x001e69c8, 72 bytes. O is the object, the first argument. The signed word at
// O+76 is lowered (the load happens before the null test of O) by one and stored; when the result is zero or below and O is not null, the entry at offset 8 of the
// table whose address is the word at O+72 is called on O (adjusted by the entry's signed halfword) with the argument 3.
// Inferred: a frame countdown that ends the object through a table call. Unknown: what table entry 8 does and the
// meaning of the argument 3. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" void msg_run__12GlowingShaft(void* self) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    const int remaining = sword(o, 76) - 1;
    sword(o, 76) = remaining;
    // The original reads the word first and tests the pointer afterwards; keep the compiler from dropping the test.
    __asm__("" : "+r"(o));
    if (remaining <= 0 && o != nullptr) call_entry(o, 8, 3);
}
