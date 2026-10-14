// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "inventory_item_check_slot.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>

extern "C" { alignas(8) unsigned char player[4 * 23984 + 64]; }

static void setSlot(int rec, int slot, int word) { std::memcpy(player + rec * 23984 + 21896 + slot * 4, &word, 4); }

int main() {
    int bad = 0;
    char* const item = reinterpret_cast<char*>(0x00345670);  // handle only, never dereferenced
    const int word = 0x00345670;
    // Not present in an empty record.
    bad |= inventoryItemCheckSlot__FiPQ24item11description(0, item) != -100000000;
    // Each slot index is found, in record 0 and in record 3.
    for (int rec : {0, 3})
        for (int s = 0; s < 16; ++s) {
            std::memset(player, 0, sizeof player);
            setSlot(rec, s, word);
            bad |= inventoryItemCheckSlot__FiPQ24item11description(rec, item) != s;
            // The other record does not see it.
            bad |= inventoryItemCheckSlot__FiPQ24item11description(rec == 0 ? 1 : 0, item) != -100000000;
        }
    // First of two matches wins.
    std::memset(player, 0, sizeof player);
    setSlot(1, 9, word); setSlot(1, 4, word);
    bad |= inventoryItemCheckSlot__FiPQ24item11description(1, item) != 4;
    // Words just outside the sixteen (slot 16 and the word before slot 0) do not count.
    std::memset(player, 0, sizeof player);
    setSlot(1, 16, word); setSlot(1, -1, word);
    bad |= inventoryItemCheckSlot__FiPQ24item11description(1, item) != -100000000;
    // A null item matches an empty (zero) slot at index 0.
    std::memset(player, 0, sizeof player);
    bad |= inventoryItemCheckSlot__FiPQ24item11description(2, nullptr) != 0;
    std::printf(bad ? "FAIL\n" : "ok: slot scan\n");
    return bad;
}
