#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_save__7TriggerPc at 0x1d1ab8, 36 bytes. The word at object+100 is
// stored at the output pointer, the byte at object+96 follows it, and the return
// value is the number of bytes written, 5. Unknown: the meaning of both fields.
extern "C" int msg_save__7TriggerPc(void* self, char* out) {
    using namespace w8c;
    const Address base = address(self);
    const Address dest = address(out);
    write_word(dest, read_word(base + 100u));
    write_u8(dest + 4u, read_u8(base + 96u));
    return 5;
}
