// Observed displayItemStats__FiiiPQ24item11descriptioniii at 0x001b49c8, 7524 bytes.
// Arguments: player index, x, y, the item cell, a tint, a shade and a line count.
// Returns nothing. Nothing here was executed on the EE.
//
// The cell: word 0 points to a base record; four 8-byte entries start at +36, each a
// signed half "kind" (-1 when unused) then three signed half values (+2, +4, +6); a 64-bit flag word
// is at +8 (bit 0x100 adds 21 to y in the first layout; bit 0x800000000 enables the two
// tag lines of the third layout). The base record has a name pointer at +0, signed bytes
// at +28 (called kind8 below: 8 is special) and +29 (the slot, 0, 1, 2 or other), and a
// word at +32. A "value of kind k" is the value half of the first entry whose kind is k,
// or 0 when none matches (the first half unless stated: kinds 21 to 25 and 38 also read the
// second half, 31, 32 and 34 the second and third, and 33 uses the second as a cloud index).
//
// Colours: c0 = 0x80000000 | tint * 0x010101, c1 = 0x80000000 | shade * 0x010101. The
// language byte (0x4844b4) picks the box width and x offset: 4 gives (299, -150), 3 gives
// (253, -129), anything else (223, -109). itemInfoHeight[player] counts text lines.
//
// Header, by slot (all through smallFont and textureDrawSuperColor with mode 6, limits 639 and
// 447):
//  - A power-up row (slots 0 and others except slot 2, kind8 8 and names containing
//    "potion" skip it): four icons from inventry.lmp, "powerupfull.tex" for the first n
//    entries whose kind is not -1 and "powerupemt.tex" for the rest, at offsets (75,2),
//    (87,2), (75,14), (87,14) (shifted by (-39,-3) for slot 1).
//  - Slot 0: icon tinyIcon[6] at (x+12, y'+23), y' = y + (flag 0x100 ? 21 : 0) + (kind8 8 ? -19 : 0);
//    the weight (getWeight times 0.5 when kind8 is 8 and playerGetFeatRank(player, 0x10000) is set,
//    printed "%.1f") centred at (x + width + 21, y' + 20); unless kind8 is 8, icon tinyIcon[8]
//    (if the base word at +32 is 1) or tinyIcon[2] at (x-33, y'), and the damage range
//    getDamageRangeAll -> "%d-%d" (or "%d" when equal; a low value below 1 reads as 1) at (x-10, y'-4).
//  - Slot 1: tinyIcon[0] at (x-31, y+3), tinyIcon[6] at (x+3, y+23), the armour bonus "%d"
//    at (x-11, y-3) and the weight at (x + width + 12, y + 20).
//  - Other slots: tinyIcon[6] at (x+20, y'+3), the weight at (x+40, y') and, when flag
//    0x800000000 is set, two lines built from the texts "WeaponTag"/"ArmorTag" plus, for the
//    first of seventeen name fragments found in the base name, that fragment's "...weapdesc"
//    and "...armordesc" texts; they are centred at the box centre, 20 and 40 below y'.
//
// Body: slot 0 non-potion non-kind8 items add an "AttackBonus" line (getAttackBonus) and start the
// stat lines at 60 instead of 40. Then for each kind 0..41 with a non-zero value and a
// non-null key in a 41-entry key table (kind 41 reads zero): a line is formed and printed; kinds
// 14..17 and 19 turn the text red (0x80050a40) when the player's stat is short (level byte at player +
// 89 for 14, class word at +80 for 19, then +144, +148, +152 for kinds 15..17; kind 18 is never
// red: the code that would test +156 is unreachable in the image; kind 12 is red when the word
// at +80 is zero). Kind 19 prints a class key from a five-entry table, kind 20 prints two pieces
// ("AttackSpeed" then a speed key), kinds 21 to 25 and 38 pass the value twice, 31, 32, 34
// three times, 33 picks a cloud key from a four-entry table; kind 13 on slot 1 prints nothing.
// Lines advance by 20 and itemInfoHeight by 1 unless suppressed. The widest line width is
// tracked and the box is closed by textureSmoothBorder with a width grown by the overshoot.
//
// Not reproduced: the original reads the stack word just past its 41-entry key copy for kind 41;
// here that entry is a null key. The 256-halfword line buffer is zero-filled at the start
// (the original leaves it unwritten until the first line).
// Inferred: the key names are text keys (display strings are not published here).
// Unknown: the role of the 32 bytes at cell+4, the meaning of kind8 8 and slot 2, and what each
// kind number denotes beyond its key.
struct Texture;
struct Font;

extern "C" {
extern signed char language;
extern Font* smallFont;
extern int itemInfoHeight[];
extern void* tinyIcon[];
extern unsigned char player[];

char* strstr(const char*, const char*);
int sprintf(char*, const char*, ...);
Texture* lumpFindResource__FPcT0(const char*, const char*);
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(Texture*, int, int, int, unsigned, int, int, int, int, char, char);
void textureSmoothBorder__FiiiiiUiiii(int, int, int, int, int, unsigned, int, int, int);
void fontStart__FP4FontiUii(Font*, int, unsigned, int);
void fontScissor__Fiiii(int, int, int, int);
void fontEnd__Fv();
void fontColor__FUiii(unsigned, int, int);
void fontPrint__FiiPci(int, int, char*, int);
void fontPrintCentered__FiiPci(int, int, char*, int);
void fontPrintu__FiiPUsi(int, int, unsigned short*, int);
void fontPrintCenteredu__FiiPUsi(int, int, unsigned short*, int);
int fontStringSize__FP4FontPci(Font*, char*, int);
int fontStringSizeu__FP4FontPUsi(Font*, unsigned short*, int);
unsigned short* textGetString__FPc(const char*);
void usprintf__FPUsT0ffffffff(unsigned short*, unsigned short*, float, float, float, float, float, float, float, float);
void ustrcat__FPUsT0(unsigned short*, unsigned short*);
int playerGetFeatRank__Fix(int, long long);
float getWeight__4itemPQ24item11description(const void*);
int getArmorBonus__4itemPQ24item11description(const void*);
int getAttackBonus__4itemPQ24item11description(const void*);
void getDamageRangeAll__4itemPQ24item11descriptionPiT2i(const void*, int*, int*, int);
unsigned long long fptodp(float);
}

namespace {
typedef unsigned long long u64;

inline const unsigned char* pointerOf(unsigned word) {
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
inline unsigned wordAt(const unsigned char* where) {
    unsigned value;
    __builtin_memcpy(&value, where, 4);
    return value;
}
inline int halfAt(const unsigned char* where) {
    short value;
    __builtin_memcpy(&value, where, 2);
    return value;
}
inline int intAt(const unsigned char* where) {
    int value;
    __builtin_memcpy(&value, where, 4);
    return value;
}
// part 0 is the value half at +2 of the entry, parts 1 and 2 the halves at +4 and +6.
inline int valueOfKind(const unsigned char* cell, int kind, int part = 0) {
    for (int i = 0; i < 4; ++i)
        if (halfAt(cell + 36 + 8 * i) == kind) return halfAt(cell + 38 + 8 * i + 2 * part);
    return 0;
}

const char* const keys[42] = {
    "FireResistanceFull", "ColdResistanceFull", "PoisonResistanceFull", "DiseaseResistanceFull", "MagicResistanceFull",
    "HealthBonus", "ManaBonus", "StrengthBonus", "IntelligenceBonus", "DexterityBonus", "StaminaBonus", "HealthRegen",
    "ManaRegen", "ArmorBonusNew", "ReqLevel", "ReqStrength", "ReqIntelligence", "ReqDexterity", "ReqStamina", "ReqClass",
    "AttackSpeed", "FireTag", "ColdTag", "PoisonTag", "LightningTag", "BugZapperTag", "BugExploderTag", "UndeadBonusTag",
    "UndeadDestroyTag", "CriticalHitTag", "StunResistanceTag", "FireBurstTag", "FrostBurstTag", "PoisonBurstTag",
    "LightningBurstTag", nullptr, nullptr, nullptr, "DiseaseDamageTag", "ReqBlunt", "ReqSlashing", nullptr};
const char* const classKeys[5] = {"WarriorOnly", "ClericOnly", "SKnightOnly", "RangerOnly", "WizardOnly"};
const char* const speedKeys[7] = {"Speed1", "Speed2", "Speed3", "Speed4", "Speed5", "Speed6", "Speed7"};
const char* const cloudKeys[4] = {"SmallPoisonCloud", "MediumPoisonCloud", "LargePoisonCloud", "HugePoisonCloud"};

struct Fragment {
    const char* name;
    const char* weapon;
    const char* armor;
};
const Fragment fragments[17] = {
    {"fireeye", "fireeyeweapdesc", "fireeyearmordesc"},
    {"bonechip", "bonechipweapdesc", "bonechiparmordesc"},
    {"venomsac", "venomsacweapdesc", "venomsacarmordesc"},
    {"poisongland", "poisonglandweapdesc", "poisonglandarmordesc"},
    {"mummytongue", "mummytongueweapdesc", "mummytonguearmordesc"},
    {"stormshard", "stormshardweapdesc", "stormshardarmordesc"},
    {"soulgem", "soulgemweapdesc", "soulgemarmordesc"},
    {"vampirefang", "vampirefangweapdesc", "vampirefangarmordesc"},
    {"bullrune", "bullruneweapdesc", "bullrunearmordesc"},
    {"starglyph", "starglyphweapdesc", "starglypharmordesc"},
    {"ghoulflesh", "ghoulfleshweapdesc", "ghoulflesharmordesc"},
    {"bloodstone", "bloodstoneweapdesc", "bloodstonearmordesc"},
    {"regenstone", "regenstoneweapdesc", "regenstonearmordesc"},
    {"manastone", "manastoneweapdesc", "manastonearmordesc"},
    {"carapace", "carapaceweapdesc", "carapacearmordesc"},
    {"mistyglobe", "mistyglobeweapdesc", "mistyglobearmordesc"},
    {"whetstone", "whetstoneweapdesc", "whetstonearmordesc"}};

const int powerOffsets[8] = {75, 2, 87, 2, 75, 14, 87, 14};

inline void drawIcon(Texture* texture, int x, int y, unsigned color) {
    textureDrawSuperColor__FP8_textureiiiUiiiiicc(texture, x, y, 6, color, 0, 0, 639, 447, 0, 0);
}
inline void printText(unsigned short* out, const char* key, float a, float b, float c) {
    usprintf__FPUsT0ffffffff(out, textGetString__FPc(key), a, b, c, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}
}  // namespace

extern "C" void displayItemStats__FiiiPQ24item11descriptioniii(int pn, int x, int y, const unsigned char* cell,
                                                               int tint, int shade, int lines) {
    const unsigned char* const base = pointerOf(wordAt(cell));
    const int kind8 = static_cast<signed char>(base[28]);
    const int slot = static_cast<signed char>(base[29]);
    const u64 flags = *reinterpret_cast<const u64*>(cell + 8);
    const char* const baseName = reinterpret_cast<const char*>(pointerOf(wordAt(base)));
    const int slotBias = kind8 == 8 ? -19 : 0;
    const int extraY = (flags & 0x100) ? 21 : 0;
    int boxWidth = 223;
    int boxOffset = -109;
    if (language == 4) {
        boxWidth = 299;
        boxOffset = -150;
    } else if (language == 3) {
        boxWidth = 253;
        boxOffset = -129;
    }
    const unsigned grey0 = static_cast<unsigned>(tint) | (static_cast<unsigned>(tint) << 8) | (static_cast<unsigned>(tint) << 16);
    const unsigned color0 = grey0 | 0x80000000u;
    const unsigned color1 = (static_cast<unsigned>(shade) | (static_cast<unsigned>(shade) << 16) | (static_cast<unsigned>(shade) << 8)) | 0x80000000u;
    unsigned short line[256] = {};
    char text[128];
    int yy = y;
    int maxWidth = 0;

    if (slot != 2 && kind8 != 8 && strstr(baseName, "potion") == nullptr) {
        int filled = 0;
        for (int i = 0; i < 4; ++i)
            if (halfAt(cell + 36 + 8 * i) != -1) ++filled;
        for (int k = 0; k < 4; ++k) {
            int dx = 0;
            int dy = 0;
            if (static_cast<signed char>(base[29]) == 1) {
                dx = -39;
                dy = -3;
            }
            Texture* icon = k < filled ? lumpFindResource__FPcT0("inventry.lmp", "powerupfull.tex")
                                       : lumpFindResource__FPcT0("inventry.lmp", "powerupemt.tex");
            drawIcon(icon, x + dx + powerOffsets[2 * k], y + dy + powerOffsets[2 * k + 1], color0);
        }
    }

    int featHalf;  // 1 when the weight is halved
    int leftEdge;  // x + box offset
    if (slot == 0) {
        yy = y + extraY;
        const int s2 = yy + slotBias;
        drawIcon(static_cast<Texture*>(tinyIcon[6]), x + 12, s2 + 23, color0);
        fontStart__FP4FontiUii(smallFont, 6, color1, -1);
        fontScissor__Fiiii(0, 0, 639, 447);
        featHalf = 0;
        if (kind8 == 8 && playerGetFeatRank__Fix(pn, 0x10000) != 0) featHalf = 1;
        sprintf(text, "%.1f", fptodp(getWeight__4itemPQ24item11description(cell) * (featHalf ? 0.5f : 1.0f)));
        {
            const int width = fontStringSize__FP4FontPci(smallFont, text, 100000);
            itemInfoHeight[pn] += 1;
            fontPrintCentered__FiiPci(x + width + 21, s2 + 20, text, 10000);
        }
        fontEnd__Fv();
        if (kind8 != 8) {
            if (intAt(base + 32) == 1) drawIcon(static_cast<Texture*>(tinyIcon[8]), x - 33, yy, color0);
            else drawIcon(static_cast<Texture*>(tinyIcon[2]), x - 33, yy, color0);
            fontStart__FP4FontiUii(smallFont, 6, color1, -1);
            fontScissor__Fiiii(0, 0, 639, 447);
            int low, high;
            getDamageRangeAll__4itemPQ24item11descriptionPiT2i(cell, &low, &high, 0);
            if (low <= 0) low = 1;
            if (low != high) sprintf(text, "%d-%d", low, high);
            else sprintf(text, "%d", low);
            fontStringSize__FP4FontPci(smallFont, text, 100000);
            itemInfoHeight[pn] += 1;
            fontPrint__FiiPci(x - 10, yy - 4, text, 100000);
            fontEnd__Fv();
        }
        leftEdge = x + boxOffset;
    } else if (slot == 1) {
        drawIcon(static_cast<Texture*>(tinyIcon[0]), x - 31, y + 3, color0);
        drawIcon(static_cast<Texture*>(tinyIcon[6]), x + 3, y + 23, color0);
        fontStart__FP4FontiUii(smallFont, 6, color1, -1);
        fontScissor__Fiiii(0, 0, 639, 447);
        sprintf(text, "%d", getArmorBonus__4itemPQ24item11description(cell));
        fontPrint__FiiPci(x - 11, y - 3, text, 100000);
        itemInfoHeight[pn] += 1;
        featHalf = 0;
        if (kind8 == 8 && playerGetFeatRank__Fix(pn, 0x10000) != 0) featHalf = 1;
        leftEdge = x + boxOffset;
        sprintf(text, "%.1f", fptodp(getWeight__4itemPQ24item11description(cell) * (featHalf ? 0.5f : 1.0f)));
        {
            const int width = fontStringSize__FP4FontPci(smallFont, text, 100000);
            fontPrint__FiiPci(x + width + 12, y + 20, text, 100000);
        }
        itemInfoHeight[pn] += 1;
        fontEnd__Fv();
    } else {
        const int s3 = y + extraY;
        drawIcon(static_cast<Texture*>(tinyIcon[6]), x + 20, s3 + 3, color0);
        featHalf = 0;
        if (kind8 == 8 && playerGetFeatRank__Fix(pn, 0x10000) != 0) featHalf = 1;
        fontStart__FP4FontiUii(smallFont, 6, color1, -1);
        fontScissor__Fiiii(0, 0, 639, 447);
        sprintf(text, "%.1f", fptodp(getWeight__4itemPQ24item11description(cell) * (featHalf ? 0.5f : 1.0f)));
        fontPrint__FiiPci(x + 40, s3, text, 100000);
        itemInfoHeight[pn] += 2;
        leftEdge = x + boxOffset;
        if ((flags & 0x800000000ull) != 0) {
            unsigned short weaponText[128];
            unsigned short armorText[128];
            usprintf__FPUsT0ffffffff(weaponText, textGetString__FPc("WeaponTag"), 0, 0, 0, 0, 0, 0, 0, 0);
            usprintf__FPUsT0ffffffff(armorText, textGetString__FPc("ArmorTag"), 0, 0, 0, 0, 0, 0, 0, 0);
            for (int f = 0; f < 17; ++f) {
                if (strstr(baseName, fragments[f].name) != nullptr) {
                    ustrcat__FPUsT0(weaponText, textGetString__FPc(fragments[f].weapon));
                    ustrcat__FPUsT0(armorText, textGetString__FPc(fragments[f].armor));
                    break;
                }
            }
            const int centre = leftEdge + (boxWidth >> 1);
            fontPrintCenteredu__FiiPUsi(centre, s3 + 20, weaponText, 1000000);
            fontPrintCenteredu__FiiPUsi(centre, s3 + 40, armorText, 1000000);
            const int sizeWeapon = fontStringSizeu__FP4FontPUsi(smallFont, weaponText, 1000000);
            const int sizeArmor = fontStringSizeu__FP4FontPUsi(smallFont, armorText, 1000000);
            maxWidth = sizeWeapon < sizeArmor ? sizeArmor : sizeWeapon;
            itemInfoHeight[pn] += 2;
        }
        fontEnd__Fv();
    }

    // stat lines
    fontStart__FP4FontiUii(smallFont, 6, color1, -1);
    int lineOffset = 40;
    fontScissor__Fiiii(0, 0, 640, 422);
    const int centreX = leftEdge + (boxWidth >> 1);
    if (slot == 0 && kind8 != 8 && strstr(baseName, "potion") == nullptr) {
        lineOffset = 60;
        const int attack = getAttackBonus__4itemPQ24item11description(cell);
        usprintf__FPUsT0ffffffff(line, textGetString__FPc("AttackBonus"), static_cast<float>(attack), 0.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f, 0.0f);
        fontPrintCenteredu__FiiPUsi(centreX, yy + 40, line, 500);
        const int size = fontStringSizeu__FP4FontPUsi(smallFont, line, 1000000);
        itemInfoHeight[pn] += 1;
        if (maxWidth < size) maxWidth = size;
    }

    const unsigned char* const record = player + pn * 23984;
    const int leftLabel = x - 83;
    const int classWord = intAt(record + 80);
    for (int kind = 0; kind < 42; ++kind) {
        const int y5 = yy + lineOffset;
        bool suppress = false;
        const int value = valueOfKind(cell, kind);
        if (value == 0) continue;
        const char* const key = keys[kind];
        if (key == nullptr) continue;
        if (kind == 12 && classWord == 0) fontColor__FUiii(0x80050a40u, -1, 0);
        else fontColor__FUiii(color1, -1, 0);
        if (kind == 19) {
            if (classWord != value) fontColor__FUiii(0x80050a40u, -1, 0);
            usprintf__FPUsT0ffffffff(line, textGetString__FPc(classKeys[valueOfKind(cell, 19) - 1]), 0, 0, 0, 0, 0, 0, 0, 0);
            fontPrintCenteredu__FiiPUsi(centreX, y5, line, 1000000);
        } else {
            if (kind == 15) {
                if (intAt(record + 144) < value) fontColor__FUiii(0x80050a40u, -1, 0);
            } else if (kind == 16) {
                if (intAt(record + 148) < value) fontColor__FUiii(0x80050a40u, -1, 0);
            } else if (kind == 17) {
                if (intAt(record + 152) < value) fontColor__FUiii(0x80050a40u, -1, 0);
            } else if (kind == 14) {
                if (static_cast<signed char>(record[89]) < value) fontColor__FUiii(0x80050a40u, -1, 0);
            }
            if (kind == 20) {
                usprintf__FPUsT0ffffffff(line, textGetString__FPc(key), 0, 0, 0, 0, 0, 0, 0, 0);
                fontPrintu__FiiPUsi(leftLabel, y5, line, 1000000);
                const int first = fontStringSizeu__FP4FontPUsi(smallFont, line, 1000000);
                usprintf__FPUsT0ffffffff(line, textGetString__FPc(speedKeys[valueOfKind(cell, 20) + 2]), 0, 0, 0, 0, 0, 0, 0, 0);
                fontPrintu__FiiPUsi(leftLabel + first + 10, y5, line, 1000000);
            } else if (kind == 21 || kind == 22 || kind == 23 || kind == 24 || kind == 25 || kind == 38) {
                const unsigned short* t = textGetString__FPc(key);
                const float a = static_cast<float>(valueOfKind(cell, kind));
                const float b = static_cast<float>(valueOfKind(cell, kind, 1));
                usprintf__FPUsT0ffffffff(line, const_cast<unsigned short*>(t), a, b, 0, 0, 0, 0, 0, 0);
                fontPrintCenteredu__FiiPUsi(centreX, y5, line, 1000000);
            } else if (kind == 31 || kind == 32 || kind == 34) {
                const unsigned short* t = textGetString__FPc(key);
                const float a = static_cast<float>(valueOfKind(cell, kind));
                const float b = static_cast<float>(valueOfKind(cell, kind, 1));
                const float c = static_cast<float>(valueOfKind(cell, kind, 2));
                usprintf__FPUsT0ffffffff(line, const_cast<unsigned short*>(t), a, b, c, 0, 0, 0, 0, 0);
                fontPrintCenteredu__FiiPUsi(centreX, y5, line, 1000000);
            } else if (kind == 33) {
                const unsigned short* t = textGetString__FPc(cloudKeys[valueOfKind(cell, 33, 1)]);
                const float a = static_cast<float>(valueOfKind(cell, 33));
                usprintf__FPUsT0ffffffff(line, const_cast<unsigned short*>(t), a, 0, 0, 0, 0, 0, 0, 0);
                fontPrintCenteredu__FiiPUsi(centreX, y5, line, 1000000);
            } else if (slot == 1 && kind == 13) {
                suppress = true;
            } else {
                const unsigned short* t = textGetString__FPc(key);
                usprintf__FPUsT0ffffffff(line, const_cast<unsigned short*>(t), static_cast<float>(valueOfKind(cell, kind)), 0,
                                         0, 0, 0, 0, 0, 0);
                fontPrintCenteredu__FiiPUsi(centreX, y5, line, 1000000);
            }
        }
        const int size = fontStringSizeu__FP4FontPUsi(smallFont, line, 1000000);
        if (maxWidth < size) maxWidth = size;
        if (!suppress) {
            lineOffset += 20;
            itemInfoHeight[pn] += 1;
        }
    }
    fontEnd__Fv();

    const int full = lines * 22;
    int trim = full - 22;
    if (slot != 2 && (flags & 0x100) != 0) {
        trim = full;
        itemInfoHeight[pn] += 1;
    }
    if (boxWidth < maxWidth) {
        const int extra = maxWidth - boxWidth + 14;
        textureSmoothBorder__FiiiiiUiiii(leftEdge - extra / 2, yy - trim - 22, boxWidth + extra, itemInfoHeight[pn] * 20 + 17, 6,
                                         0x80000000u, 0, 0, 0);
    } else {
        textureSmoothBorder__FiiiiiUiiii(leftEdge, yy - trim - 22, boxWidth, itemInfoHeight[pn] * 20 + 17, 6, 0x80000000u, 0, 0,
                                         0);
    }
}
