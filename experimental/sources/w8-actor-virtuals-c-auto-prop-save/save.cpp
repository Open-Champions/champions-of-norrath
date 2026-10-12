#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_save__8AutoPropPc at 0x224d00, 24 bytes. The word at object+376
// (the state that setState__8AutoPropi stores) is stored at the output pointer
// and the return value is the number of bytes written, 4.
extern "C" int msg_save__8AutoPropPc(void* self, char* out) {
    using namespace w8c;
    write_word(address(out), read_word(address(self) + 376u));
    return 4;
}
