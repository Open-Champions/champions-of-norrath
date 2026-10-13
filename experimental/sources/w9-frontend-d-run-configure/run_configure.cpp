// Observed: runConfigure__FiPUsi at 0x00251fe0 is 8032 bytes. It is one pass of a memory-card
// configuration screen: each call dispatches on the word configState (a jump table for 0 to
// 32; larger values only reach the shared tail) and returns 0 while the screen is running, 1
// when it finished with a card chosen and -1 when it was cancelled. The second parameter is
// not read. A first call is recognised by configState being -1.
//
// Entry. With configState -1 it calls pushPause__Fv, clears the local static fakeCardDisplay,
// stores 8 in flareDelta, 0 in flareAlpha and configFormatted, and sets configState to 30 when
// disableMC is nonzero, else 0. Every pass subtracts one from delayCount (two while
// frontEndRunning is nonzero), adds flareDelta to flareAlpha and clamps the sum to 0..127, then
// formats hudHeaderUni: when MCA_Port is -1 or configState is below 4 from text
// "memcardPS2Search", else from "MemcardPS2" with the name returned by getSlotName__FiiPc. The
// first float argument of these header strings is 137.0f.
//
// States (a plain "end" means leave the dispatch and run the tail):
//  0  MCA_Reset, sceMtapPortOpen(2) and (3), store scePadGetSlotMax(0) and (1) in the two bytes
//     of the local static originalSlotCount, queue MCA_Cmd(8, port, slot, 0) for the eight
//     port/slot pairs, clear configFormatted, wasFormatting, and go to 1; marks the tail.
//  1  waits for MCA_Busy and delayCount, clears fakeCardDisplay, goes to 2; marks the tail.
//  2  counts the present cards (MCA_Present). One card: sets MCA_Port/MCA_Slot and goes to 7
//     when formatted, else 9 (when the first parameter is zero, 10 instead if asyncMode is 2,
//     4 or 5). None: 31. Two or more: 3. With the first parameter zero a last card
//     (lastCardPort/lastCardSlot) that is present and formatted is taken directly (7); a
//     nonzero third parameter sends two or more cards to 3 before that.
//  3  like 0 without the resets; sets delayCount 60 and the animation value 1.0f, clears
//     oldCard and curCard, goes to 4; marks the tail.
//  4  waits for MCA_Busy and delayCount (marking the tail), spins a short no-op loop, goes to 5.
//  5  card chooser: draws the present cards with fontPrint, a highlight rectangle and the
//     "ConfigContinueDecline" footer; up/down masks 0x101000/0x204000 move the choice (curCard),
//     accept stores the port/slot and goes to 6 after MCA_Cmd(8), back starts the fade-out to 30.
//  6  checks the chosen card is still present (otherwise resetWithClose, state 32), then picks
//     7, 9 or 10 as in state 2; marks the tail.
//  7  MCA_Cmd(8, port, slot, &blocksFree) and MCA_Cmd(10, "/BASLUS-20565"), goes to 8.
//  8  waits; result -4 (no data) shows a message and goes to 12, 3, 20 or 30; result >= 0 goes
//     to 14; another negative result fails (resetWithClose, 32).
//  9, 10  "not formatted" screens; 10 asks hudRunYesNo and either goes to 3 or queues the format
//     command MCA_Cmd(9) with delayCount 120 and wasFormatting 1 (state 11).
//  11 waits for the format, then clears wasFormatting, sets configFormatted and goes to 14.
//  12, 13  re-read the free space (MCA_Cmd(8)) then go to 14.
//  14 opens the directory (MCA_Cmd(11), (10) and (6, "*")), goes to 15.
//  15 counts the directory entries that contain "slot"; picks 16, 19 or 21.
//  16 checks the sizes of the config file (252), icon.sys (964), icon.ico (128736) and the
//     entries containing "slot" (727496); marks bad slots in slotsCorrupted; picks 17, 19, 26 or 29.
//  17 offers to delete the corrupt slots (accept: MCA_Cmd(7) each, then 18); back goes to 3.
//  18 waits and returns to 14.   19 "Memcard2Corrupt" screen.   20 "Memcard2NeedSpace" screen.
//  21 deletes every "BASLUS-20565" entry, then writes icon.sys, the session mark and the config
//     file; 22 to 24 stream icon.ico from the disc in 0x8000-byte pieces; 25 shows "MemcardInitGood".
//  26 reads the config file back (MCA_Cmd(0, 3, 1)); 27 waits and fades out; 28 fades out.
//  29, 30 finish (see Entry): both call popPause__Fi(0) unless netNetworkMode is set, set
//     configState to -1; 29 returns 1 and remembers the card, 30 returns -1 and forgets it.
//  31 "no cards" screen; 32 error screen; back resets the choice and goes to 3.
//
// Tail. When the tail is marked and asyncMode is not 5: with fakeCardDisplay nonzero the header
// is formatted from "memcardPS2Search"; in state 11 hudShowMsg shows the "Memcard2FormatWait"
// text, otherwise "ConfigAccessPost".
//
// Inferred: that the nine-state group 12..20 is a recovery sequence follows from its transitions
// only. Unknown: names of the states, the meaning of the pad masks beyond the observed bits, and
// what the strings "*", "slot" and "elf.ico" (data after fakeCardDisplay) are for.

extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* title, unsigned short* body,
                                        unsigned short* footer, int a, int b, int c);
extern "C" int hudRunYesNo__FPUsiiii(unsigned short* text, int a, int b, int c, int d);
extern "C" void MCA_Cmd__Fiiii(int command, long second, long third, long fourth);
extern "C" int MCA_Busy__Fv();
extern "C" void MCA_Reset__Fv();
extern "C" void pushPause__Fv();
extern "C" void popPause__Fi(int clear);
extern "C" void resetWithClose__Fv();
extern "C" void resetQuickLoad__Fv();
extern "C" int dirEntryIsQuickLoad__FP14sceMcTblGetDir(void* entry);
extern "C" char* getSlotName__FiiPc(int port, int slot, char* buffer);
extern "C" int sceMtapPortOpen(int port);
extern "C" int scePadGetSlotMax(int port);
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
extern "C" void fontEnd__Fv();
extern "C" int fontPrint__FiiPci(int x, int y, char* text, int flags);
extern "C" void rectangleDraw__FiiiiiUi(int x, int y, int width, int height, int mode, unsigned color);
extern "C" float evalHermite__Ffffff(float t, float a, float b, float c, float d);
extern "C" void cdLoadAsync__FPcPvT0ii(const char* file, void* buffer, void* done, int a, int b);
extern "C" char* strstr(const char* haystack, const char* needle);
extern "C" int strcmp(const char* a, const char* b);
extern "C" void* memset(void* destination, int value, unsigned count);

extern "C" int configState;
extern "C" int delayCount;
extern "C" int disableMC;
extern "C" int flareAlpha;
extern "C" int flareDelta;
extern "C" signed char configFormatted;
extern "C" int wasFormatting;
extern "C" int frontEndRunning;
extern "C" int asyncMode;
extern "C" int errorCondition;
extern "C" signed char netNetworkMode;
extern "C" int MCA_Port;
extern "C" int MCA_Slot;
extern "C" int MCA_Result;
extern "C" int MCA_Present[8];
extern "C" int MCA_Formatted[8];
extern "C" int MCA_LastDirCount;
extern "C" unsigned char MCA_DirContents[4096];
extern "C" int lastCardPort;
extern "C" int lastCardSlot;
extern "C" int blocksFree;
extern "C" int totalClusters;
extern "C" int sessionMark;
extern "C" signed char MC_Debug;
extern "C" char slotsCorrupted[27];
extern "C" unsigned char iconHeader[964];
extern "C" char markFileName[];
extern "C" char configFileName[];
extern "C" unsigned char MC_GameConfig[252];
extern "C" unsigned char tempBuffer[];
extern "C" signed char whichPad;
extern "C" unsigned char netLocalPad[];
extern "C" unsigned padAcceptButton;
extern "C" unsigned padBackButton;
extern "C" int padAcceptButtonFontChar;
extern "C" int padBackButtonFontChar;
extern "C" void* smallFont;
extern "C" unsigned short hudHeaderUni[];
extern "C" unsigned short hudFooterUni[];

namespace {
unsigned padWord() {
    return *reinterpret_cast<const unsigned*>(netLocalPad + whichPad * 88 + 12);
}

unsigned short* key(const char* name) {
    return textGetString__FPc(const_cast<char*>(name));
}

// getSlotName__FiiPc's result is used as a string and also converted to float for usprintf.
float num(char* pointerValue) {
    return static_cast<float>(reinterpret_cast<long>(pointerValue));
}

long ptr(const void* p) {
    return reinterpret_cast<long>(p);
}

void cmd(int command, long second, long third, long fourth) {
    MCA_Cmd__Fiiii(command, second, third, fourth);
}

// Formats a text key into dest with up to three leading float arguments; the other five are zero.
void say(unsigned short* dest, const char* name, float a = 0, float b = 0, float c = 0) {
    unsigned short* text = key(name);
    usprintf__FPUsT0ffffffff(dest, text, a, b, c, 0, 0, 0, 0, 0);
}

void footerAcceptBack(const char* name) {
    say(hudFooterUni, name, static_cast<float>(padAcceptButtonFontChar),
        static_cast<float>(padBackButtonFontChar));
}

void footerAccept(const char* name) {
    say(hudFooterUni, name, static_cast<float>(padAcceptButtonFontChar));
}

void footerBack(const char* name) {
    say(hudFooterUni, name, static_cast<float>(padBackButtonFontChar));
}

void showBody(unsigned short* body) {
    hudShowMsg__FPUsN20iii(hudHeaderUni, body, hudFooterUni, flareAlpha, 0, 0);
}

unsigned char* entryAt(int index) {
    return MCA_DirContents + index * 64;
}

char* nameAt(int index) {
    return reinterpret_cast<char*>(MCA_DirContents + index * 64 + 32);
}

// Deletes the directory entries whose name contains needle (and, when onlyCorrupt is set, that
// slotsCorrupted marks); a deleted quick-load stamp entry resets the quick load.
void deleteMatching(const char* needle, bool onlyCorrupt) {
    for (int i = 0; i < MCA_LastDirCount; ++i) {
        char* name = nameAt(i);
        if (!strstr(name, needle))
            continue;
        if (onlyCorrupt && !slotsCorrupted[i])
            continue;
        cmd(7, ptr(name), 0, 0);
        if (dirEntryIsQuickLoad__FP14sceMcTblGetDir(entryAt(i)))
            resetQuickLoad__Fv();
    }
}
}

extern "C" int runConfigure__FiPUsi(int mode, unsigned short* title, int third) {
    static int oldCard;
    static int curCard;
    static int cdPos;
    static int iconBytes;
    static int fakeCardDisplay;
    static float st;
    static signed char cdDone;
    static char originalSlotCount[4];
    unsigned short text[512];
    int result = 0;
    int redraw = 0;

    (void)title;
    if (configState == -1) {
        pushPause__Fv();
        fakeCardDisplay = 0;
        flareDelta = 8;
        flareAlpha = 0;
        if (disableMC)
            configState = 30;
        else
            configState = 0;
        configFormatted = 0;
    }
    redraw = 0;
    if (frontEndRunning)
        delayCount = delayCount - 2;
    else
        delayCount = delayCount - 1;
    {
        int sum = flareAlpha + flareDelta;
        if (sum >= 128)
            flareAlpha = 127;
        else
            flareAlpha = sum < 0 ? 0 : sum;
    }
    if (MCA_Port == -1 || configState < 4) {
        say(hudHeaderUni, "memcardPS2Search", 137.0f);
    } else {
        unsigned short* name = key("MemcardPS2");
        char* slotName = getSlotName__FiiPc(MCA_Port, MCA_Slot, originalSlotCount);
        usprintf__FPUsT0ffffffff(hudHeaderUni, name, 137.0f, num(slotName), 0, 0, 0, 0, 0, 0);
    }
    if (static_cast<unsigned>(configState) > 32)
        goto finish;

    switch (configState) {
    case 0:
        MCA_Reset__Fv();
        wasFormatting = 0;
        sceMtapPortOpen(2);
        sceMtapPortOpen(3);
        originalSlotCount[0] = scePadGetSlotMax(0);
        originalSlotCount[1] = scePadGetSlotMax(1);
        redraw = 1;
        for (int port = 0; port < 2; ++port)
            for (int slot = 0; slot < 4; ++slot)
                cmd(8, port, slot, 0);
        configFormatted = 0;
        configState = configState + 1;
        goto finish;
    case 1:
        redraw = 1;
        if (MCA_Busy__Fv())
            goto finish;
        if (delayCount > 0)
            goto finish;
        fakeCardDisplay = 0;
        configState = 2;
        goto finish;
    case 2: {
        redraw = 1;
        int found = 0;
        int foundPort = -1;
        int foundSlot = -1;
        for (int port = 0; port < 2; ++port)
            for (int slot = 0; slot < 4; ++slot)
                if (MCA_Present[port * 4 + slot]) {
                    foundPort = port;
                    foundSlot = slot;
                    ++found;
                }
        if (mode) {
            if (found == 0) {
                configState = 31;
                goto finish;
            }
            if (found >= 2) {
                configState = 3;
                goto finish;
            }
            MCA_Port = foundPort;
            MCA_Slot = foundSlot;
            if (MCA_Formatted[foundPort * 4 + foundSlot])
                configState = 7;
            else
                configState = 9;
            goto finish;
        }
        if (third && found >= 2) {
            configState = 3;
            goto finish;
        }
        if (lastCardPort != -1) {
            int index = lastCardPort * 4 + lastCardSlot;
            if (MCA_Present[index] && MCA_Formatted[index]) {
                MCA_Port = lastCardPort;
                MCA_Slot = lastCardSlot;
                configState = 7;
                goto finish;
            }
        }
        if (found == 0) {
            configState = 31;
            goto finish;
        }
        if (found >= 2) {
            configState = 3;
            goto finish;
        }
        MCA_Port = foundPort;
        MCA_Slot = foundSlot;
        if (MCA_Formatted[foundPort * 4 + foundSlot]) {
            configState = 7;
            goto finish;
        }
        if (asyncMode == 2 || asyncMode == 4 || asyncMode == 5)
            configState = 10;
        else
            configState = 9;
        goto finish;
    }
    case 3:
        wasFormatting = 0;
        originalSlotCount[0] = scePadGetSlotMax(0);
        originalSlotCount[1] = scePadGetSlotMax(1);
        redraw = 1;
        for (int port = 0; port < 2; ++port)
            for (int slot = 0; slot < 4; ++slot)
                cmd(8, port, slot, 0);
        delayCount = 60;
        st = 1.0f;
        configState = configState + 1;
        oldCard = 0;
        curCard = 0;
        goto finish;
    case 4:
        if (MCA_Busy__Fv()) {
            redraw = 1;
            goto finish;
        }
        if (delayCount > 0) {
            redraw = 1;
            goto finish;
        }
        for (int i = 0; i < 2; ++i)
            asm volatile("nop\n\tnop\n\tnop\n\tnop");
        configState = configState + 1;
        goto finish;
    case 5: {
        void* font;
        if (frontEndRunning)
            font = lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"),
                                            const_cast<char*>("gamefontsmall.fnt"));
        else
            font = smallFont;
        unsigned fade = static_cast<unsigned>(static_cast<float>(flareAlpha) * 0.0078125f * 128.0f);
        fontStart__FP4FontiUii(font, 6, (fade << 24) | 0x00323c3cu, -1);
        int count = 0;
        int pickedSlot = -1;
        int pickedPort = -1;
        for (int i = 0; i < 8; ++i)
            if (MCA_Present[i])
                ++count;
        if (count == 0) {
            configState = 31;
            goto finish;
        }
        int rowTop = 210 - (count / 2) * 18;
        float blend = evalHermite__Ffffff(st, 0.0f, 1.0f, 0.0f, 0.0f);
        int from = rowTop + oldCard * 18;
        int to = rowTop + curCard * 18;
        float marker = static_cast<float>(from) + static_cast<float>(to - from) * blend;
        int shown = 0;
        for (int port = 0; port < 2; ++port) {
            for (int slot = 0; slot < 4; ++slot) {
                if (MCA_Present[port * 4 + slot]) {
                    char* name = getSlotName__FiiPc(port, slot, originalSlotCount);
                    fontPrint__FiiPci(310, rowTop + shown * 18, name, 100000);
                    if (curCard == shown) {
                        pickedSlot = slot;
                        pickedPort = port;
                    }
                    ++shown;
                }
            }
        }
        fontEnd__Fv();
        int top = static_cast<int>(marker);
        unsigned shade = static_cast<unsigned>(static_cast<int>(static_cast<float>(flareAlpha) * 0.0078125f * 64.0f));
        rectangleDraw__FiiiiiUi(260, top + 6, 380, top + 24, 6, (shade << 24) | 0x00366876u);
        footerAcceptBack("ConfigContinueDecline");
        {
            unsigned short* head = key("Memcard2SlotSelectionTitle");
            hudShowMsg__FPUsN20iii(head, 0, hudFooterUni, flareAlpha, 400, 150);
        }
        if (flareDelta < 0) {
            if (flareAlpha == 0)
                configState = 30;
            goto finish;
        }
        if (st == 1.0f) {
            if (curCard != 0) {
                if (padWord() & 0x101000u) {
                    oldCard = curCard;
                    curCard = curCard - 1;
                    st = 0.0f;
                }
            }
            if (curCard < count - 1) {
                if (padWord() & 0x204000u) {
                    oldCard = curCard;
                    curCard = curCard + 1;
                    st = 0.0f;
                }
            }
            if (padWord() & padAcceptButton) {
                resetQuickLoad__Fv();
                MCA_Port = pickedPort;
                delayCount = 60;
                MCA_Slot = pickedSlot;
                cmd(8, pickedPort, pickedSlot, 0);
                configState = 6;
                goto finish;
            }
        }
        st = st + 0.125f;
        if (1.0f < st)
            st = 1.0f;
        if (padWord() & padBackButton)
            flareDelta = -16;
        goto finish;
    }
    case 6: {
        redraw = 1;
        if (MCA_Busy__Fv())
            goto finish;
        if (delayCount > 0)
            goto finish;
        int index = MCA_Port * 4 + MCA_Slot;
        if (!MCA_Present[index])
            goto fail;
        if (MCA_Formatted[index]) {
            configState = 7;
            goto finish;
        }
        if (mode) {
            configState = 9;
            goto finish;
        }
        if (asyncMode == 2 || asyncMode == 4 || asyncMode == 5)
            configState = 10;
        else
            configState = 9;
        goto finish;
    }
    case 7:
        redraw = 1;
        cmd(8, MCA_Port, MCA_Slot, ptr(&blocksFree));
        cmd(10, ptr("/BASLUS-20565"), 0, 0);
        configState = 8;
        goto finish;
    case 8:
        redraw = 1;
        if (MCA_Busy__Fv())
            goto finish;
        if (MCA_Result == -4) {
            redraw = 0;
            if (blocksFree >= totalClusters) {
                if (asyncMode != 3) {
                    unsigned short* body = key("Memcard2NoConfigFiles");
                    usprintf__FPUsT0ffffffff(text, body, static_cast<float>(totalClusters), 0, 0, 0, 0, 0, 0, 0);
                    footerAcceptBack("MemcardContinueBack");
                    hudShowMsg__FPUsN20iii(hudHeaderUni, text, hudFooterUni, flareAlpha, 0, 0);
                    unsigned pad = padWord();
                    if (pad & padAcceptButton)
                        configState = 12;
                    else if (pad & padBackButton)
                        configState = 3;
                    goto finish;
                }
            } else if (asyncMode != 3) {
                configState = 20;
                goto finish;
            }
            footerBack("MemcardBack");
            showBody(key("Memcard2NoSaveData"));
            if (padWord() & padBackButton)
                configState = 30;
            goto finish;
        }
        if (MCA_Result >= 0) {
            configState = 14;
            goto finish;
        }
        goto fail;
    case 9: {
        if (mode) {
            footerAccept("MemcardContinue");
            if (flareDelta >= 0) {
                if (padWord() & padAcceptButton)
                    flareDelta = -16;
            } else if (flareAlpha == 0) {
                configState = 30;
            }
        } else {
            footerAcceptBack("ConfigTryAgainDecline");
            unsigned pad = padWord();
            if (pad & padBackButton)
                configState = 30;
            else if (pad & padAcceptButton)
                configState = 3;
        }
        unsigned short* body = key("Memcard2NotFormatted");
        char* slotName = getSlotName__FiiPc(-1, -1, 0);
        usprintf__FPUsT0ffffffff(text, body, 137.0f, num(slotName), 0, 0, 0, 0, 0, 0);
        hudShowMsg__FPUsN20iii(hudHeaderUni, text, hudFooterUni, flareAlpha, 0, 0);
        goto finish;
    }
    case 10: {
        unsigned short* body = key("MemcardFormat");
        char* slotName = getSlotName__FiiPc(-1, -1, 0);
        usprintf__FPUsT0ffffffff(text, body, 137.0f, num(slotName), 137.0f, 0, 0, 0, 0, 0);
        int answer = hudRunYesNo__FPUsiiii(text, 60, -1, 0, 0);
        if (answer == 1) {
            configState = 3;
        } else if (answer == 2) {
            redraw = 1;
            cmd(9, MCA_Port, MCA_Slot, 0);
            configState = 11;
            delayCount = 120;
            wasFormatting = redraw;
        }
        goto finish;
    }
    case 11:
        redraw = 1;
        if (MCA_Result < 0) {
            resetWithClose__Fv();
            configState = 32;
        }
        if (MCA_Busy__Fv())
            goto finish;
        if (delayCount >= 0)
            goto finish;
        wasFormatting = 0;
        configState = 14;
        configFormatted = redraw;
        goto finish;
    case 12:
        cmd(8, MCA_Port, MCA_Slot, ptr(&blocksFree));
        configState = 13;
        goto finish;
    case 13:
        if (MCA_Busy__Fv())
            goto finish;
        if (MCA_Result < 0)
            goto fail;
        configState = 14;
        goto finish;
    case 14:
        redraw = 1;
        cmd(8, MCA_Port, MCA_Slot, ptr(&blocksFree));
        cmd(11, ptr("/BASLUS-20565"), 0, 0);
        cmd(10, ptr("/BASLUS-20565"), 0, 0);
        cmd(6, ptr("*"), 0, 0);
        configState = configState + 1;
        goto finish;
    case 15: {
        redraw = 1;
        if (MCA_Busy__Fv())
            goto finish;
        if (MCA_Result < 0)
            goto fail;
        if (configFormatted) {
            configState = 21;
            goto finish;
        }
        int matching = 0;
        for (int i = 0; i < MCA_Result; ++i) {
            char* name = nameAt(i);
            if (*name == '.')
                continue;
            if (strstr(name, "slot"))
                ++matching;
        }
        if (MCA_Result - matching >= 6)
            configState = 16;
        else if (MCA_Result != 2)
            configState = 19;
        else
            configState = 21;
        goto finish;
    }
    case 16: {
        redraw = 1;
        memset(slotsCorrupted, 0, 27);
        int corruptSeen = 0;
        int badFile = 0;
        for (int i = 0; i < MCA_Result; ++i) {
            const unsigned char* entry = entryAt(i);
            int size = *reinterpret_cast<const int*>(entry + 16);
            char* name = nameAt(i);
            if (strcmp(configFileName, name) == 0 && size != 252) {
                badFile = 1;
                continue;
            }
            if (strcmp("icon.sys", name) == 0 && size != 964) {
                badFile = 1;
                break;
            }
            if (strcmp("icon.ico", name) == 0 && size != 0x1f6e0) {
                if (!MC_Debug)
                    badFile = 1;
                continue;
            }
            if (strstr(name, "slot") && size != 0xb29c8) {
                slotsCorrupted[i] = 1;
                corruptSeen = 1;
            }
        }
        if (badFile)
            configState = 19;
        else if (corruptSeen)
            configState = 17;
        else if (mode)
            configState = 26;
        else
            configState = 29;
        goto finish;
    }
    case 17:
        redraw = 0;
        footerAcceptBack("MemcardContinueBack");
        showBody(key("ConfigBadSlotSize1"));
        {
            unsigned pad = padWord();
            if (pad & padAcceptButton) {
                deleteMatching("slot", true);
                delayCount = 20;
                configState = 18;
            } else if (pad & padBackButton) {
                configState = 3;
            }
        }
        goto finish;
    case 18:
        if (MCA_Busy__Fv() || delayCount >= 0) {
            redraw = 1;
            goto finish;
        }
        if (MCA_Result < 0)
            goto fail;
        configState = 14;
        goto finish;
    case 19:
        redraw = 0;
        footerAcceptBack("MemcardContinueBack");
        showBody(key("Memcard2Corrupt"));
        {
            unsigned pad = padWord();
            if (pad & padAcceptButton)
                configState = 21;
            else if (pad & padBackButton)
                configState = 3;
        }
        goto finish;
    case 20: {
        redraw = 0;
        footerAcceptBack("ConfigTryAgainDecline");
        unsigned short* body = key("Memcard2NeedSpace");
        char* slotName = getSlotName__FiiPc(-1, -1, 0);
        usprintf__FPUsT0ffffffff(text, body, 137.0f, num(slotName), static_cast<float>(totalClusters),
                                 0, 0, 0, 0, 0);
        hudShowMsg__FPUsN20iii(hudHeaderUni, text, hudFooterUni, flareAlpha, 0, 0);
        unsigned pad = padWord();
        if (pad & padBackButton) {
            configState = 30;
        } else if (pad & padAcceptButton) {
            if (mode)
                configState = 3;
            else
                configState = 0;
        }
        goto finish;
    }
    case 21:
        redraw = 1;
        deleteMatching("BASLUS-20565", false);
        cmd(0, ptr("icon.sys"), 514, 0);
        cmd(5, ptr(iconHeader), 964, 0);
        cmd(1, 0, 0, 0);
        sessionMark = -1;
        cmd(0, ptr(markFileName), 514, 0);
        cmd(5, ptr(&sessionMark), 4, 0);
        cmd(1, 0, 0, 0);
        cmd(0, ptr(configFileName), 514, 0);
        cmd(5, ptr(MC_GameConfig), 252, 0);
        cmd(1, 0, 0, 0);
        cdPos = 0;
        iconBytes = 0;
        configState = 22;
        goto finish;
    case 22:
        redraw = 1;
        if (MCA_Busy__Fv())
            goto finish;
        cdLoadAsync__FPcPvT0ii("elf.ico", tempBuffer, &cdDone, 0x8000, cdPos);
        cdDone = 0;
        configState = 23;
        goto finish;
    case 23: {
        redraw = 1;
        if (cdDone == 0)
            goto finish;
        cmd(0, ptr("icon.ico"), 515, 0);
        cmd(2, cdPos, 0, 0);
        int piece = (0x1f6e0 < cdPos + 0x8000) ? 0x76e0 : 0x8000;
        cmd(5, ptr(tempBuffer), piece, 0);
        cmd(1, 0, 0, 0);
        iconBytes = iconBytes + piece;
        configState = 24;
        goto finish;
    }
    case 24:
        redraw = 1;
        if (MCA_Busy__Fv())
            goto finish;
        if (MCA_Result < 0)
            goto fail;
        cdPos = cdPos + 0x8000;
        if (0x1f6df < cdPos)
            configState = 25;
        else
            configState = 22;
        goto finish;
    case 25:
        redraw = 0;
        if (configFormatted) {
            configState = 29;
            goto finish;
        }
        footerAccept("MemcardContinue");
        showBody(key("MemcardInitGood"));
        if (flareDelta >= 0) {
            if (padWord() & padAcceptButton) {
                if (mode == 0)
                    configState = 29;
                else
                    flareDelta = -16;
            }
        } else if (flareAlpha == 0) {
            configState = 29;
        }
        goto finish;
    case 26:
        delayCount = 120;
        cmd(0, ptr(configFileName), 1, 0);
        redraw = 1;
        cmd(3, ptr(MC_GameConfig), 252, 0);
        cmd(1, 0, 0, 0);
        configState = configState + 1;
        goto finish;
    case 27:
        redraw = 1;
        if (flareDelta >= 0) {
            if (MCA_Busy__Fv())
                goto finish;
            if (delayCount >= 0)
                goto finish;
            if (errorCondition)
                goto fail;
            if (MCA_Result < 0)
                goto fail;
            flareDelta = -16;
            goto finish;
        }
        if (flareAlpha == 0)
            configState = 29;
        goto finish;
    case 28:
        flareDelta = -16;
        redraw = 1;
        if (flareAlpha != 0)
            goto finish;
        configState = 29;
        goto finish;
    case 29:
    case 30:
        if (netNetworkMode == 0)
            popPause__Fi(0);
        redraw = 1;
        if (configState == 30) {
            result = -1;
            lastCardPort = -1;
            configState = -1;
            resetQuickLoad__Fv();
            goto finish;
        }
        lastCardSlot = MCA_Slot;
        lastCardPort = MCA_Port;
        result = 1;
        configState = -1;
        goto finish;
    case 31:
        if (mode) {
            unsigned short* body = key("Memcard2NoneBoot");
            usprintf__FPUsT0ffffffff(text, body, 137.0f, static_cast<float>(totalClusters), 0, 0, 0, 0, 0, 0);
        } else {
            say(text, "Memcard2None", 137.0f);
        }
        say(hudHeaderUni, "memcardNoCardsFound", 137.0f);
        footerAcceptBack("ConfigTryAgainDecline");
        hudShowMsg__FPUsN20iii(hudHeaderUni, text, hudFooterUni, flareAlpha, 0, 0);
        if (flareDelta >= 0) {
            unsigned pad = padWord();
            if (pad & padBackButton) {
                flareDelta = -16;
            } else if (pad & padAcceptButton) {
                resetQuickLoad__Fv();
                configState = 0;
                delayCount = 60;
                fakeCardDisplay = 1;
                lastCardSlot = -1;
                lastCardPort = -1;
            }
        } else if (flareAlpha == 0) {
            configState = 30;
        }
        goto finish;
    case 32:
        if (MCA_Busy__Fv())
            goto finish;
        if (MCA_Port != -1 && MCA_Slot != -1) {
            unsigned short* name = key("MemcardPS2");
            char* slotName = getSlotName__FiiPc(MCA_Port, MCA_Slot, originalSlotCount);
            usprintf__FPUsT0ffffffff(hudHeaderUni, name, 137.0f, num(slotName), 0, 0, 0, 0, 0, 0);
        }
        footerBack("MemcardBack");
        if (wasFormatting) {
            say(text, "Memcard2BadFormat", 137.0f);
            hudShowMsg__FPUsN20iii(hudHeaderUni, text, hudFooterUni, flareAlpha, 0, 0);
        } else {
            int code = MCA_Result;
            if (code < -9 || code >= -2) {
                unsigned short* body = key("MemcardMissingError2");
                char* slotName = getSlotName__FiiPc(MCA_Port, MCA_Slot, originalSlotCount);
                usprintf__FPUsT0ffffffff(text, body, 137.0f, num(slotName), 0, 0, 0, 0, 0, 0);
                hudShowMsg__FPUsN20iii(hudHeaderUni, text, hudFooterUni, flareAlpha, 0, 0);
            } else {
                showBody(key("ConfigError"));
            }
        }
        if (padWord() & padBackButton) {
            resetQuickLoad__Fv();
            MCA_Port = -1;
            configState = 3;
            MCA_Slot = -1;
        }
        goto finish;
    default:
        goto finish;
    }

fail:
    resetWithClose__Fv();
    configState = 32;
finish:
    if (redraw && asyncMode != 5) {
        if (fakeCardDisplay)
            say(hudHeaderUni, "memcardPS2Search", 137.0f);
        if (configState == 11) {
            unsigned short* body = key("Memcard2FormatWait");
            char* slotName = getSlotName__FiiPc(-1, -1, 0);
            usprintf__FPUsT0ffffffff(text, body, 137.0f, num(slotName), 137.0f, 0, 0, 0, 0, 0);
            hudShowMsg__FPUsN20iii(hudHeaderUni, text, 0, flareAlpha, 0, 0);
        } else {
            hudShowMsg__FPUsN20iii(hudHeaderUni, key("ConfigAccessPost"), 0, flareAlpha, 0, 0);
        }
    }
    return result;
}
