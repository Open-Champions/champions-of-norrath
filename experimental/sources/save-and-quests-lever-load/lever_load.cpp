#include "../save-and-quests-bytes/bytes.hpp"
// Observed msg_load__5LeverPc at 0x0021c9e0, 112 bytes; the counterpart of
// msg_save__5LeverPc (10 bytes). The buffer holds a four-byte value stored at
// object+76, a word stored at object+12, a byte stored at object+80 and a byte
// stored at object+81, in that order. The first value moves as a bit pattern.
// When the byte stored at object+81 is nonzero the object is added to the run
// list (objectAddToRunList__FP10GameObject, not defined here). The return is
// the number of bytes consumed, 10.
extern "C" {
void objectAddToRunList__FP10GameObject(void* object);

int msg_load__5LeverPc(void* self, char* buffer) {
    using namespace sq;
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned char* in = reinterpret_cast<const unsigned char*>(buffer);
    write_word(object + 76, read_word(in));
    write_word(object + 12, read_word(in + 4));
    object[80] = in[8];
    object[81] = in[9];
    if (in[9] != 0) objectAddToRunList__FP10GameObject(self);
    return 10;
}
}
