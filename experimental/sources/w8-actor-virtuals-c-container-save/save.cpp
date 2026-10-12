#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_save__9ContainerPc at 0x21d630, 192 bytes. The five string
// pointers at object+76, +80, +84, +88 and +92 are each passed, in that order, to
// objectSaveAddToStringPool; the five results are stored as consecutive words at
// the output pointer. Then the floats at object+28 and +32 are truncated to
// integers and their low 16 bits stored as two consecutive halfwords, and the word
// at object+104 follows. The return value is the number of bytes written, 28.
// Unknown: what the strings, the two floats and the word at +104 represent.
extern "C" int objectSaveAddToStringPool__FPCc(const char* text);

extern "C" int msg_save__9ContainerPc(void* self, char* out) {
    using namespace w8c;
    const Address base = address(self);
    Address dest = address(out);
    for (unsigned offset = 76u; offset <= 92u; offset += 4u) {
        write_word(dest, static_cast<unsigned>(objectSaveAddToStringPool__FPCc(
                             static_cast<const char*>(pointer(read_word(base + offset))))));
        dest += 4u;
    }
    const int first = static_cast<int>(read_single(base + 28u));
    const int second = static_cast<int>(read_single(base + 32u));
    write_u16(dest, static_cast<unsigned>(first));
    dest += 2u;
    write_u16(dest, static_cast<unsigned>(second));
    dest += 2u;
    write_word(dest, read_word(base + 104u));
    dest += 4u;
    return static_cast<int>(dest - address(out));
}
