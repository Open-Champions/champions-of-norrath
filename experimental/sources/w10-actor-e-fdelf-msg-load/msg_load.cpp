#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_load__13FemaleDarkElfPc at 0x289548, 128 bytes.
// Runs Creature's msg_load on the object and the buffer, then reads four words at the returned
// count: the low byte of the first goes to object+960, the second to +964, the third to +1104
// and the fourth to +208. initType__13FemaleDarkElfb(object, true) runs after the last store.
// Returns the count plus sixteen. Unknown: what initType does with the stored values.
extern "C" int msg_load__8CreaturePc(void* self, char* data);
extern "C" void initType__13FemaleDarkElfb(void* self, bool flag);
extern "C" int msg_load__13FemaleDarkElfPc(void* self, char* data) {
    using namespace vslots_b;
    const int count = msg_load__8CreaturePc(self, data);
    const Address in = address(data) + static_cast<unsigned>(count);
    write_u8(address(self) + 960u, read_word(in));
    write_word(address(self) + 964u, read_word(in + 4u));
    write_word(address(self) + 1104u, read_word(in + 8u));
    write_word(address(self) + 208u, read_word(in + 12u));
    initType__13FemaleDarkElfb(self, true);
    return count + 16;
}
