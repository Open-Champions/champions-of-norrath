// Observed: runBrowser__FPUs at 0x00253f40 is 8080 bytes. It is one pass of the save-slot
// browser: each call dispatches on the word browserState (a jump table for 0 to 15; larger
// values only store the faded alpha) and returns 0 while the browser runs, 1 when a card
// operation finished and the browser closed, -2 or -1 when it was left from state 13 or
// state 14. The parameter is the title string drawn centred at y 110.
//
// Every pass. fontStart with the colour (flareAlpha << 24 | 0x1e6464), subtract one from
// delayCount (two while frontEndRunning), enter state 15 through resetWithClose__Fv when
// errorCondition is set, read the pad words at offsets 12 (pressed) and 16 (held) of the
// netLocalPad record whichPad, set flareAlpha 128 and flareDelta 8 in state 0, and compute the
// next alpha (flareAlpha + flareDelta clamped to 0..127), which each state stores in flareAlpha.
//
// States:
//  0  pushPause, delayCount 60, local newBrowserState -1, browserOverwrite and browserDelete 0.
//  1  waits for MCA_Busy and delayCount, clears the cursor words, queues MCA_Cmd(10) and
//     MCA_Cmd(6, "*").
//  2  waits; a negative MCA_Result goes to 15, otherwise generateSortedListAndInfoHeaders.
//  3  waits (also for cdIsBusy); fade in then patchSortedListWithLocalTime__Fv and fade out
//     towards newBrowserState (8 when asyncMode is 3 and the list is empty).
//  4  the list: footer text by asyncMode and the slot header name, up/down masks 0x101000 and
//     0x204000 on the held word move the cursor (relativeSlot, startSlot with the pending value
//     newStartSlot, newSlot with the animation value t), accept (padAcceptButton) opens
//     state 5 (asyncMode 2), 11 or 6 (asyncMode 3, depending on the pads), 0x80 asks to delete
//     (state 9, asyncMode 2), back fades out to 14. The fade-out end enters newBrowserState.
//  5  "MemcardConfirmOverwrite": accept sets browserOverwrite and goes to 11, back to 4.
//  6  waits until as many pads as the save has players are present, back returns to 4.
//  7  not enough free space screen (slotClusters against blocksFree), accept goes to 13, back 4.
//  8  "MemcardBack" screen, back goes to 14.
//  9  "MemcardConfirmDelete": accept fades out and then deletes the slot (MCA_Cmd(7), delayCount
//     240, browserDelete 1, quick-load reset) and goes to 1; back returns to 4.
//  11 MCA_Cmd(10) and go to 12.   12 waits and closes (returns 1) unless an error code is set.
//  13, 14 wait for MCA_Busy, popPause__Fi(0) unless netNetworkMode, close (-2 from 13, else -1).
//  15 error screen: accept re-reads the card (MCA_Cmd(8)) and goes to 13, back goes to 14.
//
// Draw. After the state fontEnd; the error screen text (state 15), then either the hud message
// (states 0 to 3 and 7 set a flag) or the title, the frame (textureDrawBorder), up to five slot
// rows of the list starting at startSlot (number, level name, date, time, quick-load star), the
// detail text for the selected slot (players, difficulty, and in MC_Debug the play time) and the
// highlight rectangle interpolated with evalHermite__Ffffff.
//
// Inferred: that the byte arrays at sortedTimes are month/day/hour/minute records in BCD
// follows from the arithmetic only. Unknown: the names of the states; what the pad word bit 0x80
// is; whether the pointer-valued floats passed to usprintf (plural suffixes) are intended; the
// branch for a play time of zero seconds exists in the instruction stream but cannot be reached
// (the minutes value is forced to at least 1 first) and is not authored.

extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);
extern "C" void usprintf__FPUsPcffffffff(unsigned short* dest, char* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* title, unsigned short* body,
                                        unsigned short* footer, int a, int b, int c);
extern "C" void MCA_Cmd__Fiiii(int command, long second, long third, long fourth);
extern "C" int MCA_Busy__Fv();
extern "C" int cdIsBusy__Fv();
extern "C" void pushPause__Fv();
extern "C" void popPause__Fi(int clear);
extern "C" void resetWithClose__Fv();
extern "C" void resetQuickLoad__Fv();
extern "C" int dirEntryIsQuickLoad__FP14sceMcTblGetDir(void* entry);
extern "C" void generateSortedListAndInfoHeaders__Fv();
extern "C" void patchSortedListWithLocalTime__Fv();
extern "C" char* getSlotName__FiiPc(int port, int slot, char* buffer);
extern "C" unsigned short* getLongLevelName__FPc(char* header);
extern "C" void memcardTitle__FPUs(unsigned short* title);
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float volume);
extern "C" void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
extern "C" void fontEnd__Fv();
extern "C" void fontColor__FUiii(unsigned color, int a, int b);
extern "C" int fontPrint__FiiPci(int x, int y, char* text, int flags);
extern "C" int fontPrintu__FiiPUsi(int x, int y, unsigned short* text, int flags);
extern "C" int fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int flags);
extern "C" int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int a, int b, int c, int d);
extern "C" void textureDrawBorder__FiiiiiUiPcii(int a, int b, int c, int d, int e, unsigned color,
                                                  char* lump, int f, int g);
extern "C" void rectangleDraw__FiiiiiUi(int x, int y, int width, int height, int mode, unsigned color);
extern "C" float evalHermite__Ffffff(float t, float a, float b, float c, float d);
extern "C" int sprintf(char* buffer, const char* format, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" void ustrcpy__FPUsPc(unsigned short* dest, char* source);
extern "C" void ustrcat__FPUsT0(unsigned short* dest, unsigned short* source);
extern "C" void ustrcat__FPUsPc(unsigned short* dest, char* source);

extern "C" int browserState;
extern "C" int browserSlot;
extern "C" int browserDelete;
extern "C" int browserOverwrite;
extern "C" int delayCount;
extern "C" int flareAlpha;
extern "C" int flareDelta;
extern "C" int errorCondition;
extern "C" int frontEndRunning;
extern "C" int asyncMode;
extern "C" int MCA_Port;
extern "C" int MCA_Slot;
extern "C" int MCA_Result;
extern "C" int blocksFree;
extern "C" int slotClusters;
extern "C" int sortedDirCount;
extern "C" signed char netNetworkMode;
extern "C" signed char fullLaunchOK;
extern "C" signed char MC_Debug;
extern "C" signed char language;
extern "C" signed char whichPad;
extern "C" unsigned char netLocalPad[];
extern "C" unsigned char pad[];
extern "C" unsigned char slotInfoHeaders[1120];
extern "C" unsigned char sortedDir[];
extern "C" unsigned char sortedTimes[];
extern "C" int sortedPlayerRace[];
extern "C" char* raceStrings[];
extern "C" unsigned padAcceptButton;
extern "C" unsigned padBackButton;
extern "C" int padAcceptButtonFontChar;
extern "C" int padBackButtonFontChar;
extern "C" void* smallFont;
extern "C" unsigned short hudHeaderUni[];

namespace {
unsigned padWord(int offset) {
    return *reinterpret_cast<const unsigned*>(netLocalPad + whichPad * 88 + offset);
}

unsigned short* key(const char* name) {
    return textGetString__FPc(const_cast<char*>(name));
}

float num(const void* pointerValue) {
    return static_cast<float>(reinterpret_cast<long>(pointerValue));
}

long ptr(const void* p) {
    return reinterpret_cast<long>(p);
}

int bcd(unsigned char value) {
    int tens = value >> 4;
    return tens * 10 + (value & 15);
}

unsigned char* header(int slot) {
    return slotInfoHeaders + slot * 112;
}

void say(unsigned short* dest, const char* name, float a = 0, float b = 0, float c = 0) {
    unsigned short* text = key(name);
    usprintf__FPUsT0ffffffff(dest, text, a, b, c, 0, 0, 0, 0, 0);
}

void sayAcceptBack(unsigned short* dest, const char* name) {
    say(dest, name, static_cast<float>(padAcceptButtonFontChar), static_cast<float>(padBackButtonFontChar));
}

void sayBack(unsigned short* dest, const char* name) {
    say(dest, name, static_cast<float>(padBackButtonFontChar));
}

void centred(int y, unsigned short* text) {
    fontPrintCenteredu__FiiPUsi(320, y, text, 1000000);
}

void* mainFont() {
    if (frontEndRunning)
        return lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"), const_cast<char*>("gamefontsmall.fnt"));
    return smallFont;
}

void playSound(const char* name) {
    void* animation = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>(name));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(animation, 0, 1.0f);
}

void cmd(int command, long second, long third, long fourth) {
    MCA_Cmd__Fiiii(command, second, third, fourth);
}

const char* plural(int count, const char* one, const char* many) {
    return count == 1 ? one : many;
}
}

extern "C" int runBrowser__FPUs(unsigned short* title) {
    static int relativeSlot;
    static int startSlot;
    static int newStartSlot;
    static int newSlot;
    static int newBrowserState = -1;
    static float t;
    char line[256];
    unsigned short wide[256];
    int hudMode = 0;
    int frame = 1;
    int errorScreen = 0;
    int next;
    unsigned pressed;
    unsigned held;

    fontStart__FP4FontiUii(mainFont(), 6, (static_cast<unsigned>(flareAlpha) << 24) | 0x1e6464u, -1);
    if (frontEndRunning)
        delayCount = delayCount - 2;
    else
        delayCount = delayCount - 1;
    if (errorCondition) {
        if (browserState != 15) {
            browserState = 15;
            resetWithClose__Fv();
        }
    }
    pressed = padWord(12);
    held = padWord(16);
    if (browserState == 0) {
        flareAlpha = 128;
        flareDelta = 8;
    }
    next = flareAlpha + flareDelta;
    if (next < 0)
        next = 0;
    else if (next >= 128)
        next = 127;
    if (static_cast<unsigned>(browserState) >= 16) {
        flareAlpha = next;
        goto finish;
    }

    switch (browserState) {
    case 0:
        flareAlpha = next;
        pushPause__Fv();
        delayCount = 60;
        newBrowserState = -1;
        browserState = browserState + 1;
        hudMode = 1;
        browserOverwrite = 0;
        browserDelete = 0;
        goto finish;
    case 1:
        flareAlpha = next;
        hudMode = 1;
        if (MCA_Busy__Fv())
            goto finish;
        if (delayCount > 0)
            goto finish;
        browserDelete = 0;
        browserSlot = 0;
        relativeSlot = 0;
        startSlot = 0;
        newStartSlot = 0;
        t = 0.0f;
        newSlot = 0;
        cmd(10, ptr("/BASLUS-20565"), 0, 0);
        cmd(6, ptr("*"), 0, 0);
        browserState = browserState + 1;
        goto finish;
    case 2:
        flareAlpha = next;
        hudMode = 1;
        if (MCA_Busy__Fv())
            goto finish;
        if (delayCount > 0)
            goto finish;
        if (MCA_Result < 0) {
            browserState = 15;
            goto finish;
        }
        generateSortedListAndInfoHeaders__Fv();
        browserState = browserState + 1;
        goto finish;
    case 3:
        flareAlpha = next;
        hudMode = 1;
        if (MCA_Busy__Fv())
            goto finish;
        if (delayCount > 0)
            goto finish;
        if (cdIsBusy__Fv())
            goto finish;
        if (flareDelta > 0) {
            patchSortedListWithLocalTime__Fv();
            if (asyncMode == 3 && sortedDirCount == 0)
                newBrowserState = 8;
            else
                newBrowserState = browserState + 1;
            flareDelta = -16;
            goto finish;
        }
        if (flareAlpha != 0)
            goto finish;
        flareDelta = 8;
        browserState = newBrowserState;
        goto finish;
    case 4: {
        flareAlpha = next;
        fontColor__FUiii((static_cast<unsigned>(next) << 24) | 0x3c4646u, -1, 0);
        if (startSlot + relativeSlot == sortedDirCount) {
            if (asyncMode == 2)
                sayAcceptBack(wide, "MemcardSelectBack");
            else
                sayBack(wide, "MemcardBack");
        } else if (asyncMode == 2) {
            say(wide, "MemcardSelectDeleteBack", static_cast<float>(padAcceptButtonFontChar), 125.0f,
                static_cast<float>(padBackButtonFontChar));
        } else if (asyncMode == 3 && strcmp("gameComplete", reinterpret_cast<const char*>(header(browserSlot))) == 0) {
            sayBack(wide, "MemcardBack");
        } else {
            sayAcceptBack(wide, "MemcardSelectBack");
        }
        centred(320, wide);
        if (flareDelta <= 0) {
            if (flareAlpha != 0)
                goto finish;
            if (newBrowserState == 7) {
                cmd(8, MCA_Port, MCA_Slot, ptr(&blocksFree));
                delayCount = 60;
                flareDelta = 8;
                browserState = newBrowserState;
                goto finish;
            }
            browserState = newBrowserState;
            goto finish;
        }
        if (t == 0.0f) {
            int moved = 0;
            if (held & 0x101000u) {
                if (relativeSlot != 0) {
                    newSlot = relativeSlot - 1;
                    moved = 1;
                } else if (startSlot != 0) {
                    if (newStartSlot == startSlot)
                        newStartSlot = startSlot - 1;
                }
            } else if (held & 0x204000u) {
                bool room;
                if (asyncMode == 2)
                    room = startSlot + relativeSlot < sortedDirCount;
                else
                    room = startSlot + relativeSlot < sortedDirCount - 1;
                if (room && browserSlot < 9) {
                    if (relativeSlot == 4) {
                        if (newStartSlot == startSlot)
                            newStartSlot = newStartSlot + 1;
                    } else {
                        newSlot = relativeSlot + 1;
                        moved = 1;
                    }
                }
            }
            if (newStartSlot != startSlot) {
                startSlot = newStartSlot;
                moved = 1;
            }
            if (moved)
                playSound("menu_main_toggle.anm");
        }
        if (t == 1.0f) {
            t = 0.0f;
            relativeSlot = newSlot;
        } else if (newSlot != relativeSlot) {
            t = t + 0x1.333332p-3f;
            if (frontEndRunning)
                t = t + 0x1.333332p-3f;
            if (1.0f < t)
                t = 1.0f;
        }
        if (t == 0.0f) {
            browserSlot = startSlot + relativeSlot;
            if (pressed & padAcceptButton) {
                playSound("menu_main_accept.anm");
                if (browserSlot == sortedDirCount) {
                    if (asyncMode == 2) {
                        flareDelta = -16;
                        newBrowserState = 7;
                    }
                } else if (asyncMode == 2) {
                    browserState = 5;
                } else if (asyncMode == 3) {
                    if (strcmp("gameComplete", reinterpret_cast<const char*>(header(browserSlot))) == 0)
                        goto finish;
                    if (fullLaunchOK) {
                        int count = *reinterpret_cast<unsigned short*>(header(browserSlot) + 106);
                        int present = 0;
                        while (present < count && *reinterpret_cast<int*>(pad + present * 88 + 4))
                            ++present;
                        if (present != count) {
                            browserState = 6;
                            goto finish;
                        }
                    }
                    flareDelta = -16;
                    browserState = 11;
                } else {
                    flareDelta = -16;
                    browserState = 11;
                }
            }
            if ((pressed & 0x80u) && asyncMode == 2 && startSlot + relativeSlot != sortedDirCount)
                browserState = 9;
        }
        if (pressed & padBackButton) {
            flareDelta = -16;
            newBrowserState = 14;
        }
        goto finish;
    }
    case 5:
        flareAlpha = next;
        say(wide, "MemcardConfirmOverwrite", static_cast<float>(browserSlot + 1));
        centred(264, wide);
        fontColor__FUiii(0x8a3c4646u, -1, 0);
        sayAcceptBack(wide, "MemcardContinueBack");
        centred(320, wide);
        if (pressed & padBackButton) {
            browserState = 4;
            goto finish;
        }
        if (pressed & padAcceptButton) {
            browserState = 11;
            browserOverwrite = 1;
            flareDelta = -16;
        }
        goto finish;
    case 6: {
        flareAlpha = next;
        int count = *reinterpret_cast<unsigned short*>(header(browserSlot) + 106);
        int present = 0;
        while (present < count && *reinterpret_cast<int*>(pad + present * 88 + 4))
            ++present;
        if (pressed & padBackButton) {
            browserState = 4;
            goto finish;
        }
        if (present == *reinterpret_cast<unsigned short*>(header(browserSlot) + 106)) {
            browserState = 4;
            goto finish;
        }
        {
            unsigned short* text = key("MemcardNeedPad");
            float players = static_cast<float>(*reinterpret_cast<unsigned short*>(header(browserSlot) + 106));
            usprintf__FPUsT0ffffffff(wide, text, players, players, 137.0f, 0, 0, 0, 0, 0);
        }
        fontPrintWrapCenteredu__FiiPUsiiii(320, 247, wide, 450, 16, 580, 21);
        fontColor__FUiii(0x8a3c4646u, -1, 0);
        sayBack(wide, "MemcardBack");
        centred(320, wide);
        goto finish;
    }
    case 7:
        flareAlpha = next;
        hudMode = 1;
        if (MCA_Busy__Fv())
            goto finish;
        if (delayCount > 0)
            goto finish;
        if (MCA_Result < 0)
            goto fail;
        if (flareDelta > 0) {
            if (blocksFree >= slotClusters) {
                flareDelta = -16;
                goto finish;
            }
            flareAlpha = 128;
            {
                unsigned short* text = key("MemcardNeedFreeSpaceBrowser");
                char* slotName = getSlotName__FiiPc(-1, -1, 0);
                hudMode = 0;
                usprintf__FPUsT0ffffffff(wide, text, 137.0f, num(slotName), static_cast<float>(slotClusters),
                                         0, 0, 0, 0, 0);
            }
            fontPrintWrapCenteredu__FiiPUsiiii(320, 248, wide, 469, -100, 600, 16);
            fontColor__FUiii(0x8a3c4646u, -1, 0);
            sayAcceptBack(wide, "ConfigTryAgainDecline");
            centred(320, wide);
            if (pressed & padBackButton)
                browserState = 4;
            if (pressed & padAcceptButton)
                browserState = 13;
            goto finish;
        }
        if (flareAlpha != 0)
            goto finish;
        browserState = 12;
        goto finish;
    case 8:
        flareAlpha = next;
        fontColor__FUiii(0x8a3c4646u, -1, 0);
        sayBack(wide, "MemcardBack");
        centred(320, wide);
        if (pressed & padBackButton)
            browserState = 14;
        goto finish;
    case 9: {
        flareAlpha = next;
        centred(247, key("MemcardConfirmDelete"));
        say(wide, "MemcardDeleteSlot", static_cast<float>(browserSlot + 1));
        centred(264, wide);
        fontColor__FUiii(0x8a3c4646u, -1, 0);
        sayAcceptBack(wide, "MemcardYesBack");
        centred(320, wide);
        if (flareDelta > 0) {
            if (pressed & padBackButton) {
                browserState = 4;
                goto finish;
            }
            if (pressed & padAcceptButton)
                flareDelta = -16;
            goto finish;
        }
        if (flareAlpha != 0)
            goto finish;
        delayCount = 240;
        flareDelta = 8;
        hudMode = 1;
        cmd(7, ptr(sortedDir) + browserSlot * 64 + 32, 0, 0);
        browserDelete = hudMode;
        if (dirEntryIsQuickLoad__FP14sceMcTblGetDir(sortedDir + browserSlot * 64))
            resetQuickLoad__Fv();
        browserState = hudMode;
        goto finish;
    }
    case 10:
        flareAlpha = next;
        goto finish;
    case 11:
        cmd(10, ptr("/BASLUS-20565"), 0, 0);
        flareAlpha = next;
        browserState = 12;
        goto finish;
    case 12:
        flareAlpha = next;
        if (MCA_Busy__Fv())
            goto finish;
        if (MCA_Result < 0)
            goto fail;
        if (flareDelta != 0 && flareAlpha != 0)
            goto finish;
        if (netNetworkMode == 0)
            popPause__Fi(0);
        browserState = 0;
        return 1;
    case 13:
    case 14: {
        flareAlpha = next;
        frame = 0;
        if (MCA_Busy__Fv())
            goto finish;
        if (netNetworkMode == 0)
            popPause__Fi(0);
        int was = browserState;
        browserState = 0;
        if (was == 13)
            return -2;
        return -1;
    }
    case 15:
        fontColor__FUiii(0x8a3c4646u, -1, 0);
        flareAlpha = 128;
        frame = 0;
        sayAcceptBack(wide, "MemcardContinueBack");
        centred(320, wide);
        errorScreen = 1;
        if (pressed & padAcceptButton) {
            cmd(8, MCA_Port, MCA_Slot, ptr(&blocksFree));
            errorScreen = 0;
            resetQuickLoad__Fv();
            browserState = 13;
            goto finish;
        }
        if (pressed & padBackButton) {
            browserState = 14;
            resetQuickLoad__Fv();
        }
        goto finish;
    }

fail:
    browserState = 15;
    resetWithClose__Fv();
finish:
    fontEnd__Fv();
    if (errorScreen) {
        fontStart__FP4FontiUii(mainFont(), 6, 0x801e6464u, -1);
        say(wide, "MemcardError", 137.0f);
        centred(196, wide);
        fontColor__FUiii(0x80323c3cu, -1, 0);
        unsigned short* text = key("MemcardMissingError2");
        char* slotName = getSlotName__FiiPc(-1, -1, 0);
        usprintf__FPUsT0ffffffff(wide, text, 137.0f, num(slotName), 0, 0, 0, 0, 0, 0);
        fontPrintWrapCenteredu__FiiPUsiiii(320, 230, wide, 450, -100, 500, 15);
        fontEnd__Fv();
    }
    if (hudMode) {
        memcardTitle__FPUs(hudHeaderUni);
        if (browserDelete != 0 && 90.0f < static_cast<float>(delayCount)) {
            unsigned short* text = key("memcardDeleteGame");
            char* slotName = getSlotName__FiiPc(-1, -1, 0);
            usprintf__FPUsT0ffffffff(wide, text, 137.0f, num(slotName), 137.0f, 0, 0, 0, 0, 0);
            hudShowMsg__FPUsN20iii(hudHeaderUni, wide, 0, flareAlpha, 0, 0);
        } else {
            unsigned short* text = key("ConfigAccessPost");
            hudShowMsg__FPUsN20iii(hudHeaderUni, text, 0, flareAlpha, 0, 0);
        }
        return 0;
    }
    centred(110, title);
    if (frame) {
        unsigned shade = (static_cast<unsigned>(flareAlpha) << 24) | 0x616369u;
        char* lumpName = frontEndRunning ? const_cast<char*>("frontend.lmp") : 0;
        textureDrawBorder__FiiiiiUiPcii(61, 126, 519, 97, 6, shade, lumpName, 1, 0);
        fontStart__FP4FontiUii(mainFont(), 6, 0x801e6464u, -1);
        fontColor__FUiii((static_cast<unsigned>(flareAlpha) << 24) | 0x323c3cu, -1, 0);
        if (asyncMode != 2 && sortedDirCount == 0) {
            fontColor__FUiii((static_cast<unsigned>(flareAlpha) << 24) | 0x1e6464u, -1, 0);
            unsigned short* text = key("MemcardNoLevels");
            char* slotName = getSlotName__FiiPc(-1, -1, 0);
            usprintf__FPUsT0ffffffff(wide, text, 137.0f, num(slotName), 0, 0, 0, 0, 0, 0);
            fontPrintWrapCenteredu__FiiPUsiiii(320, 264, wide, 500, -100, 580, 21);
        } else if (startSlot < 10) {
            int row = 0;
            do {
                int y = row * 17 + 128;
                int slot = startSlot + row;
                sprintf(line, "%d.", slot + 1);
                fontPrint__FiiPci(77, y, line, 100000);
                if (slot == sortedDirCount) {
                    if (asyncMode == 2) {
                        unsigned short* text = key("MemcardNewSave");
                        fontPrintu__FiiPUsi(100, y, text, 1000000);
                    }
                } else if (slot < sortedDirCount) {
                    const unsigned char* when = sortedTimes + slot * 16;
                    int hour = bcd(when[3]);
                    int hour12 = hour;
                    if (hour >= 13)
                        hour12 = hour - 12;
                    if (hour12 == 0)
                        hour12 = 12;
                    bool afternoon = hour >= 12;
                    unsigned short* levelName = getLongLevelName__FPc(reinterpret_cast<char*>(header(slot)));
                    fontPrintu__FiiPUsi(100, y, levelName, 1000000);
                    sprintf(line, "%d/%d/%d", bcd(when[6]), bcd(when[5]), bcd(when[7]) + 2000);
                    fontPrint__FiiPci(365, y, line, 100000);
                    int minute = bcd(when[2]);
                    sprintf(line, "%d:%s%d%s", hour12, minute < 10 ? "0" : "", minute, afternoon ? "pm" : "am");
                    fontPrint__FiiPci(487, y, line, 100000);
                    if (dirEntryIsQuickLoad__FP14sceMcTblGetDir(sortedDir + slot * 64) && browserState != 12)
                        fontPrint__FiiPci(559, y + 2, const_cast<char*>("*"), 100000);
                }
                ++row;
            } while (row < 5 && startSlot + row < 10);
        }
        if (browserSlot < sortedDirCount && browserState != 12 && browserState != 11 && browserState != 5 &&
            browserState != 6 && browserState != 9) {
            int shift = (language == 4) ? 15 : 0;
            int labelX = 220 - shift;
            int valueX = shift + 320;
            fontPrintu__FiiPUsi(labelX, 247, key("MemcardSavePlayers"), 1000000);
            fontPrintu__FiiPUsi(labelX, 264, key("MemcardDifficulty"), 1000000);
            if (MC_Debug) {
                fontPrintu__FiiPUsi(100, 281, key("MemcardSaveTime"), 1000000);
                unsigned seconds = *reinterpret_cast<const unsigned*>(header(browserSlot) + 100);
                int hours = seconds / 3600u;
                int minutes = seconds / 60u - hours * 60;
                if (minutes == 0)
                    minutes = 1;
                unsigned char lang = language;
                const char* hourSuffix;
                const char* minuteSuffix;
                if (lang < 2 || static_cast<signed char>(lang) == 3) {
                    hourSuffix = plural(hours, "", "s");
                    minuteSuffix = plural(minutes, "", "s");
                } else if (static_cast<signed char>(lang) == 4) {
                    hourSuffix = plural(hours, "", "n");
                    minuteSuffix = plural(minutes, "", "n");
                } else if (static_cast<signed char>(lang) == 2) {
                    hourSuffix = plural(hours, "a", "e");
                    minuteSuffix = plural(minutes, "o", "i");
                } else {
                    hourSuffix = "";
                    minuteSuffix = "";
                }
                if (hours != 0) {
                    unsigned short* text = key("MemcardBrowseHours");
                    usprintf__FPUsT0ffffffff(wide, text, static_cast<float>(hours), num(hourSuffix),
                                             static_cast<float>(minutes), num(minuteSuffix), 0, 0, 0, 0);
                } else {
                    unsigned short* text = key("MemcardBrowseMinutes");
                    usprintf__FPUsT0ffffffff(wide, text, static_cast<float>(minutes), num(minuteSuffix),
                                             0, 0, 0, 0, 0, 0);
                }
                fontPrintu__FiiPUsi(valueX, 281, wide, 1000000);
            }
            int players = *reinterpret_cast<unsigned short*>(header(browserSlot) + 106);
            if (players < 2) {
                ustrcpy__FPUsPc(wide, const_cast<char*>(":  1 ("));
                unsigned short* race = textGetString__FPc(raceStrings[sortedPlayerRace[browserSlot * 4]]);
                ustrcat__FPUsT0(wide, race);
                ustrcat__FPUsPc(wide, const_cast<char*>(")"));
            } else {
                usprintf__FPUsPcffffffff(wide, const_cast<char*>(":  %d"), static_cast<float>(players),
                                         0, 0, 0, 0, 0, 0, 0);
            }
            fontPrintu__FiiPUsi(valueX, 247, wide, 1000000);
            int difficulty = static_cast<signed char>(header(browserSlot)[108]);
            unsigned short* difficultyText;
            if (difficulty == 1)
                difficultyText = key("memcardDifficulty1");
            else if (difficulty == 2)
                difficultyText = key("memcardDifficulty2");
            else if (difficulty == 3)
                difficultyText = key("memcardDifficulty3");
            else
                difficultyText = key("MemcardDifficulty1");
            fontPrintu__FiiPUsi(valueX, 264, difficultyText, 1000000);
        }
        fontEnd__Fv();
        {
            float from = static_cast<float>(relativeSlot * 17 - 13);
            float to = static_cast<float>(newSlot * 17 - 13);
            float blend = evalHermite__Ffffff(t, 0.0f, 1.0f, 0.0f, 0.0f);
            float shade = static_cast<float>(flareAlpha) * 0.0078125f * 64.0f;
            float step = (to - from) * blend;
            from = from + step;
            int top = static_cast<int>(from);
            unsigned color = (static_cast<unsigned>(static_cast<int>(shade)) << 24) | 0x00366876u;
            rectangleDraw__FiiiiiUi(66, top + 146, 574, top + 165, 6, color);
        }
        textureDrawBorder__FiiiiiUiPcii(61, 126, 519, 97, 6, (static_cast<unsigned>(flareAlpha) << 24) | 0x616369u,
                                        lumpName, 0, 1);
    }
    hudShowMsg__FPUsN20iii(0, 0, 0, flareAlpha, 569, 196);
    return 0;
}
