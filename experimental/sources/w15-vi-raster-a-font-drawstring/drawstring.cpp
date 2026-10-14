// Observed: DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii at 0x01011c90 is 180 bytes in the pinned SLUS-20565 image. With (this,
// raster, dictionary, text, x, y) it always returns 0. When dictionary is nonnull it hashes the name _6VIFont$sm_FontName with
// VIHashResourceID, stores the hash in a function-local word (never read here) and calls
// FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, hash, 12, &found). When that returns nonzero it builds a two-word position
// {x, y} and calls DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs(raster, found, &position, this, text); the VIFont object
// itself is passed as the colour reference.
// Inferred: the object starts with the font's text colour, and x,y are the top-left point passed on to the raster. This is the
// 16-bit-text sibling of DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCciiUi.
// Unknown: the colour layout, and the names of the local word and of type 12. Nothing here ran on the EE.
extern "C" {
extern const char _6VIFont$sm_FontName[];
unsigned VIHashResourceID(const char* name);
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* out);
int DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs(void* raster, int font, const int* position, const void* color,
                                                        const unsigned short* text);

int DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(void* self, void* raster, void* dictionary, const unsigned short* text,
                                                      int x, int y) {
    if (dictionary) {
        static volatile unsigned ResourceID;
        int found;
        const unsigned id = VIHashResourceID(_6VIFont$sm_FontName);
        ResourceID = id;
        if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id, 12, &found)) {
            int position[2];
            position[0] = x;
            position[1] = y;
            DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs(raster, found, position, self, text);
        }
    }
    return 0;
}
}
