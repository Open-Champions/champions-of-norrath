// Observed: DrawStringEx__4VIUIPCUsiiG10VIColor32Fi at 0x0102eb88 is 284 bytes. With (this, text, x, y, color, shadow),
// where color is a 16-byte value of four floats (passed by reference in the register ABI) and the font is the object
// embedded at +56 of this, the raster is the word at +72 and the dictionary the word at +80. It saves the font's
// current colour (the 16 bytes at the start of the font). When shadow is nonzero it sets the font colour to
// {0, 0, 0, color[3]} and draws the text at (x + 2, y + 2) with DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii.
// It then sets the font colour to color, draws at (x, y), restores the saved colour and returns 0.
// Inferred: a drop-shadowed text draw in which the shadow keeps the text's alpha; the four floats are probably r, g, b
// and alpha (the alpha position is a guess from the shadow's use of the last one).
// Unknown: the field names, whether the colour type is a class with a constructor, and the result type.
extern "C" {
struct VIColor32F {
    float v[4];
};
void SetTextColor__6VIFontRC10VIColor32F(void* font, const VIColor32F* color);
void DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(void* font, void* raster, void* dictionary,
                                                         const unsigned short* text, int x, int y);

int DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(void* self, const unsigned short* text, int x, int y, VIColor32F color,
                                            int shadow) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    void* font = bytes + 56;
    void* raster = reinterpret_cast<void*>(static_cast<unsigned long>(*reinterpret_cast<unsigned*>(bytes + 72)));
    void* dictionary = reinterpret_cast<void*>(static_cast<unsigned long>(*reinterpret_cast<unsigned*>(bytes + 80)));
    const VIColor32F saved = *static_cast<const VIColor32F*>(font);
    if (shadow) {
        VIColor32F shade;
        shade.v[0] = 0.0f;
        shade.v[1] = 0.0f;
        shade.v[2] = 0.0f;
        shade.v[3] = color.v[3];
        SetTextColor__6VIFontRC10VIColor32F(font, &shade);
        DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(font, raster, dictionary, text, x + 2, y + 2);
    }
    SetTextColor__6VIFontRC10VIColor32F(font, &color);
    DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(font, raster, dictionary, text, x, y);
    SetTextColor__6VIFontRC10VIColor32F(font, &saved);
    return 0;
}
}
