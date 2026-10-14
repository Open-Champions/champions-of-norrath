#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed msg_save__8AutoPropPc at 0x224d00, 24 bytes. Writes the word at
// object+376 to the buffer and returns 4.
// Inferred: the return value is the number of bytes written. Unknown: the
// meaning of the field at +376.
extern "C" int msg_save__8AutoPropPc(void* self, char* buffer) {
    using namespace rtti_c;
    write_word(address(buffer), read_word(address(self) + 376u));
    return 4;
}
