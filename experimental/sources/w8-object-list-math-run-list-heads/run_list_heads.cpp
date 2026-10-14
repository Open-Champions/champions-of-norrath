// Zero-initialised list-head storage, sized by the distance to the next symbol
// in the private image's symbol table (an extent, not a type from debug info).
// All four live in .bss; the file image holds no bytes for them.
//   runList        0x486878, 8 bytes. objectAddToRunList passes its address (gp-4472).
//   deleteList     0x486880, 8 bytes. objectAddToDeleteList passes its address (gp-4464).
//   mediumRunList  0x4fbf90, 64 bytes. Fifteen one-word heads are indexed by the
//                  medium cycle word (0..14); the last word is not reached.
//   slowRunList    0x4fbfd0, 240 bytes. Sixty one-word heads, walked by
//                  objectIterateRunLists.
// The second word of runList and deleteList is not touched by any routine in this
// package; it is kept so the extent matches. Unknown: the declared types.
#include "../w8-object-list-math-common/list_types.hpp"

struct HeadSlot {
    ListHead head;
    unsigned unobserved_word;
};

extern "C" {
HeadSlot runList;
HeadSlot deleteList;
ListHead mediumRunList[16];
ListHead slowRunList[60];
}

#if defined(__mips__)
static_assert(sizeof(HeadSlot) == 8 && sizeof(mediumRunList) == 64 && sizeof(slowRunList) == 240, "extents");
#endif
