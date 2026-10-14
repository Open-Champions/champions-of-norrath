#include "../w10-actor-q-common/q.hpp"
// Observed leaveCommander__14WoodElfSoldier at 0x00249e10, 80 bytes.
// Reads the word at object+960. When it is zero nothing happens. Otherwise, with n the node it
// designates, calls listDestroy__FP8ListHead on the head at (word at n+12) + 960 and clears that
// head's word, then calls listDestroy__FP8ListHead on the head at object+960 and clears that
// word. Nothing is returned.
// Inferred: the word at n+12 is the commander object, so both heads are the +960 list of an
// object. Unknown: what the list holds. Nothing here ran on the EE.
extern "C" {
void listDestroy__FP8ListHead(void* head);
}

extern "C" void leaveCommander__14WoodElfSoldier(void* self) {
    using namespace w10q;
    const unsigned node = word(self, 960);
    if (node == 0u) return;
    unsigned char* own = bytes(self) + 960;
    unsigned char* other = link(from_word(node), 12) + 960;
    listDestroy__FP8ListHead(other);
    *reinterpret_cast<Word*>(other) = 0u;
    listDestroy__FP8ListHead(own);
    *reinterpret_cast<Word*>(own) = 0u;
}
