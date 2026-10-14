// Observed: DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii at 0x0102ef30, 300 bytes. Arguments: the UI object,
// a 16-bit string, x, y, a four-float colour (passed by reference), an unsigned flags word and an int shadow
// switch. The font lives at ui + 56; its first 16 bytes are the current text colour. The colour argument is
// copied and the font's current colour is saved. When the shadow switch is nonzero, SetTextColor__6VIFontRC10VIColor32F
// is called with (0, 0, 0, colour[3]) and DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi is called
// with (font, raster word at ui + 72, dictionary word at ui + 80, string, x + 2, y + 2, flags). Then SetTextColor
// is called with the colour argument, DrawCenteredString with (x, y), and SetTextColor with the saved colour. The
// result is 0.
// Inferred: a drop shadow two pixels down and right in black with the text's own alpha, then the text itself.
// Unknown: the result type (an int 0 is returned), the member names of the colour, and the units of flags.
extern "C" {
struct VIColor32F {
    float v[4];
};

void SetTextColor__6VIFontRC10VIColor32F(void* font, const VIColor32F& color);
void DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi(void* font, unsigned raster, unsigned dictionary,
                                                                   const unsigned short* text, int x, int y,
                                                                   unsigned flags);

int DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii(char* ui, const unsigned short* text, int x, int y,
                                                       VIColor32F color, unsigned flags, int shadow) {
    char* font = ui + 56;
    const VIColor32F wanted = color;
    const VIColor32F saved = *reinterpret_cast<VIColor32F*>(font);
    if (shadow) {
        VIColor32F dark;
        dark.v[0] = 0.0f;
        dark.v[1] = 0.0f;
        dark.v[2] = 0.0f;
        dark.v[3] = wanted.v[3];
        SetTextColor__6VIFontRC10VIColor32F(font, dark);
        DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi(
            font, *reinterpret_cast<unsigned*>(ui + 72), *reinterpret_cast<unsigned*>(ui + 80), text, x + 2, y + 2,
            flags);
    }
    SetTextColor__6VIFontRC10VIColor32F(font, wanted);
    DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi(
        font, *reinterpret_cast<unsigned*>(ui + 72), *reinterpret_cast<unsigned*>(ui + 80), text, x, y, flags);
    SetTextColor__6VIFontRC10VIColor32F(font, saved);
    return 0;
}
}
