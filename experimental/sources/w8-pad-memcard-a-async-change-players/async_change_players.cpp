// Observed: asyncChangePlayers__Fv at 0x00256d28 is 6532 bytes. It is the card-service state machine
// started by MC_ChangePlayers__FiPUsiii. "pad" is the 352-byte symbol read as 88-byte records;
// word N of a record is the word at byte N. Records of netLocalPad are read the same way, and
// word 4 of a netLocalPad record is nonzero for a present controller.
// Prologue. glowOffset moves up one when drawFrameNm has bit 0x20, else down one. Except in states
// 2 and 3, flareAlpha takes flareAlpha plus flareDelta limited to 0..127. hudFooterUni[0] is zeroed
// and ustrcpy__FPUsT0 copies pcTitle into hudHeaderUni. "buttons" is word 12 and "edges" word 16 of
// the pad record of whichPad. The state is syncState (13 states; any other value does nothing).
// The display mode is -1, the title text pointer null.
// State 0: curPlayer = 0. For each of four players, playerStatus[i] becomes 1 and playerRace[i] the
//   word at byte 84 of player record i (stride 23984) when the word at byte 21872 of that record is
//   nonzero or when MC_FrontEndImport is zero and i < nmPlayers; otherwise 0 and -1. Then glowOffset 32,
//   flareDelta 8, originalPort/originalSlot take MCA_Port/MCA_Slot, needReset 0, delayCount 0, flareAlpha 0,
//   and syncState advances; display mode 0.
// State 1 (choose a player): boxHeight 211, boxWidth 80 * pcNmPlayers + 292, mode 0. The footer text
//   depends on how many of the four playerStatus words are nonzero ("count"): with MC_FrontEndImport set,
//   "ImportAcceptBack" when count equals pcNmPlayers else "MemcardImportBack"; otherwise "NewImportAcceptBack"
//   when playerStatus[curPlayer] is zero (which also sets the "new" flag and puts glyph 133 in
//   hudFooterUni[0]), "ImportDeleteAcceptBack" when count is 2 or more (which sets the "delete" flag),
//   else "ImportAcceptBack". With flareDelta positive, edges 0x408000 move the selection left through
//   present controllers and edges 0x802000 right, each step playing menu_main_toggle.anm; curPlayer takes the
//   selection if that controller is present. padAcceptButton in buttons plays menu_main_accept.anm and moves to
//   state 11 unless MC_FrontEndImport is set with count different from pcNmPlayers; otherwise padBackButton
//   stores -16 in flareDelta; otherwise button 0x10 goes to state 2, button 0x80 with the delete flag to state
//   6, and button 0x8 with the new flag to state 7, each with the accept sound. Entering state 7 saves
//   per-player positions or clears the previous-level names, sets up the player-select globals, switches the
//   level name to "kelsel2" and pushes the camera (see the code). With flareDelta not positive, a zero
//   flareAlpha moves to state 10.
// State 2: runConfigure__FiPUsi(0, "MemcardChangePlayers", 1): result 1 moves to state 3, -1 to state 1.
//   State 3: runBrowser__FPUs: -1 stores 8 in flareDelta and moves to state 1; 1 reads the import count from
//   slotInfoHeaders, queues MCA_Cmd(0, name, 1, 0), loadPlayerInfo__FiPiP12playerStruct(count, 0, tempBuffer)
//   and MCA_Cmd(1, 0, 0, 0), then state 4 with flareDelta 8, curImportPlayer -1, flareAlpha 0.
// State 4 (browse the imported players): see the code.
// States 5, 6, 7, 9, 10, 11, 12: see the code. State 8 and values above 12 do nothing.
// Drawing. Display mode 1 formats "MemcardContinueBack" and "MemcardImportWarn"; mode 0 draws one framed
//   panel per local player; mode 3 draws one panel per importable player with five columns of captions;
//   mode 6 formats "MemcardBack" and "MemcardNotFound". Except in mode 5, hudShowMsg__FPUsN20iii(hudHeaderUni,
//   title, hudFooterUni, flareAlpha, boxWidth, boxHeight) ends the pass.
// Inferred: nothing.
// Unknown: the meaning of words 4, 12 and 16 of the pad record beyond their use here; the layout of
// the player record beyond bytes 0, 84, 89, 92, 172, 232 and 21872; the layout of playerSelectInfo beyond a
// 41-byte stride with the name first and a byte at 32; the source of the empty byte at 0x00485b18.

extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);
extern "C" void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* source);
extern "C" int runConfigure__FiPUsi(int mode, unsigned short* title, int extra);
extern "C" int runBrowser__FPUs(unsigned short* title);
extern "C" int MCA_Busy__Fv();
extern "C" void MCA_Cmd__Fiiii(int command, const void* text, int third, int fourth);
extern "C" void MCA_Reset__Fv();
extern "C" int MCA_Sync__FPi(int* error);
extern "C" void loadPlayerInfo__FiPiP12playerStruct(int count, int* unused, void* destination);
extern "C" void memcardTitle__FPUs(unsigned short* title);
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* title, unsigned short* text,
                                        unsigned short* footer, int a, int b, int c);
extern "C" void popPause__Fi(int clear);
extern "C" void pushPause__Fv();
extern "C" void* lumpFindResource__FPcT0(char* group, char* name);
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
extern "C" void frontEndCleanUp__Fi(int mode);
extern "C" void gameGetFullLevelName__FPc(char* name);
extern "C" void gameResetGameState__Fv();
extern "C" void scriptSetGameState__FPci(char* name, int value);
extern "C" void engineSetDisplayDisable__Fi(int value);
extern "C" void engineSyncVBlank__Fv();
extern "C" void Push__6Camera(void* camera, int value);
extern "C" unsigned playerGetMarkerColor__Fii(int player, int alpha);
extern "C" void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b);
extern "C" void fontEnd__Fv();
extern "C" void fontColor__FUiii(unsigned color, int a, int b);
extern "C" void fontPrintu__FiiPUsi(int x, int y, unsigned short* text, int width);
extern "C" void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int width);
extern "C" void fontPrintCentered__FiiPci(int x, int y, char* text, int width);
extern "C" void fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int a, int b, int c, int d);
extern "C" void fontPrintWrapCentered__FiiPciiii(int x, int y, char* text, int a, int b, int c, int d);
extern "C" void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned color,
                                                char* group, int b, int c);
extern "C" void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
extern "C" int sprintf(char* dest, const char* format, ...);
extern "C" char* strcpy(char* dest, const char* source);
extern "C" char* strchr(const char* text, int ch);
extern "C" int strcmp(const char* a, const char* b);
extern "C" void* memcpy(void* dest, const void* source, unsigned long count);
extern "C" int syncState;
extern "C" int asyncMode;
extern "C" int oldAsyncMode;
extern "C" int errorCondition;
extern "C" int delayCount;
extern "C" int flareAlpha;
extern "C" int flareDelta;
extern "C" int browserSlot;
extern "C" int MCA_Port;
extern "C" int MCA_Slot;
extern "C" int MCA_FileOpen;
extern "C" int MCA_Result;
extern "C" int lastCardPort;
extern "C" int lastCardSlot;
extern "C" int drawFrameNm;
extern "C" int nmPlayers;
extern "C" signed char whichPad;
extern "C" signed char pcNmPlayers;
extern "C" signed char pcLaunch;
extern "C" unsigned short* pcTitle;
extern "C" signed char MC_FrontEndImport;
extern "C" unsigned char MC_LastModeSuccessful;
extern "C" signed char keepPlayerPositions;
extern "C" signed char netNetworkMode;
extern "C" int frontEndRunning;
extern "C" void* smallFont;
extern "C" void* gameCam;
extern "C" unsigned padAcceptButton;
extern "C" unsigned padBackButton;
extern "C" int padAcceptButtonFontChar;
extern "C" int padBackButtonFontChar;
extern "C" int playerStatus[4];
extern "C" int playerRace[4];
extern "C" unsigned char player[];
extern "C" unsigned char pad[];
extern "C" unsigned char netLocalPad[];
extern "C" unsigned char tempBuffer[];
extern "C" unsigned char tempPlayerStruct[];
extern "C" unsigned char slotInfoHeaders[];
extern "C" char sortedDir[];
extern "C" char levelName[];
extern "C" char playerSelectNextName[64];
extern "C" char playerSelectPrevName[64];
extern "C" unsigned char playerSelectInfo[];
extern "C" unsigned short* playerSelectExitMessage;
extern "C" signed char playerSelectCount;
extern "C" signed char playerSelectStartPlayer;
extern "C" signed char playerSelectEnforce;
extern "C" signed char playerSelectSuccessful;
extern "C" float playerChangeOldPos[4][3];
extern "C" unsigned short playerChangeOldYaw[4];
extern "C" char* texStrings[];
extern "C" char* raceStrings[];
extern "C" unsigned short hudHeaderUni[];
extern "C" unsigned short hudFooterUni[];

namespace {
int curPlayer;
int glowOffset;
int needReset;
int importPlayerCount;
int curImportPlayer = 0;
int originalPort;
int originalSlot;
int boxWidth = 300;
int boxHeight = 220;
char lname[368];

unsigned wordAt(const unsigned char* base, int record, int offset) {
    return *reinterpret_cast<const unsigned*>(base + record * 88 + offset);
}

void play(const char* name) {
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>(name)), 0, 1.0f);
}

void copyBytes(unsigned char* dest, const unsigned char* source, int count) {
    memcpy(dest, source, static_cast<unsigned long>(count));
}

void saveOldPositions() {
    for (int i = 0; i < nmPlayers; ++i) {
        unsigned char* object = *reinterpret_cast<unsigned char**>(player + 23984 * i + 21872);
        if (object != 0) {
            copyBytes(reinterpret_cast<unsigned char*>(playerChangeOldPos[i]), object + 16, 12);
            playerChangeOldYaw[i] = *reinterpret_cast<unsigned short*>(object + 56);
        }
    }
}

void shortenName(char* name) {
    if (strchr(name, ' ') == 0 || strchr(name, ' ') - name >= 13) {
        name[15] = '.';
        name[16] = '.';
        name[17] = '.';
        name[18] = 0;
    }
}

unsigned panelColor(int g) {
    unsigned grey = static_cast<unsigned>(g);
    return (static_cast<unsigned>(flareAlpha) << 24) | (grey << 16) | (grey << 8) | grey;
}

void fade() {
    oldAsyncMode = asyncMode;
    asyncMode = -1;
    syncState = 0;
}
}

extern "C" void asyncChangePlayers__Fv() {
    unsigned short buffer[256];
    int mode = -1;
    unsigned short* title = 0;
    int canDelete = 0;
    if ((drawFrameNm & 0x20) != 0) {
        glowOffset = glowOffset + 1;
    } else {
        glowOffset = glowOffset - 1;
    }
    if (static_cast<unsigned>(syncState - 2) >= 2u) {
        int alpha = flareAlpha + flareDelta;
        if (alpha >= 128) {
            flareAlpha = 127;
        } else {
            flareAlpha = alpha < 0 ? 0 : alpha;
        }
    }
    hudFooterUni[0] = 0;
    unsigned buttons = wordAt(pad, whichPad, 12);
    unsigned edges = wordAt(pad, whichPad, 16);
    ustrcpy__FPUsT0(hudHeaderUni, pcTitle);
    switch (static_cast<unsigned>(syncState)) {
    case 0:
        curPlayer = 0;
        mode = 0;
        for (int i = 0; i < 4; ++i) {
            unsigned char* record = player + 23984 * i;
            if (*reinterpret_cast<void**>(record + 21872) != 0 || (MC_FrontEndImport == 0 && i < nmPlayers)) {
                playerStatus[i] = 1;
                playerRace[i] = *reinterpret_cast<int*>(record + 84);
            } else {
                playerStatus[i] = 0;
                playerRace[i] = -1;
            }
        }
        glowOffset = 32;
        flareDelta = 8;
        originalPort = MCA_Port;
        originalSlot = MCA_Slot;
        syncState = syncState + 1;
        needReset = 0;
        delayCount = 0;
        flareAlpha = 0;
        break;
    case 1: {
        boxHeight = 211;
        boxWidth = pcNmPlayers * 80 + 292;
        mode = 0;
        canDelete = 0;
        bool canNew = false;
        int count = 0;
        for (int i = 0; i < 4; ++i) {
            if (playerStatus[i] != 0) {
                ++count;
            }
        }
        float accept = static_cast<float>(padAcceptButtonFontChar);
        float back = static_cast<float>(padBackButtonFontChar);
        if (MC_FrontEndImport != 0) {
            if (count == pcNmPlayers) {
                usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("ImportAcceptBack")),
                                         124.0f, accept, back, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            } else {
                usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("MemcardImportBack")),
                                         124.0f, back, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            }
        } else if (playerStatus[curPlayer] == 0) {
            usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("NewImportAcceptBack")),
                                     124.0f, accept, back, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            canNew = true;
            hudFooterUni[0] = 133;
        } else if (count >= 2) {
            canDelete = 1;
            usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("ImportDeleteAcceptBack")),
                                     124.0f, 125.0f, accept, back, 0.0f, 0.0f, 0.0f, 0.0f);
        } else {
            usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("ImportAcceptBack")),
                                     124.0f, accept, back, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (flareDelta > 0) {
            int selection = curPlayer;
            if ((edges & 0x408000) != 0) {
                while (selection != 0) {
                    play("menu_main_toggle.anm");
                    --selection;
                    if (wordAt(netLocalPad, selection, 4) != 0) {
                        break;
                    }
                }
            }
            if ((edges & 0x802000) != 0) {
                if (selection < pcNmPlayers - 1) {
                    do {
                        play("menu_main_toggle.anm");
                        ++selection;
                        if (wordAt(netLocalPad, selection, 4) != 0) {
                            break;
                        }
                    } while (selection < pcNmPlayers - 1);
                }
            }
            if (wordAt(netLocalPad, selection, 4) != 0) {
                curPlayer = selection;
            }
            if ((buttons & padAcceptButton) != 0 && (MC_FrontEndImport == 0 || count == pcNmPlayers)) {
                play("menu_main_accept.anm");
                syncState = 11;
            } else if ((buttons & padBackButton) != 0) {
                flareDelta = -16;
            } else if ((buttons & 0x10) != 0) {
                syncState = 2;
                play("menu_main_accept.anm");
            } else if (canDelete != 0 && (buttons & 0x80) != 0) {
                syncState = 6;
                play("menu_main_accept.anm");
            } else if (canNew && (buttons & 0x8) != 0) {
                play("menu_main_accept.anm");
                syncState = 7;
                if (frontEndRunning != 0) {
                    frontEndCleanUp__Fi(1);
                    // The original copies the first byte of an empty string constant (0x00485b18).
                    playerSelectPrevName[0] = 0;
                    playerSelectNextName[0] = 0;
                } else {
                    saveOldPositions();
                    gameGetFullLevelName__FPc(playerSelectNextName);
                    gameGetFullLevelName__FPc(playerSelectPrevName);
                }
                playerSelectExitMessage = textGetString__FPc(const_cast<char*>("selectExitInGame"));
                scriptSetGameState__FPci(const_cast<char*>("needPlayers"), 1);
                playerSelectCount = 1;
                playerSelectStartPlayer = static_cast<signed char>(selection);
                playerSelectEnforce = 0;
                engineSetDisplayDisable__Fi(10);
                engineSyncVBlank__Fv();
                strcpy(levelName, "kelsel2");
                levelName[80] = -1;
                Push__6Camera(gameCam, -1);
                if (netNetworkMode == 0) {
                    popPause__Fi(0);
                }
            }
        } else if (flareAlpha == 0) {
            syncState = 10;
        }
        break;
    }
    case 2: {
        mode = 5;
        int result = runConfigure__FiPUsi(0, textGetString__FPc(const_cast<char*>("MemcardChangePlayers")), 1);
        if (result == 1) {
            syncState = 3;
        } else if (result == -1) {
            syncState = 1;
        }
        break;
    }
    case 3: {
        mode = 5;
        int result = runBrowser__FPUs(textGetString__FPc(const_cast<char*>("MemcardChangePlayers")));
        if (result == -1) {
            flareDelta = 8;
            syncState = 1;
        } else if (result == 1) {
            const char* name = sortedDir + 32 + browserSlot * 64;
            importPlayerCount = *reinterpret_cast<unsigned short*>(slotInfoHeaders + 106 + 112 * browserSlot);
            MCA_Cmd__Fiiii(0, name, 1, 0);
            loadPlayerInfo__FiPiP12playerStruct(importPlayerCount, 0, tempBuffer);
            MCA_Cmd__Fiiii(1, 0, 0, 0);
            syncState = 4;
            flareDelta = 8;
            curImportPlayer = -1;
            flareAlpha = 0;
        }
        break;
    }
    case 4: {
        if (MCA_Busy__Fv() != 0) {
            flareAlpha = 0;
            mode = 5;
            break;
        }
        if (MCA_Result < 0) {
            MCA_Reset__Fv();
            if (MCA_FileOpen != 0) {
                MCA_Cmd__Fiiii(1, 0, 0, 0);
                MCA_Sync__FPi(&errorCondition);
            }
            syncState = 9;
        }
        int count = importPlayerCount;
        boxWidth = count * 80 + 300;
        boxHeight = 245;
        mode = 3;
        int current = curImportPlayer == -1 ? 0 : curImportPlayer;
        if ((edges & 0x408000) != 0 && current != 0) {
            curImportPlayer = current - 1;
        } else if ((edges & 0x802000) != 0) {
            curImportPlayer = current < count - 1 ? current + 1 : current;
        } else {
            curImportPlayer = current;
        }
        if (flareDelta > 0) {
            if ((buttons & padAcceptButton) != 0) {
                syncState = 5;
            } else if ((buttons & padBackButton) != 0) {
                flareDelta = -16;
            }
        } else if (flareDelta == 0) {
            syncState = 3;
        }
        break;
    }
    case 5: {
        int status = playerStatus[curPlayer];
        if (status == 1) {
            mode = 1;
        }
        if ((buttons & padAcceptButton) != 0 || status != 1) {
            unsigned char* destination = tempPlayerStruct + 23984 * curPlayer;
            copyBytes(destination, tempBuffer + 23984 * curImportPlayer, 23984);
            needReset = 1;
            playerStatus[curPlayer] = 3;
            int race = *reinterpret_cast<int*>(destination + 84);
            *reinterpret_cast<int*>(destination + 21872) = 0;
            playerRace[curPlayer] = race;
            syncState = 1;
        } else if ((buttons & padBackButton) != 0) {
            syncState = 4;
        }
        break;
    }
    case 6: {
        int status = playerStatus[curPlayer];
        mode = status == 1;
        if ((buttons & padAcceptButton) != 0 || status != 1) {
            playerRace[curPlayer] = -1;
            playerStatus[curPlayer] = 0;
            needReset = 1;
            syncState = 1;
        } else if ((buttons & padBackButton) != 0) {
            syncState = 1;
        }
        break;
    }
    case 7: {
        char name[256];
        mode = 5;
        gameGetFullLevelName__FPc(name);
        if (levelName[0] != 0 || strcmp(name, playerSelectNextName) != 0) {
            keepPlayerPositions = 1;
        } else {
            if (playerSelectSuccessful != 0) {
                needReset = 1;
                playerStatus[curPlayer] = 2;
                playerRace[curPlayer] = static_cast<signed char>(playerSelectInfo[32 + 41 * playerSelectStartPlayer]);
            }
            syncState = 1;
            if (netNetworkMode == 0) {
                pushPause__Fv();
            }
        }
        break;
    }
    case 9:
        flareAlpha = 127;
        mode = 6;
        if ((buttons & padBackButton) != 0) {
            syncState = 1;
        }
        break;
    case 10:
        lastCardPort = originalPort;
        lastCardSlot = originalSlot;
        mode = 5;
        MCA_Port = originalPort;
        MCA_Slot = originalSlot;
        if (netNetworkMode == 0) {
            popPause__Fi(0);
        }
        fade();
        break;
    case 11:
        lastCardPort = originalPort;
        lastCardSlot = originalSlot;
        mode = 5;
        MC_LastModeSuccessful = 1;
        MCA_Port = originalPort;
        MCA_Slot = originalSlot;
        if (netNetworkMode == 0) {
            popPause__Fi(0);
        }
        if (pcLaunch != 0) {
            if (needReset != 0) {
                if (MC_FrontEndImport != 0) {
                    gameResetGameState__Fv();
                    engineSetDisplayDisable__Fi(10);
                    engineSyncVBlank__Fv();
                    frontEndCleanUp__Fi(0);
                    strcpy(lname, "kelethin");
                } else {
                    keepPlayerPositions = 1;
                    gameGetFullLevelName__FPc(lname);
                    saveOldPositions();
                }
                strcpy(levelName, lname);
                levelName[80] = -1;
                syncState = 12;
            } else {
                oldAsyncMode = asyncMode;
                asyncMode = -1;
                delayCount = 0;
                syncState = 0;
            }
        } else {
            fade();
        }
        break;
    case 12: {
        char name[256];
        gameGetFullLevelName__FPc(name);
        if (strcmp(name, lname) == 0) {
            fade();
        }
        break;
    }
    default:
        break;
    }
    if (mode == 1) {
        usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("MemcardContinueBack")),
                                 static_cast<float>(padAcceptButtonFontChar),
                                 static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        usprintf__FPUsT0ffffffff(buffer, textGetString__FPc(const_cast<char*>("MemcardImportWarn")),
                                 static_cast<float>(curPlayer + 1), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        title = buffer;
    } else if (mode == 0) {
        int left = 330 - (131 * pcNmPlayers) / 2;
        for (int i = 0; i < pcNmPlayers; ++i) {
            int g = i == curPlayer ? glowOffset + 96 : 64;
            unsigned color = panelColor(g);
            unsigned ink = (static_cast<unsigned>(flareAlpha) << 24) | 0x323c3c;
            int x = left + 131 * i;
            int center = x + 55;
            fontStart__FP4FontiUii(smallFont, 6, ink, -1);
            usprintf__FPUsT0ffffffff(buffer, textGetString__FPc(const_cast<char*>("MemcardImportPWho")),
                                     static_cast<float>(i + 1), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            fontPrintCenteredu__FiiPUsi(center, 105, buffer, 1000000);
            fontEnd__Fv();
            textureDrawBorder__FiiiiiUiPcii(x, 135, 112, 105, 6, color, 0, 1, 0);
            if (playerStatus[i] != 0) {
                textureDrawColor__FP8_textureiiiUi(
                    lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"), texStrings[playerRace[i]]), x + 6, 136, 6,
                    color);
            } else {
                textureDrawBorder__FiiiiiUiPcii(x, 135, 112, 108, 6, color, 0, 0, 1);
            }
            fontStart__FP4FontiUii(smallFont, 6, ink, -1);
            if (wordAt(netLocalPad, i, 4) == 0) {
                fontPrintWrapCenteredu__FiiPUsiiii(
                    center, 250, textGetString__FPc(const_cast<char*>("MemcardImportNoControllerPost")), 117, -100, 580,
                    21);
            } else if (playerStatus[i] == 0) {
                fontPrintWrapCenteredu__FiiPUsiiii(
                    center, 250, textGetString__FPc(const_cast<char*>("MemcardImportAvailablePost")), 117, -100, 580,
                    21);
            } else {
                char name[256];
                if (frontEndRunning != 0) {
                    fontColor__FUiii((static_cast<unsigned>(flareAlpha) << 24) | 0x235657, -1, 0);
                } else {
                    fontColor__FUiii(playerGetMarkerColor__Fii(i, flareAlpha), -1, 0);
                }
                if (playerStatus[i] == 1) {
                    strcpy(name, reinterpret_cast<const char*>(player + 23984 * i));
                } else if (playerStatus[i] == 2) {
                    strcpy(name, reinterpret_cast<const char*>(playerSelectInfo + 41 * i));
                } else if (playerStatus[i] == 3) {
                    strcpy(name, reinterpret_cast<const char*>(tempPlayerStruct + 23984 * i));
                }
                shortenName(name);
                fontPrintWrapCentered__FiiPciiii(center, 250, name, 105, -100, 580, 15);
            }
            fontEnd__Fv();
        }
    } else if (mode == 3) {
        ustrcpy__FPUsT0(hudHeaderUni, textGetString__FPc(const_cast<char*>("MemcardImportAvail")));
        usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("MemcardImportBack")),
                                 static_cast<float>(padAcceptButtonFontChar),
                                 static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        int first = 380 - 60 * importPlayerCount;
        unsigned ink = (static_cast<unsigned>(flareAlpha) << 24) | 0x235657;
        fontStart__FP4FontiUii(smallFont, 6, ink, -1);
        int caption = first - 103;
        fontPrintu__FiiPUsi(caption, 235, textGetString__FPc(const_cast<char*>("MemcardRace")), 1000000);
        fontPrintu__FiiPUsi(caption, 271, textGetString__FPc(const_cast<char*>("MemcardLevel")), 1000000);
        fontPrintu__FiiPUsi(caption, 289, textGetString__FPc(const_cast<char*>("MemcardExp")), 1000000);
        fontPrintu__FiiPUsi(caption, 307, textGetString__FPc(const_cast<char*>("MemcardGold")), 1000000);
        fontPrintu__FiiPUsi(caption, 325, textGetString__FPc(const_cast<char*>("MemcardHealth")), 1000000);
        fontEnd__Fv();
        int x = first;
        const unsigned char* record = tempBuffer;
        for (int i = 0; i < importPlayerCount; ++i, record += 23984, x += 120) {
            int g = i == curImportPlayer ? glowOffset + 96 : 64;
            unsigned color = panelColor(g);
            textureDrawBorder__FiiiiiUiPcii(x - 6, 94, 112, 105, 6, color, 0, 1, 0);
            if (i < importPlayerCount) {
                textureDrawColor__FP8_textureiiiUi(
                    lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"),
                                            texStrings[*reinterpret_cast<const int*>(record + 84)]),
                    x, 95, 6, color);
            }
            fontStart__FP4FontiUii(smallFont, 6, ink, -1);
            if (i < importPlayerCount) {
                char name[256];
                char text[64];
                strcpy(name, reinterpret_cast<const char*>(record));
                shortenName(name);
                int middle = x + 50;
                fontPrintWrapCentered__FiiPciiii(middle, 199, name, 117, -100, 580, 21);
                fontColor__FUiii((static_cast<unsigned>(flareAlpha) << 24) | 0x323c3c, -1, 0);
                ustrcpy__FPUsT0(buffer, textGetString__FPc(raceStrings[*reinterpret_cast<const int*>(record + 84)]));
                fontPrintWrapCenteredu__FiiPUsiiii(middle, 235, buffer, 117, -100, 580, 18);
                sprintf(text, "%d", static_cast<int>(static_cast<signed char>(record[89])));
                fontPrintCentered__FiiPci(middle, 271, text, 10000);
                sprintf(text, "%d", *reinterpret_cast<const int*>(record + 92));
                fontPrintCentered__FiiPci(middle, 289, text, 10000);
                sprintf(text, "%d", *reinterpret_cast<const int*>(record + 232));
                fontPrintCentered__FiiPci(middle, 307, text, 10000);
                float health = *reinterpret_cast<const float*>(record + 172);
                if (health == 100.0f) {
                    sprintf(text, "%3.0f", static_cast<double>(health));
                } else {
                    sprintf(text, "%2.0f", static_cast<double>(health));
                }
                fontPrintCentered__FiiPci(middle, 325, text, 10000);
            } else {
                fontPrintCenteredu__FiiPUsi(x + 50, 199, textGetString__FPc(const_cast<char*>("MemcardImportNotPresent")),
                                            1000000);
            }
            fontEnd__Fv();
        }
    } else if (mode == 6) {
        memcardTitle__FPUs(hudHeaderUni);
        usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("MemcardBack")),
                                 static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        usprintf__FPUsT0ffffffff(buffer, textGetString__FPc(const_cast<char*>("MemcardNotFound")), 137.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        title = buffer;
    }
    if (mode != 5) {
        hudShowMsg__FPUsN20iii(hudHeaderUni, title, hudFooterUni, flareAlpha, boxWidth, boxHeight);
    }
}
