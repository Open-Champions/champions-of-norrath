// Observed next__16ListSafeIterator at 0x194750, extent 124. Finds the next
// acceptable node after the marker (same test as the constructor, using the
// stored mask), unlinks the marker from its place, and relinks it directly
// after the found node; with none found both marker links become zero. The
// unlink writes through the marker's previous node without testing it, so it
// must still be in a list: advancing an exhausted marker faults. Returns nothing.
#include "../w8-object-list-math-common/safe_iterator.hpp"

extern "C" void next__16ListSafeIterator(ListSafeIterator* it) {
    ListNode* node = it->link.next;
    ListNode* successor = node;
    while (node && !iterator_accepts(node, it->mask)) node = node->next;
    ListNode* previous = it->link.prev;
    if (successor) {
        successor->prev = previous;
        successor = it->link.next;
        previous = it->link.prev;
    }
    previous->next = successor;
    if (node) {
        iterator_place_after(it, node);
    } else {
        it->link.prev = nullptr;
        it->link.next = nullptr;
    }
}
