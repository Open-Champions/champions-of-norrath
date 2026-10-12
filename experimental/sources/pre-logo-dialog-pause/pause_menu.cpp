// Observed pauseMenuFrameRun__Fv at 0x17e738, 4292 bytes, pinned SLUS_205.65.
// The first scan for an empty menu-table word is discarded; only its reads
// are kept out of the reconstruction. The table at packetBuffer+0xF7C0 holds
// 16-byte records: entries named menu_exittomenu or menu_exitmultiplayer
// are swapped to the network-mode name, and entries named menu_kickplayers
// set or clear bit 0 at offset 12 by local and connected counts. A set
// MC_Debug byte with any other menu shows coordinates, and a set controller
// config flag runs that frontend and returns. A set memory-card async mode
// returns. The exit countdown decrements each frame, shows the disconnect
// message while nonzero, and on zero exits multiplayer, clears the start
// menu, and bails out of the game loop. A set exit-to-menu byte asks a
// yes/no question; answer 1 clears it and returns, other answers than 2
// return, and answer 2 arms a short (or long, when networked) exit count,
// exits the player HUD, and clears the flag. Menu state 51 tries to save
// the config: failure and a pressed back button pop the config, a pressed
// accept button resets to the first table and saves again, and anything
// else returns. The menu walk enables the load and controls entries only
// when networked, and the recall entry by hurt, dialog, jumpgate, camera,
// and entity checks; the change-players entry ends the walk through the
// draw. The draw shows the menu, the footer buttons, and the help bar, then
// copies player names into the pre-configuration menu when it is current.
// The menu runs on the current pad, the music state reseeds from the entry
// volumes, and the back button gates the state machine. Five menu states
// dispatch through the table at 0x463DC0: return to game, save, load,
// level select, kick setup, recall, shop, clean up, exit to menu, player
// changes, options, race restart and quit, and the submenu and
// communications entries, all rejoining at a tail that restores the menu
// position unless the menu changed to another table, then applies the back
// button transitions with the accept sound. Inferred types cover accessed
// fields only; menus, sound, saves, and hardware are untested. The fifth
// and later hudShowMsg and hudRunYesNo arguments use the standard call
// layout; the original fills extra registers that are not modeled here.
extern "C" {
extern unsigned char* menu;
extern unsigned menuNm, menuPlayerNm, lastWarpPos;
extern unsigned char kickMenuList[], kickMenu[], preConfigure[], raceMainMenu[];
extern unsigned short hudFooterUni[];
extern int endTimer asm("endTimer.1059");
extern unsigned char startMenuState, useRaceMenu, exitToMenuMode, waitNetExitCount;
extern unsigned gameControlPaused, gamePause, gameBailFromGameLoop;
extern unsigned char particlesPaused, cutMode, MC_Debug, netNetworkMode;
extern unsigned nmPlayers;
extern unsigned char player[], netLocalPad[];
extern char levelName[];
extern unsigned char* gameCam;
extern unsigned padAcceptButton, padBackButton;
extern int padAcceptButtonFontChar, padBackButtonFontChar;
extern char* defaultFont;
extern unsigned drawFrameNm;
extern int frontendControllerConfigRunning;
extern float soundSfxVolume, soundMusicVolume;
extern unsigned char packetBuffer[];
extern void* mediumFont;
int strcmp(const char* first, const char* second);
char* strcpy(char* dest, const char* src);
void* lumpFindResource__FPcT0(char* lump, char* name);
void shopStart__Fiii(int player, int first, int second);
void menuSetPos__FP4Menui(unsigned char* menuPtr, int position);
void menuDraw__FiiP4MenuUiUiP4FontP8_texturei(int x, int y, void* menuPtr, unsigned a,
                                             unsigned b, void* font, void* texture, int extra);
int menuRun__FP4MenuP3padPi(void* menuPtr, void* pad, int flag);
unsigned short* textGetString__FPc(char* key);
void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format, float a, float b,
                              float c, float d, float e, float f, float g, float h);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
int fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int flags);
void fontEnd__Fv();
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int width, unsigned color);
int soundGetStreamPlayStatus__Fi(int stream);
void soundFadeStream__Fi(int stream);
void soundUpdateMusicVolume__Fv();
void soundUpdateAdjustments__Fv();
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
void soundStartGlobalMusic__FPci(char* name, int index);
int netGetLocalPlayerNm__Fi(int index);
int netPlayerIsLocal__Fi(int index);
int netPlayerIsConnected__Fi(int index);
int MC_GetAsyncMode__Fv();
void MC_SaveGameBrowser__Fi(int player);
void MC_LoadGameBrowser__Fii(int player, int flag);
void MC_ChangePlayers__FiPUsiii(int player, unsigned short* text, int a, int b, int c);
// game-boot declares this hook void; the pause menu branches on its result.
int gameMaybeSaveConfig__Fii(int first, int second);
void gamePushConfig__Fv();
void gamePopConfig__Fv();
int gameCheckNoSaveZone__Fv();
int DLG_Active__Fv();
int engineTaskActive__FPc(char* name);
void coreInterfaceFlare__Fi(int value);
void showCoords__Fv();
void frontendRunControllerConfig__Fv();
void frontendInitControllerConfig__Fi(int index);
void debugClearStartMenu__Fv();
void msgHudPlayerExit__Fv();
void msgHudRecall__Fc(signed char index);
void netRestoreButtonConfigs__Fv();
void hudShowMsg__FPUsN20iii(unsigned short* body, unsigned short* title, int extra, int a, int b, int c);
int hudRunYesNo__FPUsiiii(unsigned short* text, int a, int b, int c, int d);
void hudNetLevelWarp__FPc(char* entry);
void cleanUp__Fv();
void colorLevelNames__FP4Menu(void* menuPtr);
void netKickPlayer__Fi(int index);
}
static unsigned load_word(const unsigned char* p) {
    return unsigned(p[0]) | unsigned(p[1]) << 8 | unsigned(p[2]) << 16 | unsigned(p[3]) << 24;
}
static void store_word(unsigned char* p, unsigned v) {
    p[0] = static_cast<unsigned char>(v);
    p[1] = static_cast<unsigned char>(v >> 8);
    p[2] = static_cast<unsigned char>(v >> 16);
    p[3] = static_cast<unsigned char>(v >> 24);
}
static void* load_pointer(const unsigned char* p) {
#ifdef NORRATH_HOST_CONTRACT
    void* v = nullptr;
    unsigned char* out = reinterpret_cast<unsigned char*>(&v);
    for (unsigned i = 0; i < sizeof v; ++i) out[i] = p[i];
    return v;
#else
    return reinterpret_cast<void*>(static_cast<unsigned long>(load_word(p)));
#endif
}
static void store_pointer(unsigned char* p, const void* v) {
#ifdef NORRATH_HOST_CONTRACT
    const unsigned char* in = reinterpret_cast<const unsigned char*>(&v);
    for (unsigned i = 0; i < sizeof v; ++i) p[i] = in[i];
#else
    store_word(p, static_cast<unsigned>(reinterpret_cast<unsigned long>(v)));
#endif
}
static void copy_bytes(void* dest, const void* src, unsigned size) {
    auto* out = static_cast<unsigned char*>(dest);
    auto* in = static_cast<const unsigned char*>(src);
    for (unsigned i = 0; i < size; ++i) out[i] = in[i];
}
static unsigned char* table_entry(unsigned char* table, int index) {
    return table + index * 16;
}
// The walk flag sits at +4 on the EE, where entries are four-byte words.
// Host pointers are eight bytes wide, so the flag moves to the otherwise
// unobserved +8 slot there; the +12 slot is shared on both targets.
#ifdef NORRATH_HOST_CONTRACT
static constexpr int MENU_FLAG_OFF = 8;
#else
static constexpr int MENU_FLAG_OFF = 4;
#endif
// The fixed tables live inside the 560000-byte packet buffer; same-object
// offsets below survive linking.
static unsigned char* pause_tables(int offset) {
    return packetBuffer + offset;
}
static void fade_stream_zero() {
    if ((soundGetStreamPlayStatus__Fi(0) & 0x40) != 0) soundFadeStream__Fi(0);
}
extern "C" void pauseMenuFrameRun__Fv() {
    float sfxAtEntry = soundSfxVolume;
    float musAtEntry = soundMusicVolume;
    unsigned char* names = pause_tables(0xf7c0);
    for (int i = 0; load_word(table_entry(names, i)) != 0u; ++i) {
        unsigned char* record = table_entry(names, i);
        char* name = static_cast<char*>(load_pointer(record));
        if (strcmp(name, "menu_exittomenu") == 0 || strcmp(name, "menu_exitmultiplayer") == 0) {
            const char* swapped = netNetworkMode != 0 ? "menu_exittomenu" : "menu_exitmultiplayer";
            store_pointer(record, swapped);
            name = static_cast<char*>(load_pointer(record));
        }
        if (strcmp(name, "menu_kickplayers") != 0) continue;
        if (netNetworkMode == 0) {
            store_word(record + 12, load_word(record + 12) | 1u);
            continue;
        }
        if (netGetLocalPlayerNm__Fi(0) != 0) {
            store_word(record + 12, load_word(record + 12) | 1u);
            continue;
        }
        int connected = 1;
        for (int p = 1; p < static_cast<int>(nmPlayers) && p < 4; ++p)
            if (netPlayerIsConnected__Fi(p) != 0) ++connected;
        if (connected < 2)
            store_word(record + 12, load_word(record + 12) | 1u);
        else
            store_word(record + 12, load_word(record + 12) & ~1u);
    }
    if (MC_Debug != 0 && menu != pause_tables(0xf940)) showCoords__Fv();
    if (frontendControllerConfigRunning != 0) {
        frontendRunControllerConfig__Fv();
        return;
    }
    if (MC_GetAsyncMode__Fv() != -1) return;
    if (waitNetExitCount != 0u) {
        unsigned char left = static_cast<unsigned char>(waitNetExitCount - 1u);
        waitNetExitCount = left;
        if (left != 0u) {
            hudShowMsg__FPUsN20iii(textGetString__FPc(const_cast<char*>("netDisconnectBody")),
                                   textGetString__FPc(const_cast<char*>("netDisconnectTitle")),
                                   0, 128, 0, 0);
            return;
        }
        msgHudPlayerExit__Fv();
        netRestoreButtonConfigs__Fv();
        debugClearStartMenu__Fv();
        netNetworkMode = 0;
        gameBailFromGameLoop = 1u;
        return;
    }
    if (exitToMenuMode != 0u) {
        unsigned short* question = textGetString__FPc(const_cast<char*>(
            netNetworkMode != 0 ? (char*)"exitMultiplayerMsg" : (char*)"exitToMenuMsg"));
        int answer = hudRunYesNo__FPUsiiii(question, 0, -1, 0, 0);
        if (answer == 1) exitToMenuMode = 0;
        if (answer != 2) return;
        waitNetExitCount = static_cast<unsigned char>(netNetworkMode != 0 ? 120 : 1);
        msgHudPlayerExit__Fv();
        exitToMenuMode = 0;
        return;
    }
    if (menuNm == 51u) {
        int how = 1;
        if (gameMaybeSaveConfig__Fii(0, 1) != 0) {
            usprintf__FPUsT0ffffffff(hudFooterUni,
                                     textGetString__FPc(const_cast<char*>("MemcardYesNo")),
                                     static_cast<float>(padAcceptButtonFontChar),
                                     static_cast<float>(padBackButtonFontChar),
                                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            hudShowMsg__FPUsN20iii(textGetString__FPc(const_cast<char*>("maybeSaveConfigBody")),
                                   textGetString__FPc(const_cast<char*>("maybeSaveConfigTitle")),
                                   0, 128, 0, 0);
            int local = netGetLocalPlayerNm__Fi(0);
            unsigned padWord = load_word(netLocalPad + 12 + local * 88);
            if ((padWord & padAcceptButton) != 0u) {
                how = 2;
            } else {
                unsigned backWord = load_word(netLocalPad + 12 + local * 88);
                how = (backWord & padBackButton) != 0u ? 1 : 0;
            }
        }
        if (how == 0) return;
        menuNm = 0u;
        if (how != 2) {
            gamePopConfig__Fv();
            return;
        }
        menu = pause_tables(0xf7c0);
        gameMaybeSaveConfig__Fii(0, 0);
        return;
    }
    if (load_word(menu) != 0u) {
        int off = 0;
        while (true) {
            char* entry = static_cast<char*>(load_pointer(menu + off));
            if (entry == nullptr) break;
            if (strcmp(entry, "menu_loadgame") == 0 || strcmp(entry, "menu_controls") == 0) {
                store_word(menu + off + MENU_FLAG_OFF, netNetworkMode != 0 ? 5u : 0u);
            } else if (strcmp(entry, "menu_recall") == 0) {
                int local = netGetLocalPlayerNm__Fi(0);
                float hurt = 0.0f;
                copy_bytes(&hurt, player + local * 23984 + 172, 4);
                unsigned flag = 0u;
                if (hurt <= 0.0f) {
                    flag = 5u;
                } else if (DLG_Active__Fv() != 0) {
                    flag = 5u;
                } else if (engineTaskActive__FPc(const_cast<char*>("jumpgate")) != 0) {
                    flag = 5u;
                } else if ((load_word(gameCam + 52) ^ 1u) != 0u) {
                    flag = 5u;
                } else {
                    int again = netGetLocalPlayerNm__Fi(0);
                    auto* body = static_cast<unsigned char*>(
                        load_pointer(player + again * 23984 + 21872));
                    if (body == nullptr) {
                        flag = 0u;
                    } else {
                        int third = netGetLocalPlayerNm__Fi(0);
                        auto* same = static_cast<unsigned char*>(
                            load_pointer(player + third * 23984 + 21872));
                        flag = load_word(same + 1328) != 0u ? 5u : 0u;
                    }
                }
                store_word(menu + off + MENU_FLAG_OFF, flag);
            } else if (strcmp(entry, "menu_changeplayers") == 0) {
                unsigned flag;
                if (gameCheckNoSaveZone__Fv() != 0) {
                    flag = 5u;
                } else if (netNetworkMode != 0) {
                    flag = 5u;
                } else if (DLG_Active__Fv() != 0) {
                    flag = 5u;
                } else if ((load_word(gameCam + 52) ^ 1u) == 0u) {
                    store_word(menu + off + MENU_FLAG_OFF, 0u);
                    goto draw;
                } else {
                    flag = 5u;
                }
                store_word(menu + off + MENU_FLAG_OFF, flag);
                goto draw;
            }
            off += 16;
            if (load_word(menu + off) == 0u) break;
        }
    }
draw:
    {
        void* longTex = lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"),
                                                const_cast<char*>("menulong.tex"));
        menuDraw__FiiP4MenuUiUiP4FontP8_texturei(320, 240, menu, 0x4e646868u, 0x7d4c6a73u,
                                                defaultFont, longTex, static_cast<int>(menuPlayerNm));
        fontStart__FP4FontiUii(mediumFont, 7, 0x80464646u, -1);
        usprintf__FPUsT0ffffffff(hudFooterUni,
                                 textGetString__FPc(const_cast<char*>("AcceptExit")),
                                 static_cast<float>(padAcceptButtonFontChar),
                                 static_cast<float>(padBackButtonFontChar),
                                 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        fontPrintCenteredu__FiiPUsi(320, 412, hudFooterUni, 0xf4240);
        fontEnd__Fv();
        void* barTex = lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"),
                                               const_cast<char*>("helpbar.tex"));
        textureDrawColor__FP8_textureiiiUi(barTex, 0, 412, 7, 0x40808080u);
    }
    if (menu == preConfigure) {
        for (int i = 0; i < static_cast<int>(nmPlayers); ++i) {
            char* slot = static_cast<char*>(load_pointer(menu + i * 16));
            strcpy(slot, reinterpret_cast<char*>(player + i * 23984));
        }
        store_pointer(menu + nmPlayers * 16, nullptr);
    }
    int picked = menuRun__FP4MenuP3padPi(menu, netLocalPad + menuPlayerNm * 88, 0);
    if (sfxAtEntry != soundSfxVolume) {
        if ((drawFrameNm & 7u) != 0u) {
            void* tabTex = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"),
                                                   const_cast<char*>("menu_shop_tab.anm"));
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(tabTex, nullptr, 1.0f);
        }
    }
    if (musAtEntry != soundMusicVolume) {
        if (soundGetStreamPlayStatus__Fi(0) & 0x20) {
            soundStartGlobalMusic__FPci(const_cast<char*>("mx_main_l.vag"), 65);
            endTimer = 0;
        }
    }
    {
        int tick = endTimer + 1;
        if (tick < 241)
            endTimer = tick;
        else
            fade_stream_zero();
    }
    soundUpdateMusicVolume__Fv();
    soundUpdateAdjustments__Fv();
    if ((load_word(netLocalPad + menuPlayerNm * 88 + 12) & padAcceptButton) != 0u) {
        void* acceptTex = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"),
                                                  const_cast<char*>("menu_main_accept.anm"));
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(acceptTex, nullptr, 1.0f);
    }
    unsigned char* dispatched = menu;
    if (menuNm < 5u) {
        switch (menuNm) {
        case 0: {
            int at = picked * 16;
            char* entry = static_cast<char*>(load_pointer(dispatched + at));
            if (strcmp(entry, "menu_returntogame") == 0) {
                debugClearStartMenu__Fv();
                gameControlPaused = 60u;
                fade_stream_zero();
            } else if (strcmp(entry, "menu_savegame") == 0) {
                MC_SaveGameBrowser__Fi(static_cast<int>(menuPlayerNm));
            } else if (strcmp(entry, "menu_loadgame") == 0) {
                debugClearStartMenu__Fv();
                gameControlPaused = 60u;
                if ((soundGetStreamPlayStatus__Fi(0) & 0x40) != 0) soundFadeStream__Fi(0);
                MC_LoadGameBrowser__Fii(static_cast<int>(menuPlayerNm), 1);
            } else if (strcmp(entry, "menu_levelwarp") == 0) {
                menu = pause_tables(0xf940);
                menuNm = 1u;
                coreInterfaceFlare__Fi(0);
                colorLevelNames__FP4Menu(menu);
                int sel = 0;
                bool found = false;
                while (true) {
                    char* name = static_cast<char*>(load_pointer(menu + sel * 16));
                    if (name == nullptr) break;
                    if (strcmp(name, levelName + 81) == 0) {
                        found = true;
                        break;
                    }
                    ++sel;
                }
                menuSetPos__FP4Menui(menu, found ? sel : static_cast<int>(lastWarpPos));
            } else if (strcmp(entry, "menu_kickplayers") == 0) {
                for (int k = 0; k < 4; ++k) store_pointer(kickMenu + 64 - k * 16, nullptr);
                int n = 0;
                for (int p = 1; p < static_cast<int>(nmPlayers) && p < 4; ++p) {
                    if (netPlayerIsConnected__Fi(p) == 0) continue;
                    kickMenuList[1 + n] = static_cast<unsigned char>(p);
                    unsigned char* focus = player + p * 23984 + 21104;
                    store_pointer(kickMenu + 16 + n * 16, load_pointer(focus));
                    ++n;
                }
                menuSetPos__FP4Menui(kickMenu, 0);
                menu = kickMenu;
                menuNm = 4u;
                coreInterfaceFlare__Fi(0);
            } else if (strcmp(entry, "menu_recall") == 0) {
                msgHudRecall__Fc(static_cast<signed char>(menuPlayerNm & 0xffu));
            } else if (strcmp(entry, "Enter Shop") == 0) {
                shopStart__Fiii(0, 0, 1);
                debugClearStartMenu__Fv();
            } else if (strcmp(entry, "Clean Up") == 0) {
                cleanUp__Fv();
            } else if (strcmp(entry, "menu_exittomenu") == 0 ||
                       strcmp(entry, "menu_exitmultiplayer") == 0) {
                coreInterfaceFlare__Fi(0);
                exitToMenuMode = 1;
            } else if (strcmp(entry, "menu_changeplayers") == 0) {
                MC_ChangePlayers__FiPUsiii(static_cast<int>(menuPlayerNm),
                                           textGetString__FPc(const_cast<char*>("MemcardPlayerChangerPost")),
                                           4, 1, 0);
            } else if (strcmp(entry, "menu_option") == 0) {
                menuNm = 2u;
                gamePushConfig__Fv();
                menu = pause_tables(0xf610);
            } else if (strcmp(entry, "menu_restartrace") == 0 ||
                       strcmp(entry, "menu_quitrace") == 0) {
                copy_bytes(levelName, "lava3", 6);
                levelName[80] = static_cast<char>(0xff);
                debugClearStartMenu__Fv();
                gameControlPaused = 60u;
                gamePause = 0u;
                particlesPaused = 0;
                startMenuState = 0;
                fade_stream_zero();
            } else if (strcmp(entry, "menu_moremenu") == 0) {
                coreInterfaceFlare__Fi(0);
                menu = pause_tables(0xf7c0);
            }
            break;
        }
        case 1: {
            // The original copies the entry through a stack buffer, drops
            // anything from the first underscore, and appends ".tex"; none
            // of that text is read afterwards, so only the calls below stay.
            if (cutMode != 0) break;
            char* entry = static_cast<char*>(load_pointer(dispatched + picked * 16));
            if (netNetworkMode == 0) {
                strcpy(levelName, entry);
                debugClearStartMenu__Fv();
                levelName[80] = static_cast<char>(0xff);
                gameControlPaused = 60u;
                gamePause = 0u;
                particlesPaused = 0;
                startMenuState = 0;
                fade_stream_zero();
            } else {
                lastWarpPos = static_cast<unsigned>(picked);
                hudNetLevelWarp__FPc(entry);
            }
            break;
        }
        case 2: {
            char* entry = static_cast<char*>(load_pointer(dispatched + picked * 16));
            if (strcmp(entry, "menu_controls") == 0) {
                menuNm = 3u;
                menu = preConfigure;
            } else if (strcmp(entry, "menu_comm") == 0) {
                menuNm = 5u;
                menu = pause_tables(0xf700);
            }
            break;
        }
        case 3:
            frontendInitControllerConfig__Fi(picked);
            break;
        case 4: {
            unsigned char who = kickMenuList[picked];
            netKickPlayer__Fi(who);
            debugClearStartMenu__Fv();
            break;
        }
        }
    }
    if (menu != dispatched && menu != pause_tables(0xf940) && menu != kickMenu)
        menuSetPos__FP4Menui(menu, 0);
    if ((load_word(netLocalPad + menuPlayerNm * 88 + 12) & padBackButton) == 0u) return;
    if (menuNm == 0u) {
        if (menu != pause_tables(0xf7c0) || useRaceMenu == 0u) {
            debugClearStartMenu__Fv();
            gameControlPaused = 60u;
            if ((soundGetStreamPlayStatus__Fi(0) & 0x40) != 0) soundFadeStream__Fi(0);
            return;
        }
        menu = raceMainMenu;
        coreInterfaceFlare__Fi(0);
        return;
    }
    if (menuNm == 3u || menuNm == 5u) {
        menuNm = 2u;
        menu = pause_tables(0xf610);
    } else if (menuNm == 2u) {
        menuNm = 51u;
    } else {
        menuNm = 0u;
        menu = pause_tables(0xf7c0);
    }
    coreInterfaceFlare__Fi(0);
    {
        void* backTex = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"),
                                                const_cast<char*>("menu_main_back.anm"));
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(backTex, nullptr, 1.0f);
    }
}
