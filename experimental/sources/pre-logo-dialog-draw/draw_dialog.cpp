// Observed drawDialog__Fv at 0x1d2f28, 1448 bytes, pinned SLUS_205.65.
// It returns with no drawing while curDlg is zero or a swoop runs. Otherwise
// a fade word rises once through flaredIn, and an alpha byte comes from the
// fade value scaled by 128, or from one minus that value while flaredOut is
// set; that byte tints the five text colors. The body prints through
// textBody with letterAtATime selecting the restrict count, then options
// print only while textPageDone is set, and only when DLG_ShowDialogText is
// set, when stream 2 is idle without a sound wait, or when the stream wait
// is cancelled. An empty option list prints the "Affirmation" string.
// Otherwise each option prints its string at curDlg+84*opt+136 with its line
// position, highlighting the selected index. A nonzero word at curDlg+84
// moves the cursor between the two line positions by the Hermite value of
// the tween, else row 398 is used. The "%c" cursor glyph, the "dialogMore"
// prompt, the speaker name, and the "(%s)" remote-player line follow. The
// name reveal decrements nameChars by the 19.6-scaled count clamped at zero
// and truncates the printed name while flaredOut is set. Float-to-int
// conversions are plain C++ casts; the reviewed EE compiler emits cvt.w.s
// for those, matching the observed conversion instruction. Inferred types
// cover accessed fields only; font, texture, sound, and hardware behavior
// are untested.
extern "C" {
extern unsigned long curDlg;
extern int swooping, flaredIn, flaredOut;
extern float coreInterfaceFadeUpVal;
extern void* mediumFont;
extern int letterAtATime, textBodyChars;
extern unsigned short textBody[];
extern int scrollY;
extern unsigned DLG_ShowDialogText;
extern int textPageDone, showOptions, showMore, optionCount;
extern signed char waitForSound;
extern int cancelStreamWait;
extern int optionIndex, nextIndex;
extern unsigned optTbl[];
extern int optionLinePos[];
extern float cursorTween;
extern int nameChars;
extern unsigned short dlgName[];
extern int activePlayer;
extern unsigned char player[];
extern void* haloTexture7;
extern int padAcceptButtonFontChar;
extern int loadSampleRate asm("loadSampleRate.784");
int coreInterfaceFadeUp__Fi(int fade);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
void fontRestrict__Fi(int count);
int lineToY__Fi(int line);
void fontScissor__Fiiii(int x0, int y0, int x1, int y1);
int fontPrintWrapu__FiiPUsiiii(int x, int y, unsigned short* text, int width, int low, int high, int step);
void fontColor__FUiii(unsigned color, int flags, int extra);
int soundGetStreamPlayStatus__Fi(int stream);
unsigned short* textGetString__FPc(char* key);
int fontPrintu__FiiPUsi(int x, int y, unsigned short* text, int flags);
float evalHermite__Ffffff(float t, float p0, float p1, float m0, float m1);
int sprintf(char* dest, const char* format, ...);
int fontPrint__FiiPci(int x, int y, char* text, int flags);
void fontEnd__Fv();
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int width, unsigned color);
void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src);
int fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int flags);
int netPlayerIsLocal__Fi(int index);
int fontPrintCentered__FiiPci(int x, int y, char* text, int flags);
}
extern "C" void drawDialog__Fv() {
    if (curDlg == 0ul) return;
    if (swooping != 0) return;
    if (flaredIn == 0) {
        coreInterfaceFadeUp__Fi(0);
        flaredIn = 1;
    }
    float fade = flaredOut != 0 ? 1.0f - coreInterfaceFadeUpVal : coreInterfaceFadeUpVal;
    int alpha = static_cast<int>(fade * 128.0f) << 24;
    unsigned bright = static_cast<unsigned>(alpha) | 0x3a5262u;
    unsigned dim = static_cast<unsigned>(alpha) | 0x806464u;
    unsigned faint = static_cast<unsigned>(alpha) | 0x606060u;
    unsigned option = static_cast<unsigned>(alpha) | 0x36775au;
    unsigned plain = static_cast<unsigned>(alpha) | 0x404040u;
    fontStart__FP4FontiUii(mediumFont, 6, bright, -1);
    if (letterAtATime != 0) fontRestrict__Fi(textBodyChars);
    int top = lineToY__Fi(1);
    int bottom = lineToY__Fi(6);
    fontScissor__Fiiii(0, top + 6, 640, bottom);
    fontPrintWrapu__FiiPUsiiii(56, top - scrollY, textBody, 524, 295,
                               DLG_ShowDialogText != 0u ? 385 : 295, 18);
    fontRestrict__Fi(-1);
    fontScissor__Fiiii(0, 0, 640, 448);
    if (textPageDone != 0) {
        if (showOptions != 0) {
            bool show = false;
            if (DLG_ShowDialogText != 0u) {
                show = true;
            } else if (waitForSound != 0) {
                show = cancelStreamWait != 0;
            } else {
                show = soundGetStreamPlayStatus__Fi(2) == 0 || cancelStreamWait != 0;
            }
            if (show) {
                if (optionCount == 0) {
                    fontColor__FUiii(option, -1, 0);
                    fontPrintu__FiiPUsi(86, lineToY__Fi(6),
                                        textGetString__FPc(const_cast<char*>("Affirmation")), 0xf4240);
                } else if (optionCount > 0) {
                    for (int i = 0; i < optionCount; ++i) {
                        unsigned color = (i == optionIndex) ? option : plain;
                        fontColor__FUiii(color, -1, 0);
                        unsigned long record = curDlg + optTbl[i] * 84u;
                        fontPrintWrapu__FiiPUsiiii(86, optionLinePos[i],
                                                   textGetString__FPc(reinterpret_cast<char*>(record + 88u)),
                                                   524, -100, 580, 18);
                    }
                }
                fontColor__FUiii(option, -1, 0);
                int cursorY;
                if (*reinterpret_cast<int*>(curDlg + 84u) == 0) {
                    cursorY = 398;
                } else {
                    float blend = evalHermite__Ffffff(cursorTween, 0.0f, 1.0f, -1.0f, -1.0f);
                    float from = static_cast<float>(optionLinePos[optionIndex]);
                    float span = static_cast<float>(optionLinePos[nextIndex]) - from;
                    int stepped = static_cast<int>(from + span * blend);
                    cursorY = static_cast<int>(static_cast<float>(stepped) + 0.5f);
                }
                fontColor__FUiii(faint, -1, 0);
                char glyph[64];
                sprintf(glyph, "%c", padAcceptButtonFontChar);
                fontPrint__FiiPci(56, cursorY, glyph, 0x186a0);
                loadSampleRate = 1;
            }
        }
        if (showMore != 0) {
            fontColor__FUiii(faint, -1, 0);
            char glyph[64];
            sprintf(glyph, "%c", padAcceptButtonFontChar);
            fontPrint__FiiPci(51, 398, glyph, 0x186a0);
            fontColor__FUiii(option, -1, 0);
            fontPrintu__FiiPUsi(81, 398, textGetString__FPc(const_cast<char*>("dialogMore")), 0xf4240);
        }
    }
    fontEnd__Fv();
    textureDrawColor__FP8_textureiiiUi(haloTexture7, 35, 280, 5,
                                       static_cast<unsigned>(alpha) | 0x808080u);
    if (DLG_ShowDialogText == 0u) return;
    unsigned short name[256];
    ustrcpy__FPUsT0(name, dlgName);
    unsigned short* nameEnd = name + nameChars;
    if (flaredOut != 0) *nameEnd = 0;
    if (nameChars != 0) {
        int full = static_cast<int>(static_cast<float>(nameChars) * 0x1.399998p+4f);
        int voted = full < 1 ? full : 1;
        nameChars -= voted;
    }
    if (nameChars < 0) nameChars = 0;
    fontStart__FP4FontiUii(mediumFont, 6, dim, -1);
    fontPrintCenteredu__FiiPUsi(174, 285, name, 0xf4240);
    fontEnd__Fv();
    if (netPlayerIsLocal__Fi(activePlayer) != 0) return;
    fontStart__FP4FontiUii(mediumFont, 6, dim, -1);
    char who[256];
    sprintf(who, "(%s)", reinterpret_cast<char*>(player + activePlayer * 23984));
    fontPrintCentered__FiiPci(176, 256, who, 10000);
    fontEnd__Fv();
}
