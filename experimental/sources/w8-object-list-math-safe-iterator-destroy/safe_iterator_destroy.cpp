// Observed _$_16ListSafeIterator at 0x194700, extent 80 (the destructor of
// ListSafeIterator). Unlinks the marker: the following node's previous word
// takes the marker's previous word, then the previous node's next word takes
// the marker's next word (each only when that neighbor exists). When bit 0 of
// the second argument is set the marker's storage is released through the
// tail-called __builtin_delete at 0x2fedf0; the authored provider for that
// call is norrath_animation_state_deallocate, used here so that no new import
// is opened. The marker's own fields are left as they were.
#include "../w8-object-list-math-common/safe_iterator.hpp"

extern "C" void norrath_animation_state_deallocate(void* allocation);

extern "C" void _$_16ListSafeIterator(ListSafeIterator* it, int flags) {
    if (it->link.next) it->link.next->prev = it->link.prev;
    if (it->link.prev) it->link.prev->next = it->link.next;
    if (flags & 1) norrath_animation_state_deallocate(it);
}
