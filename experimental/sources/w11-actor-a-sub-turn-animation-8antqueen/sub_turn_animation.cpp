// Observed subTurnAnimation__8AntQueen at 0x00208b68, 128 bytes. Let R be the block at the word object+36 and
// n the signed byte at object+292 (re-read on every pass). For i from 0 while i < n, the word at
// object+296+32*i is compared with the words R+180, R+68, R+188 and R+76; if it equals any of them, the float
// at object+308+32*i is set to the constant with bit pattern 0xbd088888 (-0.0333333, one of the image's
// gp-relative constants at 0x00481360). Nothing else is touched. The records are 32 bytes apart; what the
// four R words designate (they look like animation numbers) is inferred, not observed.
// Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" void subTurnAnimation__8AntQueen(void* self) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const r = from_word(word(o, 36));
    for (int i = 0; i < static_cast<signed char>(o[292]); ++i) {
        const unsigned id = word(o, 296 + 32 * i);
        if (id == word(r, 180) || id == word(r, 68) || id == word(r, 188) || id == word(r, 76))
            real(o, 308 + 32 * i) = bits(0xbd088888u);
    }
}
