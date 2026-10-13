#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_save__5DemonPc at 0x1eb3b8, 72 bytes.
// Runs Creature's msg_save on the object and the buffer. The returned count is the offset
// at which the Demon appends the word at object+208; the routine returns that count plus four.
// msg_load__5DemonPc reads the same word back to object+208.
extern "C" int msg_save__8CreaturePc(void* self, char* data);
extern "C" int msg_save__5DemonPc(void* self, char* data) {
    using namespace vslots_b;
    const int count = msg_save__8CreaturePc(self, data);
    write_word(address(data) + static_cast<unsigned>(count), read_word(address(self) + 208u));
    return count + 4;
}
