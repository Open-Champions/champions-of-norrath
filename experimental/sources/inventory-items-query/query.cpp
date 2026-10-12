// Observed queryInventoryItem__FPci at 0x001b1b68, 184 bytes.
// Player records are 23984 bytes apart. Three lists start at record+328, each
// 4616 bytes: 64 slots of 72 bytes, then a signed count at list+4608. A count
// at or below zero skips the list. Slot+0 is a pointer to a description whose
// first word is the name passed to strcmp. A match adds the signed halfword at
// slot+4. The result is the sum across all three lists.
// Unknown: the slot fields past offset 6 and the meaning of the lists.
#include "../inventory-items-record/record.hpp"
extern "C" int strcmp(const char* left, const char* right);

extern "C" int queryInventoryItem__FPci(char* name, int owner) {
    using namespace inventory_items;
    int sum = 0;
    const List* list = player_lists(owner);
    for (int pass = 0; pass < 3; ++pass, ++list) {
        for (int index = 0; index < list->count; ++index) {
            const Slot& slot = list->slots[index];
            if (strcmp(item_name(slot), name) == 0) sum += slot.quantity;
        }
    }
    return sum;
}
