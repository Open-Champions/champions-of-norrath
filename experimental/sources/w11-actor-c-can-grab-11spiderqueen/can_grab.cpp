#include "../w11-actor-c-common/c.hpp"
// Observed canGrab__11SpiderQueen at 0x0020fa78, 80 bytes. Returns 1 only when all of the following hold,
// otherwise 0 (v the object):
//   the word at v+964 minus 16 is not (unsigned) below 2, i.e. the state is neither 16 nor 17;
//   the pointer word at v+576 is nonzero; with q the pointer stored at (v+576)+12, bit 5 (0x20) of the
//   word at q+12 is set and the float at q+756 equals 0.0.
// Inferred: v+964 is the state number (the word that setState__11SpiderQueeni stores) and the v+576
// block is the target the queen may seize. Unknown: what flag 0x20 and the float at +756 mean on the
// target (breakWeb__3Web clears the same word, so +756 belongs to the same field). Nothing here ran on the EE.
extern "C" int canGrab__11SpiderQueen(void* self) {
    using namespace w11c;
    if (word(self, 964) - 16u < 2u) return 0;
    if (word(self, 576) == 0u) return 0;
    unsigned char* q = link(link(self, 576), 12);
    if ((word(q, 12) & 0x20u) == 0u) return 0;
    return real(q, 756) == 0.0f;
}
