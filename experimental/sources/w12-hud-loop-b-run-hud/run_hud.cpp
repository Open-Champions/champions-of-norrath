// Observed runHUD__Fi at 0x001c2480, 3692 bytes: one argument, the player number pn. record is
// player + pn * 23984; obj is the pointer word at record + 21872. The function draws one player's
// status panel and does nothing when gameNoDraw, hudNoDraw (signed byte) or shopRunning is nonzero,
// when the word at gameCam + 52 is not 1, when netPlayerIsConnected__Fi(pn) is zero, when obj is
// null or its word at +1328 is nonzero, or when fewer than 3000 display-list entries are free
// (GLISTSIZE - ((glistp - gstart) >> 3)). It loads lump "hud.lmp" with lumpLoad__FPc first.
//  x0 is {12, 478, 167, 323}[pn] + 7 and x is x0 + 8. gameFlags[pn] bit 0 makes the bar alpha 255
//  (and is cleared); otherwise it is 128.
//  1. Texture "bar_red%02d.tex" ((frameNm >> 2) % 11) from the lump is drawn with
//     textureDrawSuperColor__FP8_textureiiiUiiiiicc as a bar of x + record float +172 / +176 * 99.
//  2. With record float +180 above zero a translucent rectangle (colour 0x80000060, y 47..51) extends
//     the bar by the same fraction of (+172 + +180), clipped at 180 / 605 / 333 / 447 for player
//     0 / 1 / 2 / other. With float +204 above zero the same is drawn for +196 / +200 at y 66..70
//     (colour 0x805a4632) starting at x.
//  3. With signed byte +88 (level) above zero: "bar_green%02d.tex", an experience bar from
//     experienceTable[level - 1] to experienceTable[level] using the word at +92.
//  4. "bar_blue%02d.tex" (alpha 42 when the halfword at obj + 1182 is positive, else 128, colour
//     0xacacac) drawn as +196 / +200 of the width.
//  5. The name (record text, "%s") is cut to fit 120 pixels (two strlen calls per cut, minimum six
//     characters) and "..." is added when cut; in colour playerGetMarkerColor(pn, 128) with two or
//     more players, halved while commNameMask has bit pn and bit 5 of frameNm is set; level text.
//  6. "hud_statusbar00.tex" is the frame; when the level is below the maximum byte (+89) a glow
//     "hud_statusbar00glow.tex" fades in (levelUpFade[pn] += 4, at most 128).
//  7. Seven icon slots (fadeTimer / fadeIconY / iconAlpha, seven per player): while a slot's timer is
//     nonzero and the player is local its icon is drawn from globalIcons; slots 4 to 6 also show a
//     "x %d" count (spell, health potions, mana potions). Each pass lowers the timer by one and
//     clears fadeIconY[slot] when it reaches zero. Then drawNewestQuickSpellMenu__Fiiicl(pn, 0, 0, 0, 0).
// Inferred: this is the in-game HUD for one player slot; "hud.lmp" holds the bar and frame textures.
// Unknown: the meaning of the record floats and of the slot table.
extern "C" {
extern int gameNoDraw;
extern signed char hudNoDraw;
extern int shopRunning;
extern void* gameCam;
extern int glistp;
extern int gstart;
extern int GLISTSIZE;
extern int frameNm;
extern int nmPlayers;
extern int commNameMask;
extern void* smallFont;
extern unsigned char player[];
extern unsigned gameFlags[];
extern int experienceTable[];
extern int levelUpFade[];
extern int iconAlpha[];
extern int fadeIconY[];
extern int fadeTimer[];
extern int healthPotionsLeft[];
extern int manaPotionsLeft[];
extern void* globalIcons[];
extern void* crystalTex[];
int netPlayerIsConnected__Fi(int player);
int netPlayerIsLocal__Fi(int player);
void* lumpLoad__FPc(const char* name);
void* lumpFind__FPvPc(void* lump, const char* name);
void* lumpFindResource__FPcT0(char* lump, char* name);
int sprintf(char* out, const char* format, ...);
unsigned long strlen(const char* text);
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* texture, int x, int y, int mode, unsigned color, int a, int b, int c, int d, char e, char f);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
void rectangleDraw__FiiiiiUi(int x1, int y1, int x2, int y2, int mode, unsigned color);
float isin__Fi(int angle);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
void fontEnd__Fv();
void fontColor__FUiii(unsigned color, int a, int b);
int fontStringSize__FP4FontPci(void* font, char* text, int limit);
void fontPrintCentered__FiiPci(int x, int y, char* text, int limit);
void fontPrint__FiiPci(int x, int y, char* text, int limit);
unsigned playerGetMarkerColor__Fii(int player, int alpha);
void __11ResourceMapPcT1(void* self, char* lump, char* name);
void drawNewestQuickSpellMenu__Fiiicl(int player, int a, int b, char c, long d);
}

#ifdef W12_HUD_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
// The 32-bit pointer words of the player record (the object word at +21872, the spell word at +21928)
// widen to native pointers through this on the host check; on the EE they already are pointers.
inline const unsigned char* pointerAt(const unsigned char* base, int offset) {
    unsigned word;
    __builtin_memcpy(&word, base + offset, 4);
#ifdef W12_HUD_HOST
    return reinterpret_cast<const unsigned char*>(hostWordAddress(word));
#else
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
template <class T>
inline T load(const unsigned char* base, int offset) {
    T value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
alignas(16) unsigned char levelUpIcon[16];
int levelUpIconReady;

// Right edge limit of the translucent extension, by player slot.
inline int clipRight(int slot, int x) {
    if (slot == 0) return x < 181 ? x : 180;
    if (slot == 1) return x < 606 ? x : 605;
    if (slot == 2) return x < 334 ? x : 333;
    return x < 448 ? x : 447;
}
inline unsigned grey(int level) { return static_cast<unsigned>(level) | static_cast<unsigned>(level) << 8 | static_cast<unsigned>(level) << 16; }
}

extern "C" void runHUD__Fi(int pn) {
    if (gameNoDraw != 0) return;
    if (hudNoDraw != 0) return;
    if (shopRunning != 0) return;
    if ((load<int>(static_cast<const unsigned char*>(gameCam), 52) ^ 1) != 0) return;
    if (netPlayerIsConnected__Fi(pn) == 0) return;
    void* lump = lumpLoad__FPc("hud.lmp");
    static const int barX[4] = {12, 478, 167, 323};
    const int x0 = barX[pn] + 7;
    const unsigned char* record = player + pn * 23984;
    const unsigned char* obj = pointerAt(record, 21872);
    if (obj == nullptr) return;
    if (load<int>(obj, 1328) != 0) return;
    if (GLISTSIZE - ((glistp - gstart) >> 3) < 3000) return;
    int barAlpha = 128;
    if ((gameFlags[pn] & 1u) != 0) {
        barAlpha = 255;
        gameFlags[pn] = gameFlags[pn] & ~1u;
    }
    char text[128];
    const int x = x0 + 8;
    sprintf(text, "bar_red%02d.tex", (frameNm >> 2) % 11);
    const float k99 = 99.0f;
    void* entry = lumpFind__FPvPc(lump, text);
    {
        const float current = load<float>(record, 172);
        const float maximum = load<float>(record, 176);
        const unsigned color = static_cast<unsigned>(barAlpha) | static_cast<unsigned>(barAlpha) << 16 | static_cast<unsigned>(barAlpha) << 8 | 0x80000000u;
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(load<void*>(static_cast<const unsigned char*>(entry), 4), x, 46, 6, color, x, 24,
                                                      static_cast<int>(static_cast<float>(x) + current / maximum * k99), 124, 0, 0);
    }
    if (0.0f < load<float>(record, 180)) {
        const float current = load<float>(record, 172);
        const float maximum = load<float>(record, 176);
        const float extra = load<float>(record, 180);
        const float sum = current + extra;
        const float fraction = current / maximum;
        float rounded = static_cast<float>(static_cast<int>(sum));
        const int front = static_cast<int>(fraction * k99);
        if (maximum < rounded) rounded = static_cast<float>(static_cast<int>(maximum));
        const int back = static_cast<int>(rounded / maximum * k99);
        rectangleDraw__FiiiiiUi(x + front, 47, clipRight(pn, x0 + (back - front) + (front + 8)), 51, 6, 0x80000060u);
    }
    if (0.0f < load<float>(record, 204)) {
        const float current = load<float>(record, 196);
        const float maximum = load<float>(record, 200);
        const float extra = load<float>(record, 204);
        const float sum = current + extra;
        const float fraction = current / maximum;
        float rounded = static_cast<float>(static_cast<int>(sum));
        const int front = static_cast<int>(fraction * k99);
        if (maximum < rounded) rounded = static_cast<float>(static_cast<int>(maximum));
        const int back = static_cast<int>(rounded / maximum * k99);
        rectangleDraw__FiiiiiUi(x + front, 66, clipRight(pn, x0 + (back - front) + (front + 8)), 70, 6, 0x805a4632u);
    }
    const signed char level = static_cast<signed char>(record[88]);
    if (level > 0) {
        char green[128];
        sprintf(green, "bar_green%02d.tex", (frameNm >> 2) % 11);
        const int upper = experienceTable[level];
        const int lower = experienceTable[level - 1];
        void* greenEntry = lumpFind__FPvPc(lump, green);
        const float span = static_cast<float>(upper - lower);
        const float done = static_cast<float>(load<int>(record, 92) - lower);
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(load<void*>(static_cast<const unsigned char*>(greenEntry), 4), x, 56, 6, 0x80808080u, x, 24,
                                                      static_cast<int>(static_cast<float>(x) + done / span * k99), 124, 0, 0);
    }
    char blue[128];
    sprintf(blue, "bar_blue%02d.tex", (frameNm >> 2) % 11);
    int blueAlpha = 128;
    if (load<short>(obj, 1182) > 0) blueAlpha = 42;
    void* blueEntry = lumpFind__FPvPc(lump, blue);
    textureDrawSuperColor__FP8_textureiiiUiiiiicc(load<void*>(static_cast<const unsigned char*>(blueEntry), 4), x, 63, 6,
                                                  static_cast<unsigned>(blueAlpha) << 24 | 0xacacacu, x, 24,
                                                  static_cast<int>(static_cast<float>(x) + load<float>(record, 196) / load<float>(record, 200) * k99), 124, 0, 0);
    const int pulse = static_cast<int>(isin__Fi(frameNm << 10) * 25.0f);
    fontStart__FP4FontiUii(smallFont, 5, 0x80323c3cu, -1);
    const int half = (pulse + static_cast<int>(static_cast<unsigned>(pulse) >> 31)) >> 1;
    const unsigned pulseColor = 0x80000000u | static_cast<unsigned>(half + 50) << 16 | static_cast<unsigned>(half + 60) << 8 | static_cast<unsigned>(half + 60);
    const bool growing = static_cast<signed char>(record[88]) < static_cast<signed char>(record[89]);
    if (growing) fontColor__FUiii(pulseColor, -1, 0);
    char name[128];
    sprintf(name, "%s", reinterpret_cast<const char*>(record));
    for (;;) {
        if (fontStringSize__FP4FontPci(smallFont, name, 100000) < 120) break;
        if (strlen(name) < 6) break;
        name[strlen(name) - 1] = 0;
    }
    if (strlen(name) != strlen(reinterpret_cast<const char*>(record))) __builtin_memcpy(name + strlen(name), "...", 4);
    unsigned nameColor = 0x80323c3cu;
    if (nmPlayers >= 2) nameColor = playerGetMarkerColor__Fii(pn, 128);
    if (((commNameMask >> pn) & 1) != 0 && ((frameNm + (frameNm < 0 ? 31 : 0)) >> 5 & 1) != 0)
        nameColor = (nameColor >> 24 << 24) | (((nameColor >> 16 & 0xffu) >> 1) << 16) | (nameColor >> 1 & 0x7f00u) | ((nameColor & 0xffu) >> 1);
    fontColor__FUiii(nameColor, -1, 0);
    fontPrintCentered__FiiPci(x0 + 66, 8, name, 10000);
    fontEnd__Fv();
    char status[128];
    sprintf(status, "hud_statusbar00.tex", 0);
    fontStart__FP4FontiUii(smallFont, 6, 0x80323c3cu, -1);
    if (static_cast<signed char>(record[88]) < static_cast<signed char>(record[89])) fontColor__FUiii(pulseColor, -1, 0);
    sprintf(name, "%d", static_cast<int>(static_cast<signed char>(record[89])));
    fontPrintCentered__FiiPci(x0 + 122, 43, name, 10000);
    fontEnd__Fv();
    void* statusEntry = lumpFind__FPvPc(lump, status);
    void* statusTexture = load<void*>(static_cast<const unsigned char*>(statusEntry), 4);
    const unsigned char* again = pointerAt(record, 21872);
    if (again != nullptr) {
        if ((load<unsigned>(again, 12) & 8u) == 0 && 0.0f < load<float>(record, 172) && static_cast<signed char>(record[88]) < static_cast<signed char>(record[89])) {
            levelUpFade[pn] = levelUpFade[pn] + 4;
            if (levelUpFade[pn] >= 128) levelUpFade[pn] = 128;
            if (levelUpIconReady == 0) {
                __11ResourceMapPcT1(levelUpIcon, const_cast<char*>("icons.lmp"), const_cast<char*>("levelup.tex"));
                levelUpIconReady = 1;
            }
            void* glow = lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"), const_cast<char*>("hud_statusbar00glow.tex"));
            textureDrawSuperColor__FP8_textureiiiUiiiiicc(glow, x0 - 3, 26, 6, static_cast<unsigned>(levelUpFade[pn]) << 24 | grey(pulse + 103), 0, 0, 1000,
                                                          1000, 0, 0);
        }
    }
    if (nmPlayers >= 2) {
        void* crystal = crystalTex[pn];
        const int a = static_cast<short>(load<unsigned short>(static_cast<const unsigned char*>(statusTexture), 0)) >> 1;
        const int b = static_cast<short>(load<unsigned short>(static_cast<const unsigned char*>(crystal), 0)) >> 1;
        textureDrawColor__FP8_textureiiiUi(crystal, x0 + a - b - 16, 29, 6, 0x80808080u);
    }
    textureDrawSuperColor__FP8_textureiiiUiiiiicc(statusTexture, x0 - 3, 26, 6, 0x80808080u, 0, 0, 1000, 1000, 0, 0);
    // Seven icon slots.
    const int pulse2 = static_cast<int>(isin__Fi(frameNm << 10) * 25.0f);
    static const int iconX[4] = {0, 477, 162, 321};
    const unsigned iconGrey = grey(pulse2 + 103);
    const int xs = iconX[pn] + 37;
    for (int slot = 0; slot < 7; ++slot) {
        int& timer = fadeTimer[pn * 7 + slot];
        int iconRow = fadeIconY[pn * 7 + slot];
        if (timer != 0 && netPlayerIsLocal__Fi(pn) != 0) {
            int icon = 0, offsetX = 0;
            switch (slot) {
                case 0: icon = 33; iconRow = 1; offsetX = 51; break;
                case 1: icon = 31; iconRow = 0; offsetX = -17; break;
                case 2: icon = 32; iconRow = 1; offsetX = -17; break;
                case 3: icon = 30; iconRow = 0; offsetX = 51; break;
                case 4: icon = 34; iconRow = 0; offsetX = -17; break;
                case 5: icon = 35; iconRow = 1; offsetX = -17; break;
                case 6: icon = 36; iconRow = 0; offsetX = 51; break;
            }
            if (slot == 6 || slot == 4 || slot == 5) {
                int& alpha = iconAlpha[pn * 7 - 3 + slot];
                if (81 < timer) alpha += 4;
                if (timer < 38) alpha -= 4;
                if (alpha < 0) alpha = 0;
                else if (alpha >= 129) alpha = 128;
                fontStart__FP4FontiUii(smallFont, 4, static_cast<unsigned>(alpha) << 24 | 0x4080e4u, -1);
                int count = 0;
                if (slot == 6) {
                    const unsigned char* spell = pointerAt(player + pn * 23984, 21920 + 8);
                    count = spell != nullptr ? load<short>(spell, 4) : 0;
                } else if (slot == 4) {
                    count = healthPotionsLeft[pn];
                } else if (slot == 5) {
                    count = manaPotionsLeft[pn];
                }
                if (count >= 0) {
                    char counter[64];
                    sprintf(counter, "x %d", count);
                    fontPrint__FiiPci(xs + offsetX + 31, 81 + iconRow * 27, counter, 100000);
                    fontEnd__Fv();
                    textureDrawColor__FP8_textureiiiUi(globalIcons[icon], xs + offsetX, 82 + iconRow * 27, 4, static_cast<unsigned>(alpha) << 24 | iconGrey);
                }
            } else {
                textureDrawColor__FP8_textureiiiUi(globalIcons[icon], xs + offsetX, 82 + iconRow * 27, 4, iconGrey | 0x80000000u);
            }
            timer = timer - 1;
            if (timer <= 0) fadeIconY[pn * 7 + slot] = 0;
        }
    }
    drawNewestQuickSpellMenu__Fiiicl(pn, 0, 0, 0, 0);
}
