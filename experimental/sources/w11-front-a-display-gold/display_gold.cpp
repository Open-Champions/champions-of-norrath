// Observed: displayGold__FP12playerStructP12screenStruct at 0x001b3200 is 460 bytes, arguments
// player (a player record) and screen (an inventory screen record), no return value. With gold
// the signed int at player + 232 and x, y the ints at screen + 0 and screen + 4:
//  1. n = gold / 100 (signed), limited above to 6 (a negative n is used as it is); t = moneyTex[n]
//     (a texture record: a signed halfword at +2 and a halfword at +8). It calls
//     textureDrawColor__FP8_textureiiiUi(t, x + 278, y + 38 + (bit 0x800 of the halfword at t + 8
//     ? 1 : 0) - the halfword at t + 2, 1, 0x80808080).
//  2. fontStart__FP4FontiUii(smallFont, 1, 0x80323c3c, -1).
//  3. gold is read again; a value above 99999999 is stored back at player + 232 (the player's
//     gold is clamped), then sprintf(text, "%d", gold).
//  4. Commas are put into text in place: len = strlen(text); count = (len - 1) / 3; the end
//     index end = len + count + 1; with a = len, and for k = 0 .. count - 1: r = a % 3,
//     a = r != 0 ? r : a / 2, at = a + 4 * k; the bytes text[at .. end - 1] move up one place
//     (copying from the high end, text[i] = text[i - 1] for i = end down to at) and text[at] = ','.
//     a is carried from one round to the next.
//  5. w = fontStringSize__FP4FontPci(smallFont, text, 100000); fontPrint__FiiPci(x - w + 275,
//     y + 16, text, 100000); fontEnd__Fv().
// Inferred: the rule in step 4 puts a comma after every three digits counted from the right for
// the text lengths that occur with the clamp (up to 8 digits, and 11 for the most negative
// value); for a negative number of nine characters it gives a different result, which is kept
// here because the original does the same. moneyTex holds seven pile-of-gold images by hundreds.
// Unknown: the meaning of the 0x800 bit, and the original buffer size (the frame gives text at
// least 48 bytes).
extern "C" {
extern void* moneyTex[7];
extern void* smallFont;
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
int sprintf(char* out, const char* format, ...);
unsigned long strlen(const char* text);
int fontStringSize__FP4FontPci(void* font, char* text, int limit);
void fontPrint__FiiPci(int x, int y, char* text, int limit);
void fontEnd__Fv();
}

extern "C" void displayGold__FP12playerStructP12screenStruct(unsigned char* player, int* screen) {
    int n = *reinterpret_cast<int*>(player + 232) / 100;
    if (n >= 6) n = 6;
    unsigned char* texture = static_cast<unsigned char*>(moneyTex[n]);
    const unsigned flags = *reinterpret_cast<unsigned short*>(texture + 8);
    const int top = *reinterpret_cast<short*>(texture + 2);
    textureDrawColor__FP8_textureiiiUi(texture, screen[0] + 278, ((flags & 0x800u) != 0) + screen[1] + 38 - top, 1, 0x80808080u);
    fontStart__FP4FontiUii(smallFont, 1, 0x80323c3cu, -1);
    int gold = *reinterpret_cast<int*>(player + 232);
    if (gold > 99999999) {
        *reinterpret_cast<int*>(player + 232) = 99999999;
        gold = 99999999;
    }
    char text[48];
    sprintf(text, "%d", gold);
    int a = static_cast<int>(strlen(text));
    const int count = (a - 1) / 3;
    const int end = a + count + 1;
    for (int k = 0; k < count; ++k) {
        const int r = a % 3;
        a = r != 0 ? r : a / 2;
        const int at = a + 4 * k;
        for (int i = end; i >= at; --i) text[i] = text[i - 1];
        text[at] = ',';
    }
    const int width = fontStringSize__FP4FontPci(smallFont, text, 100000);
    fontPrint__FiiPci(screen[0] - width + 275, screen[1] + 16, text, 100000);
    fontEnd__Fv();
}
