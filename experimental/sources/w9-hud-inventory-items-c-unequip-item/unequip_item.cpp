// Observed unequipItem__FiPQ24item11description at 0x001af3a8, 912 bytes.
// Arguments: a player number and an item record (the 72-byte slot shape; slot+0
// points at its row, whose signed bytes +28 and +29 are called type and kind
// here). record is the 23984-byte player record. The word at slot+8 is read as a
// 64-bit flag word but only bits below 2^27 are tested.
// Clearing, by kind (row byte +29):
//  0: when the word at record+21968 is non-zero and type is 0,
//     inventorySwitchDualWield(player, slot) is called and nothing else is
//     cleared. Otherwise, unless type is 8: when the word at record+21960+4*b
//     (b = signed byte at record+23572) equals the slot, the byte at record+23582
//     becomes 5, resetPlayerAnimations(player) runs and inventorySetAnim(player,
//     &animState[264*player], 0) runs. Then the word at record+21896+4*type is
//     cleared and the two words at record+21960 and +21964 are cleared when equal
//     to the slot (type 8 skips only the animation part).
//  1: the word at record+21896+4*type is cleared.
//  2: the word at +21920 is cleared when equal to the slot; else the word at +21916;
//     else for type 7 the word at +21924 is cleared, or for type 14 the word at +21952.
//  other kinds clear nothing.
// Sound: a file name is chosen by the first flag (or type) that matches, in order
// 0x800000, 0x200000, type 3, 0x400000, 0x40000, 0x100000, 0x80000, 0x1000000;
// if none matches (the 0x4000000/0x2000000 test that precedes it leads to the same
// text) the jewelry name is used. The name is formatted with sprintf (no
// arguments) into a local buffer; when netPlayerIsLocal(player) is non-zero the
// sequence soundPlaySequence(lumpFindResource("fx.lmp", name), 0, 1.0f) is played.
// Finally inventorySetAnim(player, &animState[264*player], 0) and
// playerRecomputeAbilityModifiers() run.
// Unknown: the meaning of the kinds, types and flag bits; the buffer size (at
// least 64 bytes of the 128-byte frame were free for it).
#include "../inventory-items-record/record.hpp"
extern "C" {
extern const unsigned char norrath_inventory_ability_sound_lump_view[];
extern unsigned char animState[];
int sprintf(char* buffer, const char* format, ...);
int netPlayerIsLocal__Fi(int player);
void inventorySwitchDualWield__FiPQ24item11description(int player, const void* slot);
void resetPlayerAnimations__Fi(int player);
void inventorySetAnim__FiP14AnimationStatei(int player, void* state, int value);
void playerRecomputeAbilityModifiers__Fv();
char* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
}

namespace {
unsigned& word(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }
}

extern "C" void unequipItem__FiPQ24item11description(int owner, const void* slot_pointer) {
    using namespace inventory_items;
    unsigned char* record = player_record(owner);
    const unsigned char* slot = static_cast<const unsigned char*>(slot_pointer);
    const unsigned address = word_of(slot);
    const unsigned char* row = resolve<unsigned char>(*reinterpret_cast<const unsigned*>(slot));
    const int kind = static_cast<signed char>(row[29]);
    const int type = static_cast<signed char>(row[28]);
    if (kind == 0) {
        if (word(record, 21968) != 0 && type == 0) {
            inventorySwitchDualWield__FiPQ24item11description(owner, slot);
        } else {
            if (type != 8) {
                if (word(record, 21960 + 4 * static_cast<signed char>(record[23572])) == address) {
                    record[23582] = 5;
                    resetPlayerAnimations__Fi(owner);
                    inventorySetAnim__FiP14AnimationStatei(owner, animState + 264 * owner, 0);
                }
            }
            word(record, 21896 + 4 * type) = 0;
            for (int held = 0; held < 2; ++held) {
                if (word(record, 21960 + 4 * held) == address) word(record, 21960 + 4 * held) = 0;
            }
        }
    } else if (kind == 1) {
        word(record, 21896 + 4 * type) = 0;
    } else if (kind == 2) {
        if (word(record, 21920) == address) word(record, 21920) = 0;
        else if (word(record, 21916) == address) word(record, 21916) = 0;
        else if (type == 7) word(record, 21924) = 0;
        else if (type == 14) word(record, 21952) = 0;
    }
    const unsigned flags = *reinterpret_cast<const unsigned*>(slot + 8);
    char name[64];
    if (flags & 0x800000u) sprintf(name, "inv_drop_mdwp_wd.anm");
    else if (flags & 0x200000u) sprintf(name, "inv_drop_chain.anm");
    else if (static_cast<signed char>(resolve<unsigned char>(*reinterpret_cast<const unsigned*>(slot))[28]) == 3) sprintf(name, "inv_drop_sm_shield.anm");
    else if (flags & 0x400000u) sprintf(name, "inv_drop_plate.anm");
    else if (flags & 0x40000u) sprintf(name, "inv_drop_mdwp_mtl.anm");
    else if (flags & 0x100000u) sprintf(name, "inv_drop_leather.anm");
    else if (flags & 0x80000u) sprintf(name, "inv_drop_cloth.anm");
    else if (flags & 0x1000000u) sprintf(name, "inv_drop_potion.anm");
    else sprintf(name, "inv_drop_jewelry.anm");
    if (netPlayerIsLocal__Fi(owner) != 0) {
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0(const_cast<char*>(reinterpret_cast<const char*>(norrath_inventory_ability_sound_lump_view)), name), 0, 1.0f);
    }
    inventorySetAnim__FiP14AnimationStatei(owner, animState + 264 * owner, 0);
    playerRecomputeAbilityModifiers__Fv();
}
