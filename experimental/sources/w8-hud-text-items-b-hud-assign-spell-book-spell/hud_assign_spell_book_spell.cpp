// Observed hudAssignSpellBookSpell__Fiiil at 0x001c5de0, 628 bytes.
// Arguments: player index, entry index e, mode, spell (a 32-bit long here).
// Player records are 23984 bytes apart (base 0x00408600). Inside one, eight
// 104-byte entries start at offset 21976. Entry fields read or written:
//   +0   nine bytes, the text "newstyle" with its terminator
//   +32  halfword, the spell code stored for the entry
//   +36  byte, +37 byte
//   +72  64-bit word
// Step 1, a scan of all eight entries without early exit (the last hit wins,
// found starts at -1): with mode 1, an entry matches when its halfword at +32
// equals the spell; with any other mode, when its 64-bit word at +72 equals the
// sign-extended spell.
// Step 2, when found is not -1: the halfword at +32 of entry e is read first;
// entry `found` gets -1 in its +72 word and that halfword at +32. When the
// halfword is not -1: if strstr finds "newstyle" in the bytes at the start of
// entry e, entry `found` receives the nine-byte text, +37 gets byte 8 of the
// object pointed to by skills[halfword], and +36 gets
// playerGetSkillRank__Fii(player, halfword); otherwise
// givePlayerFeat__Fxii is called with 0x20000, the player and `found`.
// Step 3: mode 1 copies the nine-byte text into entry e, clears the +32
// halfword of every entry that holds the spell, sets entry e +32 to the low
// 16 bits of the spell, +37 to byte 8 of skills[spell]'s object and +36 to
// playerGetSkillRank__Fii(player, spell). Any other mode calls
// givePlayerFeat__Fxii with the sign-extended spell, the player and e.
// Step 4: when netPlayerIsLocal__Fi(player) is nonzero, the resource
// "menu_spell_assign.anm" of "fx.lmp" is found with lumpFindResource__FPcT0 and
// started with soundPlaySequence__FP15AnimationHeaderP10GameObjectf with no
// object and volume 1.0.
// No range checks are made. The meanings of the fields are not established.
// All callees, the player table and the skills table are defined elsewhere.
extern "C" unsigned char player[];
extern "C" const unsigned char* skills[];
extern "C" char* strstr(const char* haystack, const char* needle);
extern "C" int playerGetSkillRank__Fii(int player_index, int spell);
extern "C" void givePlayerFeat__Fxii(long long feat, int player_index, int entry);
extern "C" int netPlayerIsLocal__Fi(int player_index);
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name);
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);

static const char newstyle_text[] = "newstyle";

static void copy_newstyle(unsigned char* entry) {
    for (int i = 0; i < 9; ++i) entry[i] = static_cast<unsigned char>(newstyle_text[i]);
}

static short& code_of(unsigned char* entry) { return *reinterpret_cast<short*>(entry + 32); }

extern "C" void hudAssignSpellBookSpell__Fiiil(int index, int entry_index, int mode, int spell) {
    unsigned char* entries = player + index * 23984 + 21976;
    unsigned char* selected = entries + entry_index * 104;
    int found = -1;
    for (int j = 0; j < 8; ++j) {
        unsigned char* candidate = entries + j * 104;
        if (mode == 1) {
            if (code_of(candidate) == spell) found = j;
        } else {
            if (*reinterpret_cast<long long*>(candidate + 72) == static_cast<long long>(spell)) found = j;
        }
    }
    if (found != -1) {
        unsigned char* target = entries + found * 104;
        const unsigned short code = static_cast<unsigned short>(code_of(selected));
        *reinterpret_cast<long long*>(target + 72) = -1;
        *reinterpret_cast<unsigned short*>(target + 32) = code;
        if (static_cast<short>(code) != -1) {
            if (strstr(reinterpret_cast<const char*>(selected), newstyle_text) != 0) {
                copy_newstyle(target);
                target[37] = skills[code_of(selected)][8];
                target[36] = static_cast<unsigned char>(playerGetSkillRank__Fii(index, code_of(selected)));
            } else {
                givePlayerFeat__Fxii(0x20000, index, found);
            }
        }
    }
    if (mode == 1) {
        copy_newstyle(selected);
        for (int j = 0; j < 8; ++j) {
            unsigned char* candidate = entries + j * 104;
            if (code_of(candidate) == spell) code_of(candidate) = 0;
        }
        *reinterpret_cast<unsigned short*>(selected + 32) = static_cast<unsigned short>(spell);
        selected[37] = skills[spell][8];
        selected[36] = static_cast<unsigned char>(playerGetSkillRank__Fii(index, spell));
    } else {
        givePlayerFeat__Fxii(static_cast<long long>(spell), index, entry_index);
    }
    if (netPlayerIsLocal__Fi(index) != 0) {
        void* sequence = lumpFindResource__FPcT0("fx.lmp", "menu_spell_assign.anm");
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sequence, 0, 1.0f);
    }
}
