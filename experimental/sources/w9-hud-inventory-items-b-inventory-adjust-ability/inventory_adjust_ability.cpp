// Observed inventoryAdjustAbility__Fii at 0x001b1938, 176 bytes.
// Arguments: a player index and an ability index. Player records are 23984 bytes
// apart at `player` (0x00408600).
//   record+240 (byte)           points left to spend (name inferred)
//   record+120 + 4*ability      signed word, one per ability
// If the byte at +240 is nonzero, the ability word is incremented and the byte
// decremented. Then, whether or not a point was spent, if netPlayerIsLocal__Fi
// reports the player as local, the resource "menu_char_incr.anm" is looked up in
// the lump named "fx.lmp" with lumpFindResource__FPcT0 and played with
// soundPlaySequence__FP15AnimationHeaderP10GameObjectf (object null, rate 1.0).
// If the byte is now zero it is stored as zero again and the byte at +242 is set
// to 1 (meaning not established). Finally playerRecomputeAbilityModifiers__Fv is
// called. The ability index is not range checked. The two names come from the
// inventory-items-views units; the player table and callees are not defined here.
extern "C" {
extern unsigned char player[];
extern const unsigned char norrath_inventory_ability_sound_lump_view[];
extern const unsigned char norrath_inventory_ability_sound_resource_view[];
int netPlayerIsLocal__Fi(int);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
void playerRecomputeAbilityModifiers__Fv();
}

extern "C" void inventoryAdjustAbility__Fii(int playerIndex, int ability) {
    unsigned char* record = player + playerIndex * 23984;
    int* word = reinterpret_cast<int*>(record + 120) + ability;
    if (record[240] != 0) {
        *word = *word + 1;
        record[240] = static_cast<unsigned char>(record[240] - 1);
    }
    if (netPlayerIsLocal__Fi(playerIndex) != 0) {
        void* sound = lumpFindResource__FPcT0(
            const_cast<char*>(reinterpret_cast<const char*>(norrath_inventory_ability_sound_lump_view)),
            const_cast<char*>(reinterpret_cast<const char*>(norrath_inventory_ability_sound_resource_view)));
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sound, nullptr, 1.0f);
    }
    if (record[240] == 0) {
        record[240] = 0;
        record[242] = 1;
    }
    playerRecomputeAbilityModifiers__Fv();
}
