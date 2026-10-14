// Host check for pack__4itemPQ24item11description: row pointer words become row indices.
#include "../inventory-items-record/record.hpp"
#include <cstdio>
unsigned char player[1];
extern "C" unsigned char norrath_item_table_view[];
unsigned char norrath_item_table_view[64 * 429];
extern "C" void pack__4itemPQ24item11description(inventory_items::Slot*);
int main() {
    using namespace inventory_items;
    const unsigned base = word_of(norrath_item_table_view);
    const int rows[] = {0, 1, 2, 63, 64, 428, -1, -5};
    int bad = 0;
    for (int row : rows) {
        Slot slot = {};
        slot.description = base + 64u * row;
        slot.quantity = 7; slot.index = 9;
        pack__4itemPQ24item11description(&slot);
        if (static_cast<int>(slot.description) != row || slot.quantity != 7 || slot.index != 9) {
            std::printf("row %d -> %d\n", row, static_cast<int>(slot.description)); bad = 1;
        }
    }
    // A pointer that is not on a row boundary truncates toward minus infinity (arithmetic shift).
    Slot odd = {}; odd.description = base + 64u * 3 + 63;
    pack__4itemPQ24item11description(&odd);
    if (odd.description != 3) bad = 2;
    odd.description = base - 1;
    pack__4itemPQ24item11description(&odd);
    if (static_cast<int>(odd.description) != -1) bad = 3;
    return bad;
}
