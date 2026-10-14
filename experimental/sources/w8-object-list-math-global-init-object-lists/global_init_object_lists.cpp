// Observed _GLOBAL_$I$objectIgnoreFlags at 0x144098 and _GLOBAL_$D$objectIgnoreFlags
// at 0x1440b8 (32 bytes each): the compiler-made static-initialization and
// destruction entries of the object translation unit. They call the unit's local
// __static_initialization_and_destruction_0 (0x143160, 0x334 bytes) with
// (1, 0xffff) and (0, 0xffff). Both are authored here because they share that
// local function.
//
// With priority 0xffff and initialize == 1 the function clears, in this order:
// runList's head word, the 15 medium run-list heads, the 60 slow run-list heads,
// deleteList's head word, alwaysDrawList's head word, the 120 x 120 objectGrid
// heads, the 120 x 120 ghostObjectGrid heads, and offTheGrid. With initialize == 0
// it destroys the same objects in the reverse order by calling listDestroy on each
// head: offTheGrid, ghostObjectGrid from the last head to the first, objectGrid
// likewise, alwaysDrawList, deleteList, the slow heads from the last to the first,
// the medium heads likewise, then runList. Any other priority does nothing; any
// other initialize value touches nothing. The clearing is the inlined ListHead
// constructor; listDestroy is its destructor. Neither entry returns a value.
// Unknown: why objectIgnoreFlags, a variable defined elsewhere, names this unit.
#include "../w8-object-list-math-common/list_types.hpp"

struct HeadSlot {
    ListHead head;
    unsigned unobserved_word;
};

extern "C" {
extern HeadSlot runList;
extern HeadSlot deleteList;
extern HeadSlot alwaysDrawList;
extern ListHead mediumRunList[16];
extern ListHead slowRunList[60];
extern ListHead objectGrid[120][120];
extern ListHead ghostObjectGrid[120][120];
extern ListHead offTheGrid;
void listDestroy__FP8ListHead(ListHead* head);
}

namespace {
template <int Count>
void clear_forward(ListHead (&heads)[Count], int count) {
    for (int i = 0; i < count; ++i) heads[i].first = nullptr;
}

template <int Count>
void destroy_backward(ListHead (&heads)[Count], int count) {
    for (int i = count - 1; i >= 0; --i) listDestroy__FP8ListHead(&heads[i]);
}

__attribute__((noipa)) void static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        runList.head.first = nullptr;
        clear_forward(mediumRunList, 15);
        clear_forward(slowRunList, 60);
        deleteList.head.first = nullptr;
        alwaysDrawList.head.first = nullptr;
        for (int row = 0; row < 120; ++row) clear_forward(objectGrid[row], 120);
        for (int row = 0; row < 120; ++row) clear_forward(ghostObjectGrid[row], 120);
        offTheGrid.first = nullptr;
    }
    if (initialize == 0) {
        listDestroy__FP8ListHead(&offTheGrid);
        for (int row = 119; row >= 0; --row) destroy_backward(ghostObjectGrid[row], 120);
        for (int row = 119; row >= 0; --row) destroy_backward(objectGrid[row], 120);
        listDestroy__FP8ListHead(&alwaysDrawList.head);
        listDestroy__FP8ListHead(&deleteList.head);
        destroy_backward(slowRunList, 60);
        destroy_backward(mediumRunList, 15);
        listDestroy__FP8ListHead(&runList.head);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$objectIgnoreFlags() {
    static_initialization_and_destruction_0(1, 0xffff);
}

extern "C" void _GLOBAL_$D$objectIgnoreFlags() {
    static_initialization_and_destruction_0(0, 0xffff);
}
