#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed msg_save__9DoorSwingPc at 0x21bf90, 56 bytes. Writes the halfword at
// object+80, then the byte at +88 sign-extended and stored as a halfword, then
// the single at +84 as a four-byte word, and returns 8.
// Inferred: the return value is the number of bytes written. Unknown: the
// meaning of the fields; 80 and 88 are not known to be door indices.
extern "C" int msg_save__9DoorSwingPc(void* self, char* buffer) {
    using namespace rtti_c;
    const Address object = address(self);
    const Address out = address(buffer);
    write_u16(out, read_u16(object + 80u));
    write_u16(out + 2u, static_cast<unsigned>(read_s8(object + 88u)));
    write_word(out + 4u, read_word(object + 84u));
    return 8;
}
