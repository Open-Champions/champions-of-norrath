// Observed objectCancelAllAlwaysDraw__Fv at 0x1434d8, extent 124.
// Walks alwaysDrawList (0x486888) with a ListSafeIterator whose mask is
// objectIgnoreFlags (0x483ac0). For each node the payload object has bit 1
// (0x2) of its flag word at +12 cleared; the store happens in the delay slot of
// the call that advances the iterator. The current node is re-read from the
// iterator after each advance. The iterator is destroyed with flags 2 and the
// list is then emptied with listDestroy.
#include "../w8-object-list-math-common/safe_iterator.hpp"

struct HeadSlot {
    ListHead head;
    unsigned unobserved_word;
};

extern "C" {
void __16ListSafeIteratorP8ListHeadi(ListSafeIterator*, ListHead*, int);
void next__16ListSafeIterator(ListSafeIterator*);
void _$_16ListSafeIterator(ListSafeIterator*, int);
void listDestroy__FP8ListHead(void* head);
extern int objectIgnoreFlags;
extern HeadSlot alwaysDrawList;
}

extern "C" void objectCancelAllAlwaysDraw__Fv() {
    ListSafeIterator iterator;
    __16ListSafeIteratorP8ListHeadi(&iterator, &alwaysDrawList.head, objectIgnoreFlags);
    for (ListNode* node = iterator.link.prev; node != nullptr; node = iterator.link.prev) {
        GameObjectLinks* object = static_cast<GameObjectLinks*>(node->value);
        object->flags &= ~2u;
        next__16ListSafeIterator(&iterator);
    }
    _$_16ListSafeIterator(&iterator, 2);
    listDestroy__FP8ListHead(&alwaysDrawList.head);
}
