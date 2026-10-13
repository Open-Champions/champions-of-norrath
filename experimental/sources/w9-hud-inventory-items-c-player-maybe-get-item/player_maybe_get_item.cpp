// Observed playerMaybeGetItem__FiPQ24item11description at 0x00148480, 692 bytes.
// Arguments: a player number and an item record (the 72-byte slot shape; slot+0
// points at its row, whose signed byte +28 is called type and +29 is the number of
// the inventory list that holds it). record is the 23984-byte player record.
// Returns 1 when the item is taken and 0 when it is refused.
//  1. A factor of 1.0 is used; when type is 8 and playerGetFeatRank(player,
//     0x10000) is non-zero the factor is 0.5 instead.
//  2. weight = getWeight(item) * factor (single precision).
//  3. When the signed word count at (list 'kind' of the player, i.e.
//     record+328+4616*kind+4608) plus 1 is not below 64, the list is full:
//     soundPlaySequence(lumpFindResource("fx.lmp", "game_butn_cantdo.anm"), 0,
//     1.0f) plays and the result is 0.
//  4. Otherwise, with the floats at record+308 and record+312, the item is refused
//     unless record+308 + weight <= record+312; a refusal calls
//     playerPlaySound(player, 0) and returns 0.
//  5. Accepted: a name is chosen from the 64-bit word at item+8 by the first of the
//     bits 0x800000, 0x200000, 0x40000, 0x100000, 0x80000, 0x1000000 that is set
//     (inv_equip_mdwp_wd, _chain, _mdwp_mtl, _leather, _cloth, _potion, each
//     with ".anm"); every other case, including bits 0x4000000, 0x2000000 and
//     0x10000000, gives the jewelry name. Then soundPlaySequence(lumpFindResource(
//     "fx.lmp", name), object, 1.0f) plays with object the pointer word at
//     record+21872, scriptStart("*gotItemNotify") runs,
//     addItemToInventory(player, item, 1) and playerRecomputeWeight(player) run
//     and the result is 1.
// Unknown: the meaning of the flag bits, the weight pair at record+308/+312 and
// what the notify script does.
#include "../inventory-items-record/record.hpp"
extern "C" {
extern const unsigned char norrath_inventory_ability_sound_lump_view[];
int sprintf(char* buffer, const char* format, ...);
int playerGetFeatRank__Fix(int player, long long feat);
float getWeight__4itemPQ24item11description(const void* item);
char* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
void scriptStart__FPc(char* name);
void addItemToInventory__FiPQ24item11descriptioni(int player, const void* item, int count);
int playerRecomputeWeight__Fi(int player);
void playerPlaySound__Fii(int player, int sound);
}

extern "C" int playerMaybeGetItem__FiPQ24item11description(int owner, const void* item_pointer) {
    using namespace inventory_items;
    const unsigned char* item = static_cast<const unsigned char*>(item_pointer);
    const unsigned char* row = resolve<unsigned char>(*reinterpret_cast<const unsigned*>(item));
    float factor = 1.0f;
    if (static_cast<signed char>(row[28]) == 8) {
        if (playerGetFeatRank__Fix(owner, 0x10000) != 0) factor = 0.5f;
    }
    const float weight = getWeight__4itemPQ24item11description(item) * factor;
    unsigned char* record = player_record(owner);
    char* lump = const_cast<char*>(reinterpret_cast<const char*>(norrath_inventory_ability_sound_lump_view));
    const int kind = static_cast<signed char>(resolve<unsigned char>(*reinterpret_cast<const unsigned*>(item))[29]);
    if (player_lists(owner)[kind].count + 1 >= 64) {
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0(lump, const_cast<char*>("game_butn_cantdo.anm")), 0, 1.0f);
        return 0;
    }
    if (!(*reinterpret_cast<float*>(record + 308) + weight <= *reinterpret_cast<float*>(record + 312))) {
        playerPlaySound__Fii(owner, 0);
        return 0;
    }
    const unsigned flags = *reinterpret_cast<const unsigned*>(item + 8);
    char name[64];
    if (flags & 0x800000u) sprintf(name, "inv_equip_mdwp_wd.anm");
    else if (flags & 0x200000u) sprintf(name, "inv_equip_chain.anm");
    else if (flags & 0x40000u) sprintf(name, "inv_equip_mdwp_mtl.anm");
    else if (flags & 0x100000u) sprintf(name, "inv_equip_leather.anm");
    else if (flags & 0x80000u) sprintf(name, "inv_equip_cloth.anm");
    else if (flags & 0x1000000u) sprintf(name, "inv_equip_potion.anm");
    else sprintf(name, "inv_equip_jewelry.anm");
    void* object = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(record + 21872)));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump, name), object, 1.0f);
    scriptStart__FPc(const_cast<char*>("*gotItemNotify"));
    addItemToInventory__FiPQ24item11descriptioni(owner, item, 1);
    playerRecomputeWeight__Fi(owner);
    return 1;
}
