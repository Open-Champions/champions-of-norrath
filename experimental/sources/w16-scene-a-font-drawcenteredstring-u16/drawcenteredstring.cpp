// Observed: DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi at 0x01011e00 is 196 bytes in the pinned SLUS-20565 image. With
// (this, raster, dictionary, text, x, y, flags) it always returns 0. When dictionary is nonnull it hashes the string _6VIFont$sm_FontName
// with VIHashResourceID, stores the hash in a function-local static word at 0x011e2834 (never read here) and calls
// FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, hash, 12, &found). When that returns nonzero it builds a two-word position
// {x, y} and calls DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUsUi(raster, found, &position, this, text, flags); the
// VIFont object itself is passed as the colour reference.
// Inferred: this is the 16-bit-text twin of DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCciiUi (same code, text type differs);
// the object starts with the font's text colour, and x,y are the centre point passed on to the raster.
// Unknown: the colour layout, the meaning of flags, and the name of the local word and of type 12. Nothing here ran on the EE.
extern "C" {
extern const char _6VIFont$sm_FontName[];
unsigned VIHashResourceID(const char* name);
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* out);
int DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUsUi(void* raster, int font, const int* position, const void* color,
                                                                  const unsigned short* text, unsigned flags);

int DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi(void* self, void* raster, void* dictionary, const unsigned short* text,
                                                                 int x, int y, unsigned flags) {
    if (dictionary) {
        static volatile unsigned ResourceID;
        int found;
        const unsigned id = VIHashResourceID(_6VIFont$sm_FontName);
        ResourceID = id;
        if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id, 12, &found)) {
            int position[2];
            position[0] = x;
            position[1] = y;
            DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUsUi(raster, found, position, self, text, flags);
        }
    }
    return 0;
}
}
