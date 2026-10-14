// Observed invMsgCallback__FPUci at 0x001ac6c8, 476 bytes. The second argument
// is not read. Byte 0 of the packet is a signed message type; a value outside
// 0..10 returns at once. The dispatch is a table of 11 code addresses at
// 0x466300 and every case ends by returning or by tail-calling the handler.
// Packet fields are naturally aligned and byte 1 is the player number except in
// type 3. Handlers are named here by their ELF symbols.
//   0 inventoryGridDropItem(p1, w4, b10, h2, h8, b11, b12)
//   1 equipItem(h2, w4, b1)           w4 is an item description address
//   2 unequipItem(h2, w4)
//   3 nothing
//   4 inventoryExit(b1, b2, b3)
//   5 stores byte 2 at record(p1)+23582, then resetPlayerAnimations(p1)
//   6 inventoryAdjustAbility(p1, h2)
//   7 inventoryAdjustSkill(p1, b2, long long from w4)
//   8 shopItemSetAttributes(p1, w4, address w8, address w12)
//   9 stores the low byte of h2 at record(p1)+88
//  10 drinkPotion(word at record(p1)+21872, h2, h4, h6, h8)
// "pN" is the signed byte at offset N, bN a signed byte, hN a signed halfword,
// wN a word. Unknown: what these messages mean beyond the handler names, and
// the sender's packet size for each type.
#include "../inventory-items-record/record.hpp"

extern "C" {
void inventoryGridDropItem__Fiiciiii(int, int, char, int, int, int, int);
void equipItem__FiPQ24item11descriptioni(int, void*, int);
void unequipItem__FiPQ24item11description(int, void*);
void inventoryExit__Fiii(int, int, int);
void resetPlayerAnimations__Fi(int);
void inventoryAdjustAbility__Fii(int, int);
void inventoryAdjustSkill__Ficx(int, char, long long);
void shopItemSetAttributes__FiiPcPQ24item11description(int, int, char*, void*);
void drinkPotion__6Playeriiii(void* player, int, int, int, int);
}

namespace {
struct Drop {
    signed char type, player;
    short h2;
    int w4;
    short h8;
    signed char b10, b11, b12;
};
struct Equip {
    signed char type, b1;
    short h2;
    int w4;
};
struct Unequip {
    signed char type, unread;
    short h2;
    int w4;
};
struct Exit {
    signed char type, b1, b2, b3;
};
struct Byte {
    signed char type, player, value;
};
struct Half {
    signed char type, player;
    short value;
};
struct Skill {
    signed char type, player, b2, unread;
    int w4;
};
struct Attributes {
    signed char type, player;
    unsigned char unread[2];
    int w4;
    unsigned w8, w12;
};
struct Potion {
    signed char type, player;
    short h2, h4, h6, h8;
};
void* address(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
}

extern "C" void invMsgCallback__FPUci(unsigned char* packet, int) {
    switch (static_cast<signed char>(packet[0])) {
    case 0: {
        const Drop* m = reinterpret_cast<const Drop*>(packet);
        inventoryGridDropItem__Fiiciiii(m->player, m->w4, m->b10, m->h2, m->h8, m->b11, m->b12);
        return;
    }
    case 1: {
        const Equip* m = reinterpret_cast<const Equip*>(packet);
        equipItem__FiPQ24item11descriptioni(m->h2, address(m->w4), m->b1);
        return;
    }
    case 2: {
        const Unequip* m = reinterpret_cast<const Unequip*>(packet);
        unequipItem__FiPQ24item11description(m->h2, address(m->w4));
        return;
    }
    case 4: {
        const Exit* m = reinterpret_cast<const Exit*>(packet);
        inventoryExit__Fiii(m->b1, m->b2, m->b3);
        return;
    }
    case 5: {
        const Byte* m = reinterpret_cast<const Byte*>(packet);
        player[m->player * 23984 + 23582] = static_cast<unsigned char>(m->value);
        resetPlayerAnimations__Fi(m->player);
        return;
    }
    case 6: {
        const Half* m = reinterpret_cast<const Half*>(packet);
        inventoryAdjustAbility__Fii(m->player, m->value);
        return;
    }
    case 7: {
        const Skill* m = reinterpret_cast<const Skill*>(packet);
        inventoryAdjustSkill__Ficx(m->player, m->b2, m->w4);
        return;
    }
    case 8: {
        const Attributes* m = reinterpret_cast<const Attributes*>(packet);
        shopItemSetAttributes__FiiPcPQ24item11description(m->player, m->w4, static_cast<char*>(address(m->w8)), address(m->w12));
        return;
    }
    case 9: {
        const Half* m = reinterpret_cast<const Half*>(packet);
        player[m->player * 23984 + 88] = static_cast<unsigned char>(m->value);
        return;
    }
    case 10: {
        const Potion* m = reinterpret_cast<const Potion*>(packet);
        const unsigned actor = *reinterpret_cast<const unsigned*>(player + m->player * 23984 + 21872);
        drinkPotion__6Playeriiii(address(actor), m->h2, m->h4, m->h6, m->h8);
        return;
    }
    default:
        return;
    }
}
