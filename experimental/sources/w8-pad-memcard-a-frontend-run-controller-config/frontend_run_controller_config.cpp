// Observed: frontendRunControllerConfig__Fv at 0x001c98f0 is 3128 bytes. "pad" below is the
// 352-byte symbol pad, read as 88-byte records; word N of a record is the word at byte N.
// Shutdown. With inGameConfigShuttingDown nonzero it only counts finishFrameCount up; when
// the count is 3 it first calls lumpUnload__FPv(myLumpBuffer) and textureReleaseCacheBuffer__Fv
// and stores 0 in frontendControllerConfigRunning. Then it returns.
// Lump load. With initializedLump zero it stores 10 in engineDisplayDisable and then: a zero
// myLumpBuffer takes (textureLockCacheBuffer__Fv() + 255) & -256; otherwise a zero
// askedForConfigLoad stores 1 and calls cdLoadAsync__FPcPvT0ii("config.lmp", myLumpBuffer,
// &configLumpLoaded, 0, 0); otherwise a nonzero configLumpLoaded byte calls
// lumpAdd__FPvPCc(myLumpBuffer, "config.lmp") and stores 1 in initializedLump. It returns
// while initializedLump is still zero.
// Labels. A stack array of 14 two-halfword strings holds the glyph 123 + i followed by zero.
// configMode 0 (choose a row). t is a float animation position.
//  - With t == 0 and the record for curFocus: a bit of 0x101000 in word 16 with curSel nonzero
//    plays menu_main_toggle.anm and stores curSel - 1 in newSel; a bit of 0x204000 with curSel
//    below 8 plays it and stores curSel + 1 in newSel.
//  - With t == 1.0 it stores 0.0 in t and newSel in curSel. Other values of t change nothing here.
//  - When curSel differs from newSel: a pending flag is set if t is 0, then 0.25 is added to t,
//    limited to 1.0.
//  - With t == 0: a padAcceptButton bit in word 12 plays menu_main_accept.anm and stores 1 in
//    configMode. A bit of 0x80 in word 12 plays it and calls gameSetDefaultGameConfig__Fi(curFocus).
//  - A padBackButton bit in word 12, or a zero word 4, plays menu_main_back.anm, stores 0 in
//    curFocus and gameNoDraw, calls coreStartLoading__Fv, stores 0 in the byte hudNoDraw and 1 in
//    inGameConfigShuttingDown and returns.
//  - Otherwise it prints text: fontStart(font, 7, 0x64464646, -1), usprintf of text
//    "SelectDefaultsBack" with (float)padAcceptButtonFontChar, 125.0, (float)padBackButtonFontChar
//    and five zeros, fontPrintCenteredu(320, 399, ..., 1000000), fontEnd. The font is smallFont, or
//    the lump resource "gamefontsmall.fnt" of "frontend.lmp" while frontEndRunning is nonzero.
// configMode 1 (choose a button). It prints "newConfigButton" at (320, 360) and the key
//  configStrings[curSel] in colour 0x802d5049 at (320, 377). It then scans the 14 words
//  buttonValues; the first one with a set bit in word 12 of the pad record picks the new value.
//  The first of the nine words at MC_GameConfig + 52 + 36 * curFocus equal to that value
//  (nine when none) swaps with word curSel of the same record: word curSel takes the value, and the
//  found word, if any, takes the old word curSel. It plays menu_main_accept.anm and stores 0 in
//  configMode.
// Any other configMode value does neither.
// Drawing. textureDrawBorder(342, 117, 268, 217, 6, 0x80808080, group, 1, 0) with group the string
//  "frontend.lmp" while frontEndRunning is nonzero, else null; the font prints "PlayerConfiguration"
//  with (float)(curFocus + 1) at (475, 47) in colour 0x802d5049; for rows 0 to 8 it finds the index of
//  the row's value among buttonValues (14 when none, which also sets the row counter to 14), prints that
//  index's glyph label at (360, 23 * row + 118) and the key configStrings[row] at (392, same y), dimmed
//  with colour 0x80162824, 64 when the row is not curSel and configMode is nonzero, else colour
//  0x802d5049, -1. Five fixed captions follow in colour 0x80323232: "cstatic_move" right-aligned to
//  104 at y 279, "cstatic_inventory" right-aligned to 134 at y 313, "cstatic_camera" at (250, 280),
//  "cstatic_pause" at (224, 312), "cstatic_spells" right-aligned to 82 at y 246. A highlight
//  rectangleDraw(349, y - 11, 603, y + 13, 6, 0x50111824) is placed at y = int(interpolated row y) + 14,
//  the row y being 23 * row + 118 and the interpolation evalHermite(t, 0, 1, 0, 0) between curSel and
//  newSel. Two textureDrawBorder calls follow: (340, 42, 270, 40, 6, 0x80808080, group, 0, 0) and
//  (342, 117, 268, 217, 6, 0x80808080, group, 0, 1). The texture dualshock2.tex (the lump resource of
//  "frontend.lmp" while frontEndRunning is nonzero, else field 4 of lumpFind(myLumpBuffer, name)) is drawn
//  with textureDrawColor(texture, 0, 0, 6, 0x80808080). A pending flag from the row move plays
//  menu_main_accept.anm.
// Sounds are lumpFindResource__FPcT0("fx.lmp", name) passed to soundPlaySequence with no object and 1.0.
// Inferred: nothing.
// Unknown: the meanings of words 4, 12 and 16 of the pad record beyond their use here; the layout of
// the config record; why the row loop stops at 14 for an unknown button value; the original kept
// configStrings[14] unread-safe only by chance.

struct Font;
struct AnimationHeader;

extern "C" void lumpUnload__FPv(void* buffer);
extern "C" void textureReleaseCacheBuffer__Fv();
extern "C" unsigned char* textureLockCacheBuffer__Fv();
extern "C" void cdLoadAsync__FPcPvT0ii(char* name, void* buffer, void* done, int a, int b);
extern "C" void lumpAdd__FPvPCc(void* buffer, const char* name);
extern "C" void* lumpFindResource__FPcT0(char* group, char* name);
extern "C" unsigned char* lumpFind__FPvPc(void* buffer, char* name);
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
extern "C" void gameSetDefaultGameConfig__Fi(int player);
extern "C" void coreStartLoading__Fv();
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);
extern "C" void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b);
extern "C" void fontEnd__Fv();
extern "C" void fontColor__FUiii(unsigned color, int a, int b);
extern "C" void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int width);
extern "C" void fontPrintu__FiiPUsi(int x, int y, unsigned short* text, int width);
extern "C" int fontStringSizeu__FP4FontPUsi(void* font, unsigned short* text, int width);
extern "C" void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned color,
                                                char* group, int b, int c);
extern "C" void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
extern "C" void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int z, unsigned color);
extern "C" float evalHermite__Ffffff(float t, float a, float b, float c, float d);
extern "C" int inGameConfigShuttingDown;
extern "C" int finishFrameCount;
extern "C" int frontendControllerConfigRunning;
extern "C" int initializedLump;
extern "C" void* myLumpBuffer;
extern "C" int askedForConfigLoad;
extern "C" signed char configLumpLoaded;
extern "C" int engineDisplayDisable;
extern "C" int configMode;
extern "C" float t;
extern "C" int curFocus;
extern "C" int curSel;
extern "C" int newSel;
extern "C" int gameNoDraw;
extern "C" signed char hudNoDraw;
extern "C" int frontEndRunning;
extern "C" Font* smallFont;
extern "C" unsigned padAcceptButton;
extern "C" unsigned padBackButton;
extern "C" int padAcceptButtonFontChar;
extern "C" int padBackButtonFontChar;
extern "C" unsigned char pad[];
extern "C" unsigned char MC_GameConfig[];
extern "C" char* configStrings[];
extern "C" unsigned buttonValues[14];

namespace {
unsigned wordAt(int record, int offset) {
    return *reinterpret_cast<const unsigned*>(pad + record * 88 + offset);
}

void play(const char* name) {
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>(name)), 0, 1.0f);
}

void* font() {
    if (frontEndRunning != 0) {
        return lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"), const_cast<char*>("gamefontsmall.fnt"));
    }
    return smallFont;
}

char* borderGroup() {
    return frontEndRunning != 0 ? const_cast<char*>("frontend.lmp") : 0;
}

unsigned* configWord(int row) {
    return reinterpret_cast<unsigned*>(MC_GameConfig + 52 + 36 * curFocus + 4 * row);
}
}

extern "C" void frontendRunControllerConfig__Fv() {
    unsigned short buffer[256];
    unsigned short labels[30];
    int pending = 0;
    if (inGameConfigShuttingDown != 0) {
        if (finishFrameCount == 3) {
            lumpUnload__FPv(myLumpBuffer);
            textureReleaseCacheBuffer__Fv();
            frontendControllerConfigRunning = 0;
        }
        finishFrameCount = finishFrameCount + 1;
        return;
    }
    if (initializedLump == 0) {
        engineDisplayDisable = 10;
        if (myLumpBuffer == 0) {
            myLumpBuffer = reinterpret_cast<void*>((reinterpret_cast<unsigned long>(textureLockCacheBuffer__Fv()) + 255) &
                                                   ~255ul);
        } else if (askedForConfigLoad == 0) {
            askedForConfigLoad = 1;
            cdLoadAsync__FPcPvT0ii(const_cast<char*>("config.lmp"), myLumpBuffer, &configLumpLoaded, 0, 0);
        } else if (configLumpLoaded != 0) {
            lumpAdd__FPvPCc(myLumpBuffer, "config.lmp");
            initializedLump = 1;
        }
        if (initializedLump == 0) {
            return;
        }
    }
    for (int i = 0; i < 14; ++i) {
        labels[i * 2] = static_cast<unsigned short>(123 + i);
        labels[i * 2 + 1] = 0;
    }
    // The original reads an uninitialised stack word for label 14; this source reads an empty string.
    labels[28] = 0;
    labels[29] = 0;
    if (configMode == 0) {
        if (t == 0.0f) {
            if ((wordAt(curFocus, 16) & 0x101000) != 0) {
                if (curSel != 0) {
                    play("menu_main_toggle.anm");
                    newSel = curSel - 1;
                }
            }
            if ((wordAt(curFocus, 16) & 0x204000) != 0) {
                if (curSel < 8) {
                    play("menu_main_toggle.anm");
                    newSel = curSel + 1;
                }
            }
        } else if (t == 1.0f) {
            t = 0.0f;
            curSel = newSel;
        }
        if (curSel != newSel) {
            if (t == 0.0f) {
                pending = 1;
            }
            t = t + 0.25f;
            if (1.0f < t) {
                t = 1.0f;
            }
        }
        if (t == 0.0f) {
            if ((wordAt(curFocus, 12) & padAcceptButton) != 0) {
                play("menu_main_accept.anm");
                configMode = 1;
            }
        }
        if ((wordAt(curFocus, 12) & 0x80) != 0) {
            play("menu_main_accept.anm");
            gameSetDefaultGameConfig__Fi(curFocus);
        }
        if ((wordAt(curFocus, 12) & padBackButton) != 0 || wordAt(curFocus, 4) == 0) {
            play("menu_main_back.anm");
            curFocus = 0;
            gameNoDraw = 0;
            coreStartLoading__Fv();
            hudNoDraw = 0;
            inGameConfigShuttingDown = 1;
            return;
        }
        fontStart__FP4FontiUii(font(), 7, 0x64464646u, -1);
        usprintf__FPUsT0ffffffff(buffer, textGetString__FPc(const_cast<char*>("SelectDefaultsBack")),
                                 static_cast<float>(padAcceptButtonFontChar), 125.0f,
                                 static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        fontPrintCenteredu__FiiPUsi(320, 399, buffer, 1000000);
        fontEnd__Fv();
    } else if (configMode == 1) {
        fontStart__FP4FontiUii(font(), 7, 0x64464646u, -1);
        fontPrintCenteredu__FiiPUsi(320, 360, textGetString__FPc(const_cast<char*>("newConfigButton")), 1000000);
        fontColor__FUiii(0x802d5049u, -1, 0);
        fontPrintCenteredu__FiiPUsi(320, 377, textGetString__FPc(configStrings[curSel]), 1000000);
        fontEnd__Fv();
        for (int i = 0; i < 14; ++i) {
            if ((wordAt(curFocus, 12) & buttonValues[i]) != 0) {
                int found = 0;
                if (*configWord(0) != buttonValues[i]) {
                    for (found = 1; found < 9; ++found) {
                        if (*configWord(found) == buttonValues[i]) {
                            break;
                        }
                    }
                }
                unsigned old = *configWord(curSel);
                *configWord(curSel) = buttonValues[i];
                if (found != 9) {
                    *configWord(found) = old;
                }
                play("menu_main_accept.anm");
                configMode = 0;
                break;
            }
        }
    }
    textureDrawBorder__FiiiiiUiPcii(342, 117, 268, 217, 6, 0x80808080u, borderGroup(), 1, 0);
    void* textFont = font();
    fontStart__FP4FontiUii(textFont, 6, 0x802d5049u, -1);
    usprintf__FPUsT0ffffffff(buffer, textGetString__FPc(const_cast<char*>("PlayerConfiguration")),
                             static_cast<float>(curFocus + 1), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    fontPrintCenteredu__FiiPUsi(475, 47, buffer, 1000000);
    for (int row = 0; row < 9;) {
        unsigned value = *configWord(row);
        int index = 0;
        if (buttonValues[0] != value) {
            for (index = 1; index < 14; ++index) {
                if (buttonValues[index] == value) {
                    break;
                }
            }
        }
        if (index == 14) {
            row = 14;
        }
        if (row == curSel || configMode == 0) {
            fontColor__FUiii(0x802d5049u, -1, 0);
        } else {
            fontColor__FUiii(0x80162824u, 64, 0);
        }
        int y = 23 * row + 118;
        fontPrintu__FiiPUsi(360, y, labels + index * 2, 1000000);
        fontPrintu__FiiPUsi(392, y, textGetString__FPc(configStrings[row]), 1000000);
        row = row + 1;
    }
    fontColor__FUiii(0x80323232u, -1, 0);
    {
        unsigned short* text = textGetString__FPc(const_cast<char*>("cstatic_move"));
        int x = 104 - fontStringSizeu__FP4FontPUsi(textFont, text, 1000000);
        fontPrintu__FiiPUsi(x, 279, textGetString__FPc(const_cast<char*>("cstatic_move")), 1000000);
    }
    {
        unsigned short* text = textGetString__FPc(const_cast<char*>("cstatic_inventory"));
        int x = 134 - fontStringSizeu__FP4FontPUsi(textFont, text, 1000000);
        fontPrintu__FiiPUsi(x, 313, textGetString__FPc(const_cast<char*>("cstatic_inventory")), 1000000);
    }
    fontPrintu__FiiPUsi(250, 280, textGetString__FPc(const_cast<char*>("cstatic_camera")), 1000000);
    fontPrintu__FiiPUsi(224, 312, textGetString__FPc(const_cast<char*>("cstatic_pause")), 1000000);
    {
        unsigned short* text = textGetString__FPc(const_cast<char*>("cstatic_spells"));
        int x = 82 - fontStringSizeu__FP4FontPUsi(textFont, text, 1000000);
        fontPrintu__FiiPUsi(x, 246, textGetString__FPc(const_cast<char*>("cstatic_spells")), 1000000);
    }
    fontEnd__Fv();
    {
        float from = static_cast<float>(23 * curSel + 118);
        float to = static_cast<float>(23 * newSel + 118);
        float blend = evalHermite__Ffffff(t, 0.0f, 1.0f, 0.0f, 0.0f);
        float y = from + (to - from) * blend;
        float center = static_cast<float>(static_cast<int>(y) + 14);
        rectangleDraw__FiiiiiUi(349, static_cast<int>(center - 11.0f), 603, static_cast<int>(center + 13.0f), 6,
                                0x50111824u);
    }
    textureDrawBorder__FiiiiiUiPcii(340, 42, 270, 40, 6, 0x80808080u, borderGroup(), 0, 0);
    textureDrawBorder__FiiiiiUiPcii(342, 117, 268, 217, 6, 0x80808080u, borderGroup(), 0, 1);
    {
        void* texture;
        if (frontEndRunning != 0) {
            texture = lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"), const_cast<char*>("dualshock2.tex"));
        } else {
            texture = *reinterpret_cast<void**>(lumpFind__FPvPc(myLumpBuffer, const_cast<char*>("dualshock2.tex")) + 4);
        }
        textureDrawColor__FP8_textureiiiUi(texture, 0, 0, 6, 0x80808080u);
    }
    if (pending != 0) {
        play("menu_main_accept.anm");
    }
}
