// Observed hudAssignQuickSpell__Fiii at 0x001c7a00, 180 bytes.
// Arguments: a player index, a slot value and a spell value. The player record
// is at player + index * 23984 (base 0x00408600). Two signed bytes of that
// record, at offsets 23580 and 23581, hold the two quick-spell choices.
// First, when netPlayerIsLocal__Fi(index) is nonzero, the routine looks up the
// resource named "menu_spell_assign.anm" in "fx.lmp" with lumpFindResource__FPcT0
// and starts it with soundPlaySequence__FP15AnimationHeaderP10GameObjectf with no
// object and volume 1.0.
// Then, with slot value zero: if byte 23581 differs from the spell, byte 23580
// becomes the spell. If it already equals the spell, byte 23581 takes the old
// byte 23580 and byte 23580 becomes the spell. With a nonzero slot value the
// roles of the two bytes are exchanged. The comparison is the full int spell
// against the sign-extended byte; stores keep the low byte. No range check is
// made on the index. Callees and the player table are not defined here.
extern "C" unsigned char player[];
extern "C" int netPlayerIsLocal__Fi(int player_index);
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name);
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);

extern "C" void hudAssignQuickSpell__Fiii(int index, int slot, int spell) {
    signed char* first = reinterpret_cast<signed char*>(player + index * 23984 + 23580);
    signed char* second = first + 1;
    if (netPlayerIsLocal__Fi(index) != 0) {
        void* sequence = lumpFindResource__FPcT0("fx.lmp", "menu_spell_assign.anm");
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sequence, 0, 1.0f);
    }
    if (slot == 0) {
        if (*second != spell) {
            *first = static_cast<signed char>(spell);
        } else {
            *second = static_cast<signed char>(*first);
            *first = static_cast<signed char>(spell);
        }
    } else {
        if (*first != spell) {
            *second = static_cast<signed char>(spell);
        } else {
            *first = static_cast<signed char>(*second);
            *second = static_cast<signed char>(spell);
        }
    }
}
