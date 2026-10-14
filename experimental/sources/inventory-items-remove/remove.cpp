// Observed removeItemFromInventory__Fiiii at 0x001ae290, 1096 bytes.
// Arguments: player, slot, list number, and a fourth value that selects a
// group in the table at invGrid. Fields are offsets inside the player record
// (23984 bytes, see record.hpp). Steps, in order:
//  1. Read the signed type byte at description+28 of the slot. For type 5 or 6
//     the words at record+21916 and record+21920 are tested, and the first
//     that equals the slot's address is zeroed. For other types the word at
//     record+21896+type*4 is zeroed if it equals the slot's address.
//  2. The signed byte at record+23572 selects the word at record+21960+4*byte.
//     When it equals the slot's address, the byte at record+23582 becomes 5 and
//     resetPlayerAnimations is called.
//  3. The words at record+21960 and record+21964 are tested in that order and
//     the first match is zeroed.
//  4. Each later slot is copied down one place. For every move, the 16 words
//     from record+21896, the 2 words from record+21960 and the word at
//     record+21968 that held the moved slot's address now hold the new address.
//     The vacated slot's first word is zeroed.
//  5. When netPlayerIsLocal is non-zero, the byte interfaceOn is non-zero and
//     invGrid is non-zero, a table of 9 x 75 records of 72 bytes is scanned at
//     invGrid + 145800*fourth + 48600*list. Row r column c is at 72*r + 5400*c.
//     For each record with a non-zero first word, every slot of the list is
//     passed to compareToItem; each non-zero result stores that slot number in
//     the record's halfword at +34.
//  6. The halfword at +34 of each slot from the removed one up to the one before
//     the old last is decremented. The first words of the slots from
//     max(slot, count-1) to 63 are zeroed. The count drops by one and
//     playerRecomputeAbilityModifiers runs.
// Unknown: what the equipped words, the byte at 23572 and the table mean.
#include "../inventory-items-record/record.hpp"

extern "C" {
extern signed char interfaceOn;
extern unsigned invGrid;
void resetPlayerAnimations__Fi(int player);
int netPlayerIsLocal__Fi(int player);
int compareToItem__Q24item11descriptionPQ24item11description(const void* left, const void* right);
void playerRecomputeAbilityModifiers__Fv();
}

namespace {
unsigned& word(unsigned char* record, unsigned offset) {
    return *reinterpret_cast<unsigned*>(record + offset);
}
}

extern "C" void removeItemFromInventory__Fiiii(int owner, int slot, int list_number, int group) {
    using namespace inventory_items;
    unsigned char* record = player + owner * 23984;
    List* list = player_lists(owner) + list_number;
    Slot* item = &list->slots[slot];
    const unsigned address = word_of(item);

    const int type = resolve<Description>(item->description)->type;
    if (static_cast<unsigned>(type - 5) < 2u) {
        if (word(record, 21916) == address) word(record, 21916) = 0;
        else if (word(record, 21920) == address) word(record, 21920) = 0;
    } else if (word(record, 21896 + type * 4) == address) {
        word(record, 21896 + type * 4) = 0;
    }

    const int selected = static_cast<signed char>(record[23572]);
    if (word(record, 21960 + selected * 4) == address) {
        record[23582] = 5;
        resetPlayerAnimations__Fi(owner);
    }
    for (int index = 0; index < 2; ++index) {
        if (word(record, 21960 + index * 4) == address) {
            word(record, 21960 + index * 4) = 0;
            break;
        }
    }

    for (int index = slot; index < list->count - 1; ++index) {
        Slot* to = &list->slots[index];
        Slot* from = to + 1;
        *to = *from;
        const unsigned old_address = word_of(from);
        const unsigned new_address = word_of(to);
        for (int k = 0; k < 16; ++k) {
            if (word(record, 21896 + k * 4) == old_address) word(record, 21896 + k * 4) = new_address;
        }
        for (int k = 0; k < 2; ++k) {
            if (word(record, 21960 + k * 4) == old_address) word(record, 21960 + k * 4) = new_address;
            if (word(record, 21968) == old_address) word(record, 21968) = new_address;
        }
        from->description = 0;
    }

    if (netPlayerIsLocal__Fi(owner) != 0 && interfaceOn != 0 && invGrid != 0) {
        Slot* table = const_cast<Slot*>(resolve<Slot>(invGrid)) + group * 2025 + list_number * 675;
        for (int row = 0; row < 75; ++row) {
            for (int column = 0; column < 9; ++column) {
                Slot* entry = table + column * 75 + row;
                if (entry->description == 0) continue;
                for (int index = 0; index < 64; ++index) {
                    if (compareToItem__Q24item11descriptionPQ24item11description(entry, &list->slots[index]) != 0) {
                        entry->index = static_cast<short>(index);
                    }
                }
            }
        }
    }

    const int count = list->count;
    int first_clear = slot;
    if (slot < count - 1) {
        for (int index = slot; index < count - 1; ++index) list->slots[index].index -= 1;
        first_clear = count - 1;
    }
    for (int index = first_clear; index < 64; ++index) list->slots[index].description = 0;
    list->count = count - 1;
    playerRecomputeAbilityModifiers__Fv();
}
