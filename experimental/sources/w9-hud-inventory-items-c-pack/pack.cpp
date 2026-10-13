// Observed pack__4itemPQ24item11description at 0x001083e8, 28 bytes.
// Reads the first word of the record it is given, subtracts the address of the
// item table (_4item$itemTable, 0x0032b320, 64-byte rows) and stores the
// difference shifted right by 6 (arithmetic) back into the same word, so the
// row pointer becomes a row index. Returns nothing.
// Inferred: the argument is the 72-byte slot-shaped record the inventory code
// keeps (slot+0 points at an item-table row); the matching routine is unpack.
// The table is imported under the name the item-table view unit defines.
#include "../inventory-items-record/record.hpp"
extern "C" unsigned char norrath_item_table_view[];

extern "C" void pack__4itemPQ24item11description(inventory_items::Slot* record) {
    record->description =
        static_cast<unsigned>(static_cast<int>(record->description - inventory_items::word_of(norrath_item_table_view)) >> 6);
}
