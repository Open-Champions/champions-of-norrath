// Observed getModel__10AutoObject at 0x00314de8, 8 bytes (weak symbol).
// Returns the word stored at object+144 and reads nothing else.
// The same word is the model argument msg_draw__10AutoObject passes to the model draw call.

namespace {
typedef unsigned Word __attribute__((may_alias));
}

extern "C" unsigned getModel__10AutoObject(void* self) {
    return *reinterpret_cast<const Word*>(static_cast<const unsigned char*>(self) + 144);
}
