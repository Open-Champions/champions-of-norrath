#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed msg_save__9AutoChestPc at 0x224ab8, 72 bytes. Writes, in order, the
// word at object+200, the halfword at +220, the bytes at +228 and +229, and
// the word at +216, packed without padding (4+2+1+1+4), and returns 12.
// Inferred: the return value is the number of bytes written. Unknown: the
// meaning of the five fields.
extern "C" int msg_save__9AutoChestPc(void* self, char* buffer) {
    using namespace rtti_c;
    const Address object = address(self);
    const Address out = address(buffer);
    write_word(out, read_word(object + 200u));
    write_u16(out + 4u, read_u16(object + 220u));
    write_u8(out + 6u, read_u8(object + 228u));
    write_u8(out + 7u, read_u8(object + 229u));
    write_word(out + 8u, read_word(object + 216u));
    return 12;
}
