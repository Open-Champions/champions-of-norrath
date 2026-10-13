#include "../w11-actor-c-common/c.hpp"
// Observed breakWeb__3Web at 0x0020fb88, 52 bytes. With w the web: when the word at w+12 is nonzero and
// the pointer word at w+28 is nonzero, take p = the pointer stored at (w+28)+12 and, if bit 5 (0x20) of
// the word at p+12 is set, store zero in the word at p+756. In every case the word at w+44 (the word
// bActive__3Web reads) is then cleared.
// Inferred: w+28 refers to the creature caught in the web and p+756 is a field of it; the branch
// order and offsets are observed, the meanings are not. Nothing here ran on the EE.
extern "C" void breakWeb__3Web(void* self) {
    using namespace w11c;
    if (word(self, 12) != 0u && word(self, 28) != 0u) {
        unsigned char* p = link(link(self, 28), 12);
        if ((word(p, 12) & 0x20u) != 0u) word(p, 756) = 0u;
    }
    word(self, 44) = 0u;
}
