// Observed _4item$itemTable at 0x32b320, 429 rows of 64 bytes. The last name word is zero.
// Word 0 is the name pointer and is relocated to the string below.
// The other 15 words keep their observed bits. Some of those bits are original
// image addresses; no selected caller dereferences them, so they are not relocated.
struct ItemObserved {
    const char* name;
    unsigned observed[15];
};
static_assert(sizeof(ItemObserved) == 64, "item row");
extern "C" const ItemObserved norrath_item_table_view[] = {
#include "rows.hpp"
};
