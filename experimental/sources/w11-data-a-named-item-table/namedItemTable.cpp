// _4item$namedItemTable at 0x31fbe0, 46912 bytes of initialized .data: 733 rows of 64 bytes, the last
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
ItemObserved _4item$namedItemTable[] = {
#include "../items-player-named-item-table/rows.hpp"
};
}
#if defined(__mips__)
static_assert(sizeof(_4item$namedItemTable) == 46912, "namedItemTable extent");
#endif
