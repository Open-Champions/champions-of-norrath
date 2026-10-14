// Observed unpack__4itemPQ24item11description at 0x00108408, 28 bytes.
// Reads the first word of the record it is given, shifts it left by 6 and adds
// the address of the item table (_4item$itemTable, 0x0032b320, 64-byte rows),
// storing the sum back into the same word: a row index becomes a row pointer.
// Returns nothing. Inferred: the inverse of pack for the 72-byte slot-shaped
// record the inventory code keeps.
#include "../inventory-items-record/record.hpp"
extern "C" unsigned char norrath_item_table_view[];

extern "C" void unpack__4itemPQ24item11description(inventory_items::Slot* record) {
    record->description = (record->description << 6) + inventory_items::word_of(norrath_item_table_view);
}
