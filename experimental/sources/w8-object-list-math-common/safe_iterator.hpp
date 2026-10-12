#pragma once
#include "object_types.hpp"
// Observed layout of the list iterator (24 bytes; callers reserve 32-byte slots
// on the stack). It is itself a list node: while valid it sits in the list right
// after the node it stands for, with a zero payload word so other walkers skip it.
//   +0  next    node after the marker (zero when exhausted)
//   +4  current the node the marker follows, which is the iterator's "current"
//               (callers read it from +4 and then the object from its +12)
//   +12 payload always stored as zero
//   +20 mask    the object flag bits that exclude an object from the walk
struct ListSafeIterator {
    ListNode link;
    unsigned mask;
};
#if defined(__mips__)
static_assert(sizeof(ListSafeIterator) == 24 && __builtin_offsetof(ListSafeIterator, mask) == 20, "iterator extent");
#endif

// True when the node can be returned: it has a payload object and none of the
// mask bits are set in that object's flag word.
inline bool iterator_accepts(const ListNode* node, unsigned mask) {
    const GameObjectLinks* object = static_cast<const GameObjectLinks*>(node->value);
    return object != nullptr && (object->flags & mask) == 0;
}

// Puts the marker directly after the chosen node.
inline void iterator_place_after(ListSafeIterator* it, ListNode* chosen) {
    ListNode* following = chosen->next;
    it->link.prev = chosen;
    it->link.next = following;
    if (following) following->prev = &it->link;
    it->link.prev->next = &it->link;
}
