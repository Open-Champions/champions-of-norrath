#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed msg_save__7TriggerPc at 0x1d1ab8, 36 bytes. Writes the word at
// object+100 to the buffer, then the byte at object+96 right after it, and
// returns the number of bytes written (5).
// Inferred: the buffer is the save stream cursor passed by the caller, so the
// return value advances it. Unknown: what the two fields hold.
extern "C" int msg_save__7TriggerPc(void* self, char* buffer) {
    using namespace rtti_c;
    const Address object = address(self);
    const Address out = address(buffer);
    write_word(out, read_word(object + 100u));
    write_u8(out + 4u, read_u8(object + 96u));
    return 5;
}
