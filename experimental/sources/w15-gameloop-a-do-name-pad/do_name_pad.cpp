// Observed doNamePad__Fv at 0x0027f2c0, 1812 bytes, 48-byte frame. One frame of the on-screen name entry grid for the
// player `curPlayer`. Returns 0 normally, 1 when the name is finished and -1 when the back button was pressed. State is
// in file-local words of the original unit: otherHermiteTime, showNoNamePrompt, nameGridPos (the highlighted cell, with the
// special values -1, -2 and -3), namePos (characters typed), nameGridW and nameGridCount (grid width and cell count), the
// 26-byte letter table fontGrid and the 41-byte per-player record playerSelectInfo (the first 32 bytes are the name).
// pad is the array of 88-byte pad records; the words at +8, +12 and +16 are read as bit sets.
//  1. While otherHermiteTime is not exactly 1.0 it grows by 0.25, limited to 1.0, and the function returns 0 (a slide-in).
//  2. With showNoNamePrompt set, a "Continue" footer is formatted into hudFooterUni with padAcceptButtonFontChar as a
//     float, and the "nameWarnTitle" / "nameWarnBody" message is shown (alpha 128); pad word +12 & padAcceptButton clears
//     the flag. Returns 0.
//  3. Otherwise the cell is `target` = nameGridPos, or 0 when it is -3. A bit of pad word +16 selects one move, each
//     playing "menu_main_toggle.anm" from fx.lmp: 0x00101000 down-row wrap (up), 0x00408000 left, 0x00802000 right,
//     0x00204000 down. The new cell is given to setNewGridPosition(target, snap). The special cells -1 and -2 are the two
//     ends of the pad (done and space) and the moves between rows use the column remainders 0..3 and 4..7 and above;
//     the exact choices are in the code and the host check.
//  4. Pad word +8 bit 8 turns fontGrid to upper case (subtract 32) when its first byte is not 'A', and to lower case
//     (add 32) when its first byte is not 'a' and the bit is clear.
//  5. Pad word +16 bit 0x80 with namePos nonzero plays "menu_main_back.anm", lowers namePos and zeroes that byte of the name.
//  6. Pad word +12 & padBackButton returns -1. Without padAcceptButton it returns 0. With it, "menu_main_accept.anm" is
//     played and: at cell -2 a space is appended (if namePos < 30); at cell -1 the name is checked: only spaces (or empty)
//     sets showNoNamePrompt and returns 0, anything else returns 1; at any other cell fontGrid[cell] is appended (if
//     namePos < 30). These return 0.
// Inferred: this is the controller of the name pad; -1 is "done", -2 is "space" and -3 means nothing highlighted yet.
// Unknown: the pad word names, the layout of the grid beyond width and count, the meaning of the column remainders (the
// grid has letter columns and button columns), and the types of the words at pad +8, +12 and +16.
typedef unsigned short UString;

#if defined(__mips__)
#define LIBRARY_SYMBOL(name) __asm__(#name)
#else
#define LIBRARY_SYMBOL(name)
#endif

extern "C" {
extern float otherHermiteTime;
extern unsigned char showNoNamePrompt;
extern int curPlayer;
extern int nameGridPos;
extern int namePos;
extern int nameGridW;
extern int nameGridCount;
extern int padAcceptButtonFontChar;
extern unsigned padAcceptButton;
extern unsigned padBackButton;
extern unsigned char pad[];
extern unsigned char fontGrid[];
extern unsigned char playerSelectInfo[];
extern UString hudFooterUni[];

UString* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(UString* destination, UString* format, float a, float b, float c, float d, float e, float f, float g, float h);
void hudShowMsg__FPUsN20iii(UString* title, UString* body, UString* footer, int alpha, int a, int b);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float volume);
void setNewGridPosition__Fii(int cell, int snap);
unsigned lengthOf(const char* text) LIBRARY_SYMBOL(strlen);
}

namespace {
inline unsigned padWord(int slot, int offset) { return *reinterpret_cast<unsigned*>(pad + 88 * slot + offset); }
inline void playMenuSound(const char* name) {
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", name), nullptr, 1.0f);
}
inline unsigned char* nameOf(int player) { return playerSelectInfo + 41 * player; }
}

extern "C" int doNamePad__Fv() {
    const float time = otherHermiteTime;
    if (time != 1.0f) {
        const float next = time + 0.25f;
        otherHermiteTime = 1.0f < next ? 1.0f : next;
        return 0;
    }
    if (showNoNamePrompt) {
        UString* footer = textGetString__FPc("MemcardContinue");
        usprintf__FPUsT0ffffffff(hudFooterUni, footer, static_cast<float>(padAcceptButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f);
        UString* title = textGetString__FPc("nameWarnTitle");
        UString* body = textGetString__FPc("nameWarnBody");
        hudShowMsg__FPUsN20iii(title, body, hudFooterUni, 128, 0, 0);
        if (padWord(curPlayer, 12) & padAcceptButton) showNoNamePrompt = 0;
        return 0;
    }
    int target = nameGridPos == -3 ? 0 : nameGridPos;
    int snap = 0;
    const unsigned buttons = padWord(curPlayer, 16);
    if (buttons & 0x00101000u) {
        playMenuSound("menu_main_toggle.anm");
        if (target < 0) {
            if (target == -2) target = nameGridCount - nameGridW + 1;
            else if (target == -1) target = nameGridCount - nameGridW + 5;
        } else {
            const int up = target - nameGridW;
            if (up >= 0) {
                target = up;
            } else {
                const int column = target % nameGridW;
                snap = 1;
                if (column < 8) {
                    if (column < 4) target += 6 * nameGridW;
                    else target = -2;
                } else {
                    target = -1;
                }
            }
        }
    } else if (buttons & 0x00408000u) {
        playMenuSound("menu_main_toggle.anm");
        if (target != -2) {
            const int column = target % nameGridW;
            if (column != 0) {
                target -= 1;
            } else {
                const int wrapped = target + nameGridW - 1;
                target = -1;
                snap = 1;
                if (nameGridPos / nameGridW != 6) target = wrapped;
            }
        } else {
            snap = 1;
            target = nameGridCount - 1;
        }
    } else if (buttons & 0x00802000u) {
        playMenuSound("menu_main_toggle.anm");
        if (target != -1) {
            const int column = target % nameGridW;
            if (column == nameGridW - 1) {
                target = target - nameGridW + 1;
                snap = 1;
            } else {
                target += 1;
                if (!(target < nameGridCount)) {
                    target = -2;
                    snap = 1;
                }
            }
        } else {
            snap = 1;
            target = nameGridCount - nameGridCount % nameGridW;
        }
    } else if (buttons & 0x00204000u) {
        playMenuSound("menu_main_toggle.anm");
        if (target >= 0) {
            const int down = target + nameGridW;
            if (down < nameGridCount) {
                target = down;
            } else {
                const int column = target % nameGridW;
                if (column < 8) {
                    if (column < 4) {
                        snap = 1;
                        target -= 6 * nameGridW;
                    } else {
                        target = -2;
                    }
                } else {
                    target = -1;
                }
            }
        } else {
            snap = 1;
            target = target == -1 ? nameGridW - 1 : nameGridW - 5;
        }
    }
    setNewGridPosition__Fii(target, snap);

    if (padWord(curPlayer, 8) & 8u) {
        if (fontGrid[0] != 'A')
            for (int i = 0; i < 26; ++i) fontGrid[i] = static_cast<unsigned char>(fontGrid[i] + 224);
    } else {
        if (fontGrid[0] != 'a')
            for (int i = 0; i < 26; ++i) fontGrid[i] = static_cast<unsigned char>(fontGrid[i] + 32);
    }
    if ((padWord(curPlayer, 16) & 0x80u) && namePos != 0) {
        playMenuSound("menu_main_back.anm");
        namePos = namePos - 1;
        nameOf(curPlayer)[namePos] = 0;
    }
    const unsigned pressed = padWord(curPlayer, 12);
    if (pressed & padBackButton) return -1;
    if (!(pressed & padAcceptButton)) return 0;
    playMenuSound("menu_main_accept.anm");
    if (nameGridPos == -2) {
        if (!(namePos < 30)) return 0;
        nameOf(curPlayer)[namePos] = ' ';
        namePos = namePos + 1;
        return 0;
    }
    if (nameGridPos == -1) {
        int i = 0;
        while (i < static_cast<int>(lengthOf(reinterpret_cast<const char*>(nameOf(curPlayer)))) &&
               static_cast<char>(nameOf(curPlayer)[i]) == ' ')
            ++i;
        if (i != static_cast<int>(lengthOf(reinterpret_cast<const char*>(nameOf(curPlayer))))) return 1;
        showNoNamePrompt = 1;
        return 0;
    }
    if (!(namePos < 30)) return 0;
    nameOf(curPlayer)[namePos] = fontGrid[nameGridPos];
    namePos = namePos + 1;
    return 0;
}
