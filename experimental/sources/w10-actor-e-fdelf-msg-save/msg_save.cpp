#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_save__13FemaleDarkElfPc at 0x2894d8, 108 bytes.
// Runs Creature's msg_save on the object and the buffer, then appends four words at the returned
// count: the sign-extended byte at object+960, the words at +964, +1104 and +208, in that order.
// Returns the count plus sixteen. msg_load__13FemaleDarkElfPc reads the same four words back.
extern "C" int msg_save__8CreaturePc(void* self, char* data);
extern "C" int msg_save__13FemaleDarkElfPc(void* self, char* data) {
    using namespace vslots_b;
    const int count = msg_save__8CreaturePc(self, data);
    const Address out = address(data) + static_cast<unsigned>(count);
    const unsigned type = read_u8(address(self) + 960u);
    write_word(out, type < 0x80u ? type : type | 0xffffff00u);
    write_word(out + 4u, read_word(address(self) + 964u));
    write_word(out + 8u, read_word(address(self) + 1104u));
    write_word(out + 12u, read_word(address(self) + 208u));
    return count + 16;
}
