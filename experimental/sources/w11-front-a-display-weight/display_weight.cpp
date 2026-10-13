// Observed: displayWeight__FP12playerStructP12screenStruct at 0x001b33d0 is 304 bytes, arguments
// player (a player record) and screen (an inventory screen record), no return value. With cur the
// float at player + 308 and max the float at player + 312, and x, y the ints at screen + 0 and
// screen + 4:
//  1. n = (int)(cur / max * 100.0f) / 14 (signed division), limited above to 6 (a smaller or
//     negative n is used as it is); textureDrawColor__FP8_textureiiiUi(scaleTex[n], x + 8, y + 4,
//     1, 0x80808080).
//  2. fontStart__FP4FontiUii(smallFont, 1, 0x80323c3c, -1).
//  3. sprintf(text, "%.1f/%d", (double)cur, (int)max) with the double made by fptodp; text is a
//     local buffer.
//  4. w = fontStringSize__FP4FontPci(smallFont, text, 100000); fontPrint__FiiPci(x - w + 127,
//     y + 17, text, 100000); fontEnd__Fv().
// Inferred: cur and max are the carried and the greatest carriable weight (the text reads like
// "12.5/80"), and scaleTex holds seven bar images chosen by the percentage in steps of 14.
// Unknown: what scaleTex[0..6] look like, the size of text in the original (the frame allows
// at least 16 bytes), and the meaning of the value 100000.
extern "C" {
extern void* scaleTex[7];
extern void* smallFont;
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
unsigned long long fptodp(float value);
int sprintf(char* out, const char* format, ...);
int fontStringSize__FP4FontPci(void* font, char* text, int limit);
void fontPrint__FiiPci(int x, int y, char* text, int limit);
void fontEnd__Fv();
}

extern "C" void displayWeight__FP12playerStructP12screenStruct(unsigned char* player, int* screen) {
    const float cur = *reinterpret_cast<float*>(player + 308);
    const float max = *reinterpret_cast<float*>(player + 312);
    int n = static_cast<int>(cur / max * 100.0f) / 14;
    if (n >= 6) n = 6;
    textureDrawColor__FP8_textureiiiUi(scaleTex[n], screen[0] + 8, screen[1] + 4, 1, 0x80808080u);
    fontStart__FP4FontiUii(smallFont, 1, 0x80323c3cu, -1);
    char text[32];
    sprintf(text, "%.1f/%d", fptodp(cur), static_cast<int>(max));
    const int width = fontStringSize__FP4FontPci(smallFont, text, 100000);
    fontPrint__FiiPci(screen[0] - width + 127, screen[1] + 17, text, 100000);
    fontEnd__Fv();
}
