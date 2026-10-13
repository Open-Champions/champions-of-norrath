// Observed inventorySyncQuestItems__Fi at 0x001b1208, 332 bytes (int source
// player). The third item list (record+9560; see record.hpp) of every player is
// examined; a slot "has the flag" when bit 39 of the 64-bit word at slot+8 is
// set. For each player i below nmPlayers other than the source player:
//   1. Every slot of i's third list that has the flag is removed with
//      removeItemFromInventory__Fiiii(i, slot, 2, 0). After a removal the slot
//      number is stepped back and the count is read again, because the removal
//      shifts later slots down.
//   2. Every slot of the source player's third list that has the flag is added
//      with addItemToInventory__FiPQ24item11descriptioni(i, &slot, 1); the
//      source count is re-read after each addition.
//   3. nmPlayers is read again before the next player.
// Inferred: bit 39 marks quest items (the symbol name says so); the effect is to
// replace the other players' copies of them with the source player's.
// Unknown: the exact meaning of that bit and of the added/removed arguments.
#include "../inventory-items-record/record.hpp"

extern "C" {
extern int nmPlayers;
void removeItemFromInventory__Fiiii(int, int, int, int);
void addItemToInventory__FiPQ24item11descriptioni(int, void*, int);
}

namespace {
const unsigned long long questBit = 0x8000000000ull;
inline bool flagged(const inventory_items::Slot& slot) {
    return (*reinterpret_cast<const unsigned long long*>(reinterpret_cast<const unsigned char*>(&slot) + 8) & questBit) != 0;
}
}

extern "C" void inventorySyncQuestItems__Fi(int source) {
    using namespace inventory_items;
    int total = nmPlayers;
    if (total <= 0) return;
    for (int i = 0; i < total; ++i) {
        if (i != source) {
            List& mine = player_lists(i)[2];
            for (int k = 0; k < mine.count; ++k) {
                if (flagged(mine.slots[k])) {
                    removeItemFromInventory__Fiiii(i, k, 2, 0);
                    --k;
                }
            }
            List& from = player_lists(source)[2];
            for (int k = 0; k < from.count; ++k) {
                if (flagged(from.slots[k])) addItemToInventory__FiPQ24item11descriptioni(i, &from.slots[k], 1);
            }
            total = nmPlayers;
        }
    }
}
