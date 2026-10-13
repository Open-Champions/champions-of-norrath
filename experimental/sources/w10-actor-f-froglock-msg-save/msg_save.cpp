#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_save__8FroglockPc at 0x002010e8, 84 bytes.
// Runs Creature's msg_save on the object and the buffer, then appends two words at the returned
// count: the word at object+960 and the word at object+208. Returns the count plus eight.
// msg_load__8FroglockPc reads the same two words back.
extern "C" int msg_save__8CreaturePc(void* self, char* data);
extern "C" int msg_save__8FroglockPc(void* self, char* data) {
    using namespace vslots_b;
    const int count = msg_save__8CreaturePc(self, data);
    const Address out = address(data) + static_cast<unsigned>(count);
    write_word(out, read_word(address(self) + 960u));
    write_word(out + 4u, read_word(address(self) + 208u));
    return count + 8;
}
