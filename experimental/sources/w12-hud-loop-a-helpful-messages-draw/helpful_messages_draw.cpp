// Observed helpfulMessagesDraw__Fv at 0x001c1fb0, 968 bytes, 64-byte frame. Two parts, both with file-
// local state that starts at zero: the word messageAlpha, the byte bossInitFlag, the halfword
// lastBossHealth, two guard words and two 16-byte ResourceMap objects.
// Part 1, when the halfword infoMessageTimer is above zero and either hudNoDraw is zero or
// infoMessageAlways is nonzero: alpha = timer * 4 when timer < 32, otherwise messageAlpha + 3; the
// result is stored in messageAlpha and capped at 128 (the cap is stored too). The mode is 7 when
// infoMessageAlways is nonzero, else 6. fontStart(defaultFont, mode, alpha << 24 | 0x1e6464, -1);
// fontPrintCenteredu(320, 78, infoMessage, 1000000); fontEnd(); w = fontStringSizeu(defaultFont,
// infoMessage, 1000000), raised to 300 when smaller; then textureSmoothBorder(300 - w / 2, 76, w + 40,
// 32, mode, (messageAlpha / 2) << 24, 0, 0, 0), with messageAlpha re-read and halves rounded toward
// zero.
// Part 2 returns early when hudNoDraw or interfaceOn is nonzero or the word at gameCam + 52 is not 1.
// bossMeterMax is raised to bossMeterCurrent. With current > 0 and max > 0 the bar is drawn; otherwise
// it is drawn only while bossMeterAlpha > 0, and when that alpha is not above zero bossInitFlag is
// cleared and the routine ends. When current <= 0 the alpha drops by 3 and current is set to 0
// (both stored). fontStart(smallFont, 5, alpha << 24 | 0x464646, -1); fontPrintCenteredu(320, 408,
// textGetString(bossMeterName), 1000000); fontEnd(). The fill width is (int)(current / max * 203.0),
// or * 315.0 for language 3 and 4, with left edge 218 (162 for those languages). With bossInitFlag
// set the fill is rectangleDraw(left, 414, left + width, 429, 5, alpha << 24 | colour) where the
// colour is 0x808080 when current < lastBossHealth and 0x181850 otherwise; with the flag clear
// bossMeterAlpha becomes 78 and the colour word is 0x4e181850. The two ResourceMap objects are built
// on first use from ("hud.lmp", "bossmeter.tex") and ("hud.lmp", "bossmeter_pal.tex"). The texture
// from the pal map for languages 3 and 4 (x 158), else from the other map (x 209), is drawn with
// textureDrawColor(texture, x, 409, 6, 0x80808080). Finally bossInitFlag = 1 and lastBossHealth is
// the low 16 bits of bossMeterCurrent.
// Inferred: a message line with a backing border, then a boss health bar; "language 3 and 4" is a
// byte compare only. Unknown: names of the languages; why the pal texture is used for them.
extern "C" {
extern short infoMessageTimer;
extern signed char infoMessageAlways;
extern unsigned char hudNoDraw;
extern unsigned short infoMessage[];
extern void* defaultFont;
extern void* smallFont;
extern signed char interfaceOn;
extern unsigned char* gameCam;
extern int bossMeterCurrent;
extern int bossMeterMax;
extern int bossMeterAlpha;
extern char bossMeterName[];
extern unsigned char language;
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int limit);
void fontEnd__Fv();
int fontStringSizeu__FP4FontPUsi(void* font, unsigned short* text, int count);
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int w, int h, int mode, unsigned color, int a, int b, int c);
unsigned short* textGetString__FPc(char* key);
void rectangleDraw__FiiiiiUi(int x1, int y1, int x2, int y2, int z, unsigned color);
void __11ResourceMapPcT1(void* map, char* group, char* name);
void* get__11ResourceMap(void* map);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
}

namespace {
int messageAlpha = 0;
signed char bossInitFlag = 0;
short lastBossHealth = 0;
int bossTexGuard = 0;
int bossPalGuard = 0;
alignas(4) unsigned char bossMeterTex[16];
alignas(4) unsigned char bossMeterTexPal[16];
}

extern "C" void helpfulMessagesDraw__Fv() {
    short timer = infoMessageTimer;
    if (timer > 0 && (hudNoDraw == 0 || infoMessageAlways != 0)) {
        int mode = infoMessageAlways != 0 ? 7 : 6;
        int alpha = timer < 32 ? timer * 4 : messageAlpha + 3;
        messageAlpha = alpha;
        if (alpha > 128) {
            alpha = 128;
            messageAlpha = 128;
        }
        fontStart__FP4FontiUii(defaultFont, mode, (static_cast<unsigned>(alpha) << 24) | 0x1e6464u, -1);
        fontPrintCenteredu__FiiPUsi(320, 78, infoMessage, 1000000);
        fontEnd__Fv();
        int width = fontStringSizeu__FP4FontPUsi(defaultFont, infoMessage, 1000000);
        if (width < 300) width = 300;
        int half = messageAlpha;
        half = (half + static_cast<int>(static_cast<unsigned>(half) >> 31)) >> 1;
        textureSmoothBorder__FiiiiiUiiii(300 - ((width + static_cast<int>(static_cast<unsigned>(width) >> 31)) >> 1),
                                         76, width + 40, 32, infoMessageAlways != 0 ? 7 : 6,
                                         static_cast<unsigned>(half) << 24, 0, 0, 0);
    }
    if (hudNoDraw != 0) return;
    if (interfaceOn != 0) return;
    if ((*reinterpret_cast<int*>(gameCam + 52) ^ 1) != 0) return;

    int current = bossMeterCurrent;
    int maximum = bossMeterMax;
    if (maximum < current) maximum = current;
    bossMeterMax = maximum;
    int alpha = bossMeterAlpha;
    if (!(current > 0 && maximum > 0) && alpha <= 0) {
        bossInitFlag = 0;
        return;
    }
    if (current <= 0) {
        alpha -= 3;
        bossMeterCurrent = 0;
        bossMeterAlpha = alpha;
    }
    fontStart__FP4FontiUii(smallFont, 5, (static_cast<unsigned>(alpha) << 24) | 0x464646u, -1);
    fontPrintCenteredu__FiiPUsi(320, 408, textGetString__FPc(bossMeterName), 1000000);
    fontEnd__Fv();

    current = bossMeterCurrent;
    float ratio = static_cast<float>(current) / static_cast<float>(bossMeterMax);
    bool asian = static_cast<unsigned>(language - 3u) < 2u;
    int left = asian ? 162 : 218;
    int fill = static_cast<int>(ratio * (asian ? 315.0f : 203.0f));
    if (bossInitFlag != 0) {
        unsigned colour = current < lastBossHealth ? 0x808080u : 0x181850u;
        alpha = bossMeterAlpha;
        rectangleDraw__FiiiiiUi(left, 414, left + fill, 429, 5, (static_cast<unsigned>(alpha) << 24) | colour);
    } else {
        bossMeterAlpha = 78;
        rectangleDraw__FiiiiiUi(left, 414, left + fill, 429, 5, 0x4e181850u);
    }
    if (bossTexGuard == 0) {
        __11ResourceMapPcT1(bossMeterTex, const_cast<char*>("hud.lmp"), const_cast<char*>("bossmeter.tex"));
        bossTexGuard = 1;
    }
    if (bossPalGuard == 0) {
        __11ResourceMapPcT1(bossMeterTexPal, const_cast<char*>("hud.lmp"), const_cast<char*>("bossmeter_pal.tex"));
        bossPalGuard = 1;
    }
    if (asian)
        textureDrawColor__FP8_textureiiiUi(get__11ResourceMap(bossMeterTexPal), 158, 409, 6, 0x80808080u);
    else
        textureDrawColor__FP8_textureiiiUi(get__11ResourceMap(bossMeterTex), 209, 409, 6, 0x80808080u);
    bossInitFlag = 1;
    lastBossHealth = static_cast<short>(static_cast<unsigned short>(bossMeterCurrent));
}
