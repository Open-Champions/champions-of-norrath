#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_save__7CounterPc at 0x21cd68, 24 bytes.
// The word at object+76 is stored at the buffer. The return is 4.
extern "C" int msg_save__7CounterPc(void* self, void* buffer) {
    using namespace vslots_b;
    write_word(address(buffer), read_word(address(self) + 76u));
    return 4;
}
