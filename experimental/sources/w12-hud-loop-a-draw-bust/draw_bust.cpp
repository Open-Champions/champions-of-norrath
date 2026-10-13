// Observed drawBust__Fv (local, not exported) at 0x001c6da8, 1412 bytes, 368-byte frame. Nothing happens when
// the byte drawBustOn is zero. Otherwise:
// 1. bustAlpha moves by 0.08444952 (bits 0x3dacf3df, both constants) down when the byte hudBustCovered is
//    nonzero and up otherwise, and is limited to 0..1 and stored; hudBustCovered is cleared and drawBustCur
//    is raised to zero if negative.
// 2. Position (x, y) = (349, 22) for nmPlayers 3, (267, 23) for fewer, (267, 75) for more.
// 3. fontStart(smallFont, 6, alpha110 << 24 | 0x395f6b, -1), alpha110 = (unsigned)(bustAlpha * 110). When
//    drawBustMax is -1: fontPrintu(x + 24, y + 24, drawBustTitle, 1000000); fontColor(alpha128 << 24 |
//    0x395f6b, -1, 0) with alpha128 = (unsigned)(bustAlpha * 128); sprintf(buffer, "%d", drawBustCur);
//    fontPrintCentered(x + 43, y + 2, buffer, 10000). Otherwise fontPrintu(x + 28, y + 20, drawBustTitle,
//    1000000). Then fontEnd().
// 4. Resources from group "bust.lmp": frameTex = "frame.tex", nameTex = the text in drawBustTexName. With
//    drawBustMax not -1: meterTex = "fieldmeter.tex"; the name "bar_yellow0%d.tex" is formatted with
//    (frameNm / 2) % 10 and looked up as barTex; the fill width is the nearest integer (halves away
//    from zero) of drawBustCur / drawBustMax * width of barTex (halfword at +0). textureDrawSuperColor(barTex,
//    x + 22, y + 8, 6, alpha128 << 24 | 0x808080, x + 22, y + 8, x + 22 + fill - 2, y + 8 + height of barTex
//    (halfword at +2), 0, 0, 0); textureDrawColor(meterTex, x + 12, y + 2, 6, same colour). With
//    drawBustMax -1: textureDrawColor(lookup "fieldnumber.tex", x + 11, y + 1, 6, same colour).
// 5. textureDrawColor(nameTex, x - width(nameTex) / 2, y + height(frameTex) - height(nameTex) - 3, 6,
//    colour), then textureDrawColor(frameTex, x - width(frameTex) / 2, y, 6, colour), with halves
//    taken signed and rounded toward zero, widths and heights from the halfwords at +0 and +2 of each
//    resource (the first read unsigned-then-signed, as the original does).
// Inferred: the character bust frame at a corner of the screen with a health-like meter. Unknown: what
// the lookups return beyond the two halfwords used, what drawBustMax counts, and the texture names set in
// drawBustTexName at run time. Of the two lookups made before the meter, the first ("frame.tex") is kept as
// frameTex and the second (drawBustTexName) as nameTex, as the saved registers show.
extern "C" {
extern signed char drawBustOn;
extern signed char hudBustCovered;
extern float bustAlpha;
extern int drawBustCur;
extern int drawBustMax;
extern int nmPlayers;
extern int frameNm;
extern void* smallFont;
extern unsigned short drawBustTitle[];
extern char drawBustTexName[];
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
void fontPrintu__FiiPUsi(int x, int y, unsigned short* text, int limit);
void fontColor__FUiii(unsigned color, int a, int b);
int sprintf(char* buffer, const char* format, ...);
void fontPrintCentered__FiiPci(int x, int y, char* text, int limit);
void fontEnd__Fv();
unsigned char* lumpFindResource__FPcT0(const char* group, const char* name);
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* texture, int x, int y, int z, unsigned color, int a, int b,
                                                   int c, int d, char e, char f);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
}

namespace {
float float_from_bits(unsigned bits) {
    union {
        unsigned word;
        float value;
    } u;
    u.word = bits;
    return u.value;
}

int half_at(const unsigned char* address) { return static_cast<short>(address[0] | (address[1] << 8)); }
unsigned half_unsigned_at(const unsigned char* address) { return static_cast<unsigned>(address[0] | (address[1] << 8)); }
}

extern "C" void drawBust__Fv() {
    if (drawBustOn == 0) return;
    const float step = float_from_bits(0x3dacf3dfu);
    float alpha = bustAlpha;
    if (hudBustCovered != 0) alpha = alpha - step;
    else alpha = alpha + step;
    if (1.0f < alpha) {
        alpha = 1.0f;
        bustAlpha = alpha;
    } else {
        bustAlpha = alpha;
    }
    if (alpha < 0.0f) {
        alpha = 0.0f;
        bustAlpha = alpha;
    }
    int current = drawBustCur;
    hudBustCovered = 0;
    if (current < 0) current = 0;
    drawBustCur = current;

    int x, y;
    if (nmPlayers == 3) {
        x = 349;
        y = 22;
    } else if (nmPlayers < 3) {
        x = 267;
        y = 23;
    } else {
        x = 267;
        y = 75;
    }
    fontStart__FP4FontiUii(smallFont, 6, (static_cast<unsigned>(alpha * 110.0f) << 24) | 0x395f6bu, -1);
    char buffer[256];
    if (drawBustMax == -1) {
        fontPrintu__FiiPUsi(x + 24, y + 24, drawBustTitle, 1000000);
        fontColor__FUiii((static_cast<unsigned>(bustAlpha * 128.0f) << 24) | 0x395f6bu, -1, 0);
        sprintf(buffer, "%d", drawBustCur);
        fontPrintCentered__FiiPci(x + 43, y + 2, buffer, 10000);
    } else {
        fontPrintu__FiiPUsi(x + 28, y + 20, drawBustTitle, 1000000);
    }
    fontEnd__Fv();

    const char* const group = "bust.lmp";
    unsigned char* frameTex = lumpFindResource__FPcT0(group, "frame.tex");
    unsigned char* nameTex = lumpFindResource__FPcT0(group, drawBustTexName);
    if (drawBustMax != -1) {
        unsigned char* meterTex = lumpFindResource__FPcT0(group, "fieldmeter.tex");
        sprintf(buffer, "bar_yellow0%d.tex", (frameNm / 2) % 10);
        unsigned char* barTex = lumpFindResource__FPcT0(group, buffer);
        const float ratio = static_cast<float>(drawBustCur) / static_cast<float>(drawBustMax);
        const float scaled = ratio * static_cast<float>(half_at(barTex));
        const int fill = static_cast<int>(0.0f <= scaled ? scaled + 0.5f : scaled - 0.5f);
        const unsigned color = (static_cast<unsigned>(bustAlpha * 128.0f) << 24) | 0x808080u;
        const int barX = x + 12 + 10;
        const int barY = y + 2 + 6;
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(barTex, barX, barY, 6, color, barX, barY, barX + fill - 2,
                                                      barY + half_at(barTex + 2), 0, 0);
        textureDrawColor__FP8_textureiiiUi(meterTex, x + 12, y + 2, 6,
                                           (static_cast<unsigned>(bustAlpha * 128.0f) << 24) | 0x808080u);
    } else {
        unsigned char* numberTex = lumpFindResource__FPcT0(group, "fieldnumber.tex");
        textureDrawColor__FP8_textureiiiUi(numberTex, x + 11, y + 1, 6,
                                           (static_cast<unsigned>(bustAlpha * 128.0f) << 24) | 0x808080u);
    }
    {
        const int signedWidth = static_cast<short>(half_unsigned_at(nameTex));
        const int heightFrame = half_at(frameTex + 2);
        const int heightName = half_at(nameTex + 2);
        const unsigned color = (static_cast<unsigned>(bustAlpha * 128.0f) << 24) | 0x808080u;
        textureDrawColor__FP8_textureiiiUi(nameTex, x - signedWidth / 2, y + heightFrame - heightName - 3, 6, color);
    }
    {
        const int signedWidth = static_cast<short>(half_unsigned_at(frameTex));
        textureDrawColor__FP8_textureiiiUi(frameTex, x - signedWidth / 2, y, 6,
                                           (static_cast<unsigned>(bustAlpha * 128.0f) << 24) | 0x808080u);
    }
}
