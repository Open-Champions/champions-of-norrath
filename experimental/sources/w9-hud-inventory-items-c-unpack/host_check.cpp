// Host check for unpack__4itemPQ24item11description, including the round trip with pack.
#include "../inventory-items-record/record.hpp"
#include <initializer_list>
unsigned char player[1];
extern "C" unsigned char norrath_item_table_view[];
unsigned char norrath_item_table_view[64 * 429];
extern "C" void pack__4itemPQ24item11description(inventory_items::Slot*);
extern "C" void unpack__4itemPQ24item11description(inventory_items::Slot*);
int main() {
    using namespace inventory_items;
    const unsigned base = word_of(norrath_item_table_view);
    int bad = 0;
    for (int row : {0, 1, 2, 63, 428, -1, -9}) {
        Slot slot = {};
        slot.description = static_cast<unsigned>(row);
        slot.quantity = 7; slot.index = 9;
        unpack__4itemPQ24item11description(&slot);
        if (slot.description != base + 64u * static_cast<unsigned>(row) || slot.quantity != 7 || slot.index != 9) bad = 1;
        pack__4itemPQ24item11description(&slot);
        if (static_cast<int>(slot.description) != row) bad = 2;
    }
    return bad;
}
