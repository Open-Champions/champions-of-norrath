// Observed runStats__Fii at 0x001bcb38, 4508 bytes, 848-byte frame. Arguments: a screen slot `slot` (0 or 1;
// statsScreen holds two 116-byte records, S = statsScreen + 116 * slot) and a mode. Returns nothing.
// S fields used (word index): [0] x offset, [5] latch, [6] selected stat, [9] y scroll, [13] player index,
// [19] state. R = player + 23984 * S[13] is the player record: name text at +0, class word +80, race word
// +84, level byte +88, experience +92, stats +144/+148/+152/+156 (strength, intelligence, dexterity,
// stamina), points remaining +236, points byte +240, byte +241 and signed byte +242, health pair +172/+176,
// energy pair +196/+200, min/max damage +212/+216, base attack +208, an item pointer +21872, selected
// weapon slot (signed byte) +23572, descriptions +21960 + 4 * slot and +21968, counts of quests in
// player + 21864 (the first player record) with 64-byte quest text records from player + 17768.
//  Setup: S[0] = 320 for slot 1 (the word of record 1), else 0.
//  Mode 2 with S[19] == 2: the task statsFrame__Fv is removed, R[241] = -1, givePlayerExperience(S[13], 0);
//    then the common tail. Otherwise:
//  Modes 2 and 3: nothing (return) unless the display list has at least 3000 words free
//    (GLISTSIZE - ((glistp - gstart) >> 3) >= 3000). With R[242] == 0 and mode 2 the point allocation
//    screen is drawn (title, points remaining, border, four stat rows, description of the selected stat,
//    a bobbing cursor and, on the accept button with S[5] == 0, the select sound and
//    msgInventoryAdjustAbility(S[13], S[6])). Else statsRunSkillTree(slot) runs; for mode 2 only, when
//    shopDisableMouse[slot] is zero and the pad's back button is down, S[19] = 2 and R[242] = 0; both
//    paths then return without the common tail.
//  Mode 1: the quest list: updateCursor, the parchment texture, each quest's two text lines, and a
//    "NoQuests" line when there are none.
//  Common tail: mode 0 draws the full statistics page; every mode then calls
//    playerRecomputeAbilityModifiers (except the returns named above and a failed display-list test).
// Inferred: the in-game statistics, quest and ability-allocation page of the inventory screens.
// Unknown: the field names of both records, the meaning of R[240]/R[241]/R[242], the screenStruct layout,
// the font constants (5, 6, colours 0x801e6464, 0x80323c3c, 0x802d2d2d) and the 1000000 / 100000 /
// 10000 widths, and whether the shared row helper printStat's trailing arguments are what they seem.
typedef unsigned short UString;
typedef unsigned long long Double;

// The image's libc and soft-float routines keep their own symbols; the host check supplies the same
// functions under these plain names.
#if defined(__mips__)
#define LIBRARY_SYMBOL(name) __asm__(#name)
#else
#define LIBRARY_SYMBOL(name)
#endif

extern "C" {
extern unsigned char statsScreen[];
extern unsigned char player[];
extern unsigned char pad[];
extern void* smallFont;
extern void* lcursor;
extern unsigned char* rcursor;
extern int shopDisableMouse[];
extern int GLISTSIZE;
extern unsigned long long* gstart;
extern unsigned long long* glistp;
extern int drawFrameNm;
extern int experienceTable[];
extern unsigned padAcceptButton;
extern unsigned padBackButton;
extern signed char language;

void statsFrame__Fv();
void engineRemoveTask__FPFv_v(void (*task)());
void givePlayerExperience__Fii(int index, int amount);
void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b);
void fontEnd__Fv();
void fontColor__FUiii(unsigned color, int a, int b);
UString* textGetString__FPc(const char* key);
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* text, int width);
void fontPrintu__FiiPUsi(int x, int y, UString* text, int width);
void fontPrint__FiiPci(int x, int y, char* text, int width);
void fontPrintCentered__FiiPci(int x, int y, char* text, int width);
int fontPrintWrapu__FiiPUsiiii(int x, int y, UString* text, int a, int b, int c, int d);
int fontStringSizeu__FP4FontPUsi(void* font, UString* text, int width);
int fontStringSize__FP4FontPci(void* font, char* text, int width);
int formatText(char* buffer, const char* format, ...) LIBRARY_SYMBOL(sprintf);
int textLength(const char* text) LIBRARY_SYMBOL(strlen);
char* textAppend(char* destination, const char* source) LIBRARY_SYMBOL(strcat);
void ustrcpy__FPUsPc(UString* destination, char* source);
void ustrcat__FPUsT0(UString* destination, UString* source);
void ustrcat__FPUsPc(UString* destination, const char* source);
void usprintf__FPUsT0ffffffff(UString* destination, UString* format, float a, float b, float c, float d, float e, float f, float g, float h);
void usprintf__FPUsPcffffffff(UString* destination, const char* format, float a, float b, float c, float d, float e, float f, float g, float h);
void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned color, char* name, int b, int c);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int a, unsigned color);
void updateCursor__FiiiiiP12screenStructi(int a, int b, int c, int d, int e, void* screen, int f);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float volume);
void msgInventoryAdjustAbility__Fii(int index, int ability);
void statsRunSkillTree__Fi(int slot);
void playerRecomputeDamage__6PlayerPQ24item11descriptioni(void* self, void* description, int flag);
void playerRecomputeAbilityModifiers__Fv();
void displayResistances__Fi(int slot);
void printStat__FiPUsiiii(int slot, UString* label, int value, int a, int b, int c);
Double litodp(int value);
Double dpmul(Double a, Double b);
Double dpadd(Double a, Double b);
Double sinDouble(Double value) LIBRARY_SYMBOL(sin);
Double fptodp(float value);
int dptoli(Double value);
}

namespace {
inline int& intAt(unsigned char* base, int offset) { return *reinterpret_cast<int*>(base + offset); }
inline void* wordPointer(unsigned char* base, int offset) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(base + offset)));
}

// A label right-aligned at x: its width is measured, then it is printed with a fresh string lookup.
inline void rightLabel(const char* key, int x, int y) {
    const int width = fontStringSizeu__FP4FontPUsi(smallFont, textGetString__FPc(key), 1000000);
    fontPrintu__FiiPUsi(x - width, y, textGetString__FPc(key), 1000000);
}

inline void statRow(unsigned char* record, const char* key, int slot, int y, int field) {
    char text[16];
    rightLabel(key, 320 * slot + 150, y);
    formatText(text, "%d", intAt(record, field));
    fontPrint__FiiPci(320 * slot + 160, y, text, 100000);
}

inline void zeroFormat(UString* destination, UString* format) {
    usprintf__FPUsT0ffffffff(destination, format, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

inline void pairFormat(UString* destination, float a, float b) {
    usprintf__FPUsPcffffffff(destination, "%d/%d", a, b, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}
}  // namespace

extern "C" void runStats__Fii(int slot, int mode) {
    unsigned char* const screen = statsScreen + 116 * slot;
    unsigned char* const record = player + 23984 * intAt(screen, 52);
    if (slot == 1) intAt(statsScreen + 116, 0) = 320;
    else intAt(screen, 0) = 0;

    if (mode == 2 && intAt(screen, 76) == 2) {
        engineRemoveTask__FPFv_v(statsFrame__Fv);
        record[241] = 0xff;
        givePlayerExperience__Fii(intAt(screen, 52), 0);
    } else if (static_cast<unsigned>(mode - 2) < 2u) {
        if (GLISTSIZE - static_cast<int>(glistp - gstart) < 3000) return;
        if (static_cast<signed char>(record[242]) == 0 && mode == 2) {
            fontStart__FP4FontiUii(smallFont, 5, 0x801e6464u, -1);
            fontPrintCenteredu__FiiPUsi(intAt(screen, 0) + 160, 32, textGetString__FPc("LevelUp"), 1000000);
            fontEnd__Fv();
            fontStart__FP4FontiUii(smallFont, 5, 0x801e6464u, -1);
            char number[80];
            UString line[80];
            formatText(number, "%d   ", static_cast<int>(record[240]));
            ustrcpy__FPUsPc(line, number);
            if (intAt(record, 236) == 1) ustrcat__FPUsT0(line, textGetString__FPc("PointRemaining"));
            else ustrcat__FPUsT0(line, textGetString__FPc("PointsRemaining"));
            fontPrintCenteredu__FiiPUsi(intAt(screen, 0) + 160, 204, line, 1000000);
            fontEnd__Fv();
            textureDrawBorder__FiiiiiUiPcii(intAt(screen, 0) + 18, 25, 292, 407, 4, 0x80616369u, nullptr, 0, 0);
            fontStart__FP4FontiUii(smallFont, 6, 0x801e6464u, -1);
            fontColor__FUiii(0x80323c3cu, -1, 0);
            fontPrintWrapu__FiiPUsiiii(intAt(screen, 0) + 30, 55, textGetString__FPc("abilityAdjust"), 266, -50, 250, 17);
            statRow(record, "StatStrength", slot, 239, 144);
            statRow(record, "StatIntelligence", slot, 259, 148);
            statRow(record, "StatDexterity", slot, 279, 152);
            statRow(record, "StatStamina", slot, 299, 156);
            fontEnd__Fv();
            fontStart__FP4FontiUii(smallFont, 5, 0x801e6464u, -1);
            const int selected = intAt(screen, 24);
            const char* description = selected == 0 ? "selectStatStrengthDesc"
                                      : selected == 1 ? "selectStatIntelDesc"
                                      : selected == 2 ? "selectStatDexterityDesc"
                                                      : "selectStatStaminaDesc";
            fontPrintWrapu__FiiPUsiiii(intAt(screen, 0) + 30, 331, textGetString__FPc(description), 266, -50, 600, 17);
            fontEnd__Fv();
            updateCursor__FiiiiiP12screenStructi(intAt(screen, 52), 0, 4, 0, 20, screen, 1);
            int x = intAt(screen, 0) - *reinterpret_cast<short*>(rcursor);
            const Double angle = dpadd(dpmul(litodp(slot), 0x3ff8000000000000ULL),
                                       fptodp(static_cast<float>(drawFrameNm) * __builtin_bit_cast(float, 0x3dccccccu)));
            x -= dptoli(dpmul(sinDouble(angle), 0x4024000000000000ULL));
            textureDrawColor__FP8_textureiiiUi(lcursor, x + 230,
                                                intAt(screen, 36) - (static_cast<short>(*reinterpret_cast<unsigned short*>(rcursor + 2)) >> 1) + 253,
                                                6, 0x80808080u);
            rectangleDraw__FiiiiiUi(intAt(screen, 0) + 18, intAt(screen, 36) + 243, intAt(screen, 0) + 309, intAt(screen, 36) + 265, 5,
                                    0x40366876u);
            if ((*reinterpret_cast<unsigned*>(pad + 88 * intAt(screen, 52) + 16) & padAcceptButton) != 0 && intAt(screen, 20) == 0) {
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "game_lvl_select.anm"), nullptr, 1.0f);
                const int stat = intAt(screen, 24);
                if (stat == 0) msgInventoryAdjustAbility__Fii(intAt(screen, 52), 0);
                else if (stat == 1) msgInventoryAdjustAbility__Fii(intAt(screen, 52), 1);
                else if (stat == 2) msgInventoryAdjustAbility__Fii(intAt(screen, 52), 2);
                else if (stat == 3) msgInventoryAdjustAbility__Fii(intAt(screen, 52), 3);
            }
        } else {
            statsRunSkillTree__Fi(slot);
            if (mode != 2) return;
            if (shopDisableMouse[slot] != 0) return;
            if ((*reinterpret_cast<unsigned*>(pad + 88 * intAt(screen, 52) + 12) & padBackButton) != 0) {
                intAt(screen, 76) = mode;
                record[242] = 0;
            }
            return;
        }
    } else if (mode == 1) {
        unsigned char* const questScreen = statsScreen + 116 * intAt(screen, 52);
        updateCursor__FiiiiiP12screenStructi(intAt(screen, 52), 0, intAt(player, 21864), 0, 40, questScreen, 1);
        textureDrawColor__FP8_textureiiiUi(lumpFindResource__FPcT0("inventry.lmp", "parchment.tex"), 320 * slot - 4, 104, 5, 0x80808080u);
        if (intAt(player, 21864) > 0) {
            int used = 0;
            int row = 0;
            int index = 0;
            unsigned char* quest = player + 17768;
            do {
                ++index;
                fontStart__FP4FontiUii(smallFont, 6, 0x80323c3cu, -1);
                fontColor__FUiii(0x80323c3cu, -1, 0);
                int y = 106 - intAt(questScreen, 36) + used + row;
                fontPrintWrapu__FiiPUsiiii(320 * slot + 24, y, textGetString__FPc(reinterpret_cast<char*>(quest)), 295, 82, 418, 21);
                fontColor__FUiii(0x802d2d2du, -1, 0);
                y = 128 - intAt(questScreen, 36) + used + row;
                row += 22;
                const int lines = fontPrintWrapu__FiiPUsiiii(320 * slot + 24, y, textGetString__FPc(reinterpret_cast<char*>(quest + 32)), 295, 82, 418, 21);
                quest += 64;
                used += lines;
                fontEnd__Fv();
            } while (index < intAt(player, 21864));
        }
        if (intAt(player, 21864) == 0) {
            fontStart__FP4FontiUii(smallFont, 6, 0x80323c3cu, -1);
            fontPrintCenteredu__FiiPUsi(320 * slot + 160, 224, textGetString__FPc("NoQuests"), 1000000);
            fontEnd__Fv();
        }
    }

    if (mode == 0) {
        char name[64];
        fontStart__FP4FontiUii(smallFont, 5, 0x80323c3cu, -1);
        formatText(name, "%s", reinterpret_cast<char*>(record));
        while (fontStringSize__FP4FontPci(smallFont, name, 100000) >= 221 && textLength(name) >= 11) name[textLength(name) - 1] = 0;
        const int shown = textLength(name);
        const int full = textLength(reinterpret_cast<char*>(record));
        if (shown != full) {
            char* end = name + textLength(name);
            end[0] = '.';
            end[1] = '.';
            end[2] = '.';
            end[3] = 0;
        }
        fontPrintCentered__FiiPci(320 * slot + 160, 107, name, 10000);

        UString level[64];
        if (intAt(record, 80) == 2) {
            UString* format = textGetString__FPc("StatShortDescription");
            usprintf__FPUsT0ffffffff(level, format, static_cast<float>(static_cast<signed char>(record[88])), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        } else {
            UString* format = textGetString__FPc("StatLevelDescription");
            usprintf__FPUsT0ffffffff(level, format, static_cast<float>(static_cast<signed char>(record[88])), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        }
        ustrcat__FPUsPc(level, " ");
        const char* className;
        if (static_cast<unsigned>(intAt(record, 84)) < 2u) className = "BarbarianWarrior";
        else if (intAt(record, 80) == 2) className = "DarkElfShadowKnight";
        else if (intAt(record, 80) == 1) className = "HighElfCleric";
        else if (intAt(record, 80) == 3) className = "WoodElfBard";
        else if (intAt(record, 80) == 4) className = "EruditeWizard";
        else className = "WoodElfBard";
        ustrcat__FPUsT0(level, textGetString__FPc(className));
        fontPrintCenteredu__FiiPUsi(320 * slot + 160, 129, level, 1000000);

        printStat__FiPUsiiii(slot, textGetString__FPc("StatExperience"), intAt(record, 92), -7, 1, 0);
        printStat__FiPUsiiii(slot, textGetString__FPc("StatNextLevel"), experienceTable[static_cast<signed char>(record[88])], -6, 1, 0);
        int offset = 0;
        if (language == 4) offset = 40;
        else if (language == 1) offset = 34;

        UString* label = textGetString__FPc("StatHealth");
        UString text[64];
        zeroFormat(text, label);
        const int base = 320 * slot;
        int x = base + offset;
        x -= fontStringSizeu__FP4FontPUsi(smallFont, text, 1000000);
        fontPrintu__FiiPUsi(x + 81, 208, text, 1000000);
        pairFormat(text, static_cast<float>(static_cast<int>(*reinterpret_cast<float*>(record + 172))),
                   static_cast<float>(static_cast<int>(*reinterpret_cast<float*>(record + 176))));
        const int center = base + offset + 147;
        fontPrintCenteredu__FiiPUsi(center, 208, text, 1000000);
        label = textGetString__FPc("StatEnergy");
        zeroFormat(text, label);
        x = base + offset - fontStringSizeu__FP4FontPUsi(smallFont, text, 1000000);
        fontPrintu__FiiPUsi(x + 81, 229, text, 1000000);
        pairFormat(text, static_cast<float>(static_cast<int>(*reinterpret_cast<float*>(record + 196))),
                   static_cast<float>(static_cast<int>(*reinterpret_cast<float*>(record + 200))));
        fontPrintCenteredu__FiiPUsi(center, 229, text, 1000000);

        printStat__FiPUsiiii(slot, textGetString__FPc("StatArmor"),
                             *reinterpret_cast<short*>(static_cast<unsigned char*>(wordPointer(record, 21872)) + 232), -3, 1, 0);
        const int weapon = static_cast<signed char>(record[23572]);
        playerRecomputeDamage__6PlayerPQ24item11descriptioni(wordPointer(record, 21872), wordPointer(record, 21960 + 4 * weapon), 1);
        printStat__FiPUsiiii(slot, textGetString__FPc("StatBaseAttack"), intAt(record, 208), -2, 1, 0);
        zeroFormat(text, textGetString__FPc("StatDamage"));
        x = base - fontStringSizeu__FP4FontPUsi(smallFont, text, 1000000);
        fontPrintu__FiiPUsi(x + 85, 306, text, 1000000);
        char range[64];
        formatText(range, "%d-%d", intAt(record, 212), intAt(record, 216));
        if (intAt(record, 21968) == 0 || static_cast<signed char>(record[23572]) == 1)
            fontPrintCentered__FiiPci(base + 163, 306, range, 10000);
        if (intAt(record, 21968) != 0 && static_cast<signed char>(record[23572]) == 0) {
            playerRecomputeDamage__6PlayerPQ24item11descriptioni(wordPointer(record, 21872), wordPointer(record, 21968), 1);
            char second[64];
            formatText(second, "/%d-%d", intAt(record, 212), intAt(record, 216));
            textAppend(range, second);
            fontPrintCentered__FiiPci(base + 163, 306, range, 10000);
        }
        const int highlight = mode == 2 ? 1 : 0;
        printStat__FiPUsiiii(slot, textGetString__FPc("StatStrength"), intAt(record, 144), 1, 0, highlight);
        printStat__FiPUsiiii(slot, textGetString__FPc("StatIntelligence"), intAt(record, 148), 2, 0, highlight);
        printStat__FiPUsiiii(slot, textGetString__FPc("StatDexterity"), intAt(record, 152), 3, 0, highlight);
        printStat__FiPUsiiii(slot, textGetString__FPc("StatStamina"), intAt(record, 156), 4, 0, highlight);
        fontEnd__Fv();
        if (highlight == 0) displayResistances__Fi(slot);
        const char* texture = (language == 4 || language == 1) ? "statsfull_german.tex" : "statsfull.tex";
        textureDrawColor__FP8_textureiiiUi(lumpFindResource__FPcT0("inventry.lmp", texture), 320 * slot + 4, 99, 4, 0x80808080u);
    }
    playerRecomputeAbilityModifiers__Fv();
}
