#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_load__9GeneratorPc at 0x001f20b0, 36 bytes. Reads two words from the buffer: the low 16 bits
// of the first go to the halfword at object+572 and the second to the word at object+976. Returns 8.
// The buffer is not otherwise inspected and no other object byte changes.
extern "C" int msg_load__9GeneratorPc(void* self, char* data) {
    using namespace vslots_b;
    const unsigned first = read_word(address(data));
    const unsigned second = read_word(address(data) + 4u);
    write_u16(address(self) + 572u, first);
    write_word(address(self) + 976u, second);
    return 8;
}
