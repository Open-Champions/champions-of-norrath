// _4item$itemTable at 0x32b320, 27456 bytes of initialized .data: 429 rows of 64 bytes, the last
// row's name word being zero. Observed: word 0 of each row is a name pointer, relocated here to
// a string literal; the other 15 words keep their observed bits (some are original-image
// addresses and are not relocated). The row values are the ones recorded for the items-player
// view of the same table; this unit defines the original symbol name over that data.
// Unknown: the meaning of most of the 15 words.
struct ItemObserved {
    const char* name;
    unsigned observed[15];
};
#if defined(__mips__)
static_assert(sizeof(ItemObserved) == 64, "item row");
#endif
extern "C" {
ItemObserved _4item$itemTable[] = {
#include "../items-player-item-table/rows.hpp"
};
}
#if defined(__mips__)
static_assert(sizeof(_4item$itemTable) == 27456, "itemTable extent");
#endif
