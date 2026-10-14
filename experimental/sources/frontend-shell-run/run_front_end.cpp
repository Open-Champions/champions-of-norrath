// Observed runFrontEnd__Fv at 0x001c8098, 5656 bytes. It returns.
// One caller reaches it: frontEndInit registers it with engineAddTask.
// The menu pointer and menuNm used here are the locals at 0x00484ba4 and
// 0x00484ba0. dialog-hud-controls defines different objects with those names.
// This file leaves both unresolved.
//
// Observed, inferred, and unknown are separated below. Fresh EE compilation
// is not an original-byte match. No EE frame has run.

extern "C" {
struct AnimationHeader;
struct GameObject;

void soundResetStream__Fi(int stream);
void soundStartGlobalMusic__FPci(char* name, int mode);
int soundGetStreamAmountPlayed__Fi(int stream);
void hudRunNoControllers__Fi(int value);
int sprintf(char* dest, const char* format, ...);
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
void fontPrintCentered__FiiPci(int x, int y, char* text, int scale);
void fontEnd__Fv();
int isin__Fi(int angle);
void frontendRunControllerConfig__Fv();
int MC_Busy__Fv();
void* lumpFindResource__FPcT0(char* lump, char* name);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
unsigned short* textGetString__FPc(char* key);
int hudRunYesNo__FPUsiiii(unsigned short* text, int a, int b, int c, int d);
void engineSetDisplayDisable__Fi(int value);
void netInit__Fv();
void netFree__Fb(int down);
void engineReboot__Fv();
int netDoNewLaunch__Fv();
void frontEndCleanUp__Fi(int value);
void gameDeathStart__Fi(int value);
void gameResetGameState__Fv();
void netStart__Fv();
int gameMaybeSaveConfig__Fii(int first, int second);
void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format, float a, float b, float c,
                              float d, float e, float f, float g, float h);
void hudShowMsg__FPUsN20iii(unsigned short* title, unsigned short* body, unsigned short* extra, int width,
                            int fifth, int sixth);
void gamePopConfig__Fv();
void startNetLaunch__Fv();
void setSRSInfo__Fv();
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int scale);
void menuDraw__FiiP4MenuUiUiP4FontP8_texturei(int x, int y, unsigned char* records, unsigned first,
                                              unsigned second, void* font, void* texture, int last);
int menuRun__FP4MenuP3padPi(unsigned char* records, unsigned char* pad_base, int* out_value);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(AnimationHeader* header, GameObject* object,
                                                          float volume);
int soundGetStreamPlayStatus__Fi(int stream);
void soundUpdateMusicVolume__Fv();
void soundUpdateAdjustments__Fv();
void soundFadeStream__Fi(int stream);
void fontColor__FUiii(unsigned color, int second, int third);
int strcmp(const char* left, const char* right);
void stopCredits__Fv();
void rectangleDraw__FiiiiiUi(int x, int y, int width, int height, int mode, unsigned color);
int paramRunning__Fv();
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int width, int height, int mode, unsigned color,
                                      int seventh, int eighth, int ninth, int tenth);
int fontStringSizeu__FP4FontPUsi(void* font, unsigned short* text, int scale);
void soundClearStreamQue__Fi(int stream);
void gamePushConfig__Fv();
void playerInit__FiiPcScScScScT2(int index, int race, char* name, signed char field, signed char one,
                                 signed char minus_one, signed char zero, char* trailing);
void memset(void* dest, int value, unsigned size);
void launchPlayerSelect__Fv();
void MC_ChangePlayers__FiPUsiii(int first, unsigned short* text, int count, int fourth, int fifth);
void MC_LoadGameBrowser__Fii(int first, int second);
void menuSetPos__FP4Menui(unsigned char* records, int position);
void frontendInitControllerConfig__Fi(int selected);
void engineSyncVBlank__Fv();

extern int musicPlaying;
extern int musicStart;
extern unsigned char gameHidePlayers;
extern unsigned char frontEndStartInCredits;
extern unsigned char MC_Debug;
extern void* smallFont;
extern int drawFrameNm;
extern int frontendControllerConfigRunning;
extern int menuNm;
extern unsigned char netAttemptLaunch;
extern unsigned char menuModeRebootQuestion;
extern unsigned char* menu;
extern unsigned char netNetworkMode;
extern unsigned char coreClearNetLogFuck;
extern unsigned char netGameLaunchInfo[];
extern int nmPlayers;
extern int gamePause;
extern unsigned char levelName[];
extern int padAcceptButtonFontChar;
extern int padBackButtonFontChar;
extern unsigned short hudFooterUni[];
extern unsigned char pad[];
extern unsigned padAcceptButton;
extern unsigned padBackButton;
extern unsigned char* statusPtr;
extern unsigned char coreLaunchSRS;
extern int gameBailFromGameLoop;
extern void* mediumFont;
extern float soundSfxVolume;
extern float soundMusicVolume;
extern int frontend_end_timer_1104 __asm__("endTimer.1104");
extern int endInit;
extern int endTimer;
extern int creditsPosY;
extern int lineShift;
extern int curFocus;
extern unsigned char creditsText[];
extern int frameNm;
extern int gameDifficulty;
extern int isNetGame;
extern unsigned char netBroadCastStart;
extern unsigned MC_GameConfig;
extern unsigned char playerSelectEnforce;
extern unsigned char playerSelectCount;
extern unsigned char playerSelectStartPlayer;
extern unsigned char playerSelectNextName[];
extern unsigned char playerSelectPrevName[];
extern unsigned short* playerSelectExitMessage;
extern unsigned char norrath_frontend_byte_484ba8;
extern unsigned char attemptLoadGame;
extern float expWarnAlpha;
extern unsigned char coreBeenInSRS;
extern unsigned char startMenuState;
extern unsigned char MC_LastModeSuccessful;
extern unsigned char playerSelectSuccessful;
extern unsigned char frontMenu[];
extern unsigned char creditMenu[];
extern unsigned char gameTypeMenu[];
extern unsigned char playerMenu[];
extern unsigned char menuNetClientServer[];
extern unsigned char menuNetImportOrNewPlayer[];
extern unsigned char menuNetImportOrNewGame[];
extern unsigned char optionMenu[];
extern unsigned char difficultyMenu[];
extern unsigned char preConfigure[];
extern unsigned char commMenu[];

static inline unsigned load_word(const unsigned char* address) {
    return (unsigned)address[0] | ((unsigned)address[1] << 8) | ((unsigned)address[2] << 16) |
           ((unsigned)address[3] << 24);
}

static inline void store_word(unsigned char* address, unsigned value) {
    address[0] = (unsigned char)value;
    address[1] = (unsigned char)(value >> 8);
    address[2] = (unsigned char)(value >> 16);
    address[3] = (unsigned char)(value >> 24);
}

static inline unsigned short load_half(const unsigned char* address) {
    return (unsigned short)(address[0] | (address[1] << 8));
}

static inline int half_width(unsigned short bits) {
    // The halfword is sign-extended, then divided by two toward zero.
    int width = (short)bits;
    return (width + (width < 0)) >> 1;
}

static inline float bits_to_float(unsigned bits) {
    union {
        unsigned u;
        float f;
    } value;
    value.u = bits;
    return value.f;
}

// Observed cvt.w.s. Inferred: the default rounder is nearest, ties to even.
// The 2^31 adjustment is present and is not reached for the clamped alphas.
static inline int cvt_w_s(float value) {
    const float bias = bits_to_float(0x4f000000u);
    int high = 0;
    if (!(value < bias)) {
        value = value - bias;
        high = (int)0x80000000u;
    }
    int truncated = (int)value;
    float fraction = value - (float)truncated;
    int rounded = truncated;
    if (fraction > 0.5f || (fraction == 0.5f && (truncated & 1) != 0)) rounded = truncated + 1;
    return rounded | high;
}

static inline char* entry_label(unsigned char* records, int index) {
    // The original loads a 32-bit word. The host contract stores a native pointer
    // in that slot so the comparison still runs; the EE path keeps the word load.
#if defined(NORRATH_HOST_CONTRACT)
    char* label = 0;
    const unsigned char* slot = records + index * 16;
    for (unsigned byte = 0; byte < sizeof label; ++byte) {
        reinterpret_cast<unsigned char*>(&label)[byte] = slot[byte];
    }
    return label;
#else
    return reinterpret_cast<char*>(load_word(records + index * 16));
#endif
}

static inline char* credits_line(int cursor) {
#if defined(NORRATH_HOST_CONTRACT)
    char* line = 0;
    const unsigned char* slot = creditsText + cursor;
    for (unsigned byte = 0; byte < sizeof line; ++byte) {
        reinterpret_cast<unsigned char*>(&line)[byte] = slot[byte];
    }
    return line;
#else
    return reinterpret_cast<char*>(load_word(creditsText + cursor));
#endif
}

static inline void copy_bytes(unsigned char* dest, const char* text, int count) {
    for (int index = 0; index < count; ++index) dest[index] = (unsigned char)text[index];
}

static inline __attribute__((always_inline)) void draw_title() {
    // menuNm 50 and 60, a busy card, or a pending launch skip the title.
    if (menuNm == 60 || menuNm == 50) return;
    if (MC_Busy__Fv() != 0 || netAttemptLaunch != 0) return;
    void* texture = lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"), const_cast<char*>("title.tex"));
    unsigned char* bytes = static_cast<unsigned char*>(texture);
    int x = 320 - half_width(load_half(bytes));
    textureDrawColor__FP8_textureiiiUi(texture, x, 36, 5, 0x80808080u);
}

static inline __attribute__((always_inline)) void draw_button_prompt() {
    // A live controller-config flag skips the hint. The flag is loaded again here.
    if (frontendControllerConfigRunning != 0) return;
    if (MC_Busy__Fv() != 0) return;
    void* font = lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"),
                                         const_cast<char*>("gamefontsmall.fnt"));
    fontStart__FP4FontiUii(font, 7, 0x64464646u, -1);
    unsigned short prompt[80];
    unsigned short* format;
    float first = 0.0f;
    float second = 0.0f;
    if (menuNm == 0) {
        format = textGetString__FPc(const_cast<char*>("Accept"));
        first = (float)padAcceptButtonFontChar;
    } else if (menuNm == 60) {
        format = textGetString__FPc(const_cast<char*>("Back"));
        first = (float)padBackButtonFontChar;
    } else {
        format = textGetString__FPc(const_cast<char*>("AcceptBack"));
        first = (float)padAcceptButtonFontChar;
        second = (float)padBackButtonFontChar;
    }
    usprintf__FPUsT0ffffffff(prompt, format, first, second, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    if (frontEndStartInCredits == 0) fontPrintCenteredu__FiiPUsi(320, 412, prompt, 0x000f4240);
    fontEnd__Fv();
}

static inline __attribute__((always_inline)) void mark_unoccupied_slots(unsigned char* records, int first_pad_only) {
    // Player menu writes entry words at +20, +36 and +52.
    // Only the first of those reads pad+92. The other two store 5.
    // Preconfigure reads pad+92, pad+180 and pad+268, one entry each.
    static const int kOffsets[3] = {92, 180, 268};
    for (int slot = 0; slot < 3; ++slot) {
        unsigned buttons = load_word(pad + kOffsets[slot]);
        unsigned value = 5u;
        if (!first_pad_only || slot == 0) {
            if (buttons != 0) value = 0u;
        }
        store_word(records + 16 * (slot + 1) + 4, value);
    }
}

static inline __attribute__((always_inline)) void draw_difficulty_warning(int selected, int menu_result) {
    unsigned short* primary = 0;
    unsigned short* secondary = 0;
    if (selected == 0) {
        primary = textGetString__FPc(const_cast<char*>("difficulty1"));
    } else if (selected == 1) {
        primary = textGetString__FPc(const_cast<char*>("difficulty2"));
        if (((MC_GameConfig ^ 1u) & 1u) != 0) {
            secondary = textGetString__FPc(const_cast<char*>("difficulty2Level"));
        }
    } else {
        primary = textGetString__FPc(const_cast<char*>("difficulty3"));
        if ((MC_GameConfig & 2u) == 0) {
            secondary = textGetString__FPc(const_cast<char*>("difficulty3Level"));
        }
    }
    if (menu_result != 0) expWarnAlpha = 0.0f;
    float sum = expWarnAlpha + bits_to_float(0x3d84a444u);
    expWarnAlpha = (1.0f < sum) ? 1.0f : sum;
    void* font = lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"),
                                         const_cast<char*>("gamefontsmall.fnt"));
    int alpha = cvt_w_s(expWarnAlpha * bits_to_float(0x43000000u));
    unsigned color = ((unsigned)alpha << 24) | 0x002d5049u;
    fontStart__FP4FontiUii(font, 6, color, -1);
    fontPrintCenteredu__FiiPUsi(320, 363, primary, 0x000f4240);
    if (secondary != 0) fontPrintCenteredu__FiiPUsi(320, 381, secondary, 0x000f4240);
    fontEnd__Fv();
    font = lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"), const_cast<char*>("gamefontsmall.fnt"));
    int width = fontStringSizeu__FP4FontPUsi(font, primary, 0x000f4240);
    if (secondary != 0) {
        font = lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"),
                                       const_cast<char*>("gamefontsmall.fnt"));
        int extra = fontStringSizeu__FP4FontPUsi(font, secondary, 0x000f4240);
        if (width < extra) width = extra;
    }
    int span = width + 38;
    int x = 320 - half_width((unsigned short)span);
    int border_alpha = cvt_w_s(expWarnAlpha * bits_to_float(0x425c0000u));
    unsigned border = ((unsigned)border_alpha << 24) | 0x80808080u;
    // The tenth argument's stack word is not stored. Zero is not an observation.
    textureSmoothBorder__FiiiiiUiiii(x, 362, span, secondary != 0 ? 54 : 34, 5, border, 0, 0, 0, 0);
}

static inline __attribute__((always_inline)) void draw_credits() {
    fontStart__FP4FontiUii(mediumFont, 7, 0x80000000u, -1);
    if (credits_line(4) != 0) {
        int cursor = 4;
        int line_offset = 0;
        while (credits_line(cursor) != 0) {
            int y = creditsPosY + line_offset;
            int alpha;
            if (y >= 301) {
                alpha = 428 - y;
            } else if (y < 128) {
                alpha = y;
            } else {
                alpha = 128;
            }
            if (alpha < 0) alpha = 0;
            if (alpha > 128) alpha = 128;
            unsigned char flag = creditsText[cursor - 4];
            unsigned rgb = flag != 0 ? 0x001a444cu : 0x00405e5eu;
            unsigned scale_bits = flag != 0 ? 0x3f370a80u : 0x3f3cac3au;
            unsigned color = ((unsigned)cvt_w_s((float)alpha * bits_to_float(scale_bits)) << 24) | rgb;
            if ((unsigned)(y + 99) < 699u) {
                char* line = credits_line(cursor);
                if (strcmp(line, "Design") == 0) {
                    if (endInit == 0) {
                        endTimer = 1800;
                        endInit = 1;
                    }
                } else {
                    int shaded = alpha;
                    if (shaded < 0) shaded += 3;
                    fontColor__FUiii((unsigned)(shaded >> 2) << 24, -1, 0);
                    fontPrintCentered__FiiPci(322, y + 2, line, 10000);
                    fontColor__FUiii(color, -1, 0);
                    fontPrintCentered__FiiPci(320, y, line, 10000);
                }
            }
            cursor += 8;
            line_offset += 19;
        }
    }
    fontEnd__Fv();
    if (endTimer > 0) {
        if (frameNm % 3 == 0) creditsPosY -= 1;
        if (endInit != 0) endTimer -= 1;
        if ((load_word(pad + 12) & 0x20u) != 0) stopCredits__Fv();
    } else {
        stopCredits__Fv();
    }
}

static inline __attribute__((always_inline)) int config_blocks(int selected) {
    if (selected == 2 && (MC_GameConfig & 2u) == 0) return 1;
    if (selected == 1 && ((MC_GameConfig ^ 1u) & 1u) != 0) return 1;
    return 0;
}

static inline __attribute__((always_inline)) void launch_info_bits(unsigned clear_mask, unsigned set_mask) {
    netGameLaunchInfo[0] = (unsigned char)((netGameLaunchInfo[0] & clear_mask) | set_mask);
}

static inline __attribute__((always_inline)) void copy_level(const char* text, int count) {
    copy_bytes(levelName, text, count);
    levelName[80] = 0xffu;
}

static inline __attribute__((always_inline)) int accept_menu(int selected, int accept_pad) {
    // Accept clears the network-mode byte even when the switch is skipped.
    netNetworkMode = 0;
    if (accept_pad == 0) return 0;
    if (menuNm != 60) {
        void* sequence = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"),
                                                 const_cast<char*>("menu_main_accept.anm"));
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(static_cast<AnimationHeader*>(sequence), 0, 1.0f);
    }
    expWarnAlpha = 0.0f;
    unsigned char* menu_before = menu;
    if ((unsigned)menuNm < 52u) {
        char* label = entry_label(menu, selected);
        switch (menuNm) {
        case 0:
            if (strcmp(label, "menu_startnewgame") == 0) {
                menu = gameTypeMenu;
                menuNm = 1;
            } else if (strcmp(label, "menu_loadsavedgame") == 0) {
                MC_LoadGameBrowser__Fii(accept_pad - 1, 1);
                attemptLoadGame = 1;
            } else if (strcmp(label, "menu_options") == 0) {
                gamePushConfig__Fv();
                menu = optionMenu;
                menuNm = 50;
            } else if (strcmp(label, "menu_credits") == 0) {
                menu = creditMenu;
                creditsPosY = 448;
                menuNm = 60;
                lineShift = 0;
                soundClearStreamQue__Fi(0);
                soundStartGlobalMusic__FPci(const_cast<char*>("mx_endcredits_ext_l.vag"), 3);
            } else if (strcmp(label, "Start Net Game") == 0) {
                netBroadCastStart = 1;
                netNetworkMode = 1;
                curFocus = 0;
                engineSetDisplayDisable__Fi(10);
                engineSyncVBlank__Fv();
                frontEndCleanUp__Fi(0);
                gamePause = 0;
                gameDifficulty = 1;
                gameResetGameState__Fv();
                for (int index = 0; index < 4; ++index) {
                    playerInit__FiiPcScScScScT2(index, 1, const_cast<char*>("Toonces"), 0, 0, 0, 0, 0);
                }
                copy_level("kelethin", 9);
                return 1;
            }
            break;
        case 1:
            isNetGame = selected;
            if (selected == 0) {
                menu = playerMenu;
                menuNm = 20;
            } else {
                menu = menuNetClientServer;
                menuNm = 2;
                memset(netGameLaunchInfo, 0, 328u);
            }
            break;
        case 2:
            if (selected == 0) {
                launch_info_bits(0xfeu, 0u);
                menu = menuNetImportOrNewPlayer;
                menuNm = 10;
            } else {
                launch_info_bits(0xfeu, 1u);
                menu = menuNetImportOrNewGame;
                menuNm = 15;
            }
            break;
        case 10:
            if (selected == 0) {
                launch_info_bits(0xf9u, 2u);
                statusPtr = &playerSelectSuccessful;
                launchPlayerSelect__Fv();
            } else {
                launch_info_bits(0xf9u, 4u);
                statusPtr = &MC_LastModeSuccessful;
                unsigned short* text = textGetString__FPc(const_cast<char*>("selectExit"));
                MC_ChangePlayers__FiPUsiii(0, text, 1, 0, 1);
            }
            menuNm = 11;
            break;
        case 15:
            launch_info_bits(0xf9u, 0u);
            if (selected == 0) {
                menuNm = 16;
                menu = difficultyMenu;
            } else {
                statusPtr = &MC_LastModeSuccessful;
                menuNm = 17;
                MC_LoadGameBrowser__Fii(accept_pad - 1, 0);
                attemptLoadGame = 1;
            }
            return 1;
        case 16:
            if (config_blocks(selected)) break;
            menuNm = 17;
            gameDifficulty = selected + 1;
            if (selected == 0) {
                statusPtr = &playerSelectSuccessful;
                netGameLaunchInfo[0] = (unsigned char)(netGameLaunchInfo[0] | 2u);
                launchPlayerSelect__Fv();
            } else {
                statusPtr = &MC_LastModeSuccessful;
                netGameLaunchInfo[0] = (unsigned char)(netGameLaunchInfo[0] | 4u);
                unsigned short* text = textGetString__FPc(const_cast<char*>("selectExit"));
                MC_ChangePlayers__FiPUsiii(0, text, 1, 0, 1);
            }
            break;
        case 20:
            if (selected < 0 || selected >= 4) break;
            menu = difficultyMenu;
            nmPlayers = selected + 1;
            menuNm = 21;
            menuSetPos__FP4Menui(menu, 0);
            break;
        case 21:
            if (config_blocks(selected)) break;
            gameDifficulty = selected + 1;
            if (selected == 0) {
                playerSelectEnforce = 1;
                playerSelectCount = (unsigned char)nmPlayers;
                curFocus = 0;
                gamePause = 0;
                playerSelectStartPlayer = 0;
                unsigned short* text = textGetString__FPc(const_cast<char*>("selectExit"));
                copy_bytes(playerSelectNextName, "kelsel", 7);
                // The byte after the copy lands in the first byte of the adjacent
                // playerSelectPrevName, not inside the 64-byte next name.
                playerSelectPrevName[0] = norrath_frontend_byte_484ba8;
                playerSelectExitMessage = text;
                gameResetGameState__Fv();
                engineSetDisplayDisable__Fi(10);
                engineSyncVBlank__Fv();
                frontEndCleanUp__Fi(0);
                copy_level("kelsel", 7);
                return 1;
            }
            if ((unsigned char)nmPlayers == 1) {
                unsigned short* text = textGetString__FPc(const_cast<char*>("MemcardImportPlayer"));
                MC_ChangePlayers__FiPUsiii(curFocus, text, nmPlayers, 1, 1);
            } else {
                unsigned short* text = textGetString__FPc(const_cast<char*>("MemcardImportPlayers"));
                MC_ChangePlayers__FiPUsiii(curFocus, text, nmPlayers, 1, 1);
            }
            break;
        case 50:
            if (strcmp(label, "menu_combatCam") == 0) {
                menu = preConfigure;
                menuNm = 51;
            } else if (strcmp(label, "menu_comm") == 0) {
                menuNm = 52;
                menu = commMenu;
                gamePushConfig__Fv();
            }
            break;
        case 51:
            frontendInitControllerConfig__Fi(selected);
            menuNm = 55;
            break;
        default:
            break;
        }
    }
    if (menu != menu_before) menuSetPos__FP4Menui(menu, 0);
    return 0;
}

static inline __attribute__((always_inline)) void back_menu() {
    if (menuNm == 0) {
        startMenuState = 0;
        return;
    }
    void* sequence = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"),
                                             const_cast<char*>("menu_main_back.anm"));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(static_cast<AnimationHeader*>(sequence), 0, 1.0f);
    int current = menuNm;
    expWarnAlpha = 0.0f;
    int next = current - 1;
    if ((unsigned)next < 60u) {
        switch (current) {
        case 1:
            menu = frontMenu;
            break;
        case 2:
            if (coreBeenInSRS != 0) {
                menuModeRebootQuestion = 1;
                next = current;
            } else {
                menu = gameTypeMenu;
            }
            break;
        case 10:
        case 15:
            menu = menuNetClientServer;
            next = 2;
            break;
        case 11:
        case 12:
            menu = menuNetImportOrNewPlayer;
            next = 10;
            break;
        case 13:
            menu = difficultyMenu;
            break;
        case 16:
        case 17:
        case 18:
            menu = menuNetImportOrNewGame;
            next = 15;
            break;
        case 20:
            menu = gameTypeMenu;
            next = 1;
            break;
        case 21:
            menu = playerMenu;
            break;
        case 50:
            next = 55;
            break;
        case 51:
            menu = optionMenu;
            next = 50;
            break;
        case 60:
            stopCredits__Fv();
            next = 0;
            break;
        default:
            break;
        }
    }
    menuNm = next;
}

void runFrontEnd__Fv() {
    int menu_result = 0;
    gameHidePlayers = 1;
    if (musicPlaying == 0) {
        soundResetStream__Fi(0);
        if (frontEndStartInCredits != 0) {
            soundStartGlobalMusic__FPci(const_cast<char*>("mx_endcredits_ext_l.vag"), 1);
        } else {
            soundStartGlobalMusic__FPci(const_cast<char*>("mx_maintheme_l.vag"), 2);
            soundStartGlobalMusic__FPci(const_cast<char*>("mx_mainmenu_l.vag"), 5);
        }
        musicPlaying = 1;
        musicStart = soundGetStreamAmountPlayed__Fi(0);
    }
    hudRunNoControllers__Fi(1);
    if (MC_Debug != 0) {
        char line[160];
        sprintf(line, "Build: %s,  %s\n", "Dec 30 2003", "18:59:04");
        fontStart__FP4FontiUii(smallFont, 4, 0x80808080u, -1);
        fontPrintCentered__FiiPci(320, 30, line, 10000);
        fontEnd__Fv();
    }
    isin__Fi(drawFrameNm << 9);
    if (frontendControllerConfigRunning != 0) {
        frontendRunControllerConfig__Fv();
        return;
    }
    draw_title();
    if (MC_Busy__Fv() != 0) return;
    if (menuModeRebootQuestion != 0) {
        int answer = hudRunYesNo__FPUsiiii(textGetString__FPc(const_cast<char*>("rebootquestion")), 0, 0, 0, 120);
        if (answer == 2) {
            engineSetDisplayDisable__Fi(10);
            netInit__Fv();
            netFree__Fb(1);
            engineReboot__Fv();
        }
        if (answer == 1) menuModeRebootQuestion = 0;
        return;
    }
    if (netAttemptLaunch != 0) {
        int launch = netDoNewLaunch__Fv();
        if (launch == 0) return;
        netAttemptLaunch = 0;
        if (launch > 0) {
            coreClearNetLogFuck = 1;
            frontEndCleanUp__Fi(0);
            if (launch == 1) {
                gameDeathStart__Fi(1);
            } else {
                netNetworkMode = 1;
                gameResetGameState__Fv();
                copy_level("kelethin", 9);
                nmPlayers = (int)(signed char)netGameLaunchInfo[4];
                gamePause = 0;
            }
            netStart__Fv();
            return;
        }
        menu = menuNetClientServer;
        menuNm = 2;
        netFree__Fb(0);
        return;
    }
    if (menuNm == 55) {
        int choice = 1;
        if (gameMaybeSaveConfig__Fii(0, 1) != 0) {
            unsigned short* question = textGetString__FPc(const_cast<char*>("MemcardYesNo"));
            usprintf__FPUsT0ffffffff(hudFooterUni, question, (float)padAcceptButtonFontChar,
                                     (float)padBackButtonFontChar, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            unsigned short* title = textGetString__FPc(const_cast<char*>("maybeSaveConfigTitle"));
            unsigned short* body = textGetString__FPc(const_cast<char*>("maybeSaveConfigBody"));
            hudShowMsg__FPUsN20iii(title, body, hudFooterUni, 128, 0, 0);
            choice = 2;
            unsigned buttons = load_word(pad + 12);
            if ((buttons & padAcceptButton) == 0) choice = (buttons & padBackButton) != 0 ? 1 : 0;
        }
        if (choice == 0) return;
        menuNm = 0;
        menu = frontMenu;
        if (choice == 2) {
            gameMaybeSaveConfig__Fii(0, 0);
        } else {
            gamePopConfig__Fv();
        }
        return;
    }
    if (menuNm == 18 || menuNm == 12) {
        startNetLaunch__Fv();
        return;
    }
    if (menuNm == 17) {
        if (statusPtr[0] != 0) {
            frontEndCleanUp__Fi(1);
            coreLaunchSRS = 1;
            setSRSInfo__Fv();
            gameBailFromGameLoop = 1;
            menuNm = 18;
            return;
        }
        menuNm = 15;
    }
    if (menuNm == 11) {
        if (statusPtr[0] != 0) {
            frontEndCleanUp__Fi(1);
            coreLaunchSRS = 1;
            setSRSInfo__Fv();
            gameBailFromGameLoop = 1;
            menuNm = 12;
            return;
        }
        menuNm = 10;
    }
    draw_button_prompt();
    if (menu == playerMenu) mark_unoccupied_slots(menu, 1);
    float saved_sfx = soundSfxVolume;
    float saved_music = soundMusicVolume;
    int selected = 0;
    if (menuNm != 60) {
        void* med = lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"),
                                            const_cast<char*>("gamefontmed.fnt"));
        void* plate = lumpFindResource__FPcT0(const_cast<char*>("frontend.lmp"),
                                              const_cast<char*>("frontendmenu.tex"));
        int y = menu == preConfigure ? 331 : 238;
        menuDraw__FiiP4MenuUiUiP4FontP8_texturei(320, y, menu, 0x6f1d392cu, 0x80446b6au, med, plate, 0);
        selected = menuRun__FP4MenuP3padPi(menu, pad, &menu_result);
    }
    if (menu == optionMenu) {
        if (soundSfxVolume != saved_sfx && (drawFrameNm & 3) == 0) {
            void* sequence = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"),
                                                     const_cast<char*>("game_butn_tab.anm"));
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(static_cast<AnimationHeader*>(sequence), 0, 1.0f);
        }
        if (soundMusicVolume != saved_music) {
            if ((soundGetStreamPlayStatus__Fi(0) & 0x20) != 0) {
                frontend_end_timer_1104 = 0;
            } else {
                soundStartGlobalMusic__FPci(const_cast<char*>("mx_mainmenu_l.vag"), 65);
                frontend_end_timer_1104 = 0;
            }
        }
        soundUpdateMusicVolume__Fv();
        soundUpdateAdjustments__Fv();
    }
    int timer = frontend_end_timer_1104 + 1;
    frontend_end_timer_1104 = timer;
    if (timer >= 121 && (soundGetStreamPlayStatus__Fi(0) & 0x40) != 0) soundFadeStream__Fi(0);
    if (menuNm == 60) {
        int credits_timer = endTimer;
        draw_credits();
        if (credits_timer <= 0) return;
        if (frontEndStartInCredits != 0) {
            rectangleDraw__FiiiiiUi(0, 0, 640, 512, 6, 0x80000000u);
            return;
        }
    }
    int accept_pad = 0;
    if (paramRunning__Fv() == 0) {
        // The stride loop leaves after pad 0. Later pads are not read here.
        if ((load_word(pad + 12) & padAcceptButton) != 0) accept_pad = 1;
    }
    if (menu == difficultyMenu) draw_difficulty_warning(selected, menu_result);
    if (menu == preConfigure) mark_unoccupied_slots(menu, 0);
    if (accept_menu(selected, accept_pad) != 0) return;
    // The back scan reads pad 0 before the second paramRunning call.
    // A running parameter check drops the back action after that read.
    if ((load_word(pad + 12) & padBackButton) != 0 && paramRunning__Fv() == 0) back_menu();
}
}
