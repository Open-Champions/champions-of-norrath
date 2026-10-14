#include "../w11-actor-c-common/c.hpp"
// Observed convertToBat__11VampireLord at 0x002436d8, 108 bytes. With v the object, in the order of the image:
//   the word at v+964 becomes 1; the byte at v+292 (inside the animation state at v+288) becomes 0;
//   the "bat_move.anm" animation is looked up in the "bat.lmp" lump and added to the state at v+288 with
//   animAdd__FP14AnimationStateP15AnimationHeaderi (flags 0x801);
//   bit 0 of the word at v+12 is set, and bit 20 (0x100000) of the 64-bit word at v+160 is set.
// Inferred: the +964 word is the "is a bat" mode that convertToLord__11VampireLord clears again, and the
// two flag bits mark the bat form. Unknown: what the flag bits control. Nothing here ran on the EE.
extern "C" {
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
}

extern "C" void convertToBat__11VampireLord(void* self) {
    using namespace w11c;
    word(self, 964) = 1u;
    ubyte(self, 292) = 0;
    animAdd__FP14AnimationStateP15AnimationHeaderi(bytes(self) + 288, lumpFindResource__FPcT0("bat.lmp", "bat_move.anm"), 0x801);
    word(self, 12) |= 1u;
    *reinterpret_cast<Double*>(bytes(self) + 160) |= 0x100000ull;
}
