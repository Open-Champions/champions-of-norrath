// Observed: GetHeight__6VIFontP8VIRasterP12VIDictionary at 0x01011f90 is 124 bytes in the pinned SLUS-20565 image. With (this,
// raster, dictionary) it returns 0 when dictionary is null. Otherwise it hashes the 8-byte name _6VIFont$sm_FontName with
// VIHashResourceID, stores that hash in a function-local 4-byte word (never read here) and calls
// FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, hash, 12, &found). When that returns zero the result is 0; otherwise it is
// GetPointSize__8VIRasteri(raster, found). this is unused.
// Inferred: type 12 is the font resource type and found is a font id inside the raster; the result is the font's point size.
// Unknown: the name of the local word and of type 12, and the unit of the result (forwarded unchanged from the raster).
// Nothing here ran on the EE.
extern "C" {
extern const char _6VIFont$sm_FontName[];
unsigned VIHashResourceID(const char* name);
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* out);
int GetPointSize__8VIRasteri(void* raster, int font);

int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* self, void* raster, void* dictionary) {
    (void)self;
    int height = 0;
    if (dictionary) {
        static volatile unsigned ResourceID;
        int found;
        const unsigned id = VIHashResourceID(_6VIFont$sm_FontName);
        ResourceID = id;
        if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id, 12, &found))
            height = GetPointSize__8VIRasteri(raster, found);
    }
    return height;
}
}
