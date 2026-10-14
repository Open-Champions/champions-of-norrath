#include "../w10-actor-l-common/access.hpp"
// Observed msg_trigger__5Rondoi at 0x00233238, 72 bytes. Acts only when the incoming integer is
// zero. Then it stores 2 in the word at object+244, asks modelGetActiveChunks for the 64-bit
// mask of the model whose address is at +12 of the group at object+36 (second argument 2),
// stores 1 in the word at +964 and the mask in the doubleword at +552. Nothing is returned.
// The store to +244 precedes the call. Unknown: what states 2 and 1 mean. Nothing here ran on
// the EE.
extern "C" {
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which);

void msg_trigger__5Rondoi(void* self, int value) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (value != 0) return;
    w10l::word(o, 244) = 2;
    const unsigned long long chunks = modelGetActiveChunks__FP12_modelHeaderi(
        w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), 12)), 2);
    w10l::word(o, 964) = 1;
    w10l::dword(o, 552) = chunks;
}
}
