// Observed deleteCommander__3Orc at 0x0022b810, 108 bytes. Walks the twenty heads of the class list
// _3Orc$m_commanderList (0x0042ce10, 80 bytes) in increasing address order (the loop counter runs from 19
// down to 0). For every head that is non-zero and whose first node's word at +12
// equals the object, listDestroy__FP8ListHead is called with the address of that head; the walk then goes
// on (a later head can match again). Nothing is returned.
// Inferred: the heads are commander slots and +12 of a node is the commander object. Unknown: what a node
// holds beyond that. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern unsigned Orc_commanderList[20] __asm__("_3Orc$m_commanderList");
void listDestroy__FP8ListHead(void* head);
}

extern "C" void deleteCommander__3Orc(void* self) {
    using namespace w11a;
    unsigned* head = Orc_commanderList;
    for (int n = 19; n >= 0; --n, ++head) {
        const unsigned node = *head;
        if (node != 0u && word(from_word(node), 12) == to_word(self))
            listDestroy__FP8ListHead(head);
    }
}
