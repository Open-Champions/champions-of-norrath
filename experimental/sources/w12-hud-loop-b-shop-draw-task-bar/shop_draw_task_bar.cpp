// Observed shopDrawTaskBar__FP12screenStruct at 0x001b80a8, 592 bytes: one argument (a screen record
// of ints; word 0 is its x position and word 20, at +80, is the selected tab), no result.
// A pulsing alpha is made as (int)(isin(drawFrameNm << 10) * 32.0 + 132.0) shifted left by 24 and
// or-ed with 0x808080. All draws use textureDrawColor__FP8_textureiiiUi(texture, x, y, 0, colour).
//  - With shopRunning nonzero: the glow texture named glowNames[tab] in lump "shop.lmp" is drawn at
//    (x + 97 / 142 / 188 for tab 0 / 1 / 2, 59) with the pulsing colour; then the texture named
//    "tabweapons.tex" / "tabarmor.tex" / "tabmagic.tex" for the tab at (x, 48) with 0x80808080;
//    then the texture invHeader at (x, 0) with 0x80808080. The glow names are
//    "tabweaponglow.tex", "tabarmorglow.tex", "tabmagicglow.tex". The first lookup happens before
//    isin is called.
//  - Otherwise: isin is called first, taskBar[tab + 6] is drawn at (x + offset[tab], 59) with the
//    pulsing colour, with offsets 27, 72, 118, 165, 211, 256, then taskBar[tab] at (x, 48) with
//    0x80808080.
// Inferred: this draws the shop tab strip, tab 0..2 for the three item categories when the shop is
// running, and a taskBar strip of six tabs otherwise; the glow marks the selected tab.
// Unknown: the meaning of the fourth texture argument (0 here) and what screen words other than 0
// and 20 hold.
extern "C" {
extern int shopRunning;
extern int drawFrameNm;
extern void* invHeader;
extern void* taskBar[12];
float isin__Fi(int angle);
void* lumpFindResource__FPcT0(char* lump, char* name);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);

void shopDrawTaskBar__FP12screenStruct(int* screen) {
    static const char* const names[6] = {"tabweapons.tex", "tabarmor.tex", "tabmagic.tex",
                                         "tabweaponglow.tex", "tabarmorglow.tex", "tabmagicglow.tex"};
    static const int shopOffsets[3] = {97, 142, 188};
    static const int stripOffsets[6] = {27, 72, 118, 165, 211, 256};
    if (shopRunning != 0) {
        void* glow = lumpFindResource__FPcT0(const_cast<char*>("shop.lmp"), const_cast<char*>(names[3 + screen[20]]));
        const unsigned colour = static_cast<unsigned>(static_cast<int>(isin__Fi(drawFrameNm << 10) * 32.0f + 132.0f)) << 24 | 0x808080u;
        textureDrawColor__FP8_textureiiiUi(glow, screen[0] + shopOffsets[screen[20]], 59, 0, colour);
        void* tab = lumpFindResource__FPcT0(const_cast<char*>("shop.lmp"), const_cast<char*>(names[screen[20]]));
        textureDrawColor__FP8_textureiiiUi(tab, screen[0], 48, 0, 0x80808080u);
        textureDrawColor__FP8_textureiiiUi(invHeader, screen[0], 0, 0, 0x80808080u);
        return;
    }
    const unsigned colour = static_cast<unsigned>(static_cast<int>(isin__Fi(drawFrameNm << 10) * 32.0f + 132.0f)) << 24 | 0x808080u;
    textureDrawColor__FP8_textureiiiUi(taskBar[6 + screen[20]], screen[0] + stripOffsets[screen[20]], 59, 0, colour);
    textureDrawColor__FP8_textureiiiUi(taskBar[screen[20]], screen[0], 48, 0, 0x80808080u);
}
}
