#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_load__7CounterPc at 0x21cd80, 24 bytes.
// The first buffer word is stored at object+76. The return is 4.
extern "C" int msg_load__7CounterPc(void* self, void* buffer) {
    using namespace vslots_b;
    write_word(address(self) + 76u, read_word(address(buffer)));
    return 4;
}
