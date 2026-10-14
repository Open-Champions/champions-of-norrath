// Observed: doHelpfulMessage__FiiP12screenStructiiPQ24item11description at 0x001b6730 is 1868
// bytes with a 608-byte frame. Arguments: side (0 or 1), player (index), screen (an inventory
// screen record), mode, flag, desc (a description pointer or zero); no result. buf is a text
// buffer of 16-bit characters on the stack; n is the int localNmPlayersAlive; K is the three-way
// call textGetString__FPc(name) then usprintf__FPUsT0ffffffff(buf, text, a0 .. a7), where the
// eight floats are the ints named (converted) or zero.
//  1. show = 1. When the int at screen + 88 is zero and mode is neither 0, 1 nor -2 (a test made
//     on the unsigned value), mode becomes 2. offset = 0 when n < 2, else 160 when side is
//     nonzero and -160 otherwise.
//  2. When flag is nonzero: K with "OtherEnter" when n > 2, otherwise "Player1Enter" for side 0
//     and "Player2Enter" for side 1, a0 = padAcceptButtonFontChar. fontStart__FP4FontiUii(
//     mediumFont, 7, 0x80464646, -1); the print step P below; fontEnd__Fv(); then the border step
//     B below.
//  3. P: when n >= 2, the byte language is nonzero, mode is 2 and the int at screen + 80 is
//     below 3: fontPrintWrapCenteredu__FiiPUsiiii(offset + 320, 382, buf, 320, -100, 580, 31).
//     Otherwise every character 10 in buf becomes 32, and fontPrintWrapCenteredu(offset + 320,
//     412, buf, 640, -100, 580, 21).
//     B: w = fontStringSizeu__FP4FontPUsi(mediumFont, buf, 1000000);
//     textureSmoothBorder__FiiiiiUiiii(offset + 306 - w / 2, 410, w + 30, 31, 6, 0x40808080, 0, 0, 0).
//  4. rec is the player record player + 23984 * player. When the word at rec + 21872 is zero the
//     function returns here. Otherwise, when shopPromptOK[side] is nonzero, player is in
//     [0, nmPlayers) and bit 8 of the word at p + 12 is clear (p being that word at rec + 21872):
//     fontStart(mediumFont, 7, 0x80464646, -1), then, with screen int 20 = tab:
//      mode -1: "AcceptExit", (accept, back).
//      else tab 3 or 4: "Back", (back).
//      else tab 5: when byte rec + 242 is zero "ChooseBack", (accept, back); else when the int at
//        screen + 48 is nonzero "SelectChooseDone", (127.0, 128.0, 129.0, 130.0, accept, back);
//        else "SpellBookChooseDone", (info, accept, back).
//      else mode -2: "ChooseDone", (accept, back); mode 0: "BuyBack", (info, back); mode 1:
//        "SellBack", (info, back).
//      mode 2: when desc is zero "Back", (back); else item = rec + 328 + 4616 * tab + 72 * (the
//        halfword at desc + 34); when itemEquipped__FiPQ24item11description(player, item) is
//        nonzero "UnequipDropBack", (equip, drop, back); else with q the word at item and
//        the 64-bit value at q + 40: bit 41 set "DropBack", (drop, back); clear "EquipDropBack",
//        (equip, drop, back).
//      mode 3: when desc is zero show = 0; else when the int at screen + 88 is nonzero
//        "CombineWith", (equip, back); else no text is made.
//      any other mode: no text is made (the buffer is then printed as it was).
//     Then, when show, the print step P; fontEnd().
//  5. drew (the int drewHelpfulMessageHelpBar) becomes 0 when it is below drawFrameNm. When
//     shopPromptOK[side] is nonzero and show: step B, and drew becomes drawFrameNm. Last,
//     shopPromptOK[side] = 1.
//     accept, back, info, equip, drop are the ints padAcceptButtonFontChar,
//     padBackButtonFontChar, infoButtonFontChar, equipButtonFontChar, dropButtonFontChar.
// Inferred: this draws the button hint bar of the inventory and shop screens; the sign of offset
// places it in the left or right half in a split-screen game.
// Unknown: the meaning of shopPromptOK (it is 1 after the first call for a side), of the bit
// 41 at q + 40, of the byte at rec + 242, and of the floats 127.0 to 130.0 passed as the first
// four format values. Where no text is made the buffer is printed uninitialised; that cannot be
// reproduced on the host.
extern "C" {
unsigned short* textGetString__FPc(const char* name);
void usprintf__FPUsT0ffffffff(unsigned short* to, unsigned short* format, float a0, float a1, float a2, float a3, float a4, float a5,
                              float a6, float a7);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
void fontEnd__Fv();
void fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int a, int b, int c, int d);
int fontStringSizeu__FP4FontPUsi(void* font, unsigned short* text, int limit);
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int w, int h, int a, unsigned color, int b, int c, int d);
int itemEquipped__FiPQ24item11description(int player_index, void* item);
extern unsigned char player[];
extern int localNmPlayersAlive;
extern int nmPlayers;
extern int drawFrameNm;
extern int drewHelpfulMessageHelpBar;
extern unsigned char shopPromptOK[2];
extern unsigned char language;
extern void* mediumFont;
extern int padAcceptButtonFontChar;
extern int padBackButtonFontChar;
extern int infoButtonFontChar;
extern int equipButtonFontChar;
extern int dropButtonFontChar;
}

#ifdef W11_FRONT_A_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
inline unsigned readWord(__UINTPTR_TYPE__ address) {
    unsigned v;
    __builtin_memcpy(&v, reinterpret_cast<const void*>(address), 4);
    return v;
}
inline __UINTPTR_TYPE__ pointerOf(unsigned word) {
#ifdef W11_FRONT_A_HOST
    return hostWordAddress(word);
#else
    return word;
#endif
}
void format(unsigned short* buf, const char* name, float a0, float a1 = 0, float a2 = 0, float a3 = 0, float a4 = 0, float a5 = 0) {
    usprintf__FPUsT0ffffffff(buf, textGetString__FPc(name), a0, a1, a2, a3, a4, a5, 0, 0);
}
void printText(unsigned short* buf, int offset, int mode, int tab) {
    if (localNmPlayersAlive >= 2 && language != 0 && mode == 2 && tab < 3) {
        fontPrintWrapCenteredu__FiiPUsiiii(offset + 320, 382, buf, 320, -100, 580, 31);
    } else {
        for (unsigned short* c = buf; *c != 0; ++c)
            if (*c == 10) *c = 32;
        fontPrintWrapCenteredu__FiiPUsiiii(offset + 320, 412, buf, 640, -100, 580, 21);
    }
}
void border(unsigned short* buf, int offset) {
    const int w = fontStringSizeu__FP4FontPUsi(mediumFont, buf, 1000000);
    textureSmoothBorder__FiiiiiUiiii(offset + 306 - w / 2, 410, w + 30, 31, 6, 0x40808080u, 0, 0, 0);
}
}

extern "C" void doHelpfulMessage__FiiP12screenStructiiPQ24item11description(int side, int playerIndex, int* screen, int mode, int flag,
                                                                           unsigned char* desc) {
    unsigned short buf[256];
    int show = 1;
    if (screen[22] == 0 && static_cast<unsigned>(mode) >= 2u) mode = mode == -2 ? -2 : 2;
    int offset = 0;
    if (localNmPlayersAlive >= 2) offset = side != 0 ? 160 : -160;
    if (flag != 0) {
        const char* name = localNmPlayersAlive > 2 ? "OtherEnter" : side == 0 ? "Player1Enter" : "Player2Enter";
        format(buf, name, static_cast<float>(padAcceptButtonFontChar));
        fontStart__FP4FontiUii(mediumFont, 7, 0x80464646u, -1);
        printText(buf, offset, mode, screen[20]);
        fontEnd__Fv();
        border(buf, offset);
    }
    unsigned char* rec = player + 23984 * playerIndex;
    const unsigned pWord = readWord(reinterpret_cast<__UINTPTR_TYPE__>(rec) + 21872);
    if (pWord == 0) return;
    if (shopPromptOK[side] != 0 && playerIndex >= 0 && playerIndex < nmPlayers && (readWord(pointerOf(pWord) + 12) & 8u) == 0) {
        fontStart__FP4FontiUii(mediumFont, 7, 0x80464646u, -1);
        const int tab = screen[20];
        if (mode == -1) {
            format(buf, "AcceptExit", static_cast<float>(padAcceptButtonFontChar), static_cast<float>(padBackButtonFontChar));
        } else if (static_cast<unsigned>(tab - 3) < 2u) {
            format(buf, "Back", static_cast<float>(padBackButtonFontChar));
        } else if (tab == 5) {
            if (rec[242] == 0) {
                format(buf, "ChooseBack", static_cast<float>(padAcceptButtonFontChar), static_cast<float>(padBackButtonFontChar));
            } else if (screen[12] != 0) {
                format(buf, "SelectChooseDone", 127.0f, 128.0f, 129.0f, 130.0f, static_cast<float>(padAcceptButtonFontChar),
                       static_cast<float>(padBackButtonFontChar));
            } else {
                format(buf, "SpellBookChooseDone", static_cast<float>(infoButtonFontChar), static_cast<float>(padAcceptButtonFontChar),
                       static_cast<float>(padBackButtonFontChar));
            }
        } else if (mode == -2) {
            format(buf, "ChooseDone", static_cast<float>(padAcceptButtonFontChar), static_cast<float>(padBackButtonFontChar));
        } else if (mode == 0) {
            format(buf, "BuyBack", static_cast<float>(infoButtonFontChar), static_cast<float>(padBackButtonFontChar));
        } else if (mode == 1) {
            format(buf, "SellBack", static_cast<float>(infoButtonFontChar), static_cast<float>(padBackButtonFontChar));
        } else if (mode == 2) {
            if (desc == nullptr) {
                format(buf, "Back", static_cast<float>(padBackButtonFontChar));
            } else {
                unsigned char* item = rec + 328 + 4616 * tab + 72 * *reinterpret_cast<short*>(desc + 34);
                if (itemEquipped__FiPQ24item11description(playerIndex, item) != 0) {
                    format(buf, "UnequipDropBack", static_cast<float>(equipButtonFontChar), static_cast<float>(dropButtonFontChar),
                           static_cast<float>(padBackButtonFontChar));
                } else {
                    unsigned long long bits;
                    __builtin_memcpy(&bits, reinterpret_cast<const void*>(pointerOf(readWord(reinterpret_cast<__UINTPTR_TYPE__>(item))) + 40), 8);
                    if ((bits & (1ull << 41)) != 0) {
                        format(buf, "DropBack", static_cast<float>(dropButtonFontChar), static_cast<float>(padBackButtonFontChar));
                    } else {
                        format(buf, "EquipDropBack", static_cast<float>(equipButtonFontChar), static_cast<float>(dropButtonFontChar),
                               static_cast<float>(padBackButtonFontChar));
                    }
                }
            }
        } else if (mode == 3) {
            if (desc == nullptr) {
                show = 0;
            } else if (screen[22] != 0) {
                format(buf, "CombineWith", static_cast<float>(equipButtonFontChar), static_cast<float>(padBackButtonFontChar));
            }
        }
        if (show != 0) printText(buf, offset, mode, screen[20]);
        fontEnd__Fv();
    }
    if (drewHelpfulMessageHelpBar < drawFrameNm) drewHelpfulMessageHelpBar = 0;
    if (shopPromptOK[side] != 0 && show != 0) {
        border(buf, offset);
        drewHelpfulMessageHelpBar = drawFrameNm;
    }
    shopPromptOK[side] = 1;
}
