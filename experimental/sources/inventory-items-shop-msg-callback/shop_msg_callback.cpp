// Observed shopMsgCallback__FPUci at 0x001b2028, 544 bytes. The second argument
// is not read. Byte 0 of the packet is a signed message type.
//  0: playerLogInventory(p1), then tail call shopBuy(p1, word at 4).
//  1: playerLogInventory(p1). The item is slot w4 of list b10 in the player's
//     record (see record.hpp). value = (float)getValue(item). When the signed
//     type byte at description+28 is 8, or norrath_factory_strstr(name, "spear") is non-null,
//     value *= 20.0f. Then value /= (float)slot quantity, and the product with
//     the pool float 0x3e4ccccc is truncated to an int, replaced by 1 when it is
//     not positive, and added to the word at record+232. Finally it tail calls
//     inventoryGridDropItem(p1, w4, b10, h2, h8, b11, b12) with the same field
//     layout as invMsgCallback type 0.
//  Any other type returns.
// "pN" is the signed byte at offset N, bN a signed byte, hN a signed halfword,
// wN a word. Unknown: what the value, the factor 0.2 and the word at +232 mean.
#include "../inventory-items-record/record.hpp"

extern "C" {
char* norrath_factory_strstr(const char* haystack, const char* needle);
void playerLogInventory__Fi(int player);
void shopBuy__FcPQ24item11description(char player, void* item);
int getValue__4itemPQ24item11description(void* item);
void inventoryGridDropItem__Fiiciiii(int, int, char, int, int, int, int);
}

namespace {
struct Buy {
    signed char type, player;
    short unread;
    unsigned w4;
};
struct Sell {
    signed char type, player;
    short h2;
    int w4;
    short h8;
    signed char b10, b11, b12;
};
}

extern "C" void shopMsgCallback__FPUci(unsigned char* packet, int) {
    using namespace inventory_items;
    const signed char type = static_cast<signed char>(packet[0]);
    if (type == 0) {
        const Buy* m = reinterpret_cast<const Buy*>(packet);
        playerLogInventory__Fi(m->player);
        shopBuy__FcPQ24item11description(m->player, reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(m->w4)));
    } else if (type == 1) {
        const Sell* m = reinterpret_cast<const Sell*>(packet);
        const int owner = m->player;
        const int list_number = m->b10;
        const int slot_number = m->w4;
        const int h2 = m->h2, h8 = m->h8, b11 = m->b11, b12 = m->b12;
        playerLogInventory__Fi(owner);
        Slot* item = &player_lists(owner)[list_number].slots[slot_number];
        float value = static_cast<float>(getValue__4itemPQ24item11description(item));
        const Description* description = resolve<Description>(item->description);
        if (description->type == 8 || norrath_factory_strstr(resolve<char>(description->name), "spear") != nullptr) value *= 20.0f;
        value /= static_cast<float>(item->quantity);
        const float factor = __builtin_bit_cast(float, 0x3e4cccccu);
        int gain = static_cast<int>(value * factor);
        if (gain <= 0) gain = 1;
        *reinterpret_cast<int*>(player + owner * 23984 + 232) += gain;
        inventoryGridDropItem__Fiiciiii(owner, slot_number, list_number, h2, h8, b11, b12);
    }
}
