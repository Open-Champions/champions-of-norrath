// Observed __16ListSafeIteratorP8ListHeadi at 0x194660, extent 160 (the
// constructor of ListSafeIterator). Stores the mask at +20, then walks the list
// from the head's first node for the first node whose payload is nonzero and
// whose payload flag word (+12) has no bit of the mask. A node with a zero
// payload is skipped. If one is found, the marker is linked directly after it
// (see safe_iterator.hpp); if not, both marker links are zeroed. The payload
// word of the marker is always zeroed last. The first candidate is tested with
// the incoming mask register and later ones with the stored word, which hold
// the same value. Returns nothing.
#include "../w8-object-list-math-common/safe_iterator.hpp"

extern "C" void __16ListSafeIteratorP8ListHeadi(ListSafeIterator* it, ListHead* head, int mask) {
    ListNode* node = head->first;
    it->mask = static_cast<unsigned>(mask);
    while (node && !iterator_accepts(node, it->mask)) node = node->next;
    if (node) {
        iterator_place_after(it, node);
    } else {
        it->link.next = nullptr;
        it->link.prev = nullptr;
    }
    it->link.value = nullptr;
}
