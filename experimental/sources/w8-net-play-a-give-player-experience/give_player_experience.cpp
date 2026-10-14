// Observed givePlayerExperience__Fii at 0x001bdf08, 420 bytes. `player` records
// are 23984 bytes. Adds `amount` to the int at record offset 92. While that
// total is not below experienceTable[(signed char)record[89]], one level-up
// step runs:
//   * the int at offset 236 grows by 1, 2, 3, 4 or 5 when the old level in
//     offset 89 is below 6, 16, 26, 36, or at least 36;
//   * the level byte at 89 grows by 1 and the byte at 240 by 3;
//   * playerRecomputeAbilityModifiers() is called;
//   * when the float at 172 is greater than zero, floats at 176 and 200 are
//     copied over those at 172 and 196;
//   * for a local player (netPlayerIsLocal(index)) the sequence found in "fx.lmp"
//     as "menu_lvlup.anm" is played with a null object and rate 1.0;
//   * the message "LevelUpMessage" is shown with hudSetInfoMessage(text, 180,
//     index, 0).
// The table is re-read after each step, so a large grant can raise several
// levels. The meaning of the fields at 172..200 and 236/240 is not established.
extern "C" {
extern unsigned char player[];
extern int experienceTable[];
void playerRecomputeAbilityModifiers__Fv();
int netPlayerIsLocal__Fi(int index);
char* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
unsigned short* textGetString__FPc(char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int time, int player, char flag);

void givePlayerExperience__Fii(int index, int amount) {
    unsigned char* record = player + index * 23984;
    int& experience = *reinterpret_cast<int*>(record + 92);
    experience += amount;
    if (experience < experienceTable[static_cast<signed char>(record[89])]) return;
    do {
        int level = static_cast<signed char>(record[89]);
        int& bonus = *reinterpret_cast<int*>(record + 236);
        if (level < 6) bonus += 1;
        else if (level < 16) bonus += 2;
        else if (level < 26) bonus += 3;
        else if (level < 36) bonus += 4;
        else bonus += 5;
        record[89] = static_cast<unsigned char>(record[89] + 1);
        record[240] = static_cast<unsigned char>(record[240] + 3);
        playerRecomputeAbilityModifiers__Fv();
        float* floats = reinterpret_cast<float*>(record);
        if (floats[43] > 0.0f) {
            floats[43] = floats[44];
            floats[49] = floats[50];
        }
        if (netPlayerIsLocal__Fi(index)) {
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
                lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("menu_lvlup.anm")), 0, 1.0f);
        }
        hudSetInfoMessage__FPUsiic(textGetString__FPc(const_cast<char*>("LevelUpMessage")), 180, index, 0);
    } while (experience >= experienceTable[static_cast<signed char>(record[89])]);
}
}
