// Observed inventoryGetItemIndex__FiPcPiT2 at 0x001b1d78, 212 bytes.
// The third argument is stored as -1 before any list is read. For each of the
// player's three lists, in order, every slot below the list count is tested
// with strstr, called with the caller's string as the haystack and the slot's
// item name as the needle. A non-null result stores the slot number through
// the third argument and the list number through the fourth. Both stores
// repeat for later matches, so the last match wins. The fourth argument is
// untouched when nothing matches. There is no return value.
// Unknown: what the callers use the list number for.
#include "../inventory-items-record/record.hpp"
extern "C" char* strstr(const char* haystack, const char* needle);

extern "C" void inventoryGetItemIndex__FiPcPiT2(int owner, char* text, int* slot_out, int* list_out) {
    using namespace inventory_items;
    *slot_out = -1;
    const List* list = player_lists(owner);
    for (int pass = 0; pass < 3; ++pass, ++list) {
        for (int index = 0; index < list->count; ++index) {
            if (strstr(text, item_name(list->slots[index])) != nullptr) {
                *slot_out = index;
                *list_out = pass;
            }
        }
    }
}
