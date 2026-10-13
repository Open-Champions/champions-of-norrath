// Observed msg_poke__11AutoChestLPi at 0x225430, 84 bytes. If the word at object+216 is
// zero and the pointer word at object+248 is nonzero, it writes into that second
// object: 1 as a byte at +160, 1 at +92, 1 at +96, and at +156 the sum of the global
// word frameNm and the word at +144. Then, always, it calls msg_poke__9AutoChesti
// (defined elsewhere) with the original arguments.
// Unknown: what the second object is, what those fields mean, and what frameNm
// counts here.

#include "../w8-actor-virtuals-b-common/virtual_call.hpp"

extern "C" void msg_poke__9AutoChesti(void* object, int value);
// The 4-byte word the original reads at gp-13644 is the symbol frameNm (0x4844a4).
extern "C" unsigned frameNm;

using w8avb::UWord;

extern "C" void msg_poke__11AutoChestLPi(void* self, int value) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<UWord*>(object + 216) == 0) {
        const unsigned other_word = *reinterpret_cast<UWord*>(object + 248);
        if (other_word != 0) {
            unsigned char* other = const_cast<unsigned char*>(w8avb::address_of(other_word));
            unsigned base = *reinterpret_cast<UWord*>(other + 144);
            other[160] = 1;
            *reinterpret_cast<UWord*>(other + 92) = 1;
            *reinterpret_cast<UWord*>(other + 96) = 1;
            *reinterpret_cast<UWord*>(other + 156) = frameNm + base;
        }
    }
    msg_poke__9AutoChesti(self, value);
}
