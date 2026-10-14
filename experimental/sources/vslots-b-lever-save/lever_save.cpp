#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_save__5LeverPc at 0x21c9a0, 60 bytes.
// Four bytes at object+76, the word at object+12, and bytes 80 and 81 are
// copied in that order. The return is 10. The four bytes move as a bit
// pattern; this is not a numeric conversion.
extern "C" int msg_save__5LeverPc(void* self, void* buffer) {
    using namespace vslots_b;
    const Address base = address(self);
    const Address out = address(buffer);
    write_word(out, read_word(base + 76u));
    write_word(out + 4u, read_word(base + 12u));
    write_u8(out + 8u, read_u8(base + 80u));
    write_u8(out + 9u, read_u8(base + 81u));
    return 10;
}
