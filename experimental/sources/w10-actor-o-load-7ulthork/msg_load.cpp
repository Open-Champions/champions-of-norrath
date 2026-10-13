// Observed msg_load__7UlthorkPc at 0x001f6240, 168 bytes. Calls msg_load__8CreaturePc(object, buffer),
// which returns a byte count n. Then, with a and b the 32-bit words at buffer+n and buffer+n+4:
//  - the word at object+960 becomes a; the byte at +156 and the word at +244 become 0;
//  - the byte at +1104 becomes the low byte of b; when that byte, as a signed value, is 2, the byte at
//    +156 becomes 6 and the word at +244 becomes 1;
//  - the 64-bit result of modelGetActiveChunks(model, word at +244) is stored at +552, where model is
//    the word at (the word at +36) + 12;
//  - when the signed halfword at +572 is 10, the word at +960 becomes 0.
// Returns n + 8.
// Unknown: what the fields mean. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
int msg_load__8CreaturePc(void* self, char* buffer);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which);

int msg_load__7UlthorkPc(void* self, char* buffer) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int n = msg_load__8CreaturePc(self, buffer);
    word(o, 960) = *reinterpret_cast<unsigned*>(buffer + n);
    const unsigned b = *reinterpret_cast<unsigned*>(buffer + n + 4);
    o[156] = 0;
    word(o, 244) = 0;
    o[1104] = static_cast<unsigned char>(b);
    if (static_cast<signed char>(b) == 2) {
        o[156] = 6;
        word(o, 244) = 1;
    }
    dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(from_word(word(o, 36)), 12)), sword(o, 244));
    if (half(o, 572) == 10) word(o, 960) = 0;
    return n + 8;
}
}
