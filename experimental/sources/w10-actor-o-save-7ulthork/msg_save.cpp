// Observed msg_save__7UlthorkPc at 0x001f61e8, 84 bytes. Calls msg_save__8CreaturePc(object, buffer),
// which returns a byte count n. Then it stores the 32-bit word at object+960 at buffer+n and the
// sign-extended byte at object+1104 at buffer+n+4, and returns n + 8.
// Unknown: what the word at +960 and the byte at +1104 mean (the draw routine uses them as indexes
// into the model group at the word at +36). Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
int msg_save__8CreaturePc(void* self, char* buffer);

int msg_save__7UlthorkPc(void* self, char* buffer) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int n = msg_save__8CreaturePc(self, buffer);
    *reinterpret_cast<unsigned*>(buffer + n) = word(o, 960);
    *reinterpret_cast<int*>(buffer + n + 4) = static_cast<signed char>(o[1104]);
    return n + 8;
}
}
