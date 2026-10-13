// Observed: jumpGateTaskDraw__Fv at 0x00283768 is 916 bytes with a 240-byte frame
// (a local text buffer of at most 128 bytes at sp+16). It returns at once when
// engineTaskActive__FPc("pauseMenu") is nonzero. Otherwise, when flareAlpha is
// nonzero, with c = flareAlpha << 24:
//  1. textureDrawBorder__FiiiiiUiPcii(186, 122, 268, 197, 6, c | 0x808080, 0, 1, 0);
//  2. fontStart__FP4FontiUii(smallFont, 6, c | 0x323c3c, -1);
//  3. for i from 0 while i < jumpGateDestCount (signed byte, re-read each pass):
//     if i == thisLevel, fontColor__FUiii(c | 0x202020, -1, 0); strcpy the
//     32-byte entry i of jumpGateEntries into the local buffer; then, starting
//     with no text, while strlen(buffer) >= 2: text = textGetString__FPc(buffer)
//     and, if the halfword at text + 4 is ':', drop the last character of buffer
//     and try again, else stop. If no text was found, text =
//     textGetString__FPc(entry). Print it with fontPrintCenteredu__FiiPUsi(320,
//     122 + 21 * i, text, 1000000); and if i == thisLevel, fontColor__FUiii(
//     c | 0x323c3c, -1, 0);
//  4. fontEnd__Fv();
//  5. y0 = 125 + 21 * lastDest, y1 = 125 + 21 * curDest; the highlight top is
//     y0 + (y1 - y0) * evalHermite__Ffffff(cursorTime, 0, 1, 0.5, 0.5), and a
//     rectangleDraw__FiiiiiUi(194, top, 446, top + 23.0f, 6, ((flareAlpha / 2) << 24)
//     | 0x366876) is made (both ends truncated to int);
//  6. looks up "Back" when curDest == thisLevel, else "AcceptBack", and formats it
//     with usprintf__FPUsT0ffffffff into hudFooterUni with the button font
//     character words as floats (Back: padBackButtonFontChar then zeros;
//     AcceptBack: padAcceptButtonFontChar, padBackButtonFontChar, zeros);
//  7. hudShowMsg__FPUsN20iii(textGetString__FPc("JumpGateTitle"), 0, hudFooterUni,
//     flareAlpha, 305, 181).
// Finally, whether or not flareAlpha was zero, calls doJumpEffect__Fi(1) when
// jumping is nonzero (not when the pause menu task is active).
// Inferred: this is the jump-gate destination list drawing; the entries are
// short level keys and the highlight slides between rows 21 pixels apart.
// Unknown: what text keys beginning with the stripped suffixes mean, the exact
// buffer size (at most 128 bytes from the frame), and what the text halfword at
// offset 4 being ':' signals.

struct Font;

extern "C" int engineTaskActive__FPc(const char* name);
extern "C" void textureDrawBorder__FiiiiiUiPcii(int a, int b, int c, int d, int e, unsigned color,
                                                char* p, int f, int g);
extern "C" void fontStart__FP4FontiUii(Font* font, int a, unsigned color, int b);
extern "C" void fontColor__FUiii(unsigned color, int a, int b);
extern "C" void fontEnd__Fv();
extern "C" char* strcpy(char* dst, const char* src);
extern "C" unsigned long strlen(const char* s);
extern "C" unsigned short* textGetString__FPc(const char* key);
extern "C" void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int z);
extern "C" float evalHermite__Ffffff(float t, float a, float b, float c, float d);
extern "C" void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int mode, unsigned color);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* out, unsigned short* format, float a,
                                         float b, float c, float d, float e, float f, float g,
                                         float h);
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* title, unsigned short* body,
                                       unsigned short* footer, int a, int b, int c);
extern "C" void doJumpEffect__Fi(int a);
extern "C" int flareAlpha;
extern "C" int thisLevel;
extern "C" int jumping;
extern "C" int padAcceptButtonFontChar;
extern "C" int padBackButtonFontChar;
extern "C" float cursorTime;
extern "C" signed char jumpGateDestCount;
extern "C" signed char lastDest;
extern "C" signed char curDest;
extern "C" char jumpGateEntries[];
extern "C" Font* smallFont;
extern "C" unsigned short hudFooterUni[];

extern "C" void jumpGateTaskDraw__Fv() {
    if (engineTaskActive__FPc("pauseMenu") != 0) return;
    if (flareAlpha != 0) {
        char buffer[128];
        textureDrawBorder__FiiiiiUiPcii(186, 122, 268, 197, 6,
                                        (static_cast<unsigned>(flareAlpha) << 24) | 0x808080u, 0,
                                        1, 0);
        fontStart__FP4FontiUii(smallFont, 6,
                               (static_cast<unsigned>(flareAlpha) << 24) | 0x323c3cu, -1);
        for (int i = 0; i < jumpGateDestCount; ++i) {
            char* entry = jumpGateEntries + 32 * i;
            if (i == thisLevel)
                fontColor__FUiii((static_cast<unsigned>(flareAlpha) << 24) | 0x202020u, -1, 0);
            strcpy(buffer, entry);
            unsigned short* text = 0;
            while (strlen(buffer) >= 2) {
                text = textGetString__FPc(buffer);
                if (text[2] != ':') break;
                buffer[strlen(buffer) - 1] = 0;
            }
            if (text == 0) text = textGetString__FPc(entry);
            fontPrintCenteredu__FiiPUsi(320, 122 + 21 * i, text, 1000000);
            if (i == thisLevel)
                fontColor__FUiii((static_cast<unsigned>(flareAlpha) << 24) | 0x323c3cu, -1, 0);
        }
        fontEnd__Fv();
        float from = static_cast<float>(21 * lastDest + 125);
        float delta = static_cast<float>(21 * curDest + 125) - from;
        from += delta * evalHermite__Ffffff(cursorTime, 0.0f, 1.0f, 0.5f, 0.5f);
        rectangleDraw__FiiiiiUi(194, static_cast<int>(from), 446,
                                static_cast<int>(from + 23.0f), 6,
                                (static_cast<unsigned>(flareAlpha / 2) << 24) | 0x366876u);
        if (curDest == thisLevel) {
            unsigned short* text = textGetString__FPc("Back");
            usprintf__FPUsT0ffffffff(hudFooterUni, text,
                                     static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f);
        } else {
            unsigned short* text = textGetString__FPc("AcceptBack");
            usprintf__FPUsT0ffffffff(hudFooterUni, text,
                                     static_cast<float>(padAcceptButtonFontChar),
                                     static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f);
        }
        hudShowMsg__FPUsN20iii(textGetString__FPc("JumpGateTitle"), 0, hudFooterUni, flareAlpha,
                               305, 181);
    }
    if (jumping != 0) doJumpEffect__Fi(1);
}
