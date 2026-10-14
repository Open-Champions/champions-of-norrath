// Observed getModel__11VampireLord at 0x00243800, 68 bytes. When the word at object+964 is
// nonzero it returns lumpFindResource__FPcT0("bat.lmp", "bat.vif") as a tail call. Otherwise it
// returns the word at offset 12 of the block that the word at object+36 points to.
// Unknown: what the +964 flag means. Both results are read as pointer-sized words. Nothing here
// ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* resource);

extern "C" void* getModel__11VampireLord(void* self) {
    using namespace w10p;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 964) != 0u) return lumpFindResource__FPcT0("bat.lmp", "bat.vif");
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word(from_word(word(o, 36)), 12)));
}
