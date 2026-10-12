#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_save__15PushPhysicsPropPc at 0x155bd0, 276 bytes. The output words
// are, in order: the 32-bit pattern of the float at object+256; then the results of
// objectSaveAddToStringPool for the string pointers at object+268, +272 and +276
// (called in that order); then the thirteen consecutive 32-bit float patterns at
// object+96 through +144 (steps of 4). The return value is the number of bytes
// written, 68. Floats are copied as bit patterns.
// Unknown: what the float fields and the three strings represent.
extern "C" int objectSaveAddToStringPool__FPCc(const char* text);

extern "C" int msg_save__15PushPhysicsPropPc(void* self, char* out) {
    using namespace w8c;
    const Address base = address(self);
    Address dest = address(out);
    write_word(dest, read_word(base + 256u));
    dest += 4u;
    for (unsigned offset = 268u; offset <= 276u; offset += 4u) {
        write_word(dest, static_cast<unsigned>(objectSaveAddToStringPool__FPCc(
                             static_cast<const char*>(pointer(read_word(base + offset))))));
        dest += 4u;
    }
    for (unsigned offset = 96u; offset <= 144u; offset += 4u) {
        write_word(dest, read_word(base + offset));
        dest += 4u;
    }
    return static_cast<int>(dest - address(out));
}
