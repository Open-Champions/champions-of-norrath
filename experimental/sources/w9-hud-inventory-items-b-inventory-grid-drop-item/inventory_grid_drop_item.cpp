// Observed inventoryGridDropItem__Fiiciiii at 0x001b0578, 1696 bytes.
// Arguments: player index `p`, slot `slot`, list `list` (a signed byte), and four
// integers `gridCol`, `gridRow`, `group`, `mode`. The item is slot `slot` of list
// `list` of player p (record+328, lists of 4616 bytes of 72-byte slots, see
// inventory-items-record/record.hpp). Item fields read here: +0 description word
// (nothing happens when zero), +4 signed halfword (the stack count), +8 64-bit
// flags. Description fields: +0 name pointer, +28, +29 and +30 bytes.
// Steps, in order (record = player record, 23984 bytes apart at `player`):
//  1. The byte at (word at record+21872) + 1130 is cleared.
//  2. When mode < 2, a resource name is chosen and written with sprintf:
//       flags bit 23 set: kind = description+30; kind in 1..4 -> inv_drop_lgwp_wd.anm,
//         else flags bit 29 -> inv_drop_smwp_wd.anm, else inv_drop_mdwp_wd.anm;
//       else bit 21 -> inv_drop_chain.anm; else description+28 == 3 ->
//         inv_drop_sm_shield.anm; else bit 22 -> inv_drop_plate.anm; else bit 18:
//         kind 1..3 -> inv_drop_lgwp_mtl.anm, else bit 29 -> inv_drop_smwp_mtl.anm,
//         else inv_drop_mdwp_mtl.anm; else bit 20 -> inv_drop_leather.anm; else
//         bit 19 -> inv_drop_cloth.anm; else bit 24 -> inv_drop_potion.anm; else
//         inv_drop_jewelry.anm.
//     If netPlayerIsLocal__Fi(p), the name is looked up in the lump "fx.lmp" and
//     played (object null, rate 1.0).
//  3. If the byte record+23582 is 7: when the word record+21960 holds this item's
//     address, an 8-byte message {1, 0, short p, word record+21968} is sent with
//     netSendMessage__FiPUci(invMsgHandle, message, 8) and the words at +21960,
//     +21896, +21900 and +21968 are zeroed (the message is built before the
//     zeroing); otherwise, when the word record+21968 holds this item's address,
//     byte +23582 and words +21968 and +21900 are zeroed,
//     resetPlayerAnimations__Fi(p) is called and inventorySetAnim is called with
//     (p, &animState[p], 0).
//  4. If the word at record+21960+4*(signed byte record+23572) is this item's
//     address, byte +23582 becomes 5 and resetPlayerAnimations__Fi(p) is called.
//  5. The quantity taken is min(count, 20) when description+28 == 8 or the name
//     contains "spear", otherwise 1; it is subtracted from the halfword at +4.
//  6. If netPlayerIsLocal__Fi(p) and invGrid is nonzero: in the shop-style table
//     at invGrid + 145800*group + 48600*(signed byte description+29), 75 rows
//     of 72 bytes by 9 columns of 5400 bytes, every record for which
//     compareToItem returns nonzero loses the taken quantity from its halfword
//     at +4.
//  7. If mode == 0, an ItemProp is created (blockAlloc__Fi(400) and the
//     constructor) at objectFindEmptySpot__FG6Point3iiPc(position, 48, 12, 0),
//     where position is the three floats at (word at record+21872)+16; when the
//     allocation succeeds its halfword at +332 is set to the taken quantity.
//  8. If the stack count is now positive nothing more is removed. Otherwise: with
//     netPlayerIsLocal and invGrid nonzero, every record of the same table that
//     matches (compareToItem) is cleared (first word zero), except the one at
//     column gridCol, row gridRow, which is skipped by the scan and then cleared
//     unconditionally; then removeItemFromInventory__Fiiii(p, slot,
//     description+29, group) runs, and inventorySetAnim is called with (p,
//     &animState[p], 0).
//  9. playerRecomputeWeight__Fi(p) and playerRecomputeAbilityModifiers__Fv().
// The meaning of the flag bits, kinds, mode and the four integers is not
// established. Names are not established beyond the file names. The callees and
// objects are not defined here.
#include "../inventory-items-record/record.hpp"

// A user-provided copy constructor makes this type non-trivial, so (as in the original)
// it is passed by invisible reference and returned through a hidden pointer.
struct DropPoint {
    float x, y, z;
    DropPoint() {}
    DropPoint(const DropPoint& other) : x(other.x), y(other.y), z(other.z) {}
};

extern "C" {
extern unsigned invGrid;
extern int invMsgHandle;
extern unsigned char animState[];
int sprintf(char*, const char*, ...);
char* strstr(const char*, const char*);
int netPlayerIsLocal__Fi(int);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
void netSendMessage__FiPUci(int handle, unsigned char* message, int length);
void resetPlayerAnimations__Fi(int);
void inventorySetAnim__FiP14AnimationStatei(int, void*, int);
int compareToItem__Q24item11descriptionPQ24item11description(const void*, const void*);
DropPoint objectFindEmptySpot__FG6Point3iiPc(DropPoint, int, int, char*);
void* blockAlloc__Fi(int);
void __8ItemPropG6Point3PQ24item11descriptioni(void*, DropPoint, const void*, int);
void removeItemFromInventory__Fiiii(int, int, int, int);
void playerRecomputeWeight__Fi(int);
void playerRecomputeAbilityModifiers__Fv();
extern const unsigned char norrath_inventory_ability_sound_lump_view[];
}

using namespace inventory_items;

static const unsigned char* bytesOf(unsigned word) { return resolve<unsigned char>(word); }

extern "C" void inventoryGridDropItem__Fiiciiii(int p, int slot, char list, int gridCol, int gridRow, int group, int mode) {
    unsigned char* record = player_record(p);
    Slot* item = &player_lists(p)[list].slots[slot];
    if (item->description == 0) return;
    unsigned& objectWord = *reinterpret_cast<unsigned*>(record + 21872);
    const_cast<unsigned char*>(bytesOf(objectWord))[1130] = 0;
    const unsigned char* description = bytesOf(item->description);
    long long flags = *reinterpret_cast<const long long*>(reinterpret_cast<const unsigned char*>(item) + 8);
    unsigned itemWord = word_of(item);
    if (mode < 2) {
        char name[64];
        if (flags & 0x800000LL) {
            unsigned kind = description[30];
            if (kind - 1 < 4) sprintf(name, "inv_drop_lgwp_wd.anm");
            else if (flags & 0x20000000LL) sprintf(name, "inv_drop_smwp_wd.anm");
            else sprintf(name, "inv_drop_mdwp_wd.anm");
        } else if (flags & 0x200000LL) {
            sprintf(name, "inv_drop_chain.anm");
        } else if (static_cast<signed char>(description[28]) == 3) {
            sprintf(name, "inv_drop_sm_shield.anm");
        } else if (flags & 0x400000LL) {
            sprintf(name, "inv_drop_plate.anm");
        } else if (flags & 0x40000LL) {
            unsigned kind = description[30];
            if (kind - 1 < 3) sprintf(name, "inv_drop_lgwp_mtl.anm");
            else if (flags & 0x20000000LL) sprintf(name, "inv_drop_smwp_mtl.anm");
            else sprintf(name, "inv_drop_mdwp_mtl.anm");
        } else if (flags & 0x100000LL) {
            sprintf(name, "inv_drop_leather.anm");
        } else if (flags & 0x80000LL) {
            sprintf(name, "inv_drop_cloth.anm");
        } else if (flags & 0x1000000LL) {
            sprintf(name, "inv_drop_potion.anm");
        } else {
            sprintf(name, "inv_drop_jewelry.anm");
        }
        if (netPlayerIsLocal__Fi(p) != 0) {
            void* sound = lumpFindResource__FPcT0(
                const_cast<char*>(reinterpret_cast<const char*>(norrath_inventory_ability_sound_lump_view)), name);
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sound, nullptr, 1.0f);
        }
    }
    unsigned& equippedA = *reinterpret_cast<unsigned*>(record + 21960);
    unsigned& equippedB = *reinterpret_cast<unsigned*>(record + 21968);
    unsigned& wordC = *reinterpret_cast<unsigned*>(record + 21896);
    unsigned& wordD = *reinterpret_cast<unsigned*>(record + 21900);
    if (static_cast<signed char>(record[23582]) == 7) {
        if (itemWord == equippedA) {
            unsigned char message[8];
            message[0] = 1;
            message[1] = 0;
            *reinterpret_cast<short*>(message + 2) = static_cast<short>(p);
            *reinterpret_cast<unsigned*>(message + 4) = equippedB;
            equippedA = 0;
            wordC = 0;
            netSendMessage__FiPUci(invMsgHandle, message, 8);
            wordD = 0;
            equippedB = 0;
        } else if (itemWord == equippedB) {
            record[23582] = 0;
            equippedB = 0;
            wordD = 0;
            resetPlayerAnimations__Fi(p);
            inventorySetAnim__FiP14AnimationStatei(p, animState + p * 264, 0);
        }
    }
    if (*(reinterpret_cast<unsigned*>(record + 21960) + static_cast<signed char>(record[23572])) == itemWord) {
        record[23582] = 5;
        resetPlayerAnimations__Fi(p);
    }
    int taken;
    if (static_cast<signed char>(description[28]) == 8 || strstr(resolve<char>(*reinterpret_cast<const unsigned*>(description)), "spear") != nullptr) {
        taken = item->quantity < 20 ? item->quantity : 20;
    } else {
        taken = 1;
    }
    item->quantity = static_cast<short>(item->quantity - taken);
    unsigned char* table = nullptr;
    if (netPlayerIsLocal__Fi(p) != 0 && invGrid != 0) {
        table = const_cast<unsigned char*>(bytesOf(invGrid)) + group * 145800 + static_cast<signed char>(description[29]) * 48600;
        for (int row = 0; row < 75; ++row) {
            for (int column = 0; column < 9; ++column) {
                unsigned char* cell = table + row * 72 + column * 5400;
                if (compareToItem__Q24item11descriptionPQ24item11description(cell, item) != 0) {
                    *reinterpret_cast<short*>(cell + 4) = static_cast<short>(*reinterpret_cast<unsigned short*>(cell + 4) - taken);
                }
            }
        }
    }
    if (mode == 0) {
        const unsigned char* object = bytesOf(objectWord);
        DropPoint position(*reinterpret_cast<const DropPoint*>(object + 16));
        DropPoint spot = objectFindEmptySpot__FG6Point3iiPc(position, 48, 12, nullptr);
        void* prop = blockAlloc__Fi(400);
        if (prop != nullptr) {
            __8ItemPropG6Point3PQ24item11descriptioni(prop, spot, item, 0);
            *reinterpret_cast<short*>(static_cast<unsigned char*>(prop) + 332) = static_cast<short>(taken);
        }
    }
    if (item->quantity <= 0) {
        int kind = static_cast<signed char>(description[29]);
        if (netPlayerIsLocal__Fi(p) != 0 && invGrid != 0) {
            unsigned char* base = const_cast<unsigned char*>(bytesOf(invGrid)) + group * 145800 + static_cast<signed char>(description[29]) * 48600;
            for (int row = 0; row < 75; ++row) {
                for (int column = 0; column < 9; ++column) {
                    unsigned char* cell = base + row * 72 + column * 5400;
                    if (column == gridCol && row == gridRow) continue;
                    if (compareToItem__Q24item11descriptionPQ24item11description(cell, item) != 0) {
                        *reinterpret_cast<int*>(cell) = 0;
                    }
                }
            }
            *reinterpret_cast<int*>(base + gridRow * 72 + gridCol * 5400) = 0;
        }
        removeItemFromInventory__Fiiii(p, slot, kind, group);
        inventorySetAnim__FiP14AnimationStatei(p, animState + p * 264, 0);
    }
    playerRecomputeWeight__Fi(p);
    playerRecomputeAbilityModifiers__Fv();
}
